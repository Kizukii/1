/*
 Прошивка для блока координатора
*/
// Блок 1: Подключение необходимых библиотек и инициализация подключенных модулей
#define BLYNK_PRINT Serial
// Данные аунтентификации для поиска сервисом Blynk данного утройства
#define BLYNK_TEMPLATE_ID "***********"
#define BLYNK_DEVICE_NAME "***********"
#define BLYNK_AUTH_TOKEN "***********"

#include <ESP8266_HardSer.h>
#include <BlynkSimpleShieldEsp8266_HardSer.h>
#include <SoftwareSerial.h>

SoftwareSerial XBee(2,3);

// Переменные для подлкючения к сети Wi-Fi
char auth[] = " ";
char ssid[] = " ";
char pass[] = " ";

// // МАС-АДРЕС платы, на которую отправляем данные
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Вводим переменные для хранения отправляемых данных
float temperature; 
float humidity;
char status; 
double P; 
String presure;
float humidity_soil;

String analogMQ2;
String analogPir;

// Блок 2: Настройка устройств и подключений
void setup()
{
    WiFi.mode(WIFI_AP_STA);

    Serial.begin(9600);
    XBee.begin(9600);

    // Инициализация подключения к сервису Blynk
    Blynk.begin(auth, ssid, pass);
   
    // Попытка подключиться к серверу
    resultConnection = Blynk.connect();
}

void loop()
{  
    // Блок 3: Запуск обмена данных с сервисом Blynk при успешном подключении к сервису
    if(resultConnection) Blynk.run();
    
    // Блок 4: Указание данных для отправки на другие блоки
    Readings.temp = temperature;
    Readings.hum = humidity; 
    Readings.pres = pressure;
    Readings.hum_soil = humidity_soil;
 
    // Отправка сообщения и принятие данных от других блоков
    if(XBee.read() > 0)
    {
      sendData();  
    }
    XBee.write(relay_status);
}

// Блок 5: Отправка полученных данных на сервис
void sendData()
{
    Blynk.virtualWrite(V5, temperature);
    Blynk.virtualWrite(V6, humidity);
    Blynk.virtualWrite(V7, pressure);
    Blynk.virtualWrite(V8, humidity_soil);
    Blynk.virtualWrite(V9, analogMQ2);
    Blynk.virtualWrite(V10, analogPir);
}

// Блок 6: Вызов функции при обновлении виртуального порта V0 
BLYNK_WRITE(V0) 
{
    // Считываем новое значение порта
    relay_status = param.asInt(); 
}
