#ifdef ENABLE_DEBUG
  #define DEBUG_ESP_PORT Serial
  #define NODEBUG_WEBSOCKETS
  #define NDEBUG
#endif

#include <Arduino.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>
#endif

#ifdef ESP32
  #include <WiFi.h>
#endif

#include "SinricPro.h"
#include "SinricProContactsensor.h"

/**
  * WIFI CONFIG
  */
#define WIFI_SSID "YOUR_WIFI_SSID_HERE"
#define WIFI_PASS "YOUR_WIFI_PASSWORD_HERE"

/**
  * SINRIC CONFIG
  */
#define APP_KEY "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx" // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx" // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
#define CONTACT_ID "5dc1564130xxxxxxxxxxxxxx" // Should look like "5dc1564130xxxxxxxxxxxxxx"

/**
  * BAUDRATE CONFIG
  */
#define BAUD_RATE 9600

/**
  * PINS CONFIG
  */
#define FLOATER_PIN 4
#define BEEP_PIN 22
#define EMIT_BEEP true // Change to false to disable beep

/**
  * GLOBAL VARIABLES CONFIG
  */
bool WIFI_IS_CONNECTED = false;
bool MY_POWER_STATE = true;
bool LAST_CONTACT_STATE = false;
unsigned long LAST_MILISECONDS_CHANGE = 0;

void setup() {
  pinMode(FLOATER_PIN, INPUT);
  pinMode(BEEP_PIN, OUTPUT);

  Serial.begin(BAUD_RATE);
  Serial.printf("\r\n\r\n");

  WIFI_IS_CONNECTED = connectWifi(); // Initialise wifi connection

  if (WIFI_IS_CONNECTED) {
    Serial.println("Wifi connected");
    setupSinricPro();
  } else {
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
}
void loop() {
  SinricPro.handle();

  if (SinricPro.isConnected()){
    handleContactSensor();
    if(reservoir_is_full() && EMIT_BEEP) beep(500);
  }else if(!SinricPro.isConnected() && reservoir_is_full() && EMIT_BEEP){
    beep(500);
  }
}

/**
 * @brief Checks contactsensor connected to CONTACT_PIN
 *
 * If contactsensor state has changed, send event to SinricPro Server
 * state from reservoir_is_full():
 *      true = contactsensor is closed
 *      false  = contactsensor is open
 */
void handleContactSensor() {
  unsigned long actualMillis = millis();
  if (actualMillis - LAST_MILISECONDS_CHANGE < 250) return; // debounce contact state transitions (same as debouncing a pushbutton)

  bool actualContactState = reservoir_is_full(); // read actual state of contactsensor

  if (actualContactState != LAST_CONTACT_STATE) { // if state has changed
    Serial.printf("Contactsensor is %s now\r\n", actualContactState?"closed":"open");
    LAST_CONTACT_STATE = actualContactState; // update last known state
    LAST_MILISECONDS_CHANGE = actualMillis; // update debounce time
    SinricProContactsensor &myContact = SinricPro[CONTACT_ID]; // get contact sensor device
    myContact.sendContactEvent(actualContactState); // send event with actual state
  }
}

/**
 * @brief Callback for setPowerState request
 *
 * @param deviceId      String containing deviceId (useful if this callback used by multiple devices)
 * @param[in] state     bool true=turn on device / false=turn off device
 * @param[out] state    bool true=device turned on / false=device turned off
 * @return true         request handled properly
 * @return false        request can't be handled because some kind of error happened
 */
bool onPowerState(const String& deviceId, bool& state) {
  Serial.printf("Device %s turned %s (via SinricPro) \r\n", deviceId.c_str(), reservoir_is_full() ? "on" : "off");
  MY_POWER_STATE = reservoir_is_full();
  return true; // request handled properly
}

// setup function for SinricPro
void setupSinricPro() {
  // add device to SinricPro
  SinricProContactsensor& myContact = SinricPro[CONTACT_ID];

  // set callback function to device
  myContact.onPowerState(onPowerState);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); });
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// connect to wifi – returns true if successful or false if not
bool connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false;
      break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Connection failed.");
  }
  return state;
}

// reservoir is full – returns true if reservoil is full or false if not
bool reservoir_is_full() {
  return touchRead(FLOATER_PIN) > 0 ? false : true;
}

// Emit beep
void beep(int delay_in_microseconds) {
  for (int i = 0; i < 1000; i++) {
    digitalWrite(BEEP_PIN, HIGH);
    delayMicroseconds(delay_in_microseconds);
    digitalWrite(BEEP_PIN, LOW);
    delayMicroseconds(delay_in_microseconds);
  }
}