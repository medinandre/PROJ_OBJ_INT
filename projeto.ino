// ===================
// Incluir bibliotecas
// ===================
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <ESP8266WiFi.h> //inclui suporte ao ArduinoUno
#include <WiFiClient.h>
#include <PubSubClient.h> //inclui suporte ao MQTT no HiveMQ Cloud

// Configuracoes da rede WiFi
const char* ssid = "APARTAMENTO93"; //SSID da rede WiFi
const char* password = "senha"; //senha da rede WiFi
const char* mqtt_server = "broker.mqtt-dashboard.com"; //URL do broker MQTT
const int mqtt_server_port = 1883; //porta do broker MQTT

// Variaveis globais e objetos
#define MSG_BUFFER_SIZE (500) //define MSG_BUFFER_SIZE como 500
WiFiClient client; //cria o objeto client
PubSubClient mqtt_client(client); //cria o objeto mqtt_client
long lastMsg = 0;

String clientID = "ESP8266Client-"; //identificacao do cliente

String topicoPrefixo = "MACK21002851"; //para o prefixo do topico, utilizar MACK seguido do TIA
String topicoTodos = topicoPrefixo + "/#"; //para retornar todos os topicos
String topico_0 = topicoPrefixo + "/hello"; //topico para teste
String mensagem_0 = "ArduinoUNO Connected"; //mensagem para o topico 0
String topico_11 = topicoPrefixo + "/sensor2"; //topico para o sensor de temperatura
String mensagem_11 = ""; //mensagem para o topico 11
String topico_2 = topicoPrefixo + "/atuador1"; //topico para o atuador servo 1
String topico_22 = topicoPrefixo + "/atuador1"; //topico para o atuador servo 2
String topico_222 = topicoPrefixo + "/atuador1"; //topico para o atuador ICD
String mensagem_2 = ""; //mensagem para o topico 2
String mensagem_22 = ""; //mensagem para o topico 22
String mensagem_222 = ""; //mensagem para o topico 222
String mensagemTemp = ""; //armazena temporariamente as mensagens recebidas via MQTT


// Funcao para configurar a conexao WiFi
void setup_wifi() {
  //WiFi.mode(WIFI_STA); //ativa o modo de estacao
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

// Funcao para receber mensagens
void callback(char* topic, byte* payload, unsigned int length) {
  mensagemTemp = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    mensagemTemp += (char)payload[i];
  }

  if (String(topic) == topico_2) {
    mensagem_11 = mensagemTemp;
  }

  if (String(topic) == topico_22) {
    mensagem_222 = mensagemTemp;
  }

// Funcao para conectar no broker MQTT
void reconnect() {
  // Loop until we’re reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection…");

  // Create a random client ID
    randomSeed(micros()); //inicializa a semente do gerador de numeros aleatorios
    clientID += String(random(0xffff), HEX);

  // Attempt to connect
    if (mqtt_client.connect(clientID.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqtt_client.publish(topico_0.c_str(), mensagem_0.c_str());
      // ... and resubscribe
      mqtt_client.subscribe(topicoTodos.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
} 

//=======  
// inicio
//======= 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27,20,4);

Servo myservo1;  
Servo myservo2; 

#define red 13
#define haha 4
int pos1,pos2;

void setup() {
  Serial.begin(9600);
  pinMode(red,OUTPUT);
  pinMode(haha,INPUT);
  myservo1.attach(7);  
  myservo2.attach(8); 
  myservo1.write(90);
  myservo2.write(90);
  mlx.begin();  
 
  lcd.init();
  lcd.backlight();

  setup_wifi();

  mqtt_client.setServer(mqtt_server, mqtt_server_port); //configura a conexao com o broker MQTT
  mqtt_client.setCallback(callback); //configura o recebimento de mensagens 
  
}

void loop() {
  if (!mqtt_client.connected()) {
    reconnect();
  }
  mqtt_client.loop(); //processa as mensagens e mantem a conexao com o broker MQTT
    
        int montion = digitalRead(haha);
        
        if(montion == 1){
          
          Serial.print("made");
          Serial.print("\n");
          lcd.setCursor(0,0);
          lcd.print("ObjectTemp:");
          lcd.setCursor(13,3);
          lcd.print("-MEDINA");          
          
          readtemp();
          }   
}
          
void readtemp(){
  
   int temp_obj = mlx.readObjectTempC();
   Serial.print(temp_obj);
   Serial.print("\n");
          if(temp_obj < 31){
           lcd.setCursor(0,1);
           lcd.print(temp_obj);
           lcd.setCursor(0,2);
           lcd.print("Not detected,retest!");
           mensagem_222 = "Not detected,retest!";
           mensagem_11 = temp_obj;
              
          }if(temp_obj > 30 && temp_obj <38){
           
           lcd.setCursor(0,1);
           lcd.print(temp_obj);
           lcd.setCursor(0,2);
           lcd.print("body temperature ok "); 
           mensagem_222 = "body temperature ok ";
           mensagem_11 = temp_obj;
           for(pos1 = 90; pos1 <= 180; pos1 += 1)
          {                                 
            myservo1.write(pos1);
            myservo2.write(180-pos1);         
            delay(15);
          }
            delay(5000);

           for(pos1 = 180; pos1>=90; pos1 -=1)
           {
            myservo1.write(pos1);
            myservo2.write(180-pos1);     
            delay(15);                     
           }

          } 
          if(temp_obj>37){
            digitalWrite(red,HIGH);
            lcd.setCursor(0,1);
            lcd.print(temp_obj);
            lcd.setCursor(0,2);
            lcd.print("     Keep out!       ");
            mensagem_222 = "     Keep out!       ";
            mensagem_11 = temp_obj;
            delay(500);
            
  }
  digitalWrite(red,LOW);
  
  //Publica mensagem
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    mqtt_client.publish(topico_11.c_str(), mensagem_11.c_str());
    mqtt_client.publish(topico_222.c_str(), mensagem_222.c_str());
  }

}            
