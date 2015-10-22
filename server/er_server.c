#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <getopt.h>
#include <microhttpd.h>
#include "wheels.h"
#include "machine_control.h"

#define TRUE	( 1 == 1 )
#define FALSE	( !TRUE )

#define PORT			80
#define MAIN_PAGE		"index.html"
#define SHELL_GET_MIME_TYPE	"file -b --mime-type "
#define ERROR_PAGE		"<html><body>An internal server error has occured!</body></html>"

#define send_OK()	send_text("OK", tmp_connect, MHD_HTTP_OK)
#define isval( val )	strcmp(value, val) == 0

static int send_file ( const char* file_name, const char* mime_type, struct MHD_Connection *connection );
static int run_actions ( void *cls, enum MHD_ValueKind kind, const char *key, const char *value );
static int send_text ( const char* text, struct MHD_Connection* connection, int mhd_responce);


static struct MHD_Connection* tmp_connect;
static int silent = FALSE, quiet = FALSE;


static int
answer_to_connection ( void *cls, struct MHD_Connection *connection,
                       const char *url, const char *method,
                       const char *version, const char *upload_data,
                       size_t *upload_data_size, void **con_cls ) {
    printf ( "New %s request for %s using version %s\n", method, url, version );

    int ret = 0;

    if ( strlen ( url+1 ) > 1000 ) {
        printf ( "ERROR: Too long url string\n" );
        return ret;
    }

    if ( strcmp ( url, "/cmd" ) == 0 ) {
        tmp_connect = connection;
        ret = MHD_get_connection_values ( connection, MHD_GET_ARGUMENT_KIND, run_actions, NULL );
    } else {

        const char* url_file = ( strcmp ( url, "/" ) == 0 ) ? MAIN_PAGE : url+1;

        char cmd[1024] = SHELL_GET_MIME_TYPE;
        strcat ( cmd, url_file );

        FILE *f = popen ( cmd, "r" );
        if ( f == NULL ) {
            printf ( "ERROR: Run shell command : %d (%s)\n", errno, strerror ( errno ) );
            return ret;
        }

        char mime[255];
        if ( fgets ( mime, 255, f ) == NULL )
            return ret;
        mime[strlen ( mime ) - 1] = '\0';

        pclose ( f );

        ret = send_file ( url_file, mime, connection );

    }

    return ret;
}

static int
send_file ( const char* file_name, const char* mime_type, struct MHD_Connection *connection ) {

    struct stat statbuf;
    struct MHD_Response *response;
    int ret, fd;

    if ( ( -1 == ( fd = open ( file_name, O_RDONLY ) ) ) ||
            ( 0 != fstat ( fd, &statbuf ) ) ) {
        /* error accessing file */
        if ( fd != -1 ) close ( fd );
        const char *errorstr = ERROR_PAGE;

        response =
            MHD_create_response_from_buffer ( strlen ( errorstr ),
                                              ( void * ) errorstr,
                                              MHD_RESPMEM_PERSISTENT );
        if ( response ) {
            ret =
                MHD_queue_response ( connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response );
            MHD_destroy_response ( response );

            return MHD_YES;
        } else
            return MHD_NO;
    }

    response =
        MHD_create_response_from_fd_at_offset64 ( statbuf.st_size, fd, 0 );
    MHD_add_response_header ( response, "Content-Type", mime_type );
    ret =
        MHD_queue_response ( connection, MHD_HTTP_OK, response );
    MHD_destroy_response ( response );

    printf ( "SEND FILE: %s (%s)\n", file_name, mime_type );

    return ret;
}

static int
send_text ( const char* text, struct MHD_Connection* connection, int mhd_responce ) {
    struct MHD_Response *response;
    int ret;

    response =
        MHD_create_response_from_buffer ( strlen ( text ), ( void * ) text, MHD_RESPMEM_PERSISTENT );

    ret = MHD_queue_response ( connection, mhd_responce, response );
    MHD_destroy_response ( response );

    return ret;
}

static int
run_actions ( void *cls, enum MHD_ValueKind kind, const char *key, const char *value ) {
    if ( strcmp ( key, "action" ) == 0 ) {

        if ( isval ( "checkServerWorker" ) )
            send_text ( "OK", tmp_connect, MHD_YES );
        else if ( isval ( "getStatus" ) )
	  send_text ( "It's work!", tmp_connect, MHD_HTTP_OK );
        else if ( isval ( "stop" ) ) {
            stop();
            send_OK();
        } else if ( isval ( "left" ) ) {
            step_yaw ( LFT );
            send_OK();
        } else if ( isval ( "right" ) ) {
            step_yaw ( RGH );
            send_OK();
        } else if ( isval ( "up" ) ) {
            step_speed ( UP );
            send_OK();
        } else if ( isval ( "down" ) ) {
            step_speed ( DW );
            send_OK();
        } else
            send_text ( "Unknown action", tmp_connect, MHD_NO );
    }

    return MHD_YES;
}

int
main ( int argc, char** argv ) {
    int c;

    while ( ( c = getopt ( argc, argv, "sq" ) ) != -1 )
        switch ( c ) {
        case 's':
            silent = TRUE;
            break;
        case 'q':
            quiet = TRUE;
            break;
        default:
            abort();
        }

    init_wheels();
    stop();

    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon ( MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                                &answer_to_connection, NULL, MHD_OPTION_END );
    if ( NULL == daemon ) {
        stop();
        return 1;
    }

    getchar();

    stop();

    MHD_stop_daemon ( daemon );
    return 0;
}
