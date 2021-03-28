#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/adc.h"
#include "analog_util.h"
#include "jsmn.h"

#ifndef EMCUTE_ID
#define EMCUTE_ID           ("nucleo-f401re")
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
    int relay_stauts;
    int motor_status;

    jsmn_parser parser;
    jsmntok_t tok[10];

	jsmn_init(&parser);
	int elem = jsmn_parse(&parser, in, len, tok, 10);

	if (elem < 5) {
		printf("Error reading json from topic \"%s\"\n", topic->name);
	}
	else{
		char relay_str[2];
		char motor_str[2];
		sprintf(relay_str,"%.*s", tok[2].end - tok[2].start, in + tok[2].start);
		sprintf(motor_str,"%.*s", tok[4].end - tok[4].start, in + tok[4].start);
		relay_stauts = atoi(relay_str);
        motor_status = atoi(motor_str);

        printf("\nGot actuation commands:\nRelay status: %i\nMotor status: %i\n",relay_stauts,motor_status);
        puts("");

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

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR) == NULL) {
        printf("Error parsing IPv6 address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%i\n", SERVER_ADDR,
               (int)gw.port);
        return 1;
    }

    printf("Successfully connected to gateway at [%s]:%i\n",
           SERVER_ADDR, (int)gw.port);

    subscriptions[0].cb = on_pub;
    strcpy(topics[0], MQTT_TOPIC_A);
    subscriptions[0].topic.name = MQTT_TOPIC_A;

    if (emcute_sub(&subscriptions[0], EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: unable to subscribe to %s\n", MQTT_TOPIC_A);
        return 1;
    }

    printf("Now subscribed to %s\n", MQTT_TOPIC_A);
    puts("");

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
	    	printf("Projector: OPEN\n");
	    }
	    else{
	    	printf("Projector: CLOSED\n");
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

		char message[100];
        sprintf(message, "{\"light_level\":%i, \"projector_status\":%i}", light_level, projector_status);
		emcute_topic_t t;

	    // Get topic id
	    t.name = MQTT_TOPIC_S;
	    if (emcute_reg(&t) != EMCUTE_OK) {
	        puts("Error: unable to obtain topic ID");
	        return 1;
	    }

	    // Publish data
	    if (emcute_pub(&t, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
	        printf("Error: unable to publish data to topic '%s [%i]'\n", t.name, (int)t.id);
	        return 1;
	    }

	    printf("Sensor readings published\n");
	    puts("");
	    
		xtimer_sleep(10);
	}

	return 0;

}