#include <OneWire.h> // for DS18b20
#include <DallasTemperature.h> //for DS18b20
#include <stdlib.h> // included for floatToString function
#include <DHT.h> // For DHT22
#include <avr/sleep.h>
#include <DS3232RTC.h> 
#include <SPI.h> 


#define interruptPin 2 //Pin we are going to use to wake up the Arduino
#define DHTPIN 7     // pin connected to DHT
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor
#define ONE_WIRE_BUS 5 // DS18b20 pin connect it with 4.7k and vcc - pin 5 ---4.7k---vcc
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
const int time_interval=1;// Sets the wakeup intervall in minutes

const int AirValue = 650;   //you need to replace this value with Value_1
const int WaterValue = 280;  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent=0;
int soil_temp_float; //Stores soil temperature value
int atm_hum;  //Stores humidity value
int atm_temp; //Stores atmospheric temperature value
int soil_mois; //Stores soil moisture value
byte value;
int Atm_temp_theresh = 30;
int Soil_temp_theresh = 30;

// Function for converting float to string
String floatToString(float x, byte precision = 2) {
  char tmp[7];
  dtostrf(x, 0, precision, tmp);
  return String(tmp);
}

bool check = true;

void setup()
{
  Serial.begin(9600);
  pinMode(interruptPin,INPUT_PULLUP);//Set pin d2 to input using the buildin pullup resistor
  dht.begin();//Start the DHT sensor
  RTC.begin();
  sensors.begin();
}

void loop() {
  while(check){
    Serial.print("Soil Temperature");
    Serial.print(",");
    Serial.print("Atmosphere Temperature");
    Serial.print(",");
    Serial.print("Humidity");
    Serial.print(",");
    Serial.print("Soil Moisture");
    Serial.print("\n");
    Serial.print(Atm_temp_theresh);
    Serial.print(",");
    Serial.print(Soil_temp_theresh);
    Serial.print("\n");
    check = false;
  }
  delay(1000);
  //Main_Loop();
  send_param();
}

  void  send_param(){
     sensors.requestTemperatures();
     soil_temp_float = sensors.getTempCByIndex(0);
     soil_temp_float = round(soil_temp_float);

     
     atm_temp = dht.readTemperature();
     atm_temp = (round(atm_temp));

     if(((soil_temp_float - Soil_temp_theresh) + 8) >=16 | ((atm_temp - Atm_temp_theresh) + 8)>=16){
            if(((soil_temp_float - Soil_temp_theresh) + 8) >= 16){
              value = 15;  
              value = value << 4;  
            }
            else{
              value = ((((soil_temp_float - Soil_temp_theresh) + 8) & 00001111)<<4);
            }
            if(((atm_temp - Atm_temp_theresh) + 8) >=16 ){  
              value = value | 00001111;
            }
            else{
              value = value | (((atm_temp - Atm_temp_theresh) + 8) & 00001111);
            }
     }
     else{
        value = ((((soil_temp_float - Soil_temp_theresh) + 8) & 00001111)<<4) | (((atm_temp - Atm_temp_theresh) + 8) & 00001111);
     }
     
     atm_hum = dht.readHumidity();
     atm_hum = (round(atm_hum));
     
     soil_mois = analogRead(A1); //put Sensor insert into soil
     soilmoisturepercent = map(soil_mois, AirValue, WaterValue, 0, 100);
     
     Serial.print(value);
     Serial.print(',');

     Serial.print((atm_hum));
     Serial.print(',');

     Serial.print(soilmoisturepercent);
     Serial.print("\n");
  }
