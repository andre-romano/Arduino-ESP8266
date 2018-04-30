#include "ESP8266.h"

ESP8266::ESP8266(int tx, int rx) : wifi(tx, rx), prev_time(millis()) { } 

void ESP8266::begin(){    
    // start the ESP8266 connection
    wifi.begin(115200);
    // configure baud rate to 9600
    wdt_reset(); //reset watchdog
    wifi.println(STR_F("AT+CIOBAUD=9600"));  
    delay(1000);
    wdt_reset(); //reset watchdog
    wifi.end();
    // initiate 9600 ESP8266 connection
    wifi.begin(9600);
    wifi.setTimeout(SERIAL_TIMEOUT);

    close_conns();
    // // disable echo
    // add(STR_F("ATE0"));  
}

void ESP8266::connect(String ssid, String pass){        
    // set Wifi Mode to ST
    add(STR_F("AT+CWMODE=1"));      
    // connect to wifi        
    add(STR_F("AT+CWJAP=\"") + ssid + STR_F("\",\"") + pass + STR_F("\""));              
}

void ESP8266::start_server(int port, int timeout){    
    // allow multiple connections
    add(STR_F("AT+CIPMUX=1"));     
    // initiate TCP server
    add(STR_F("AT+CIPSERVER=1,") + port);     
    // set server timeout
    add(STR_F("AT+CIPSTO=") + timeout);           
}

void ESP8266::stop_server(){
    // CLOSE SERVER
    add(STR_F("AT+CIPSERVER=0"));
}

void ESP8266::close_conns(){
    // CLOSE ALL PREVIOUSLY ESTABLISHED CONECTIONS
    stop_server();
    add(STR_F("AT+CIPCLOSE=5"));
}

void ESP8266::execute(){
    if ( elapsedTime() && commands.length() != 0 ){
        int pos = commands.indexOf(STR_F(COMMAND_DELIMITER));        
        wifi.println(commands.substring(0, pos));
        commands.remove(0, pos+1);        
        prev_time = millis();
    }
}

// LEGACY COMMANDS

int ESP8266::available(){
    return wifi.available();
}

char ESP8266::read(){    
    return wifi.read();
}

// execute serial AT command
void ESP8266::write(char c){  
    wifi.write(c);
}

//   -----------
//     PRIVATE
//   -----------

bool ESP8266::elapsedTime(){
  return ((unsigned long)(millis() - prev_time) >= DELAY_EXEC);
}

// add serial AT command
void ESP8266::add(String s){         
    commands += s + STR_F(COMMAND_DELIMITER);
}

//void wifi_send(String s){
//  wifi_write(STR_F("AT+CIPSTART=\"TCP\",\"") + STR_F(SERVER_IP) + STR_F("\",\"") + STR_F(SERVER_PORT) + STR_F("\",0"));
//  wifi_write(STR_F("AT+CIPSEND=") + s.length());
//  wifi_write(s);
//  wifi_write(STR_F("AT+CIPCLOSE"));
//}