# PROJ_OBJ_INT
O propósito deste projeto com arduino é construir um protótipo de porta ou portão inteligente, captando a presença de um corpo humano a partir do uso de um sensor de presença capaz dedetectar a radiação infravermelha emitida pelos corpos, somado a um sensor de temperatura infravermelho o qual funcionará como termômetro para medições de temperatura sem contato.

Os seguintes elementos foram utilizados no projeto:<br />
-Arduino UNO<br />
-LCD2004 I2C <br />
-Módulo Sensor De Temperatura Infravermelho GY-906-BCC IR<br />
-Micro Servo MG90S TowerPro<br />
-Sensor de presença HC-SR501<br />
-Esp-01 Esp8266<br />

Software utilizado:<br />
-Arduino Software (IDE)<br />

*É realizada uma conectividade com a nuvem através de MQTT (Message Queue Telemetry Transport), dispolibilizando as medidas de temperatura e mensagens do display pelo microcontrolador ESP-01.


<br /><br /><br />
![projeto_bb](https://user-images.githubusercontent.com/53839422/169926782-3a75cdce-1299-4f2a-b131-80570bd50a4b.jpg)
