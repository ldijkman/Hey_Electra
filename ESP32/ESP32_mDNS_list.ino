
/*
https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS/examples/mDNS-SD_Extended/mDNS-SD_Extended.ino
mdns service discovery browser list .local on serial monitor 115200
  ESP8266 mDNS-SD responder and query sample
  This is an example of announcing and finding services.
  
  Instructions:
  - Update WiFi SSID and password as necessary.
  - Flash the sketch to two ESP8266 boards  == tested on wemos uno sized esp32
  - The last one powered on should now find the other.
  
  - Install host software: (so that browser can find http://*.local URL's
    - my raspberry pi 400 chromium works with .local    
    - For Linux, install Avahi (http://avahi.org/).
    - For Windows, install Bonjour (http://www.apple.com/support/bonjour/).
    - For Mac OSX and iOS support is built in through Bonjour already.
    - android phone app bonjourbrowser also lists http://*.local devices in network
                    https://github.com/wellenvogel/BonjourBrowser
                    https://play.google.com/store/apps/details?id=de.wellenvogel.bonjourbrowser&hl=gsw
  
  my serial monitor result
  
Browsing for service _http._tcp.local. ... 3 service(s) found
  1: art (10.10.100.100:80)
  2: kitchen (10.10.100.102:80)
  3: Living (10.10.100.103:80)

that makes
http://art.local
http://kitchen.local
http://Living.local


esp8266 webserver webpage at http://Living.local lists also this device http://electra.local
Local HTTP services are :
electra.local
10.10.100.106

art.local
10.10.100.100

kitchen.local
10.10.100.102
 */

#include <WiFi.h>
#include <ESPmDNS.h>

const char* ssid     = "Bangert_30_Andijk";     // wifi router name broadcasted in the air
const char* password = "ookikwilerin";          // wifi router password

const char* mdnsdotlocalurl = "electra";    // becomes http://electra.local give each device a unique name
// const char* mdnsdotlocalurl = "living";  // becomes http://living.local give each device a unique name
// const char* mdnsdotlocalurl = "kitchen"; // becomes http://kitchen.local give each device a unique name
// const char* mdnsdotlocalurl = "garage";  // becomes http://garage.local give each device a unique name

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (!MDNS.begin(mdnsdotlocalurl)) {
        Serial.println("Error setting up MDNS responder!");
        while(1){
            delay(1000);
        }
    }

 MDNS.addService("http", "tcp", 80);
    
}

void loop() {
    browseService("http", "tcp");
    delay(1000);
    //browseService("arduino", "tcp");
    //delay(1000);
    //browseService("workstation", "tcp");
    //delay(1000);
    //browseService("smb", "tcp");
    //delay(1000);
    //browseService("afpovertcp", "tcp");
    //delay(1000);
    //browseService("ftp", "tcp");
   // delay(1000);
    //browseService("ipp", "tcp");
    //delay(1000);
    //browseService("printer", "tcp");
   // delay(10000);
}

void browseService(const char * service, const char * proto){
    Serial.printf("Browsing for service _%s._%s.local. ... ", service, proto);
    int n = MDNS.queryService(service, proto);
    if (n == 0) {
        Serial.println("no services found");
    } else {
        Serial.print(n);
        Serial.println(" service(s) found");
        for (int i = 0; i < n; ++i) {
            // Print details for each service found
            Serial.print("  ");
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(MDNS.hostname(i));
            Serial.print(" (");
            Serial.print(MDNS.IP(i));
            Serial.print(":");
            Serial.print(MDNS.port(i));
            Serial.println(")");
        }
    }
    Serial.println();
}
