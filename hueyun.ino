/**
 * Activate Hue Light with a PIR.
 */

#include <Bridge.h>
#include <Process.h>

#define DEBUG 1
#ifdef DEBUG
  #include <Console.h>
  #define SLOG(...) slog(__VA_ARGS__)
#else
  #define SLOG(...)
#endif

/**
 * HW related
 */
const int readPin = 7;

/**
 * SW related
 */
const unsigned long TIME_ON = 60000;

/**
 * API related 
 */
const int light_id = 3;
const String cmd = "curl -X PUT -d ";
const String p_off = "'{\"on\":false}' ";
const String p_on = "'{\"on\":true,\"sat\":1,\"bri\":254,\"hue\":10000}' ";
const String ip = "<IP>";
const String KEY = "<KEY>";
const String api = "http://" + ip + "/api/" + KEY + "/";
const String endpoint = "lights/" + String(light_id) + "/state";
const String API_ON = cmd + p_on + api + endpoint;
const String API_OFF = cmd + p_off + api + endpoint;

/**
 * vars
 */
bool f = false; // light state flag
unsigned long tl = 0;

void slog(String msg) {
  /**
   * Logger
   */
  #ifdef DEBUG
  Console.print(msg + "\n");
  Console.flush();
  #endif
}

void run_cmd(String c) {
  Process p;
  p.runShellCommand(c);
  while(p.running());
}

void light_on() {
  /**
   * Turn Hue lights on
   */
  run_cmd(API_ON);
  SLOG(API_ON + "\n" + "Lights ON");
}

void light_off(){
  /**
   * Turn Hue lights off
   */
  run_cmd(API_OFF);
  SLOG(API_OFF + "\n" + "Lights OFF");
}

void setup() {
  pinMode(readPin, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Bridge.begin();
  #ifdef DEBUG
  Console.begin();
  while (!Console);
  #endif
  digitalWrite(13, LOW);
}

void loop() {
  int m = digitalRead(readPin);
  if(m == 1 && f == false) {
    light_on();
    f = true;
    tl = millis();
  } else if (f == true && millis() - tl >= TIME_ON) {
    light_off();
    f = false;
  }
}
