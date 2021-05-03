#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "xtimer.h"
#include "jsmn.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

#define NUMOFSUBS           (1U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

// Actuators variables
int relay_stauts=0;   // 0: lights off, 1: lights on
int motor_status=2;   // 0: no action, 1: curatin closed, 2: curtain open

// Emcute thread
static void *emcute_thread(void *arg)
{
    (void)arg;
    char id_emcute[12]; 
    strcat(id_emcute, "m3-node-");
    strcat(id_emcute, NODE_ID);
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, id_emcute);
    return NULL;
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
        if (atoi(node_id) == atoi(NODE_ID)){

            // First value is relay command, the second is the motor command
            char relay_str[2];
            char motor_str[2];
            sprintf(relay_str,"%.*s", tok[4].end - tok[4].start, in + tok[4].start);
            sprintf(motor_str,"%.*s", tok[6].end - tok[6].start, in + tok[6].start);

            printf("\nGot actuation commands:\nRelay status: %s\nMotor status: %s\n",relay_str,motor_str);
            puts("");

            // Store new actuators status
            relay_stauts = atoi(relay_str);
            
            if (atoi(motor_str) != 0){
                motor_status = atoi(motor_str);
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

    // Setup MQTT-SN connection
    printf("Setting up MQTT-SN.\n");
    setup_mqtt();

    // SENSING LOOP

    while(1){

        // GET SENSORS READINGS

        // Projector
        int projector_status;
        projector_status = 1;

        // Illuminance
        int light_level;
        light_level = 56;

        // PUBLISH VIA MQTT THE SENSORS DATA

        char message[100];
        sprintf(message, "{\"id\":%i, \"light_level\":%i, \"projector_status\":%i,\"relay\":%i,\"motor\":%i}", atoi(NODE_ID), light_level, projector_status, relay_stauts, motor_status);
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
