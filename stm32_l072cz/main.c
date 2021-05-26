#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "random.h"
#include "net/emcute.h"
#include "xtimer.h"
#include "jsmn.h"


#include "msg.h"
#include "thread.h"
#include "fmt.h"

#include "periph/rtc.h"

#include "net/loramac.h"
#include "semtech_loramac.h"

/*
#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
static kernel_pid_t sender_pid;
static char sender_stack[THREAD_STACKSIZE_MAIN / 2];
*/

semtech_loramac_t loramac;

static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

/*
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

#define NUMOFSUBS           (1U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];
*/

// Actuators variables
int relay_stauts=0;   // 0: lights off, 1: lights on
int motor_status=2;   // 0: no action, 1: curatin closed, 2: curtain open
/*
// Emcute thread
static void *emcute_thread(void *arg)
{
    (void)arg;
    char id_emcute[12];
    sprintf(id_emcute,"m3-node-%d",NODE_ID);
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, id_emcute);
    return NULL;
}
*/

/*
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

            // Store new actuators status
            relay_stauts = atoi(relay_str);
            
            if (atoi(motor_str) != 0){
                motor_status = atoi(motor_str);
            }
        }

    }

}
*/
// Setup the EMCUTE, open a connection to the MQTT-S broker and subscribe to default topic

int setup_loramac(void)
{
    /* Convert identifiers and application key */
    fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);

    /* Initialize the loramac stack */
    semtech_loramac_init(&loramac);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    /* Start the Over-The-Air Activation (OTAA) procedure to retrieve the
     * generated device address and to get the network and application session
     * keys.
     */
    printf("Starting join procedure\n");
    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        printf("Join procedure failed\n");
        return 1;
    }
    
    printf("Join procedure succeeded\n");
    return 0;    
}

int main(void)
{

    // INITIALIZATION

    // Setup LoRa connection
    printf("Setting LoRa\n");
    if (setup_loramac() != 0) {
        return 1;
    }

    // SENSING LOOP

    while(1){

        // GET SENSORS READINGS

        // Projector
        int projector_status;
        projector_status = random_uint32_range(0,2);

        // Illuminance
        int light_level;
        light_level = random_uint32_range(10,100);

        // PUBLISH VIA LORAMAC THE SENSORS DATA

        char message[100];
        sprintf(message, "{\"id\":%d, \"light_level\":%i, \"projector_status\":%i,\"relay\":%i,\"motor\":%i}", NODE_ID, light_level, projector_status, relay_stauts, motor_status);
        uint8_t ret = semtech_loramac_send(&loramac, (uint8_t *)message, strlen(message));
        if (ret != SEMTECH_LORAMAC_TX_DONE){
            printf("Cannot send message '%s', ret code: %d\n", message, ret);
        }
        else{
            printf("Sensor readings published\n");
        }

        xtimer_sleep(10);
    }

    return 0;
    
}