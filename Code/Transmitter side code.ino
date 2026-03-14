#include <SPI.h>
#include <LoRa.h>
#include "HX711.h"

// ---------- HX711 ----------
#define DT 3
#define SCK 2
HX711 scale;

// ---------- Ultrasonic ----------
#define TRIG_PIN 4
#define ECHO_PIN 5

// ---------- PIR + IR ----------
#define PIR_PIN 6
#define IR_PIN 7

// ---------- LoRa ----------
#define SS 10
#define RST 9
#define DIO0 8

int peopleCount = 0;
int lastSentCount = 0;

// trigger flags
bool pirTrig=false;
bool irTrig=false;
bool ultraTrig=false;
bool loadTrig=false;

// trigger timestamps
unsigned long pirTime=0;
unsigned long irTime=0;
unsigned long ultraTime=0;
unsigned long loadTime=0;

unsigned long eventStart=0;
unsigned long eventWindow = 7000;


// -------- Ultrasonic --------
long readUltrasonic()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if(duration == 0)
    return -1;

  return duration * 0.034 / 2;
}


void setup()
{
  Serial.begin(115200);
  delay(2000);

  Serial.println("System Starting...");

  SPI.begin();
  pinMode(SS, OUTPUT);

  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  delay(50);
  digitalWrite(RST, HIGH);
  delay(50);

  LoRa.setPins(SS,RST,DIO0);

  if(!LoRa.begin(433E6))
  {
    Serial.println("LoRa Failed!");
    while(1);
  }

  Serial.println("LoRa Ready");

  pinMode(PIR_PIN,INPUT);
  pinMode(IR_PIN,INPUT);

  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);

  digitalWrite(TRIG_PIN,LOW);

  scale.begin(DT,SCK);
  delay(500);
  scale.set_scale(420);
  scale.tare();

  Serial.println("Sensors Ready");

  Serial.println("PIR warming...");
  delay(20000);
}


void loop()
{
  unsigned long now = millis();

  int pir = digitalRead(PIR_PIN);
  int ir = digitalRead(IR_PIN);

  long distance = readUltrasonic();

  float weight = 0;

  if(scale.is_ready())
    weight = scale.get_units(5);


  Serial.print("PIR:");
  Serial.print(pir);

  Serial.print(" IR:");
  Serial.print(ir);

  Serial.print(" Dist:");
  Serial.print(distance);

  Serial.print(" Weight:");
  Serial.print(weight);

  Serial.print(" Count:");
  Serial.println(peopleCount);


  if(pir==HIGH && !pirTrig)
  {
    pirTrig=true;
    pirTime=now;
  }

  if(ir==LOW && !irTrig)
  {
    irTrig=true;
    irTime=now;
  }

  if(distance>5 && distance<120 && !ultraTrig)
  {
    ultraTrig=true;
    ultraTime=now;
  }

  if(weight>200 && !loadTrig)
  {
    loadTrig=true;
    loadTime=now;
  }


  if((pirTrig || irTrig || ultraTrig || loadTrig) && eventStart==0)
    eventStart = now;


  if(eventStart!=0 && now-eventStart > eventWindow)
  {
    processEvent();
    resetState();
  }

  delay(200);
}


void processEvent()
{

  // ENTRY
  if(irTrig && loadTrig && (pirTrig || ultraTrig) && irTime < loadTime)
  {
    peopleCount++;
    Serial.println("ENTRY DETECTED");
  }

  // EXIT
  else if(irTrig && loadTrig && (pirTrig || ultraTrig) && loadTime < irTime)
  {
    if(peopleCount > 0)
      peopleCount--;

    Serial.println("EXIT DETECTED");
  }

  else
  {
    Serial.println("Event ignored");
  }


  if(peopleCount != lastSentCount)
  {
    sendPacket();
    lastSentCount = peopleCount;
  }

}


void sendPacket()
{
  LoRa.beginPacket();
  LoRa.print(peopleCount);   // send only count
  LoRa.endPacket();

  Serial.print("LoRa Sent -> ");
  Serial.println(peopleCount);
}


void resetState()
{
  pirTrig=false;
  irTrig=false;
  ultraTrig=false;
  loadTrig=false;

  pirTime=0;
  irTime=0;
  ultraTime=0;
  loadTime=0;

  eventStart=0;
}