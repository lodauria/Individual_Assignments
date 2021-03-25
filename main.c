#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/adc.h"
#include "analog_util.h"

#ifndef EMCUTE_ID
#define EMCUTE_ID           ("stm32")
#endif
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

#define NUMOFSUBS           (1U)
#define TOPIC_MAXLEN        (64U)

#define OPEN 1024
#define ADC_IN_USE  ADC_LINE(0)
#define ADC_RES		ADC_RES_12BIT

static char stack[THREAD_STACKSIZE_DEFAULT];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static gpio_t motorA = GPIO_PIN(PORT_A, 5);
static gpio_t motorB = GPIO_PIN(PORT_A, 6);
static gpio_t relay = GPIO_PIN(PORT_B, 10);

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, EMCUTE_ID);
    return NULL;
}

static void actuate_DC_motor(int m1, int m2, int status)
{
	if (status >= 2){
        gpio_write(m1, 0);
        gpio_write(m2, 1);
	}
	else{
        if (status == 0) gpio_write(m1, 0);
        else gpio_write(m1, 1);
        gpio_write(m2, 0);	
	}

	xtimer_msleep(1000);
}

// WHAT TO DO WHEN A MESSAGE IS RECEIVED
static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;

    // Ignore irrelevant messages
    // Correct command format = a;(relay);(motor)     e.g. = "a;1;1"
    if (in[0]=='a' && in[1]==';'){
        printf("Got command for topic '%s' \n", topic->name);
        for (size_t i = 0; i < len; i++) {
            printf("%c", in[i]);
        }

        puts("");

        // Get commands from the message
        char in1[2];
        char in2[2];
        sprintf(in1, "%c", in[2]);
        sprintf(in2, "%c", in[4]);

        int relay_stauts = atoi(in1);
        int motor_status = atoi(in2);

        // APPLY ACTUATION
        gpio_write(relay, relay_stauts);
        actuate_DC_motor(motorA, motorB, motor_status);
    }

}

// Setup the EMCUTE, open a connection to the MQTT-S broker and subscribe to default topic

int setup_mqtt(void)
{
    // Subscription buffer
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));

    // Start the emcute thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");
    printf("Connecting to MQTT-SN broker %s port %d.\n", SERVER_ADDR, SERVER_PORT);

    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };
    char *topic = MQTT_TOPIC;
    char *message = "connected";
    size_t len = strlen(message);

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR) == NULL) {
        printf("error parsing IPv6 address\n");
        return 1;
    }

    if (emcute_con(&gw, true, topic, message, len, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", SERVER_ADDR,
               (int)gw.port);
        return 1;
    }

    printf("Successfully connected to gateway at [%s]:%i\n",
           SERVER_ADDR, (int)gw.port);

    unsigned flags = EMCUTE_QOS_0;
    subscriptions[0].cb = on_pub;
    strcpy(topics[0], MQTT_TOPIC);
    subscriptions[0].topic.name = MQTT_TOPIC;

    if (emcute_sub(&subscriptions[0], flags) != EMCUTE_OK) {
        printf("error: unable to subscribe to %s\n", MQTT_TOPIC);
        return 1;
    }

    printf("Now subscribed to %s\n", MQTT_TOPIC);

    return 1;
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
	gpio_init(relay, GPIO_IN_PD);

    if (gpio_is_valid(relay)){
        printf("Relay pin ready\n");
    }
    else {
        printf("Failed to initialize relay pin\n");
        return 1;
    }

	// Initialize the DC motor digital pins
	gpio_init(motorA, GPIO_IN);
	gpio_init(motorB, GPIO_IN);

    if (gpio_is_valid(motorA) && gpio_is_valid(motorB) ){
        printf("DC motor pins ready\n");
    }
    else {
        printf("Failed to initialize DC motor pins\n");
        return 1;
    }

    // Setup MQTT-SN connection
    printf("Setting up MQTT-SN.\n");
    setup_mqtt();

    // SENSING LOOP

    while(1){

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

		// PUBLISH VIA MQTT SENSORs DATA

		char message[10];
        sprintf(message, "s;%i;%i", light_level, projector_status);
		emcute_topic_t t;

	    // Get topic id
	    t.name = MQTT_TOPIC;
	    if (emcute_reg(&t) != EMCUTE_OK) {
	        puts("error: unable to obtain topic ID");
	        return 1;
	    }

	    // Publish data
	    if (emcute_pub(&t, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
	        printf("error: unable to publish data to topic '%s [%i]'\n", t.name, (int)t.id);
	        return 1;
	    }

	    printf("Sensor readings published\n");
		xtimer_sleep(10);
	}

	return 0;

}