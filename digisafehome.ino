
#define BLYNK_TEMPLATE_ID "TMPL3ksUfkY35"
#define BLYNK_TEMPLATE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "fuCReUl8KUvRcYkIMuvGrSctc5vu61Js"


#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


#define MQ8_Sensor D0
#define trig D1  
#define echo D2  
#define Relay1 D3
#define Relay2 D4
#define PIR_Sensor D5
#define BUZZER D6
#define LED D7
#define LED2 D8


char ssid[] = "Swarnim";        
char pass[] = "12345678";  
int tankdepth = 15;        
int tankfull = 2;          



long level = 0;
unsigned long current = 0;
unsigned long old = 0;
long interval = 15;
BlynkTimer timer;

char auth[] = BLYNK_AUTH_TOKEN;



BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V3);
}


BLYNK_WRITE(V0)  
{
  int relay1Status = param.asInt();
  digitalWrite(Relay1, !relay1Status);  
  Serial.print("Relay 1 Status: ");
  if (relay1Status == 0)
    Serial.println("ON");
  else
    Serial.println("OFF");
}

BLYNK_WRITE(V1)  
{
  int relay2Status = param.asInt();
  digitalWrite(Relay2, !relay2Status);  
  Serial.print("Relay 2 Status: ");
  if (relay2Status == 0)
    Serial.println("ON");
  else
    Serial.println("OFF");
}
BLYNK_WRITE(V3)  
{
  int LEDStatus = param.asInt();
  digitalWrite(LED2, LEDStatus);  
  Serial.print("LED Status: ");
  if (LEDStatus == 1)
    Serial.println("ON");
  else
    Serial.println("OFF");
}


void PGM() {
  Ultra();
  MQ8_Reading();
  PIR_Reading();
  Serial.println();
}


void setup() {
  pinMode(MQ8_Sensor, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(PIR_Sensor, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2,LOW);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  Serial.begin(115200);
  Serial.println("\nConnecting to Wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  wifi_testing();
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, PGM);
}

void loop() {

  Blynk.run();
  timer.run();
  wifi_testing();
}




void wifi_testing() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
    delay(250);
    current = millis();
    if ((current - old) / 1000 > interval)
      ESP.restart();
  }
}

void Ultra() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  long cm = t / 29 / 2;
  Serial.println("Distance: " + String(cm) + "cm");
  level = map(cm, tankdepth, tankfull, 0, 100);  //0-100 %
  if (level < 0) level = 0;
  if (level > 100) level = 100;
  //level = random(1, 50);
  Serial.println("Water Level: " + String(level) + "%");
  Blynk.virtualWrite(V2, level);
}

void MQ8_Reading() {
  int MQ8_status = !digitalRead(MQ8_Sensor);
  Serial.println("MQ8 Sensor Reading: " + String(MQ8_status));
  if (MQ8_status == 1) {
    tone(BUZZER, 1000, 2000);
  } else {
  }
}
void PIR_Reading() {
  int PIR_status = digitalRead(PIR_Sensor);
  Serial.println("PIR Sensor Reading: " + String(PIR_status));
  if (PIR_status == 1) {
    tone(LED, 1000, 5000);
  } else {
  }
}
