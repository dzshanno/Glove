/*
  Run on XIAO BLESensor:

  This example creates a BLE peripheral with service that contains a
  characteristic to control an LED and OLED. The callback features of the
  library are used.

  Modified from ArduinoBLE > Peripheral > CallbackLED example

*/

#include <ArduinoBLE.h>
#include <Arduino.h>
#include <U8x8lib.h>
#include <Wire.h>

#define MyLocalName "XIAO BLE"
#define MyStringChar_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd70"

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/U8X8_PIN_NONE);

BLEService flexService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic switchCharacteristic(
    "19B10001-E8F2-537E-4F6C-D104768A1214",
    BLERead | BLEWrite);
BLEStringCharacteristic flex1Characteristic(
    MyStringChar_UUID,
    BLERead | BLEWrite, 16);

const int ledPin = LED_BUILTIN; // pin to use for the LED

void setup()
{
    u8x8.begin();
    u8x8.setFlipMode(1); // rotary 180

    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.setCursor(0, 0);
    u8x8.print("XIAO BLE");

    Serial.begin(9600);

    // comment to skip Serial port waiting,
    // such that it can sork stand alone without computer.
    // while (!Serial);

    pinMode(ledPin, OUTPUT); // use the LED pin as an output

    // begin initialization
    if (!BLE.begin())
    {
        Serial.println("starting BLE failed!");

        while (1)
            ;
    }

    // set the local name peripheral advertises
    BLE.setLocalName(MyLocalName);
    // set the UUID for the service this peripheral advertises
    BLE.setAdvertisedService(flexService);

    // add the characteristic to the service
    flexService.addCharacteristic(switchCharacteristic);
    flexService.addCharacteristic(flex1Characteristic);

    // add service
    BLE.addService(flexService);

    // assign event handlers for connected, disconnected to peripheral
    BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
    BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

    // assign event handlers for characteristic
    switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);
    flex1Characteristic.setEventHandler(BLEWritten, myStringCharacteristicWritten);
    // set an initial value for the characteristic
    switchCharacteristic.setValue(0);
    flex1Characteristic.setValue("XIAO BLE");

    // start advertising
    BLE.advertise();

    Serial.println(("Bluetooth device active, waiting for connections..."));
}

void loop()
{
    // poll for BLE events
    BLE.poll();
}

void blePeripheralConnectHandler(BLEDevice central)
{
    // central connected event handler
    Serial.print("Connected event, central: ");
    Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central)
{
    // central disconnected event handler
    Serial.print("Disconnected event, central: ");
    Serial.println(central.address());
}

void switchCharacteristicWritten(BLEDevice central,
                                 BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update LED
    Serial.print("Characteristic event, written: ");

    if (switchCharacteristic.value())
    {
        Serial.println("LED on");
        digitalWrite(ledPin, !HIGH);
    }
    else
    {
        Serial.println("LED off");
        digitalWrite(ledPin, !LOW);
    }
}

void myStringCharacteristicWritten(BLEDevice central,
                                   BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update SCREEN
    Serial.println("mySttringCharacteristic event, written: ");

    Serial.println("flex1Characteristic received: len=" +
                   String(flex1Characteristic.valueLength()));
    String valString = flex1Characteristic.value();
    Serial.println(valString);

    u8x8.clear();
    u8x8.setCursor(0, 0);
    u8x8.print(valString);

    Serial.println();
}