//#include <SPI.h> // for SPI LoRa
//#include <LoRa.h> // For LoRa
#include <OneWire.h> // for DS18b20
#include <DallasTemperature.h> //for DS18b20
#include <stdlib.h> // included for floatToString function
#include <DHT.h> // For DHT22
#include <avr/sleep.h>
#include <DS3232RTC.h> 
#include <SPI.h> 


#define interruptPin 2 //Pin we are going to use to wake up the Arduino
#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino

#define ONE_WIRE_BUS 5 // DS18b20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
const int time_interval=1;// Sets the wakeup intervall in minutes

const int AirValue = 620;   //you need to replace this value with Value_1
const int WaterValue = 310;  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent=0;

String data_string = " ";
String data_string_p = " ";
String temp_string = " ";
String hum_string = " ";
String airtemp_string = " ";
String soilmois_string = " ";
String p_string = " ";
float soil_temp_float; //Stores soil temperature value
float atm_hum;  //Stores humidity value
float atm_temp; //Stores atmospheric temperature value
int soil_mois; //Stores soil moisture value

// Function for converting float to string
String floatToString(float x, byte precision = 2) {
  char tmp[7];
  dtostrf(x, 0, precision, tmp);
  return String(tmp);
}

void setup()
{
  Serial.begin(9600);
  //Serial.println(" Welcome to AgriTech Sensor Node ");
  //Serial.println(" It includes \n Soil temperature,\n Soil moisture,\n Atmospheric temperature,\n Atmospheric humidity \n");
  pinMode(interruptPin,INPUT_PULLUP);//Set pin d2 to input using the buildin pullup resistor
  //digitalWrite(LED_BUILTIN,HIGH);//turning LED on
  dht.begin();//Start the DHT sensor
  RTC.begin();
  sensors.begin();
  
  // initialize the alarms to known values, clear the alarm flags, clear the alarm interrupt flags
    RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
    RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
    RTC.alarm(ALARM_1);
    RTC.alarm(ALARM_2);
    RTC.alarmInterrupt(ALARM_1, false);
    RTC.alarmInterrupt(ALARM_2, false);
    RTC.squareWave(SQWAVE_NONE);
    
    time_t t; //create a temporary time variable so we can set the time and read the time from the RTC
    t=RTC.get();//Gets the current time of the RTC
   // Serial.println("Check time : " + String(hour(t))+":"+String(minute(t))+":"+String(second(t)));
    RTC.setAlarm(ALM1_MATCH_MINUTES, 0, minute(t)+time_interval, 0, 0);// Setting alarm 1 to go off 1 minute from now
    //RTC.setAlarm(ALM1_MATCH_SECONDS , second(t)+time_interval, 0, 0, 0);
    // clear the alarm flag
    RTC.alarm(ALARM_1);
    // configure the INT/SQW pin for "interrupt" operation (disable square wave output)
    RTC.squareWave(SQWAVE_NONE);
    // enable interrupt output for Alarm 1
    RTC.alarmInterrupt(ALARM_1, true);
    //Serial.println("initialization done.\n");
}

void loop() {
  delay(500);
  //Main_Loop();
  Going_To_Sleep();
}

void Going_To_Sleep(){
    sleep_enable();//Enabling sleep mode
    attachInterrupt(0, wakeUp, LOW);//attaching a interrupt to pin d2
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
    digitalWrite(LED_BUILTIN,LOW);//turning LED off
    time_t t;// creates temp time variable
    t=RTC.get(); //gets current time from rtc
    //Serial.println("Sleep  Time: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));//prints time stamp on serial monitor
    delay(1000); //wait a second to allow the led to be turned off before going to sleep
    sleep_cpu();//activating sleep mode
   // Serial.println("just woke up!");//next line of code executed after the interrupt 
    digitalWrite(LED_BUILTIN,HIGH);//turning LED on
    //collect_sensor_data();//function that reads the temp and the humidity
    t=RTC.get();
    //Serial.println("WakeUp Time: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));//Prints time stamp 
    collect_sensor_data();//function that reads the temp and the humidity
    //Set New Alarm
    t=RTC.get();
    RTC.setAlarm(ALM1_MATCH_MINUTES , 0, minute(t)+time_interval, 0, 0);
    //RTC.setAlarm(ALM1_MATCH_SECONDS, second(t)+time_interval, 0, 0, 0);
  
  // clear the alarm flag
  RTC.alarm(ALARM_1);
  }

void wakeUp(){
  //Serial.println("Interrrupt Fired");//Print message to serial monitor
  sleep_disable();//Disable sleep mode
  detachInterrupt(0); //Removes the interrupt from pin 2;
}

void collect_sensor_data(){
  sensors.requestTemperatures();
  soil_temp_float = sensors.getTempCByIndex(0);
  delay(25);
  atm_hum = dht.readHumidity();
  delay(25);
  atm_temp = dht.readTemperature();
  delay(25);
  soil_mois = analogRead(A1); //put Sensor insert into soil
  soilmoisturepercent = map(soil_mois, AirValue, WaterValue, 0, 100);
  if(soilmoisturepercent >= 100)
  {
    //Serial.println("100 %");
  }
  else if(soilmoisturepercent <=0)
  {
    //Serial.println("0 %");
  }
  else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
  {
    //Serial.print(soilmoisturepercent);
    //Serial.println("%");
  
  }
  delay(25);
  myfunction();
  }

void myfunction(){
  time_t p; //create time object for time and date stamp
  p=RTC.get();//gets the time from RTC
  //Serial.println("time : "+ String(hour(p))+":"+String(minute(p)));
  p_string = String(day(p)) + String(month(p)) + String(year(p));
  temp_string = floatToString(soil_temp_float);
  hum_string = floatToString(atm_hum);
  airtemp_string = floatToString(atm_temp);
  soilmois_string = String(soilmoisturepercent);

  data_string = (temp_string + soilmois_string + airtemp_string + hum_string );
  data_string_p = (temp_string + "," + soilmois_string + "," + airtemp_string + "," + hum_string );
  Serial.println(data_string_p);

  //Transmit code.....
  /*LoRa.beginPacket();
  uint8_t data[(data_string.length() + 2)];
  data_string.toCharArray(data, (data_string.length() + 2));
  LoRa.write(data, sizeof(data));
  Serial.println("Packet sent");
  LoRa.endPacket();
  */
  delay(6); 
  }
