#include <OneWire.h>  
#include <DallasTemperature.h>
#include <ESP32Servo.h>
#define sensorTemp 14
#define pinBotao 34
//#include <ESP8266WiFi.h> 
#include <WiFi.h>
#include <PubSubClient.h>
OneWire oneWire(sensorTemp);
DallasTemperature sensors(&oneWire);


Servo myservo;
int posicaoServo = 0;
int servoPin = 32;
int ledGreen = 13;
int ledBlue = 12;
int ledRed = 25;
int luz = 35; 
int LDR = GPIO_NUM_26;

//const char* ssid = "192.168.15.1";
//const char* ssid = "Rede_Casa_2.4G";
//const char* password = "admin43210";
const char* ssid = "AndroidAP";
const char* password = "admin123";
WiFiClient wifiClient;
WiFiServer server(80);

//char mqtt_host[] = "122.233.12.341"
//char mqtt_host[] = "mqtt.eclipseprojects.io";
char mqtt_host[] = "mqtt.eclipseprojects.io";
char clientid[] = "topicoServo01";
const int mqtt_port = 1883;
#define TOPIC_PUBLISH "topico1"
PubSubClient MQTT(wifiClient);

int estadoBotao = 0;
int estadoBotaoAnt = 0;

void conectaWiFi() {

  if (WiFi.status() == WL_CONNECTED) {
     return;
  }
  
  WiFi.begin(ssid, password);
  Serial.println(WiFi.status());
  Serial.println(ssid);
  Serial.println(password);
  
  //while (WiFi.waitForConnectResult() != WL_CONNECTED) {
  while (WiFi.status() != WL_CONNECTED) {

    delay(1000); 
    Serial.println(WiFi.status());
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println(WiFi.localIP()); //endereço de IP local

}

void conectaMQTT() { 
  while (!MQTT.connected()) {
    Serial.print("Conectando ao Broker MQTT: ");
    Serial.println(mqtt_host);
    if (MQTT.connect(clientid)) {
      Serial.println("Conectado ao Broker com sucesso!");
    } 
    else {
      Serial.println("Noo foi possivel se conectar ao broker.");
      Serial.println("Nova tentatica de conexao em 10s");
      delay(10000);
    }
  }
}

void enviaDadosViaMQTT() {
  
  estadoBotao = digitalRead(pinBotao);
  int posicaoServo = 0;

  if (estadoBotao == HIGH && estadoBotao != estadoBotaoAnt) {

    MQTT.publish(TOPIC_PUBLISH, "1");
    Serial.println("Botão pressionado");
    
    myservo.write(posicaoServo);
    delay(15);
  }
  //else if (estadoBotao == LOW && estadoBotao != estadoBotaoAnt) {

    //MQTT.publish(TOPIC_PUBLISH, "0");
    //Serial.println("Botão solto");
  //}

  estadoBotaoAnt = estadoBotao;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(luz, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(sensorTemp, INPUT);
  pinMode(pinBotao, INPUT);
  sensors.begin();
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); 
  myservo.attach(servoPin, 500, 2400);
  
  Serial.begin(115200);

  conectaWiFi();
  
  server.begin();

  MQTT.setServer(mqtt_host, mqtt_port);
//  while(!client.connected()) {
//    Serial.println("Conectando ao broker mqtt...");
//    if (client.connect(
//  }
}

void loop() {

    if (!MQTT.connected()) {
       conectaMQTT(); 
    }
    
    conectaWiFi(); 
    
    // put your main code here, to run repeatedly:
    digitalWrite(luz, HIGH);
    int ldrValor = analogRead(LDR);
    Serial.print("Valor LDR: ");
    Serial.println(ldrValor);
    sensors.requestTemperatures();
    Serial.print("Temperatura: ");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.println(" ºC\n");
    delay(1500);

    Serial.println(WiFi.status());
    
    if (sensors.getTempCByIndex(0) > 30 ) {
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledGreen, LOW);
      digitalWrite(ledBlue, LOW);
      Serial.println("*** TEMPERATURA ALTA! ***");
    }
    else if (sensors.getTempCByIndex(0) <= 30 && sensors.getTempCByIndex(0) > 27){
      digitalWrite(ledGreen, HIGH);
      digitalWrite(ledBlue,LOW);
      digitalWrite(ledRed, LOW);
      Serial.println("Temperatura Normal");
    }
    else {
      digitalWrite(ledGreen, LOW);
      digitalWrite(ledBlue,HIGH);
      digitalWrite(ledRed, LOW);
      Serial.println("*** TEMPERATURA BAIXA! ***");
    }

    if (ldrValor < 300){
      digitalWrite(luz, HIGH);
    }
    else {
      digitalWrite(luz, LOW);
    }

    enviaDadosViaMQTT();
    MQTT.loop();

    //SERVO

    
/*
    for (posicaoServo = 0; posicaoServo <= 180; posicaoServo += 1){
      myservo.write(posicaoServo);
      delay(15);
    }
    for (posicaoServo = 180; posicaoServo >= 0; posicaoServo -= 1){
      myservo.write(posicaoServo);
      delay(15);
    }
*/
}
