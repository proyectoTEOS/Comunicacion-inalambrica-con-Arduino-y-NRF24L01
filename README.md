# Comunicación inalámbrica con Arduino y NRF24L01
En este proyecto usaremos los módulos NRF24L01 para la transmisión y recepción de datos. En la parte de la transmisión tendremos un sensor de temperatura y humedad relativa, el DHT22. Para detectar la humedad en la tierra usaremos el sensor capacitivo de humedad de suelo. Debido a que es un proyecto de transmisión inalámbrica, lo ideal es que su fuente de energía sea autónoma, por lo que usaremos una celda solar, un gestor de carga y descarga y una batería para poder alimentar a los sensores y a un Arduino NANO. Recordemos que el voltaje que entrega la batería es de hasta 4.2 volts y la entrada mínima de voltaje recomendado para operar el Arduino NANO es de 7 volts por los que necesitaremos un elevador de voltaje, conocidos como boost. Para el dispositivo receptor solo necesitamos un display LCD 20 x 4, un Arduino UNO y una antena NRF. También necesitaremos de componentes electrónicos, pero estos son más de forma opcional. Podrás encontrar este proyecto con más detalles aquí [Comunicación inalámbrica con Arduino y NRF24L01](https://www.proyecto-teos.com/comunicacion-inalambrica-con-arduino-y-nrf24l01).

### Empezando
Este proyecto se basa en los sensores que hemos usado anteriormente, si no tienes mucha experiencia en el manejo de dichos sensores puedes consultar nuestros repositorios.

- LCD 20x4 https://github.com/proyectoTEOS/Display-LCD-16x2-con-Arduino
- Buzzer https://github.com/proyectoTEOS/Buzzer-con-Arduino
- Sensor de temperatura y humedad DHT22 https://github.com/proyectoTEOS/Sensor-de-temperatura-y-humedad-DHT22-con-Arduino
- Sensor capacitivo de humedad de suelo https://github.com/proyectoTEOS/Sensor-capacitivo-de-humedad-de-suelo-con-Arduino

### Requisitos
- Tener el IDE de [Arduino](https://www.arduino.cc/en/Main/Software) (o el de tu preferencia) con la versión más actual
- Contar con las librerías [NRF24](https://github.com/PaulStoffregen/RadioHead), [DHT](https://github.com/adafruit/DHT-sensor-library),
[LiquidCrystal_I2C]https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library y Wire (se incluye con el IDE).
- Tener el material para hacer el proyecto (claro)

### Materiales
Enlistaremos los materiales que se necesitan para realizar el proyecto, pero puedes conseguir componentes similares.
- 1 Arduino UNO R3
- 1 Arduino NANO R3
- 1 Sensor capacitivo de humedad de suelo
- 1 Sensor de temperatura y humedad DHT22
- 1 Resistencia de 4.7K ohm
- 1 Resistencia de 1K ohm
- 1 Transistor NPN BC547
- 1 Batería Lipo 1000mah 3.7v
- 1 Interruptor 2 polos 1 tiro
- 1 Boost Step Up 6W 2A
- 1 Cargador de baterías LiPo 5V 1A TP4056
- 1 Diodo rectificador 1N4007
- 1 Celda Solar 5V
- 1 Display LCD de 20x4
- 1 Módulo (adaptador) I2C
- 1 Buzzer pasivo de 5v
- 2 Módulos NRF24L01
- 2 Protoboards

### Códigos
Este proyecto requiere de 2 códigos, por ello es que necesitas 2 Arduinos. El dispositivo emisor será el **[esclavo](https://github.com/proyectoTEOS/Comunicacion-inalambrica-con-Arduino-y-NRF24L01/blob/master/Comunicacion-inalambrica-con-Arduino-y-NRF24L01-Slave/Comunicacion-inalambrica-con-Arduino-y-NRF24L01-Slave.ino)** (Arduino NANO) y el dispositivo receptor será en **[maestro](https://github.com/proyectoTEOS/Comunicacion-inalambrica-con-Arduino-y-NRF24L01/blob/master/Comunicacion-inalambrica-con-Arduino-y-NRF24L01-Master/Comunicacion-inalambrica-con-Arduino-y-NRF24L01-Master.ino)** (Arduino UNO). 

En los enlaces anteriores podrás obtener los códigos, están comentados de tal forma que puedas entender su estructura y función.

### Diagrama
Al igual que los códigos, necesitaras 2 diagramas. Toma en cuenta que el dispositivo con los sensores es el esclavo y el dispositivo con la 
pantalla es el maestro.
![](/resources/Comunicacion-inalambrica-con-Arduino-y-NRF24L01.jpg)

Toma en cuenta las siguientes consideraciones:

- No dejes que el voltaje de la batería baje a menos de 3 volts, ya que esto puede dañar y disminuir su vida útil. Para frenar un poco este evento, colocamos un buzzer el cual emite un sonido para darte aviso de que debes cargar la batería. Si bien el sistema cuenta con una celda solar, no garantiza que pueda cargarla lo suficiente para que pueda operar todo el tiempo, es por ello por lo que debes conseguir una celda con buena capacidad. 
- El dispositivo esclavo puede estar expuesto a la intemperie siempre y cuando tenga una carcasa con certificación que pueda soportar condiciones que desfavorezcan el circuito


Si requieres más información de este y otros proyectos, puedes ir a nuestra [página web](https://www.proyecto-teos.com) y dar clic en la parte de proyectos en donde encontraras los códigos, diagramas, hojas de datos, librerías, recursos y una documentación más extensa.
