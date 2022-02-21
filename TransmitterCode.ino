#include <SPI.h>  
#include "RF24.h"
#include<dht.h>
dht DHT11;

RF24 myRadio (2, 4);      // These can be changed CE, CSN
byte addrs[][6] = {"T"};  // Data pipe to send on

#define moisturePin A0  
#define ldrPin A1        
#define dht11Pin A2
#define batteryPin A3

double moistureBit, moistureVoltage, moisturePercentage,humidity,temperature,luminosity,lightBit,lightVoltage,batteryBits,batteryVoltage,batteryPercentage;
String farmData,stringMoisture,stringTemperature,stringHumidity,stringLuminosity;
char charFarmData[1000];

void setup()
{
  pinMode(moisturePin, INPUT);
  pinMode(dht11Pin, INPUT);
  pinMode(ldrPin, INPUT);
  Serial.begin(115200);                  // Start Serial Monitor
  delay(1000);
  // Set up the radio
  myRadio.begin();                       // Start radio
  myRadio.setChannel(100);               // Could be changed 1 ... 125
  myRadio.setPALevel(RF24_PA_MIN);       // Power level minimum - radios are close together
  myRadio.setDataRate( RF24_250KBPS );
  myRadio.openWritingPipe( addrs[0]);    // Write to pipe called "T"
  delay(750);
}

void loop() 
{
  measureMoisture();
  measureTempAndHum();
  measureLuminosity();
    
  MoistureData = stringMoisture   
  TempData = stringTemperature
  HumidityData = stringHumidity    
  Luminosity = stringLuminosity;
  unsigned int dataLength = farmData.length()+1; 
  farmData.toCharArray(charFarmData,dataLength);
  
  myRadio.write(&MoistureData, sizeof(charMoistureData)); 
  myRadio.write(&TempData, sizeof(charTempData));
  myRadio.write(&HumidityData, sizeof(charHumidityData));
  myRadio.write(&LuminosityData, sizeof(charLuminosityData));

  // Report sent values to Monitor
  Serial.println(charMoistureData);
  delay(800);
  Serial.println(charTempData);
  delay(800);
  Serial.println(charHumidityData);
  delay(800);
  Serial.println(charLuminosityData);
  delay(800);
} 

double measureMoisture()
{
  moistureBit = analogRead(moisturePin);  
  moistureVoltage = (moistureBit*5)/1023;
  moisturePercentage = (moistureVoltage*100)/5;
  stringMoisture = String(moisturePercentage);
}

double measureTempAndHum()
{
  int chk = DHT11.read11(dht11Pin);
  humidity = DHT11.humidity;
  stringHumidity = String(humidity);
  temperature = DHT11.temperature;
  stringTemperature = String(temperature);
}  

double measureLuminosity()
{
  lightBit = analogRead(ldrPin);
  lightVoltage = (lightBit*5)/1023;
  luminosity = -(500*(lightVoltage-5))/(lightVoltage*10000);
  stringLuminosity = String(luminosity);
}
