function log(s) {
    $('#log').html(s + "\n" + $('#log').html());
}

function checkServerWorker(func) {
    $.get('cmd?action=checkServerWorker',
        function (res, status) {
            if (status == 'success') {
                if (res != "OK") {
                    $('p.error').html("К серверу подключились, но скрипт вернул не 0");
                    $('p.error').css('display', 'block');
                    return;
                } else $('p.error').css('display', 'none');
            }
            $('p.error').html("Не могу подключиться к серверу");
            $('p.error').css('display', 'block');
        }
    );
}


function checkWidth() {
    var winH = $(window).height();
    var topH = $("#top").height();
    $("#workpanel").css("height", winH - topH);
    $("#workpanel").css("margin-top", topH);

    var newHeight = $("#cpanel").height();
    var newWidth = $("#cpanel").width();
    var picw = newHeight / 3 * 4 < newWidth ? newHeight / 3 * 4 : newWidth;
    var pich = newHeight / 3 * 4 < newWidth ? newHeight : newWidth / 4 * 3;
    $("input.button_rule").css("width", picw / 15);
    $("#camera img").css("height", pich);
    $("#camera img").css("width", picw);

    var top = Math.max((newHeight - newWidth / 4 * 3) / 2, 0);
    $("#camera").css("top", top);
    
//    $("#speech").css("width", );
}

function command(cmd) {
    $.get('cmd?action=' + cmd,
        function (res, status) {
            if (status == 'success' && res != "OK") alert(res);
        }
    );

    log('Отправили команду ' + cmd);
}

function getStatus() {
    $get('cmd?action=getStatus', function (res, status) {
        if (status == 'success') $('#status').html(res);

        ////////////////////////

        $('#attr-wireless').get(0).src = "img/wireless/stat-" + +".png";
    });
}

function say() {
    s = $('#speech').get(0).value;
    $.get("cmd?action=say&text=" + encodeURIComponent(s),
        function (res, status) {
            if (status == 'success' && res != "OK") log('Сказали: ' + s);
            else log('Не получилось сказать: status=' + status + ' res=' + res);
            $('#speech').get(0).value = "";
        });

}