# mack_projeto-IoT
Mackenzie - IoT ESP32/MQTT

Projeto de automação para aquário utilizando ESP32 versão WROOM32 programado em Arduino IDE. 
O Sensor de temperatura DS18B20 tipo sonda realiza a leitura na água do aquário e o programa interpreta na faixa abaixo de 27ºC (Temperatura baixa e led azul aceso), entre
27 e 30 ºC (temperatura normal e led verde aceso) ou acima de 30ºC (temperatura alta e led vermelho aceso) com alerta no Serial Monitor. O microcontrolador ESP32 é alimentado
diretamente através da conexão USB. 
O servomotor SG90 é alimentado por fonte externa independente e ligado ao regulador de tensão para trabalhar com 5V, sendo sua função de girar o recipiente com alimento
para despejar no aquário. O acionamento do servo ocorre por botão simples tipo "push button" que utiliza protocolo MQTT para lançar publish ao BROKER (eclipse foi utilizado
por ser alternativa confiável e gratuita) e enviar o comando ao motor.
Sugestão de futuro desenvolvimento: substituir o botão de comando ao servo por um aplicativo (possivelmente desenvolvido em react native) para o acionamento remoto através
de celular.
