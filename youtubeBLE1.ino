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

// include servo library
#include <Servo.h>

// Set the output pins that the servos are connected to

int Finger_pins[5] = {A4, A0, A1, A4, A5};
int fullbent[5] = {10, 10, 10, 10, 10};
int straight[5] = {170, 170, 170, 170, 170};

// create servo objects and put in an array
Servo servos[5];

#define MyLocalName "XIAO BLE"
#define FLEX1_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd70"
#define FLEX2_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd71"
#define FLEX3_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd72"
#define FLEX4_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd73"
#define FLEX5_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd74"

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/U8X8_PIN_NONE);

BLEService flexService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic switchCharacteristic(
    "19B10001-E8F2-537E-4F6C-D104768A1214",
    BLERead | BLEWrite);
BLEStringCharacteristic flex1Characteristic(
    FLEX1_CHARACTERISTIC_UUID,
    BLERead | BLEWrite, 16);
BLEStringCharacteristic flex2Characteristic(
    FLEX2_CHARACTERISTIC_UUID,
    BLERead | BLEWrite, 16);
BLEStringCharacteristic flex3Characteristic(
    FLEX3_CHARACTERISTIC_UUID,
    BLERead | BLEWrite, 16);
BLEStringCharacteristic flex4Characteristic(
    FLEX4_CHARACTERISTIC_UUID,
    BLERead | BLEWrite, 16);
BLEStringCharacteristic flex5Characteristic(
    FLEX5_CHARACTERISTIC_UUID,
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

    // attach servos

    servos[0].attach(Finger_pins[0]);
    servos[1].attach(Finger_pins[1]);
    servos[2].attach(Finger_pins[2]);
    servos[3].attach(Finger_pins[3]);
    servos[4].attach(Finger_pins[4]);

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
    flexService.addCharacteristic(flex2Characteristic);
    flexService.addCharacteristic(flex3Characteristic);
    flexService.addCharacteristic(flex4Characteristic);
    flexService.addCharacteristic(flex5Characteristic);

    // add service
    BLE.addService(flexService);

    // assign event handlers for connected, disconnected to peripheral
    BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
    BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

    // assign event handlers for characteristic
    switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);
    flex1Characteristic.setEventHandler(BLEWritten, Flex1CharacteristicWritten);
    flex2Characteristic.setEventHandler(BLEWritten, Flex2CharacteristicWritten);
    flex3Characteristic.setEventHandler(BLEWritten, Flex3CharacteristicWritten);
    flex4Characteristic.setEventHandler(BLEWritten, Flex4CharacteristicWritten);
    flex5Characteristic.setEventHandler(BLEWritten, Flex5CharacteristicWritten);
    // set an initial value for the characteristic
    switchCharacteristic.setValue(0);
    flex1Characteristic.setValue("I'm flex 1");
    flex2Characteristic.setValue("I'm flex 2");
    flex3Characteristic.setValue("I'm flex 3");
    flex4Characteristic.setValue("I'm flex 4");
    flex5Characteristic.setValue("I'm flex 5");

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

void Flex1CharacteristicWritten(BLEDevice central,
                                BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update SCREEN
    Serial.println("mySttringCharacteristic event, written: ");

    Serial.println("flex1Characteristic received: len=" +
                   String(flex1Characteristic.valueLength()));
    String valString = flex1Characteristic.value();
    Serial.println(valString);
    MoveServo(1, valString.toInt());

    // u8x8.clear();
    u8x8.setCursor(0, 0);
    u8x8.print(valString);

    Serial.println();
}

void Flex2CharacteristicWritten(BLEDevice central,
                                BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update SCREEN
    Serial.println("mySttringCharacteristic event, written: ");

    Serial.println("flex2Characteristic received: len=" +
                   String(flex2Characteristic.valueLength()));
    String valString = flex2Characteristic.value();
    Serial.println(valString);
    MoveServo(2, valString.toInt());

    // u8x8.clear();
    u8x8.setCursor(0, 1);
    u8x8.print(valString);

    Serial.println();
}
void Flex3CharacteristicWritten(BLEDevice central,
                                BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update SCREEN
    Serial.println("mySttringCharacteristic event, written: ");

    Serial.println("flex3Characteristic received: len=" +
                   String(flex3Characteristic.valueLength()));
    String valString = flex3Characteristic.value();
    Serial.println(valString);
    MoveServo(3, valString.toInt());

    // u8x8.clear();
    u8x8.setCursor(0, 2);
    u8x8.print(valString);

    Serial.println();
}
void Flex4CharacteristicWritten(BLEDevice central,
                                BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update SCREEN
    Serial.println("mySttringCharacteristic event, written: ");

    Serial.println("flex4Characteristic received: len=" +
                   String(flex4Characteristic.valueLength()));
    String valString = flex4Characteristic.value();
    Serial.println(valString);
    MoveServo(4, valString.toInt());

    // u8x8.clear();
    u8x8.setCursor(0, 3);
    u8x8.print(valString);

    Serial.println();
}
void Flex5CharacteristicWritten(BLEDevice central,
                                BLECharacteristic characteristic)
{
    // central wrote new value to characteristic, update SCREEN
    Serial.println("myStringCharacteristic event, written: ");

    Serial.println("flex5Characteristic received: len=" +
                   String(flex5Characteristic.valueLength()));
    String valString = flex5Characteristic.value();
    Serial.println(valString);

    MoveServo(5, valString.toInt());
    // u8x8.clear();
    u8x8.setCursor(0, 4);
    u8x8.print(valString);

    Serial.println();
}

// move a given servo to a specific position
void MoveServo(int finger, int position)
{
    position = map(position, 0, 100, straight[finger], fullbent[finger]);
    servos[finger].write(position);
}