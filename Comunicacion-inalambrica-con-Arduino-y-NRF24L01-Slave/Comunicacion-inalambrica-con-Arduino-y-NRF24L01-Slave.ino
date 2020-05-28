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

//Library for the NRF module.
#include <RH_NRF24.h> //https://github.com/PaulStoffregen/RadioHead
//Library for the DHT module.
#include "DHT.h" //https://github.com/adafruit/DHT-sensor-library

//Constants for the Arduino pins.
const uint8_t sensorPinDHT = 2, cePinT = 9, csnPinT = 10, batPinT = A0,
              soilPinT = A1, typeT = DHT22;

//Structure for data transmission.
struct dataCollectionT {
  float temperatureT, voltageT;
  uint16_t soilMoistureT;
  uint8_t humidityT;
} _dataCT;

//Constructors.
RH_NRF24 nrf24(cePinT, csnPinT);
DHT dht(sensorPinDHT, typeT);

void setup() {
  Serial.begin(9600);
  dht.begin();

  const uint8_t channelRFT = 1;

  //NRF module verification.
  while (!nrf24.init())
    Serial.print(F("Error al iniciar el modulo RF\n"));
  Serial.print(F("Modulo RF iniciado\n"));
  while (!nrf24.setChannel(channelRFT))
    Serial.print(F("Error en el conjunto de canal\n"));
  Serial.print(F("Canal de conjunto iniciado\n"));
  while (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.print(F("Error en el conjunto RF\n"));
  Serial.print(F("RF de conjunto iniciado\n\n"));
}

void loop() {
  char toCharTempT[5], toCharHumidityT[5], toCharVoltageT[5], toCharSoilT[3],
       resultSerialT[35];

  //Get the DHT sensor data.
  _dataCT.temperatureT = dht.readTemperature();
  _dataCT.humidityT = dht.readHumidity();

  //Check if the obtained data are numbers.
  if (isnan(_dataCT.temperatureT) || isnan(_dataCT.humidityT)) {
    Serial.println(F("No es posible obtener datos del sensor DHT"));
    return;
  }

  //Get the battery voltage.
  _dataCT.voltageT = (float)analogRead(batPinT) * (5.0 / 1023.0);

  //Collects the percentage of soil moisture.
  _dataCT.soilMoistureT = map(analogRead(soilPinT), 250, 1023, 100, 0);
  _dataCT.soilMoistureT = constrain (_dataCT.soilMoistureT, 0, 100);

  uint8_t zizeBufferT = sizeof(_dataCT);
  uint8_t bufferForStructT[zizeBufferT] = {0};
  //The memcpy statement copies the byte values from the location pointed
  //to by the source directly to the memory block pointed to by
  //the destination.
  memcpy(bufferForStructT, &_dataCT, zizeBufferT);

  //Send a frame to the receiving module.
  nrf24.send((uint8_t *)bufferForStructT, zizeBufferT);

  //The dtostrf instruction allows you to convert a number to a string.
  dtostrf(_dataCT.voltageT, 1, 2, toCharVoltageT);
  dtostrf(_dataCT.temperatureT, 2, 1, toCharTempT);
  dtostrf(_dataCT.humidityT, 3, 0, toCharHumidityT);
  dtostrf(_dataCT.soilMoistureT, 3, 0, toCharSoilT);

  //To copy one string to another you can use strcpy;
  //strcat concatenates the strings adjacent to a new string (array)
  strcpy(resultSerialT, toCharVoltageT);
  strcat(resultSerialT, "V || ");
  strcat(resultSerialT, toCharTempT);
  strcat(resultSerialT, "°C || ");
  strcat(resultSerialT, toCharHumidityT);
  strcat(resultSerialT, "%H || ");
  strcat(resultSerialT, toCharSoilT);
  strcat(resultSerialT, "%HT");

  //Everything in the string is printed. The String function can be
  //replaced by all strings of type char, but this will affect the
  //performance of the code.
  Serial.println(resultSerialT);
  delay(10000);
}
