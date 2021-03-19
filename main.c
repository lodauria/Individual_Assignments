#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/adc.h"
#include "periph/rtc.h"
#include "periph/pm.h"
#include "analog_util.h"

#define OPEN 1024
#define ADC_IN_USE  ADC_LINE(0)
#define ADC_RES		ADC_RES_12BIT

static void callback_rtc(void *arg)
{
    puts(arg);
}

static void actuate_DC_motor(int m1, int m2, int status)
{
	if (status >= 0){
		if (status == 0) gpio_write(m1, 0);
		else gpio_write(m1, 1);
		gpio_write(m2, 0);
	}
	else{
		gpio_write(m2, 1);
		gpio_write(m1, 0);
	}
}

int main(void)
{

	// INITIALIZATION

    // Initialize the Hall sensor digital pin
    gpio_t projectorSens = GPIO_PIN(PORT_A, 10);
	gpio_init(projectorSens, GPIO_OUT);

    if (gpio_is_valid(projectorSens)){
        printf("Projector sensor ready\n");
    }
    else {
        printf("Failed to initialize projectr sensor\n");
        return 1;
    }

    // Initialize the analogic pin
	if (adc_init(ADC_IN_USE) < 0) {
	    printf("Failed to initialize photoresistor\n");
	    return 1;
	} else {
	    printf("Photoresistor ready\n");
	}

	// Initialize the relay digital pin
    gpio_t relay = GPIO_PIN(PORT_B, 10);
	gpio_init(relay, GPIO_IN_PD);

    if (gpio_is_valid(relay)){
        printf("Relay pin ready\n");
    }
    else {
        printf("Failed to initialize relay pin\n");
        return 1;
    }

    // Initialize the DC motor digital pins
    gpio_t motorA = GPIO_PIN(PORT_A, 5);
    gpio_t motorB = GPIO_PIN(PORT_A, 6);
	gpio_init(motorA, GPIO_IN);
	gpio_init(motorB, GPIO_IN);

    if (gpio_is_valid(motorA) && gpio_is_valid(motorB) ){
        printf("DC motor pins ready\n");
    }
    else {
        printf("Failed to initialize DC motor pins\n");
        return 1;
    }

	// GET SENSORS READINGS

	// Projector
	int projector_status;
    projector_status = gpio_read(projectorSens);
    if (projector_status == OPEN){
    	printf("Projector open\n");
    }
    else{
    	printf("Projector closed\n");
    }

    // Illuminance
    int light_raw;
    int light_level = 0;
    int num_samples = 5;
    int samples_delay = 500;
    for (int i = 0; i<num_samples; i++){
    	light_raw = adc_sample(ADC_IN_USE, ADC_RES);
	    if (light_raw < 0){
	    	printf("Photoresistor resolution error");
	    	return 1;
	    }
	    else{
	    	light_level += adc_util_map(light_raw, ADC_RES, 10, 100);
	    }
	    xtimer_msleep(samples_delay);
	}
	light_level = light_level/num_samples;
	printf("Illuminance: %i lx\n", light_level);

	// PUBLISH VIA MQTT SENSOR DATA
	// TODO

	// GET MQTT MESSAGES
	// TODO
	int relay_stauts = 0; //DEBUG
	int motor_status = 0; //DEBUG

	// APPLY ACTUATION
	gpio_write(relay, relay_stauts);
	actuate_DC_motor(motorA, motorB, motor_status);

	// GO TO LOW POWER MODE

    const int mode = 0;
    const int delay = 10;

    printf("Setting wakeup from mode %d in %d seconds.\n", mode, delay);
    fflush(stdout);

    struct tm time;
    rtc_get_time(&time);
    time.tm_sec += delay;
    rtc_set_alarm(&time, callback_rtc, "Wakeup alarm");
    pm_set(mode);

	return 0;

}