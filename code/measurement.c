#include <stdio.h>
#include "stdlib.h"
#include "periph/adc.h"
#include "periph/gpio.h"
#include "analog_util.h"
#include "xtimer.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "u8g2.h"
#include "u8x8_riotos.h"

#include "paho_mqtt.h"
#include "MQTTClient.h"

#define PARKING_SPOT_0 ADC_LINE(0)
#define PARKING_SPOT_1 ADC_LINE(1)
#define PARKING_SPOT_2 ADC_LINE(2)
#define PARKING_SPOT_3 ADC_LINE(3)
#define ADC_RES ADC_RES_12BIT

#define DELAY (2LU * US_PER_SEC)

#define SSD1306_I2C_ADDR (0x3c)

#define SPOT_0 GPIO21
#define SPOT_1 GPIO12
#define SPOT_2 GPIO13
#define SPOT_3 GPIO14

#define MQTT_VERSION_v311 4 /* MQTT v3.1.1 version is 4 */
#define COMMAND_TIMEOUT_MS 4000
#define BUF_SIZE 1024
#define BROKER_ADDRESS "192.168.156.45"
#define DEFAULT_MQTT_PORT 1883
#define DEFAULT_KEEPALIVE_SEC 10
#define TOPIC "smartpark"

#define IS_RETAINED_MSG 0

static MQTTClient client;
static Network network;
static unsigned char buf[BUF_SIZE];
static unsigned char readbuf[BUF_SIZE];

u8g2_t u8g2;
uint32_t screen = 0;

int32_t free_spot_value_0 = 0;
int32_t free_spot_value_1 = 0;
int32_t free_spot_value_2 = 0;
int32_t free_spot_value_3 = 0;

u8x8_riotos_t user_data = {
    .device_index = I2C_DEV(0),
    .pin_cs = GPIO_UNDEF,
    .pin_dc = GPIO_UNDEF,
    .pin_reset = GPIO16,
};

void drawPark(void)
{
    u8g2_ClearBuffer(&u8g2);

    // Draw park lines
    u8g2_DrawFrame(&u8g2, 0, 0, 128, 64);
    u8g2_DrawLine(&u8g2, 42, 0, 42, 64);
    u8g2_DrawLine(&u8g2, 84, 0, 84, 64);
    u8g2_DrawLine(&u8g2, 0, 32, 128, 32);
    u8g2_SendBuffer(&u8g2);
}

void spotUpdate(bool occupied, uint16_t spotId, gpio_t spot)
{

    if (occupied)
    {
        u8g2_SetDrawColor(&u8g2, 1);
        gpio_set(spot);
    }
    else
    {
        u8g2_SetDrawColor(&u8g2, 0);
        gpio_clear(spot);
    }
    if (spotId < 3)
        u8g2_DrawBox(&u8g2, (42 * spotId + 6), 6, 32, 22);
    else
        u8g2_DrawBox(&u8g2, (42 * (spotId - 3) + 6), 32 + 6, 32, 22);
    u8g2_SendBuffer(&u8g2);
}

int initADC(void)
{

    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 0, 15, "Initializing ADC...");
    u8g2_SendBuffer(&u8g2);

    if (adc_init(PARKING_SPOT_0) < 0 || adc_init(PARKING_SPOT_1) < 0 || adc_init(PARKING_SPOT_2) < 0 || adc_init(PARKING_SPOT_3) < 0)
    {
        printf("Failed to initialize ADC\n");
        return 1;
    }
    else
        printf("ADC OK\n");
    return 0;
}

int initLEDs(void)
{

    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 0, 15, "Initializing LEDs...");
    u8g2_SendBuffer(&u8g2);

    if (gpio_init(SPOT_0, GPIO_OUT) || gpio_init(SPOT_2, GPIO_OUT) || gpio_init(SPOT_2, GPIO_OUT))
        puts("Led initialization failed.\n");

    else
        printf("LEDs OK\n");
    return 0;
}

void initSensorValues(void)
{
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 0, 15, "Initializing sensors values...");
    u8g2_SendBuffer(&u8g2);

    puts("Initializing sensors values...\n");
    int32_t sample_0 = 0;
    int32_t sample_1 = 0;
    int32_t sample_2 = 0;
    int32_t sample_3 = 0;

    for (int i = 0; i < 10; i++)
    {
        sample_0 = adc_sample(PARKING_SPOT_0, ADC_RES);
        sample_1 = adc_sample(PARKING_SPOT_1, ADC_RES);
        sample_2 = adc_sample(PARKING_SPOT_2, ADC_RES);
        sample_3 = adc_sample(PARKING_SPOT_3, ADC_RES);
        xtimer_usleep(1000000);
    }
    free_spot_value_0 = sample_0;
    free_spot_value_1 = sample_1;
    free_spot_value_2 = sample_2;
    free_spot_value_3 = sample_3;

    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 0, 15, "Calibrating sensors...");
    u8g2_SendBuffer(&u8g2);

    puts("Calibrating sensors...");

    for (int i = 0; i < 25; i++)
    {
        free_spot_value_0 = ((free_spot_value_0 + adc_sample(PARKING_SPOT_0, ADC_RES)) / 2);
        free_spot_value_1 = ((free_spot_value_1 + adc_sample(PARKING_SPOT_1, ADC_RES)) / 2);
        free_spot_value_2 = ((free_spot_value_2 + adc_sample(PARKING_SPOT_2, ADC_RES)) / 2);
        free_spot_value_3 = ((free_spot_value_3 + adc_sample(PARKING_SPOT_3, ADC_RES)) / 2);

        xtimer_usleep(1000000);
    }
    printf("%d\n", free_spot_value_2);
}

int initDisplay(void)
{
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_hw_i2c_riotos, u8x8_gpio_and_delay_riotos);
    u8g2_SetUserPtr(&u8g2, &user_data);
    u8g2_SetI2CAddress(&u8g2, SSD1306_I2C_ADDR);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
    return 0;
}

int init_mqtt(void)
{
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 0, 15, "Initializing MQTT...");
    u8g2_SendBuffer(&u8g2);

    xtimer_sleep(5);

    NetworkInit(&network);
    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, buf, BUF_SIZE, readbuf, BUF_SIZE);
    MQTTStartTask(&client);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = MQTT_VERSION_v311;
    data.clientID.cstring = "";
    data.username.cstring = "";
    data.password.cstring = "";
    data.keepAliveInterval = 60;
    data.cleansession = 1;

    printf("MQTT: Connecting to MQTT Broker from %s %d\n",
           BROKER_ADDRESS, DEFAULT_MQTT_PORT);
    printf("MQTT: Trying to connect to %s, port: %d\n",
           BROKER_ADDRESS, DEFAULT_MQTT_PORT);

    int res = NetworkConnect(&network, BROKER_ADDRESS, DEFAULT_MQTT_PORT);

    if (res)
    {
        printf("MQTT unable to connect: Error %d\n", res);
        return res;
    }
    res = MQTTConnect(&client, &data);

    if (res < 0)
    {
        printf("MQTT: Unable to connect client %d\n", res);
        int res = MQTTDisconnect(&client);
        if (res < 0)
        {
            printf("MQTT: Unable to disconnect\n");
        }
        else
        {
            printf("MQTT: Disconnect successful\n");
        }
        NetworkDisconnect(&network);
        return res;
    }
    else
    {
        printf("MQTT: Connection success!\n");
    }

    printf("MQTT client succesfully connected to the broker\n");
    return 0;
}

int publish(char *msg)
{
    // MQTT publish
    MQTTMessage message;
    message.qos = QOS1;
    message.retained = IS_RETAINED_MSG;
    message.payload = msg;
    message.payloadlen = strlen(message.payload);

    int rp = MQTTPublish(&client, TOPIC, &message);
    if (rp)
    {
        printf("MQTT error %d: unable to publish!\n", rp);
        return 1;
    }
    else
    {
        printf("MQTT message published succesfully to topic %s\n", TOPIC);
    }
    return 0;
}

int main(void)
{

    xtimer_ticks32_t last = xtimer_now();

    int sample_0 = 0;
    int sample_1 = 0;
    int sample_2 = 0;
    int sample_3 = 0;

    bool spot0 = false;
    bool spot1 = false;
    bool spot2 = false;
    bool spot3 = false;

    int totalCarNumbers = 0;
    int parkedCars = 0;

    char json[200];

    initDisplay();
    initADC();
    initLEDs();
    init_mqtt();
    initSensorValues();
    drawPark();

    while (1)
    {

        sample_0 = adc_sample(PARKING_SPOT_0, ADC_RES);

        sample_1 = adc_sample(PARKING_SPOT_1, ADC_RES);

        sample_2 = adc_sample(PARKING_SPOT_2, ADC_RES);

        sample_3 = adc_sample(PARKING_SPOT_3, ADC_RES);

        printf("0: %d\n", free_spot_value_0 - sample_0);
        printf("1: %d\n", free_spot_value_1 - sample_1);
        printf("2: %d\n", free_spot_value_2 - sample_2);
        printf("3: %d\n", free_spot_value_3 - sample_3);

        if (free_spot_value_0 - sample_0 > 10)

        {
            printf("Spot 0 occupied.\n");
            spotUpdate(true, 0, SPOT_0);
            if (!spot0)
            {
                spot0 = true;
                totalCarNumbers++;
            }
        }
        else
        {
            spotUpdate(false, 0, SPOT_0);
            spot0 = false;
        }
        if (free_spot_value_1 - sample_1 > 10)

        {
            printf("Spot 1 occupied.\n");
            spotUpdate(true, 1, SPOT_1);
            if (!spot1)
            {
                spot1 = true;
                totalCarNumbers++;
            }
        }
        else
        {
            spotUpdate(false, 1, SPOT_1);
            spot1 = false;
        }

        if (free_spot_value_2 - sample_2 > 10)
        {
            printf("Spot 2 occupied.%d     second   %d\n", free_spot_value_2, sample_2);
            spotUpdate(true, 2, SPOT_2);
            if (!spot2)
            {
                spot2 = true;
                totalCarNumbers++;
            }
        }
        else
        {
            spotUpdate(false, 2, SPOT_2);
            spot2 = false;
        }

        if (free_spot_value_3 - sample_3 > 10)
        {
            printf("Spot 3 occupied.\n");
            spotUpdate(true, 3, SPOT_3);
            if (!spot3)
            {
                spot3 = true;
                totalCarNumbers++;
            }
        }
        else
        {
            spotUpdate(false, 3, SPOT_3);
            spot3 = false;
        }

        parkedCars = spot0 + spot1 + spot2 + spot3;
        sprintf(json, "{\"spot0\": \"%d\", \"spot1\": \"%d\", \"spot2\": \"%d\", \"spot3\": \"%d\", \"totalCarNumbers\": \"%d\", \"parkedCars\": \"%d\"}",
                spot0, spot1, spot2, spot3, totalCarNumbers, parkedCars);

        char *msg = json;
        if (publish(msg))
            printf("Error publishing on topic: %s", TOPIC);

        xtimer_periodic_wakeup(&last, DELAY);
    }

    return 0;
}
