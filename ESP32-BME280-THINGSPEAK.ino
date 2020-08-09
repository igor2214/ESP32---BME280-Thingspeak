//Bibliotecas 
#include <WiFi.h>// WIFI
#include <Wire.h>//CONEXA0 I2C
#include <Adafruit_BME280.h>//SENSOR BME280
#include <Adafruit_Sensor.h>//SENSORES 
#include <NTPClient.h> //RELOGIO ONLINE 
#include <SPI.h>//COMUNICACAO

//DEFINICAO DOS PINOS DOS SENSORES 

#define BME_SCK 18 //BME280
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/
#define LIGHTSENSORPIN 33//SENSOR DE LUX

// DADOS DA REDE EM QUE IRA SE COMUNICAR 

const char* ssid     = "VIVOFIBRA-FF00";
const char* password = "33d73cff00";

//DADOS DO CANAL THINGSPEAK
const char* host = "api.thingspeak.com";
String api_key = "CV83M4XVS3STTIVC";


//Definir variaveis e os seus valores 

WiFiUDP udp;/
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);   //Cria um objeto "NTP" com as configurações.utilizada no Brasil 
String hora;                                           // Variável que armazena relogio online 
int hora1;                                            // variavel que armazena relogio online
#define SEALEVELPRESSURE_HPA (1013.25)
float temperature = 0;
float humidity = 0;
float pressure = 0;
float altitude = 0;

//iniciar o sensor e comucicao wifi
Adafruit_BME280 bme;
//Modo de operacao wifi 
WiFiClient client;
//estrutura principal
void setup() {
  pinMode(LIGHTSENSORPIN, INPUT);
  Serial.begin(115200);  
delay(500);
 
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  ntp.begin();               
ntp.forceUpdate();  
delay(500);
 
}
//estrutura repetitiva 
void loop() {

 //ler os sensores 
temperature = bme.readTemperature();
humidity = bme.readHumidity();
pressure = bme.readPressure();
altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
float reading = analogRead(LIGHTSENSORPIN); 

//imprimir valores 

Serial.println(reading);
hora = ntp.getFormattedTime(); 
Serial.println(hora);     
delay(500); 

//estrutura repetitiva do relogio online 

   if ( hora >= "07:15:00" && hora <= "07:20:50" )
  { // Ao atingir o período definido
    hora1=1;   // sinal 1 
    delay(500);
  }
  if ( hora >= "12:15:00" && hora <= "12:20:50" )
  { // Ao atingir o período definido
    hora1=2;   // sinal 2 
    delay(500);
  }
  if ( hora >= "19:00:00" && hora <= "19:05:50" )
  { // Ao atingir o período definido
    hora1=3;   // sinal 3
    delay(500);
  }
    if ( hora >= "00:50:00" && hora <= "00:50:50" )
  { // Ao atingir o período definido
    hora1=4;   // sinal 4
  delay(500);              
}
else  {
  hora1=0; // qualquer outra hora 
}

//imprimi hora 
Serial.println(hora1); 
delay(500); 



//postagem da temperatura no thingspeak 
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    return;
  }
{
 String data_to_send = api_key;
    data_to_send += "&field1=";
    data_to_send += String(temperature);
    data_to_send += "&field2=";
    data_to_send += String(pressure);
     data_to_send += "&field3=";
    data_to_send += String(humidity);
    data_to_send += "&field4=";
    data_to_send += String(altitude);
    data_to_send += "&field5=";
    data_to_send += String(reading);
    data_to_send += "&field6=";
    data_to_send += String(hora1);    
    data_to_send += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data_to_send.length());
    client.print("\n\n");
    client.print(data_to_send);
  
    delay(1000);
  }
}
