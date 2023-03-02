//knockknock


#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <Servo.h> // servo library  
 Servo s1;  


#define DEVICEID 1


#define DELAYVAL 1500 // Time (in milliseconds) to pause between pixels

const int analogInPin=A0;
const int DigitalInPin=D5; //inPositionDetector
const int servoPin=D3;




float sensorV=0.0;
float touching=0.0;

const char ssid[] ="JamNet";
const char pass[] ="PlayDesign";
WiFiClient net;
MQTTClient client;

bool stopsignal;

int systemStatus; // idel =0 , start = 1 


unsigned long lastMillis = 0;





void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("Bell1", "birdturner418", "4ftt9krwHqEGhseR")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  if(DEVICEID==1){
    client.subscribe("/Bell1");
    }else{
      
         client.subscribe("/Bell2");
      }
  

}

bool blockSingnal=false;

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  int recivestat=payload.toInt();
    if(systemStatus!=recivestat){
        systemStatus = recivestat;
    }
  }

  
  



float LastValue;

void setup() {
  Serial.begin(115200);
 WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin("public.cloud.shiftr.io", net);
  client.onMessage(messageReceived);

  s1.attach(0);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.



  connect();
}




void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
  
sensorV=digitalRead(DigitalInPin);
touching = analogRead(analogInPin);

if(systemStatus ==0){

  if(touching>136){
    systemStatus=1;
     StateChange();
   }
  
  
 }
 else{

  if(sensorV==HIGH){
      Kncok(); 
    }

    if(systemStatus == 0){


      
    if(touching>136){
      systemStatus=0;
      StateChange();
    }  
     }
  
  }



}

void StateChange(){
  
  
  if(DEVICEID ==1)
  client.publish("/P1", String(systemStatus));
  else 
  client.publish("/P2", String(systemStatus));
  
   
}


void Kncok(){
  int pos;

    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    s1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    s1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  
 
}
