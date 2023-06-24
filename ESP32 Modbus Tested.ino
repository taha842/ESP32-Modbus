/*
The ESP is connected to a max485 with pins 17 (RX), 4 (TX) and 16 as RTS.
*/

#include <Arduino.h>
#include <esp32ModbusRTU.h>
#include <algorithm>  // for std::reverse

esp32ModbusRTU modbus(&Serial1, 4);  // use Serial1 and pin 16 as RTS

void handleData(uint8_t serverAddress, esp32Modbus::FunctionCode fc, uint8_t* data, size_t length) {
  Serial.printf("received data: id: %u, fc %u\ndata: 0x", serverAddress, fc);
  for (uint8_t i = 0; i < length; ++i) {
    Serial.printf("%02x", data[i]);
  }
  Serial.print("\n");
}

void setup() {
  Serial.begin(115200);                             // Serial output
  Serial1.begin(19200, SERIAL_8N1, 16, 17, false);  // Modbus connection

  modbus.onData([](uint8_t serverAddress, esp32Modbus::FunctionCode fc, uint16_t address, uint8_t* data, size_t length) {
    Serial.printf("id 0x%02x fc 0x%02x len %u: 0x", serverAddress, fc, length);
    for (size_t i = 0; i < length; ++i) {
      Serial.printf("%02x", data[i]);
    }
    Serial.print("\n\n");
  });
  modbus.onError([](esp32Modbus::Error error) {
    Serial.printf("error: 0x%02x\n\n", static_cast<uint8_t>(error));
  });
  modbus.begin();
}

void loop() {
  static uint32_t lastMillis = 0;
  if (millis() - lastMillis > 1500) {
    lastMillis = millis();
    Serial.print("sending Modbus request...\n");
    //modbus.readInputRegisters(0x01, 1, 1);
    modbus.readHoldingRegisters(0x01, 1, 1);
    modbus.readInputRegisters(0x01, 3, 1);

    modbus.readDiscreteInputs(1, 1, 5);
  }
}
