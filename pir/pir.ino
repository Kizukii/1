/* 
  Прошивка для блока HC-SR501
*/
// Блок 1: Подключение необходимых библиотек и объявление входов датчика
#include <SoftwareSerial.h>

#define PIR_PIN A0;
float pir_value;

SoftwareSerial XBee(2,3);

// Блок 2: Настройка последовательной передачи данных и настройка входа датчика
void setup()
{
    Serial.begin(9600);
    XBee.begin(9600);
    pinMode(PIR_PIN, OUTPUT);
}

void loop()
{
  // Блок 3: Считывание данных с датчика
  pir_value = analogRead(PIR_PIN);
  String analogPir = String(pir_value, DEC);
  // Блок 4: Передача данных в общий поток сети
  if(XBee.connected())
  {
    XBee.write(analogPir);  
  }
}
