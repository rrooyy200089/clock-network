#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
#include <WiFiUdp.h>

// const char *ssid     = "<SSID>";
// const char *password = "<PASSWORD>";

WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);

// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void setup(){
  Serial.begin(115200);
  WiFi.begin("I need this cool thing", "241852051886537614");

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
  timeClient.setTimeOffset(28800);
}

void loop() {
  timeClient.update();

  int h = timeClient.getHours();
  int m = timeClient.getMinutes();
  int s = timeClient.getSeconds();
  char buf[20] = "";
  sprintf(buf, "%2d:%2d:%2d\n", h, m, s);
  Serial.println(buf);

  delay(500);
}
