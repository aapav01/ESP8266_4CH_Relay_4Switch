/*
   Copyright (C) 2021-2022 Apavayan Sinha

   The Application create a link to Blynk cloud server to controller
   4 relay attached to the Hardware (ESP8266) with 4 switch for Manual
   Control. The chip should connect to a Wi-Fi network and allow to us
   to controll the appliances connected to the IoT Extension Borad.

   The Application is based on Blynk.Edgent.Edgent_ESP8266 example code
   Provide in the Blynk Libary. It is used to have full features access
   like OTA from Blynk Dashboard for Future Updatation of the product.

   Author: Apavayan Sinha (info@apavayan.com)
*/

// Blynk Cloud Setup
#define BLYNK_TEMPLATE_ID "TMPL34npI7-p"
#define BLYNK_DEVICE_NAME "IoT Extension"
#define BLYNK_AUTH_TOKEN "I_X9wjUj-nOaqg7_MJYXgn6MOn4KDCU8"

#define BLYNK_FIRMWARE_VERSION "0.1.3" // Application Version

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG // Debug

//#define APP_DEBUG
#define USE_NODE_MCU_BOARD // Chip Name

#include "BlynkEdgent.h"

/*
   GPIO PINs

   Which will used to to controll the relay and Switch
*/
#define RELAY_OUTPUT_1 4  // D2
#define RELAY_OUTPUT_2 5  // D1
#define RELAY_OUTPUT_3 12 // D6
#define RELAY_OUTPUT_4 14 // D5

#define SWITCH_INPUT_1 13 // D7
#define SWITCH_INPUT_2 D3 // D3
#define SWITCH_INPUT_3 3  // RX
#define SWITCH_INPUT_4 10 // D0

/*
   Virtual PINs

   Which will used to to controll the State of relay from the
   Application.
*/
#define VPIN_BUTTON_1 V1
#define VPIN_BUTTON_2 V2
#define VPIN_BUTTON_3 V3
#define VPIN_BUTTON_4 V4

/*
   Relay State

   Define V0 (LOW) To Set Default Toggle State for Relay.

   0 (LOW) Means 0 Volt
   1 (HIGH) Means 5/3.3 Volt
*/
bool ToggleState_1 = 0;
bool ToggleState_2 = 0;
bool ToggleState_3 = 0;
bool ToggleState_4 = 0;

bool prev_state_switch_1 = 0;
bool prev_state_switch_2 = 0;
bool prev_state_switch_3 = 0;
bool prev_state_switch_4 = 0;


void setup()
{
  Serial.begin(115200); // Our BORAD is spec for Baud for 115200


  // Will Wait for Boot as few I/0 pin takes 100ms to boot
  // so we will wait 500ms just to be safe.
  delay(500);

  // Set ALl Switchs as Input
#ifdef SWITCH_INPUT_1
  pinMode(SWITCH_INPUT_1, INPUT_PULLUP);
#endif
#ifdef SWITCH_INPUT_2
  pinMode(SWITCH_INPUT_2, INPUT_PULLUP);
#endif
#ifdef SWITCH_INPUT_3
  pinMode(SWITCH_INPUT_3, INPUT_PULLUP);
#endif
#ifdef SWITCH_INPUT_4
  pinMode(SWITCH_INPUT_4, INPUT_PULLUP);
#endif

  // Set ALl Relay as Output
  pinMode(RELAY_OUTPUT_1, OUTPUT);
  pinMode(RELAY_OUTPUT_2, OUTPUT);
  pinMode(RELAY_OUTPUT_3, OUTPUT);
  pinMode(RELAY_OUTPUT_4, OUTPUT);

  //Initialize all pin to LOW (0/OFF)
  digitalWrite(RELAY_OUTPUT_1, HIGH);
  digitalWrite(RELAY_OUTPUT_2, HIGH);
  digitalWrite(RELAY_OUTPUT_3, HIGH);
  digitalWrite(RELAY_OUTPUT_4, HIGH);

  BlynkEdgent.begin();
}

void loop()
{
  BlynkEdgent.run();
#ifdef SWITCH_INPUT_1
  if (digitalRead(SWITCH_INPUT_1) == LOW) {
    digitalWrite(RELAY_OUTPUT_1, LOW);
    ToggleState_1 = LOW;
    prev_state_switch_1 = 1;
    Blynk.virtualWrite(VPIN_BUTTON_1, 1);
  }
#endif
#ifdef SWITCH_INPUT_2
  if (digitalRead(SWITCH_INPUT_2) == LOW) {
    digitalWrite(RELAY_OUTPUT_2, LOW);
    ToggleState_2 = LOW;
    prev_state_switch_2 = 1;
    Blynk.virtualWrite(VPIN_BUTTON_2, 1);
  }
#endif
#ifdef SWITCH_INPUT_3
  if (digitalRead(SWITCH_INPUT_3) == LOW) {
    digitalWrite(RELAY_OUTPUT_3, LOW);
    ToggleState_3 = LOW;
    prev_state_switch_3 = 1;
    Blynk.virtualWrite(VPIN_BUTTON_3, 1);
  }
#endif
#ifdef SWITCH_INPUT_4
  if (digitalRead(SWITCH_INPUT_4) == LOW) {
    digitalWrite(RELAY_OUTPUT_4, LOW);
    ToggleState_4 = LOW;
    prev_state_switch_4 = 1;
    Blynk.virtualWrite(VPIN_BUTTON_4, 1);
  }
#endif
  delay(150);
#ifdef SWITCH_INPUT_1
  if (prev_state_switch_1 == 1 && digitalRead(SWITCH_INPUT_1) == HIGH) {
    digitalWrite(RELAY_OUTPUT_1, HIGH);
    ToggleState_1 = HIGH;
    prev_state_switch_1 = 0;
    Blynk.virtualWrite(VPIN_BUTTON_1, 0);
  }
#endif
#ifdef SWITCH_INPUT_2
  if (prev_state_switch_2 == 1 && digitalRead(SWITCH_INPUT_2) == HIGH) {
    digitalWrite(RELAY_OUTPUT_2, HIGH);
    ToggleState_2 = HIGH;
    prev_state_switch_2 = 0;
    Blynk.virtualWrite(VPIN_BUTTON_2, 0);
  }
#endif
#ifdef SWITCH_INPUT_3
  if (prev_state_switch_3 == 1 && digitalRead(SWITCH_INPUT_3) == HIGH) {
    digitalWrite(RELAY_OUTPUT_3, HIGH);
    ToggleState_3 = HIGH;
    prev_state_switch_3 = 0;
    Blynk.virtualWrite(VPIN_BUTTON_3, 0);
  }
#endif
#ifdef SWITCH_INPUT_4
  if (prev_state_switch_4 == 1 && digitalRead(SWITCH_INPUT_4) == HIGH) {
    digitalWrite(RELAY_OUTPUT_4, HIGH);
    ToggleState_4 = HIGH;
    prev_state_switch_4 = 0;
    Blynk.virtualWrite(VPIN_BUTTON_4, 0);
  }
#endif
  delay(150);
}

/*
   Control relay using Blynk Virtual Pins

   This special function is called BLYNK_WRITE. Think of it
   as meaning that the Blynk.Cloud is telling your hardware
   “there a new value written to your virtual pin”.
*/

BLYNK_WRITE(VPIN_BUTTON_1) {
  digitalWrite(RELAY_OUTPUT_1, !param.asInt());
  ToggleState_1 = !param.asInt();
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  digitalWrite(RELAY_OUTPUT_2, !param.asInt());
  ToggleState_2 = !param.asInt();
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  digitalWrite(RELAY_OUTPUT_3, !param.asInt());
  ToggleState_3 = !param.asInt();
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  digitalWrite(RELAY_OUTPUT_4, !param.asInt());
  ToggleState_4 = !param.asInt();
}

/*
   Request the latest state from Cloud

   Requests all latest stored values for all widgets. All
   analog/digital/virtual pin values and states will be
   set to the latest stored value. Every virtual pin will
   generate BLYNK_WRITE() event.
*/
BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_BUTTON_1, VPIN_BUTTON_2, VPIN_BUTTON_3, VPIN_BUTTON_4);
  //    Blynk.syncAll();
}
