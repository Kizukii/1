/* 
  Прошивка для блока MQ2
*/
// Блок 1: Подключение необходимых библиотек и объявление входов датчика
#include <SoftwareSerial.h>

#define MQ2_PIN A0;
float MQ2value;

SoftwareSerial XBee(2,3);
// Блок 2: Настройка последовательной передачи данных и настройка входа датчика
void setup()
{
    Serial.begin(9600);
    XBee.begin(9600);
    pinMode(MQ2_PIN, OUTPUT);
}

void loop()
{
  // Блок 3: Считывание данных с датчика
  MQ2value = analogRead(MQ2_PIN);
  String analogValue = String(MQ2value, DEC);
  // Блок 4: Передача данных в общий поток сети
  if(XBee.connected())
  {
    XBee.write(analogMQ2);  
  }
}
