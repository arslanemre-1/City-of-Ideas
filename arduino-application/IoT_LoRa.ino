#include <SPI.h>
#include <LoRa.h>
#include <millisDelay.h>
#define UPBUTTON 4
#define DOWNBUTTON 3
#define UPLED 5
#define DOWNLED 6

#define DEVICE_ID "KdG10"
millisDelay timer;
byte voteCount = 0;
byte downVoteCount = 0;
int count = 1;

void setup() {
 Serial.begin(9600);
 Serial.println("Setup LoRa shield...");
 if (!LoRa.begin(868100000)) {
   Serial.println("Starting LoRa failed!");
   while(1);
}
    LoRa.setSyncWord(0x34);
    Serial.println("LoRa shield initialized");
    pinMode(UPBUTTON, INPUT);
    pinMode(DOWNBUTTON, INPUT);
    Serial.println("Signalling ready by flashing LED 3 times");
    pinMode(UPLED, OUTPUT);
    pinMode(DOWNLED, OUTPUT);
    for(int i=0; i<3; i++) {
    digitalWrite(UPLED, HIGH);
    digitalWrite(DOWNLED, HIGH);

    delay(200);
    digitalWrite(UPLED, LOW);
    digitalWrite(DOWNLED, LOW);

    delay(400);
 }
 Serial.println("Ready!");
 timer.start(10000);  // start a 15s timer
}

void sendUpVote() {
 Serial.print("Sending packet ");
 Serial.print(count);
 Serial.println("... ");
 int status = LoRa.beginPacket();
 if (status) {
   LoRa.print("<");
   LoRa.print(DEVICE_ID);
   LoRa.print(">");
   LoRa.print("1;");//ideaID 
   LoRa.print("1;");//value
   LoRa.print(voteCount);//count
   LoRa.endPacket();
   count++;
   Serial.println("Packet sent");
   Serial.print("1;");
   Serial.print("1;");
   Serial.println(voteCount);
 } else {
   Serial.println("Error sending packet");
 }
}

void sendDownVote() {
 Serial.println("Sending packet ");
 Serial.print(count);
 Serial.println("... ");
 int status = LoRa.beginPacket();
 if (status) {
   LoRa.print("<");
   LoRa.print(DEVICE_ID);
   LoRa.print(">");
   LoRa.print("1;");//ideaID 
   LoRa.print("-1;");//value
   LoRa.print(downVoteCount);//count
   LoRa.endPacket();
   count++;
   Serial.println("Packet sent");

   Serial.print("1;");
   Serial.print("-1;");
   Serial.println(downVoteCount);
 } else {
   Serial.println("Error sending packet");
 }
}






int lastUpButtonValue = 0;
int lastDownButtonValue = 0;

void loop() {
  
  int upButtonValue = digitalRead(UPBUTTON);
  int downButtonValue = digitalRead(DOWNBUTTON);

  
    if(voteCount==3 && upButtonValue){
      sendUpVote();
      timer.start(10000);
      voteCount=0;

    Serial.println("sending ups");
    }else if(downVoteCount==3 && downButtonValue){
      sendDownVote();
      timer.start(10000);
      downVoteCount=0;
    Serial.println("sending downs");

    }else if (timer.justFinished()){
      Serial.println("Time reached! Sending up&down votes.");
       sendDownVote();
       sendUpVote();
       downVoteCount=0;
       voteCount=0;

       timer.start(10000);
    
      }else {
        
         if (!upButtonValue) digitalWrite(UPLED, LOW);
  if (!lastUpButtonValue && upButtonValue) {
  voteCount++;
  Serial.println("up button Pushed...");
  digitalWrite(UPLED, HIGH);
  delay(75); // simple debouncing
     
        } 

        if(!downButtonValue) digitalWrite(DOWNLED, LOW);
        if (!lastDownButtonValue && downButtonValue){
  downVoteCount++;
  Serial.println("down button Pushed...");
  digitalWrite(DOWNLED, HIGH);
  delay(75); // simple debouncing
     
        } 
 
}
lastUpButtonValue = upButtonValue;
lastDownButtonValue = downButtonValue;

}
