/* 
  Прошивка для блока теплицы
*/
// Блок 1: Подключение библиотек и инициализация подключенных модулей
#include <math.h>
#include <SFE_BMP180.h> 
#include <Wire.h>
#include <DHT.h>
#include <SoftwareSerial.h>

SoftwareSerial XBee(2,3);

#define DHTPIN 5 // Подключение DHT22 к пину 5
#define DHTPIN1 3 // Подключение DHT11 к пину 3
#define relay_pin 6 // Подключение реле к пину 6
SFE_BMP180 pressure; // Переменная для датчика BMP180

#define ALTITUDE 151.0 // Высота над уровнем моря в метрах 

DHT dht(DHTPIN, DHT22); //Инициация датчиков
DHT dht1(DHTPIN1, DHT11);

float h1;
float t1;
float h;
float t;
char status;
double T,P;
int relay_status;

// Блок 2: Настройка устройств и подключений
void setup()
  {
  // Запуск последовательного порта
  Serial.begin(9600);
  XBee.begin(9600);

  dht.begin(); // Инициализируем DHT22
  dht1.begin(); // Инициализируем DHT11

  if (pressure.begin())
  { 
    // Проверка на подключение BMP180    
    Serial.print("BMP180 ok");
  }
  else
  {
    Serial.print("BMP180 fail");
    delay(2000);
    while(1); // Бесконечный цикл. Если есть сообщение об ошибке, нужно проверить подключение BMP180 и перезапустить плату (выключить и включить питание)
  }
 }

 void loop()
 {
  // Блок 3: Снятие показаний с датчиков
  String analogValue0;
  String analogValue1;
  h1 = dht1.readHumidity(); //Измеряем влажность
  t1 = dht1.readTemperature(); //Измеряем температуру
  if (isnan(h1) || isnan(t1)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
    Serial.println("Ошибка считывания");
    return;
  }
  status = pressure.startTemperature(); // Старт измерения температуры
  delay(status);
  status = pressure.getTemperature(T);
        
  analogValue0 = String(T,DEC);
  // Определяем атм. давление:
  status = pressure.startPressure(1);
  delay(status);
  status = pressure.getPressure(P,T);
                   
  analogValue1 = String(P*0.750064);
    
  h = dht.readHumidity(); //Измеряем влажность
  t = dht.readTemperature(); //Измеряем температуру

  if (isnan(h) || isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
    Serial.println("Ошибка считывания DHT22");
    return;
  }
  
  // Запись данных в переменные String 
  String analogValue2 = String(t1,DEC);
  String analogValue3 = String(h1);
  String analogValue4 = String(t, DEC);
  String analogValue5 = String(h);
  
  // Блок 4: Отправка данных
  if(XBee.connected())
  {
    XBee.write("&field4="+analogValue3+"&field5="+analogValue4+"&field6="+analogValue5+"&field1="+analogValue0+"&field2="+analogValue1+"&field3="+analogValue2);
    Serial.println(analogValue0);
    Serial.println(analogValue1);
    Serial.println(analogValue2);
    Serial.println(analogValue3);
    Serial.println(analogValue4);
    Serial.println(analogValue5);
  }
  // Блок 5: Управление реле
  if(relay_status == 1)
  {
    digitalWrite(relay_pin, HIGH);
  } else {
    digitalWrite(relay_pin, LOW);
  }
}
