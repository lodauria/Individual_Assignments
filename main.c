#include <stdio.h>
#include "periph/gpio.h"

#define OPEN 1024

int main(void)
{
    
    gpio_t projectorSens = GPIO_PIN(PORT_A, 10);
	gpio_init(projectorSens, GPIO_OUT);

    if (gpio_is_valid(projectorSens)){
        printf("Projector sensor ready\n");
    }
    else {
        printf("Failed to initialize projectr sensor\n");
        return 1;
    }

    int projector_status = gpio_read(projectorSens);

    if (projector_status == OPEN){
    	printf("Projector open\n");
    }
    else{
    	printf("Projector closed\n");
    }

	return 0;
}
