/*
  ESP8266.h - Library for ESP8266 ESP-01 WiFi AT Mode
  Created by Andre Madureira, April 8, 2018.
  Released into the public domain.
*/

#ifndef ESP8266_h
#define ESP8266_h

#include <SoftwareSerial.h>
#include "Arduino.h"
#include <avr/wdt.h>  // Watchdog Library (needs optiboot bootloader to work well)

// execution delay of serial AT command
#define DELAY_EXEC        5000
#define SERIAL_TIMEOUT    1500
#define COMMAND_DELIMITER ";"

#define STR_F(X) (String(F(X)))

class ESP8266 {
  public:
    ESP8266(int tx, int rx);    
    void begin();
    void connect(String ssid, String pass);
    void start_server(int port, int timeout);
    void stop_server();
    void close_conns();    
    void execute();
    // LEGACY COMMANDS 
    int  available();
    char read();
    void write(char c);            
  private:
    unsigned long prev_time;
    String commands;
    SoftwareSerial wifi;
    bool elapsedTime();
    void add(String s);
}; 

#endif
