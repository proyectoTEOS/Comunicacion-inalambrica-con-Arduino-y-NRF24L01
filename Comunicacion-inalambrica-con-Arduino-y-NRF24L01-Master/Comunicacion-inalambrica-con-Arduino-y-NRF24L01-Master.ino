/*
  Created by TEOS
  Domotic with Arduino https://goo.gl/btZpjs
  YouTube https://goo.gl/k7TUSZ
  Instagram https://goo.gl/zEIjia
  Facebook https://goo.gl/eivraR
  Twitter https://goo.gl/du5Wgn
  Github https://goo.gl/Xl5IiS
  Google Plus https://goo.gl/gBnUdh
  WEB https://www.proyecto-teos.com/
*/

//Library for the NRF module
#include <RH_NRF24.h> //https://github.com/PaulStoffregen/RadioHead
//Library that allows communication with I2C / TWI devices.
#include <Wire.h>
//Library for LCD with I2C protocol.
#include <LiquidCrystal_I2C.h> //https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library

//Constants for the Arduino pins.
const uint8_t cePinT = 9, csnPinT = 10, speakerPinT = 3;

//Variables with start values.
uint8_t speakerStatusT = LOW, varVolumeT = 20, volumeT;
bool continueT = true;
unsigned long previousMillisT;

//Structure for data transmission.
struct dataCollectionT {
  float temperatureT, voltageT;
  uint16_t soilMoistureT;
  uint8_t humidityT;
} _dataCT;

//Constructors.
RH_NRF24 nrf24(cePinT, csnPinT);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  lcd.begin();
  lcd.setCursor(2, 0);
  lcd.print("powered by TEOS");
  lcd.setCursor(1, 1);
  lcd.print("Iniciando sistema");

  Serial.begin(9600);
  pinMode(speakerPinT, OUTPUT);

  //NRF module verification.
  const uint8_t channelRFT = 1;
  while (!nrf24.init())
    Serial.print(F("Error al iniciar el modulo RF\n"));
  Serial.print(F("Modulo RF iniciado\n"));
  while (!nrf24.setChannel(channelRFT))
    Serial.print(F("Error en el conjunto de canal\n"));
  Serial.print(F("Canal de conjunto iniciado\n"));
  while (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.print(F("Error en el conjunto RF\n"));
  Serial.print(F("RF de conjunto iniciado\n\n"));

  previousMillisT = millis();
}

void loop() {
  //Check module availability.
  if (nrf24.available()) {
    //Determines the size of the received message.
    uint8_t bufferT[RH_NRF24_MAX_MESSAGE_LEN], bufLenT = sizeof(bufferT);
    //If parity is achieved with the device and you receive messages,
    //then there is new information in the buffer.
    if (nrf24.recv(bufferT, &bufLenT)) {
      //The memcpy statement copies the byte values from the location pointed
      //to by the source directly to the memory block pointed to by the
      //destination. Data can now be accessed using _dataCT.
      memcpy(&_dataCT, bufferT, sizeof(_dataCT));

      //Send the data to a function that displays the data using a 20x4 display.
      printData(_dataCT.temperatureT, _dataCT.humidityT, _dataCT.soilMoistureT,
                _dataCT.voltageT);
    }
  }
  emergingAction();
}

void printData(float temperatureT, uint8_t humidityT, uint8_t soilMoistureT, float voltageT) {
  //Tells the program which messages to display.
  if (continueT) {
    lcd.clear();
    lcd.print("Humedad suelo: ");
    lcd.print(soilMoistureT);
    lcd.setCursor(17, 0);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("Temperatura: ");
    lcd.print(temperatureT, 1);
    lcd.setCursor(17, 1);
    lcd.print("C");

    lcd.setCursor(0, 2);
    lcd.print("Humedad: ");
    lcd.print(humidityT);
    lcd.setCursor(11, 2);
    lcd.print("%");

    //To indicate the value of the battery in percentage levels, a function
    //is invoked that returns the value by remaining approximation of the same.
    lcd.setCursor(0, 3);
    lcd.print("Bateria: ");
    lcd.print(getLevelBattery(voltageT));
    lcd.setCursor(11, 3);
    lcd.print("%");
  } else {
    lcd.clear();
    lcd.print("  La bateria tiene");
    lcd.setCursor(0, 1);
    lcd.print("menos del 10% de su");
    lcd.setCursor(0, 2);
    lcd.print("capacidad, evita un");
    lcd.setCursor(0, 3);
    lcd.print(" deterioro, cargala");
  }
}

uint8_t getLevelBattery(float voltageT) {
  //The following variables correspond to the battery values:
  //voltageMaxT = maximum voltage given by the battery
  //voltageMinT = minimum battery voltage
  //The variable scaleT corresponds to the number of jumps it will make
  //from the minimum value to the maximum battery voltage. Seen in
  //another way, it goes from 0 to 100 with 20 jumps from 5 to 5 (20 in total)

  uint8_t percMaxT = 100, percMinT = 0, percentageT;
  float voltageMaxT = 4.20, voltageMinT = 3.20;
  float scaleT = 0.05, refVoltageT = voltageMaxT;

  //The next cycle uses a mathematical method known as interpolation, we use
  //it to find out the next average percentages given the maximum and
  //minimum voltage as a reference at their ends.
  for (uint8_t incT = 0; incT < 21; incT++) {
    uint8_t interpolationPercT = percMaxT + (percMinT - percMaxT) *
                                 ((voltageMaxT - (scaleT * incT)) - voltageMaxT) /
                                 (voltageMinT - voltageMaxT);
    refVoltageT -= scaleT;

    if (refVoltageT <= voltageT) {
      percentageT = interpolationPercT;
      break;
    } else
      percentageT = (voltageT < refVoltageT) ? 0 : 100;
  }

  //continueT has a boolean value which is assigned by a ternary operation
  //that evaluates the voltage level.
  continueT = (percentageT <= 10) ? false : true;
  return percentageT;
}

//You can do any action with the values you receive, in this case, we sound
//a buzzer with modulated pulses that notify that something is wrong
//in the system. To prevent this action from interrupting other tasks,
//it is triggered as a parallel task, similar to multitasking.
void emergingAction() {
  const uint16_t intervalT = 500;
  unsigned long currentMillisT = millis();

  if ((unsigned long)(currentMillisT - previousMillisT) >= intervalT) {
    if (!continueT) {
      //Due to the connection of the buzzer with the PWM pin of our Arduino,
      //we can modify the frequency with which we make the buzzer sound,
      //to increase or decrease the volume, modify the value of the
      //variable volumeT.
      speakerStatusT = (speakerStatusT) ? LOW : HIGH;
      volumeT = (speakerStatusT) ? varVolumeT : 0;
      analogWrite(speakerPinT, volumeT);
    }
    previousMillisT = millis();
  }
}
