#define _BSD_SOURCE

#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <liber_tools.h>
#include "rpp_pwm.h"

#define MAX_PWM_VALUE (100)
#define MIN_FREQ_HZ   (1)
#define MAX_FREQ_HZ   (10000)

struct rpp_pwm {
    volatile bool isrun;
    pthread_t thrd;
    enum rpp_pin pin;

    u32 peroid_us;

    struct {
        u32 up;
        u32 down;
    } duty;

    struct {
        volatile bool refresh;
        volatile u32 up;
        volatile u32 down;
    } update;
};

rpp_pwm_t rpp_pwm_create(enum rpp_pin pin, u32 freq_hz) {
    struct rpp_pwm * pwm;

    if (freq_hz < MIN_FREQ_HZ || MAX_FREQ_HZ < freq_hz) return NULL;

    pwm = bxi_malloc(sizeof(struct rpp_pwm));
    pwm->pin = pin;
    pwm->peroid_us = BXI_USEC_IN_SEC / freq_hz;

    pwm->duty.down = pwm->peroid_us;
    pwm->duty.up   = 0;

    pwm->update.refresh = false;
    pwm->update.down = 0;
    pwm->update.up   = 0;

    pwm->isrun = false;

    rpp_gpio_out(pin);

    return pwm;
}

void rpp_pwm_destroy(rpp_pwm_t pwm) {
    if (!pwm) return;
    rpp_pwm_stop(pwm);
    bxi_free(pwm);
}

static void _pwm_update(struct rpp_pwm * pwm) {
    pwm->update.refresh = false;
    pwm->duty.up   = pwm->update.up;
    pwm->duty.down = pwm->update.down;
}

static void * _pwm_thread(void * data) {
    struct rpp_pwm * pwm = data;

    while (pwm->isrun) {
        if (pwm->update.refresh)
            _pwm_update(pwm);

        if (pwm->duty.up != 0)
            rpp_gpio_set(pwm->pin, RPP_PIN_ST_HI);
        usleep(pwm->duty.up);

        if (pwm->duty.down != 0)
            rpp_gpio_set(pwm->pin, RPP_PIN_ST_LW);
        usleep(pwm->duty.down);
    }

    return NULL;
}

bool rpp_pwm_start(rpp_pwm_t pwm) {
    i32 r;

    if (!pwm) return false;

    pwm->isrun = true;
    r = pthread_create(&pwm->thrd, NULL, _pwm_thread, pwm);
    if (r != 0) {
        ert_err("pwm (%d pin) thread create failed", pwm->pin);
        pwm->isrun = false;
        return false;
    }
    return true;
}

bool rpp_pwm_stop(rpp_pwm_t pwm) {
    i32 r;

    pwm->isrun = false;

    r = pthread_join(pwm->thrd, NULL);
    if (r != 0) {
        ert_err("pwm (%d pin) thread stop failed", pwm->pin);
        return false;
    }
    return true;
}

void rpp_pwm_update(rpp_pwm_t pwm, u8 value) {
    value = BXI_MIN(value, MAX_PWM_VALUE);

    pwm->update.up = (value * pwm->peroid_us) / MAX_PWM_VALUE;
    pwm->update.down = pwm->peroid_us - pwm->update.up;

    pwm->update.refresh = true;
}
