#include <SPI.h>
#include <ESP8266WiFi.h>
#include "RF24.h"

String apiKey = "58SOCIWI57LXLSIX";

const char *ssid =  "Chisimdiri";     // replace with your wifi ssid and wpa2 key
const char *pass =  "mount1951";
const char* server = "api.thingspeak.com";

WiFiClient client;

RF24 myRadio (2, 4);
byte addrs[][6] = {"T"}; // Data pipe
char receivedData[1000];
String temp = "";
String humidity = "";
String lum = "";
String moisture = "";

void collectData(String s) {
  temp = "";
  humidity = "";
  lum = "";
  moisture = "";
  int count = 0;
  String data = "";
  for (int i = 0; i < s.length(); i++) {
    if (s.charAt(i) != ' ') {
      data = data + s[i];
    }
    if (s.charAt(i) == ' ') {
      if (data != "") {
        switch (count) {
          case 0:
            moisture = data;
            count += 1;
            break;
          case 1:
            temp = data;
            count += 1;
            break;
          case 2:
            humidity = data;
            count += 1;
            break;
        }
        data = "";
      }
    }
  }
  lum = data;
}

void setup()
{
  Serial.begin(115200); // Start Serial Monitor
  delay(10);
  // Set up the radio
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  myRadio.begin();                       // Start radio
  myRadio.setChannel(100);
  myRadio.setPALevel(RF24_PA_MIN);       // radios are close together
  myRadio.setDataRate( RF24_250KBPS );
  myRadio.openReadingPipe(1, addrs[0]);  // Read from pipe called "T"
  myRadio.startListening();              // Listen for signal from Boss
}

void loop()
{
  if (myRadio.available())
  {
    while (myRadio.available())
    {
      myRadio.read(&receivedData, sizeof(receivedData));
    }
    // Print package contents to Monitor for checking
    Serial.println("Moisure  Temp   Humidity  Luminosity");
    String s = "";

    for (int i = 0; i < sizeof(receivedData); i++) {
      s = s + receivedData[i];
    }
    Serial.println(s);

    collectData(s);
    if (temp != "") {
      if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
      {

        String postStr = apiKey;
        postStr += "&field1=";
        postStr += String(moisture);
        postStr += "&field2=";
        postStr += String(temp);
        postStr += "&field3=";
        postStr += String(humidity);
        postStr += "&field4=";
        postStr += String(lum);
        postStr += "\r\n\r\n";

        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);
        Serial.println(moisture);
        Serial.println(temp);
        Serial.println(humidity);
        Serial.println(lum);

        Serial.println("%. Send to Thingspeak.");
      }
      client.stop();
    }
    delay(3000);
  }

}
