#include "EEPROM.h"
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Arduino.h>
/*Это проект ключницы с датчиком отпечатка пальца*/
#define EEPROM_SIZE 64
#define typeOfLock 0 //0-сервопривод 1-электромагнитная защелка 2-откл
#define debug 0 //0-выкл 1-вкл Serial port
#define RX 2
#define TX 3 //Подключение датчика
#define servPin 5
#define lockButPin 4
#define servClose 0
#define servOpen 180 //крайние значения для закрыто-открыто
#define setupBut 6
int addr = 1;//ячейка для записи количества сохраненных отпечатков
uint8_t id;//id 
boolean doorFlag,openFlag;
unsigned long last_press;

SoftwareSerial mySerial(RX, TX);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
Servo lockServ;
void setup() {
pinMode(lockButPin ,INPUT_PULLUP);
pinMode(setupBut ,INPUT_PULLUP);
  finger.begin(57600);
    if(byte(EEPROM.read(addr)==0))EEPROM.write(addr,1);
  id=byte(EEPROM.read(addr));

    if(debug){Serial.begin(9600);while(!Serial);delay(100);}
    if(debug)Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
    if(debug){Serial.print("Saved:");Serial.print(id-1);Serial.print("fingerprints");}
    if(typeOfLock==0)lockServ.attach(servPin);
    if(id==1){while(!getFingerprintEnroll());
  id++;
  EEPROM.write(addr,id);
    }
}
void loop() {
  doorFlag=digitalRead(lockButPin);
    if(typeOfLock==0&&digitalRead(!doorFlag)){lockServ.write(servClose);openFlag=0;return;}//закрытие двери с сервоприводом
    else if(typeOfLock==1&&digitalRead(!doorFlag)){openFlag=0;return;}//закрытие двери с щеколдой
    if(debug)Serial.println("Waiting...");
  if(openFlag==0&&getFingerprintIDez()!=-1){
      if(typeOfLock==0){lockServ.write(servOpen);openFlag=1;}//открытие двери с сервоприводом после проверки отпечатка
      else if(typeOfLock==1){openFlag=1;}//открытие двери с щеколдой после проверки отпечатка
  }
  else if(openFlag==1&&digitalRead(setupBut)==1&&millis()-last_press>50){
    while(!getFingerprintEnroll());
    id++;
    EEPROM.write(addr,id);
    last_press=millis();
  }//добавление отпечатка после нажатия на кнопку добавления
}
