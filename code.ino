#include<SoftwareSerial.h> 
#include <DFRobot_SIM808.h>

#define rxPin 11
#define txPin 10

 SoftwareSerial mySerial(txPin, rxPin);
 DFRobot_SIM808 sim808(&mySerial);
 char wlat[12];
 char wlon[12];
 char wwspeed[12];
 
void setup(){
mySerial.begin(9600);
Serial.begin(115200); 
Serial.println("Starting...");
 while(!sim808.init()) {
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  }
  //************* Turn on the GPS power************
if( sim808.attachGPS())
{
  Serial.println("Open the GPS power success");
}
else 
{
    Serial.println("Open the GPS power failure");
}

}
 void loop(){
if (sim808.getGPS()) {
   Serial.print(sim808.GPSdata.year);
   Serial.print("/");
   Serial.print(sim808.GPSdata.month);
   Serial.print("/");
   Serial.print(sim808.GPSdata.day);
  Serial.print(" ");
   Serial.print(sim808.GPSdata.hour);
   Serial.print(":");
  Serial.print(sim808.GPSdata.minute);
  Serial.print(":");
  Serial.print(sim808.GPSdata.second);
  Serial.print(":");
  Serial.println(sim808.GPSdata.centisecond);

   Serial.print("latitude :");
   Serial.println(sim808.GPSdata.lat,6);

  sim808.latitudeConverToDMS();
  Serial.print("latitude :");
  Serial.print(sim808.latDMS.degrees);
  Serial.print("\^");
  Serial.print(sim808.latDMS.minutes);
  Serial.print("\'");
  Serial.print(sim808.latDMS.seconeds,6);
  Serial.println("\"");
  Serial.print("longitude :");
  Serial.println(sim808.GPSdata.lon,6);
  sim808.LongitudeConverToDMS();
 Serial.print("longitude :");
 Serial.print(sim808.longDMS.degrees);
 Serial.print("\^");
 Serial.print(sim808.longDMS.minutes);
 Serial.print("\'");
 Serial.print(sim808.longDMS.seconeds,6);
Serial.println("\"");

 Serial.print("speed_kph :");
 Serial.println(sim808.GPSdata.speed_kph);
Serial.print("heading :");
 Serial.println(sim808.GPSdata.heading);
 
 
    float la = sim808.GPSdata.lat;
     float lo = sim808.GPSdata.lon;
    float ws = sim808.GPSdata.speed_kph;


     delay(1000);
     SubmitHttpRequest(la,lo,ws);
       }


  }
 

 void SubmitHttpRequest(float la,float lo, float ws)
 {
  
  dtostrf( la,6, 6, wlat); 
  dtostrf( lo,6, 6, wlon); 
  dtostrf( ws,6, 6, wwspeed); 

 mySerial.println("AT");
delay(100);
ShowSerialData();

  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode

  delay(1000);  // Delay of 1000 milli seconds or 1 second

  mySerial.println("AT+CMGS=\"9163147491\"\r"); // Replace x with mobile number

  delay(1000);
  String msg = "Latitude: ";
  msg+=wlat;
  msg+="  ";
  msg+="Longitude: ";
  msg+=wlon;
  mySerial.println(msg);// The SMS text you want to send

  delay(100);

  mySerial.println((char)26);// ASCII code of CTRL+Z

  delay(10000);
 }

 void ShowSerialData()
 {
   while(mySerial.available()!=0)
     {
       Serial.write(mySerial.read());
     }

 } 
