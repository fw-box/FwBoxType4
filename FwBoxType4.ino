//
// Copyright (c) 2020 Fw-Box (https://fw-box.com)
// Author: Hartman Hsieh
//
// Description :
//   None
//
// Connections :
//
// Required Library :
//

#include "FwBox.h"

#define DEVICE_TYPE 4
#define FIRMWARE_VERSION "1.1.8"

//
// Debug definitions
//
#define FW_BOX_DEBUG 1

#if FW_BOX_DEBUG == 1
  #define DBG_PRINT(VAL) Serial.print(VAL)
  #define DBG_PRINTLN(VAL) Serial.println(VAL)
  #define DBG_PRINTF(FORMAT, ARG) Serial.printf(FORMAT, ARG)
#else
  #define DBG_PRINT(VAL)
  #define DBG_PRINTLN(VAL)
  #define DBG_PRINTF(FORMAT, ARG)
#endif

#define PIN_LED 2
#define PIN_RELAY 12 // D6

#define VAL_INDEX_SOIL_MOISTURE 0
#define VAL_INDEX_RELAY 1

//
// Global variable
//
unsigned long ReadingTime = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_LED, HIGH); // Turn off
  digitalWrite(PIN_RELAY, LOW); // Turn off

  //
  // Initialize the fw-box core
  //
  fbBegin(DEVICE_TYPE, FIRMWARE_VERSION);

  //
  // Setup MQTT subscribe callback
  //
  setRcvValueCallback(onReceiveValue);

} // void setup()

void loop()
{
  if((millis() - ReadingTime) > 2000) {
    DBG_PRINT("Device UUID is ");
    DBG_PRINTLN(FwBoxIns.getDeviceConfig()->Uuid);
    DBG_PRINT("Device Type is ");
    DBG_PRINTLN(FwBoxIns.getDeviceConfig()->Type);

    int a_val = analogRead(A0);
    DBG_PRINT("Analog value is ");
    DBG_PRINTLN(a_val);

    //
    // Update the value 0
    //
    FwBoxIns.setValue(VAL_INDEX_SOIL_MOISTURE, a_val);
    ReadingTime = millis();
  }

  //
  // Run the handle
  //
  fbHandle();

} // END OF "void loop()"


void onReceiveValue(int valueIndex, String* payload)
{
  DBG_PRINT("onReceiveValue valueIndex = ");
  DBG_PRINTLN(valueIndex);
  DBG_PRINT("onReceiveValue *payload = ");
  DBG_PRINTLN(*payload);

  if(valueIndex == VAL_INDEX_RELAY) {
    payload->toUpperCase();
    if(payload->equals("ON") == true)
    {
      digitalWrite(PIN_RELAY, HIGH);

      //
      // Sync the status to LED
      //
      digitalWrite(PIN_LED, LOW); // This value is reverse
    }
    else
    {
      digitalWrite(PIN_RELAY, LOW);

      //
      // Sync the status to LED
      //
      digitalWrite(PIN_LED, HIGH); // This value is reverse
    }
  }
}
