#ifndef _DIAGNOSTICS_H
#define _DIAGNOSTICS_H

#ifdef NDEBUG

#define init_diagnostic()	(void)
#define systime()		(void)

#else /* !NDEBUG */

#define init_diagnostics()	__init_diagnostics()
#define systime()		__system_time_print()


extern void __init_diagnostics();
extern void __system_time_print();

#endif /* NDEBUG */

#endif /* _DIAGNOSTICS_H */
