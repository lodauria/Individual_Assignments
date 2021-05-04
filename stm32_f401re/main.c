#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/adc.h"
#include "analog_util.h"
#include "jsmn.h"

#define NODE_ID 0
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

#define NUMOFSUBS           (1U)
#define TOPIC_MAXLEN        (64U)

#define ADC_IN_USE  ADC_LINE(0)			// pin A0
#define ADC_RES		ADC_RES_12BIT

static char stack[THREAD_STACKSIZE_DEFAULT];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

// Pins initialization
static gpio_t projectorSens = GPIO_PIN(PORT_A, 10); // pin D2
static gpio_t motorA = GPIO_PIN(PORT_A, 8); 	// pin D7
static gpio_t motorB = GPIO_PIN(PORT_A, 9); 	// pin D8
static gpio_t relay = GPIO_PIN(PORT_B, 5); 		// pin D4

// Actuators variables
int relay_stauts=0;   // 0: lights off, 1: lights on
int motor_status=2;   // 0: no action, 1: curatin closed, 2: curtain open

// Emcute thread
static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, "nucleo-f401re");
    return NULL;
}

// Spin the motor for 3 seconds in one of the two directions
static void actuate_DC_motor(int m1, int m2, int status)
{
	if (status >= 2){
        gpio_clear(m1);
        gpio_set(m2);
	}
	else{
        if (status == 0) gpio_clear(m1);
        else gpio_set(m1);
        gpio_clear(m2);	
	}

	xtimer_msleep(3000);
	gpio_clear(m1);
	gpio_clear(m2);
}

// When a message is received
static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    // Interpret the JSON message 
    char *in = (char *)data;
    jsmn_parser parser;
    jsmntok_t tok[10];

	jsmn_init(&parser);
	int elem = jsmn_parse(&parser, in, len, tok, 10);

	if (elem < 7) {
		printf("Error reading json from topic \"%s\"\n", topic->name);
	}
	else{

		char node_id[3];
        sprintf(node_id,"%.*s", tok[2].end - tok[2].start, in + tok[2].start);
        if (atoi(node_id) == NODE_ID){

			// First value is relay command, the second is the motor command
			char relay_str[2];
			char motor_str[2];
			sprintf(relay_str,"%.*s", tok[4].end - tok[4].start, in + tok[4].start);
			sprintf(motor_str,"%.*s", tok[6].end - tok[6].start, in + tok[6].start);

	        printf("\nGot actuation commands:\nRelay status: %s\nMotor status: %s\n",relay_str,motor_str);
	        puts("");

	        // Apply actuation
			relay_stauts = atoi(relay_str);
	        if (relay_stauts == 0){
	            gpio_clear(relay);
	        }
	        else{
	            gpio_set(relay);
	        }
	        if (atoi(motor_str) != 0){
	            motor_status = atoi(motor_str);
	            actuate_DC_motor(motorA, motorB, motor_status);
	        }
	    }
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

    // Subscribe to the MQTT actuation topic
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
	gpio_init(projectorSens, GPIO_IN);

    if (!gpio_is_valid(projectorSens)){
        printf("Failed to initialize projectr sensor\n");
        return -1;
    }
    printf("Projector sensor ready\n");

    // Initialize the analogic pin
	if (adc_init(ADC_IN_USE) < 0) {
	    printf("Failed to initialize photoresistor\n");
	    return -1;
	}
	printf("Photoresistor ready\n");

	// Initialize the relay digital pin
    if (gpio_init(relay, GPIO_OUT)) {
    	printf("Failed to initialize relay pin\n");
        return -1;
    }
    printf("Relay pin ready\n");

    // Initialize the DC motor digital pins
    if (gpio_init(motorA, GPIO_OUT)) {
    	printf("Failed to initialize motor pin\n");
        return -1;
    }
    if (gpio_init(motorB, GPIO_OUT)) {
    	printf("Failed to initialize motor pin\n");
        return -1;
    }
    printf("Motor pin ready\n");

    // Setup MQTT-SN connection
    printf("Setting up MQTT-SN.\n");
    setup_mqtt();

    // SENSING LOOP

    while(1){

		// GET SENSORS READINGS

		// Projector
		int projector_status;
	    projector_status = gpio_read(projectorSens);
	    if (projector_status > 0){
	    	printf("Projector: OPEN, %i\n", projector_status);
	    	projector_status = 1;
	    }
	    else{
	    	printf("Projector: CLOSED\n");
	    }

	    // Illuminance
	    int light_raw;
	    int light_level;
	    light_raw = adc_sample(ADC_IN_USE, ADC_RES);
	    if (light_raw < 0){
	    	printf("Photoresistor resolution error");
	    	return 1;
	    }
		light_level = adc_util_map(light_raw, ADC_RES, 10, 100);
		printf("Illuminance: %i lx\n", light_level);

		// PUBLISH VIA MQTT THE SENSORS DATA

		char message[100];
        sprintf(message, "{\"id\":%d, \"light_level\":%i, \"projector_status\":%i,\"relay\":%i,\"motor\":%i}", NODE_ID, light_level, projector_status, relay_stauts, motor_status);
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