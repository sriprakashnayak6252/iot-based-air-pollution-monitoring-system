#include <SoftwareSerial.h>///////////////////////air 
#include <LiquidCrystal.h>

#define RX 2
#define TX 3
String AP = "12345678";       // CHANGE ME
String PASS = "12345678"; // CHANGE ME
String API = "3ISG9WEYPDKECLGU";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";

String field1 = "field1";
String field2 = "field2";
String field3 = "field3";
String field4 = "field4";

int countTrueCommand;
int countTimeCommand; 

boolean found = false; 
float valSensor = 1;
SoftwareSerial esp8266(RX,TX); 

const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include "dht.h"
#define dht_apin A0// Analog Pin sensor is connected to

dht DHT;

const int x_out = A1; //
int x_adc_value;
const int y_out = A2; //
int y_adc_value;
const int z_out = A3; //
int z_adc_value;

 
void setup()
{

// set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
 lcd.setCursor(1, 0);
  lcd.print("AIR  monitor..");
  delay(10);
 lcd.setCursor(0, 1);
 lcd.print("  system");
// delay(1000);
  
  Serial.begin(9600);
  esp8266.begin(9600);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  



}
void loop() {


  DHT.read11(dht_apin);

  lcd.clear();
  lcd.print("temp =");
  lcd.print(DHT.temperature);
  lcd.print(" degC");
  delay(1000);

 
  
 valSensor =DHT.temperature;
 
 String getData1 = "GET /update?api_key="+ API +"&"+ field1 +"="+String(valSensor);
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData1.length()+4),4,">");
 esp8266.println(getData1);
 delay(1500);
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
////////////////////////////////////////////////////

delay(3000);
 
  lcd.clear();
  lcd.print("humi(%) =");
  lcd.print(DHT.humidity);
  lcd.print(" %");


valSensor = DHT.humidity;


 String getData2 = "GET /update?api_key="+ API +"&"+ field2 +"="+String(valSensor);
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData2.length()+4),4,">");
 esp8266.println(getData2);
 delay(1500);
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");


//////////////////////////////////////////////////
///////////////////////////////////////////////// LPG gas

    delay(3000);
   
    x_adc_value = analogRead(x_out);
    
    lcd.clear();
    lcd.print("LPG counts:");
    lcd.print(x_adc_value);
    delay(1000);

    valSensor = x_adc_value;


 String getData3 = "GET /update?api_key="+ API +"&"+ field3 +"="+String(valSensor);
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData2.length()+4),4,">");
 esp8266.println(getData2);
 delay(1500);
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");



//////////////////////////////////////////////////////////LPG

//////////////////////////////////////////////////////mq135/// 1

    delay(3000);
    y_adc_value = analogRead(y_out);

    lcd.clear();
    lcd.print("MO :");
    lcd.print(y_adc_value);
    delay(1000);
    
    valSensor = y_adc_value;


 String getData4 = "GET /update?api_key="+ API +"&"+ field4 +"="+String(valSensor);
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData2.length()+4),4,">");
 esp8266.println(getData2);
 delay(1500);
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
 

//////////////////////////////////////////////////////////LPG



////////////////////////////////////////////
}
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }

