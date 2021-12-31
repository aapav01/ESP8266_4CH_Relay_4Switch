/* 
 * Copyright (C) 2021-2022 Apavayan Sinha
 * 
 * The Application create a link to Blynk cloud server to controller
 * 4 relay attached to the Hardware (ESP8266) with 4 switch for Manual
 * Control. The chip should connect to a Wi-Fi network and allow to us
 * to controll the appliances connected to the IoT Extension Borad.
 *
 * The Application is based on Blynk.Edgent.Edgent_ESP8266 example code
 * Provide in the Blynk Libary. It is used to have full features access
 * like OTA from Blynk Dashboard for Future Updatation of the product.
 * 
 * Author: Apavayan Sinha (info@apavayan.com)
 */

// Blynk Cloud Setup
#define BLYNK_TEMPLATE_ID "TMPL34npI7-p"
#define BLYNK_DEVICE_NAME "IoT Extension"
#define BLYNK_AUTH_TOKEN "I_X9wjUj-nOaqg7_MJYXgn6MOn4KDCU8"

#define BLYNK_FIRMWARE_VERSION "0.1.1" // Application Version

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG // Debug

//#define APP_DEBUG
#define USE_NODE_MCU_BOARD // Chip Name

#include "BlynkEdgent.h"

/*
 * GPIO PINs 
 * 
 * Which will used to to controll the relay and Switch
 */
#define RELAY_OUTPUT_1 4  // D2
#define RELAY_OUTPUT_2 5  // D1
#define RELAY_OUTPUT_3 12 // D6
#define RELAY_OUTPUT_4 15 // D8

#define SWITCH_INPUT_1 0  // D3
#define SWITCH_INPUT_2 9  // S2
#define SWITCH_INPUT_3 10 // S3
#define SWITCH_INPUT_4 14 // D5

#define WIFI_LED 13

/*
 * Virtual PINs 
 * 
 * Which will used to to controll the State of relay from the
 * Application.
 */
#define VPIN_BUTTON_1 V1
#define VPIN_BUTTON_2 V2
#define VPIN_BUTTON_3 V3
#define VPIN_BUTTON_4 V4

/*
 * Relay State
 * 
 * Define V0 (LOW) To Set Default Toggle State for Relay.
 * 
 * 0 (LOW) Means 0 Volt
 * 1 (HIGH) Means 5/3.3 Volt
 */
bool ToggleState_1 = 0;
bool ToggleState_2 = 0;
bool ToggleState_3 = 0;
bool ToggleState_4 = 0;


void setup()
{
    Serial.begin(9600); // Our BORAD is spec for Baud for 9600
//    Serial.begin(115200); // Our BORAD is spec for Baud for 115200
    // Will Wait for Boot as few I/0 pin takes 100ms to boot
    // so we will wait 150ms just to be safe.
    delay(150);
    
    // Set ALl Switchs as Input
    pinMode(SWITCH_INPUT_1, INPUT_PULLUP);
    pinMode(SWITCH_INPUT_2, INPUT_PULLUP);
    pinMode(SWITCH_INPUT_3, INPUT_PULLUP);
    pinMode(SWITCH_INPUT_4, INPUT_PULLUP);

    // Set ALl Relay as Output
    pinMode(RELAY_OUTPUT_1, OUTPUT);
    pinMode(RELAY_OUTPUT_2, OUTPUT);
    pinMode(RELAY_OUTPUT_3, OUTPUT);
    pinMode(RELAY_OUTPUT_4, OUTPUT);

    // Set Wi-Fi LED
//    pinMode(WIFI_LED, OUTPUT);

    //Initialize all pin to LOW (0/OFF)
    digitalWrite(RELAY_OUTPUT_1, LOW);
    digitalWrite(RELAY_OUTPUT_2, LOW);
    digitalWrite(RELAY_OUTPUT_3, LOW);
    digitalWrite(RELAY_OUTPUT_4, LOW);

    BlynkEdgent.begin();
}

void loop()
{
    BlynkEdgent.run();

//    ToggleState_1 = state_management(SWITCH_INPUT_1, RELAY_OUTPUT_1, VPIN_BUTTON_1);
//    ToggleState_2 = state_management(SWITCH_INPUT_2, RELAY_OUTPUT_2, VPIN_BUTTON_2);
//    ToggleState_3 = state_management(SWITCH_INPUT_3, RELAY_OUTPUT_3, VPIN_BUTTON_3);
//    ToggleState_4 = state_management(SWITCH_INPUT_4, RELAY_OUTPUT_4, VPIN_BUTTON_4);

}

bool state_management(int inputPin, int outputPin, int virtualPin)
{
  boolean state = LOW;
  if(digitalRead(inputPin) == HIGH)
  {
    digitalWrite(outputPin, HIGH);
    state = HIGH;
    Blynk.virtualWrite(virtualPin, state);
  }
  else {
    digitalWrite(outputPin, LOW);
    state = LOW;
    Blynk.virtualWrite(virtualPin, state);
  }

  delay(300);
  return state;
}

/*
 * Control relay using Blynk Virtual Pins
 * 
 * This special function is called BLYNK_WRITE. Think of it
 * as meaning that the Blynk.Cloud is telling your hardware 
 * “there a new value written to your virtual pin”.
 */

BLYNK_WRITE(VPIN_BUTTON_1)
{
    digitalWrite(RELAY_OUTPUT_1, param.asInt());
    ToggleState_1 = param.asInt();
    Serial.print("Virtual Pin 1 changed");
    Serial.println(param.asInt());
}

BLYNK_WRITE(VPIN_BUTTON_2)
{
    digitalWrite(RELAY_OUTPUT_2, param.asInt());
    ToggleState_2 = param.asInt();
    Serial.print("Virtual Pin 2 changed");
    Serial.println(param.asInt());
}

BLYNK_WRITE(VPIN_BUTTON_3)
{
    digitalWrite(RELAY_OUTPUT_3, param.asInt());
    ToggleState_3 = param.asInt();
    Serial.print("Virtual Pin 3 changed to ");
    Serial.println(param.asInt());
}

BLYNK_WRITE(VPIN_BUTTON_4)
{
    digitalWrite(RELAY_OUTPUT_4, param.asInt());
    ToggleState_4 = param.asInt();
    Serial.print("Virtual Pin 4 changed to ");
    Serial.println(param.asInt());
}

/*
 * Request the latest state from Cloud
 * 
 * Requests all latest stored values for all widgets. All 
 * analog/digital/virtual pin values and states will be
 * set to the latest stored value. Every virtual pin will
 * generate BLYNK_WRITE() event.
 */
BLYNK_CONNECTED() {
    Blynk.syncVirtual(VPIN_BUTTON_1, VPIN_BUTTON_2, VPIN_BUTTON_3, VPIN_BUTTON_4);
//    Blynk.syncAll();
}
