#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/adc.h"
#include "analog_util.h"

#define OPEN 1024
#define ADC_IN_USE  ADC_LINE(0)
#define ADC_RES		ADC_RES_12BIT

int main(void)
{
    // Initialize the digital pin
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

	// DEBUG: Print sensor values
	// TODO: send/receive information to/from the cloud
	// TODO: Actuation
    int projector_status;
    int light_raw;
    int light_level = 0;
    xtimer_ticks32_t last = xtimer_now();

    while (1){

    	// Projector
	    projector_status = gpio_read(projectorSens);
	    if (projector_status == OPEN){
	    	printf("Projector open\n");
	    }
	    else{
	    	printf("Projector closed\n");
	    }

	    // Illuminance
	    light_level = 0;
	    for (int i = 0; i<10; i++){
	    	light_raw = adc_sample(ADC_IN_USE, ADC_RES);
		    if (light_raw < 0){
		    	printf("Photoresistor resolution error");
		    	return 1;
		    }
		    else{
		    	light_level += adc_util_map(light_raw, ADC_RES, 10, 100);
		    }
		    xtimer_msleep(100);
		}
		printf("Illuminance: %i lx\n", light_level/10);

		// Go to sleep for 30 seconds
		xtimer_periodic_wakeup(&last, 30000000);

	}
	return 0;
}
