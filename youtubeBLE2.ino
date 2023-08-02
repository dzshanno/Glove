/*
  run on Seeed Xiao with glove and flex sensors

  This example scans for BLE peripherals until one with the advertised service
  "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found.

*/

#include <ArduinoBLE.h>

#define MyLocalName "XIAO BLE"
#define FLEX1_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd70"
#define FLEX2_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd71"
#define FLEX3_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd72"
#define FLEX4_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd73"
#define FLEX5_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd74"

// Set the input pins the flex sensors are connected to

int Flex1_pin = A0;
int Flex2_pin = A1;
int Flex3_pin = A2;
int Flex4_pin = A4;
int Flex5_pin = A5;

void setup()
{
    Serial.begin(9600);

    // initialize the BLE hardware
    BLE.begin();

    Serial.println("BLE Central - SERVO control");

    // start scanning for peripherals
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
}

void loop()
{
    // check if a peripheral has been discovered
    BLEDevice peripheral = BLE.available();

    if (peripheral)
    {
        // discovered a peripheral, print out address, local name, and advertised service
        Serial.print("Found ");
        Serial.print(peripheral.address());
        Serial.print(" '");
        Serial.print(peripheral.localName());
        Serial.print("' ");
        Serial.print(peripheral.advertisedServiceUuid());
        Serial.println();

        if (peripheral.localName() != MyLocalName)
        {
            return;
        }

        // stop scanning
        BLE.stopScan();

        controlLed(peripheral);

        // peripheral disconnected, start scanning again
        Serial.print("scanning for peripheral again after disconnect");
        Serial.println();
        BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
    }
}

void controlLed(BLEDevice peripheral)
{
    // connect to the peripheral
    Serial.println("Connecting ...");

    if (peripheral.connect())
    {
        Serial.println("Connected");
    }
    else
    {
        Serial.println("Failed to connect!");
        return;
    }

    // discover peripheral attributes
    Serial.println("Discovering attributes ...");
    if (peripheral.discoverAttributes())
    {
        Serial.println("Attributes discovered");
    }
    else
    {
        Serial.println("Attribute discovery failed!");
        peripheral.disconnect();
        return;
    }

    // retrieve the LED/OLED characteristic
    BLECharacteristic flex1Characteristic = peripheral.characteristic(FLEX1_CHARACTERISTIC_UUID);
    BLECharacteristic flex2Characteristic = peripheral.characteristic(FLEX2_CHARACTERISTIC_UUID);
    BLECharacteristic flex3Characteristic = peripheral.characteristic(FLEX3_CHARACTERISTIC_UUID);
    BLECharacteristic flex4Characteristic = peripheral.characteristic(FLEX4_CHARACTERISTIC_UUID);
    BLECharacteristic flex5Characteristic = peripheral.characteristic(FLEX5_CHARACTERISTIC_UUID);

    while (peripheral.connected())
    {
        // while the peripheral is connected

        sendflexvalue(Flex1_pin, flex1Characteristic, "Flex1", 240, 170);
        sendflexvalue(Flex2_pin, flex2Characteristic, "Flex2", 450, 260);
        sendflexvalue(Flex3_pin, flex3Characteristic, "Flex3", 380, 260);
        sendflexvalue(Flex4_pin, flex4Characteristic, "Flex4", 320, 230);
        sendflexvalue(Flex5_pin, flex5Characteristic, "Flex5", 415, 280);
        delay(100);
    }

    Serial.println("Peripheral disconnected");
}

void sendflexvalue(int pin, BLECharacteristic characteristic, String name, int straight, int bent)
{

    int Flexvalue = analogRead(pin);
    Flexvalue = map(Flexvalue, straight, bent, 10, 90);
    // BLE can only take a value up to 255 so check if its too high
    if (Flexvalue > 255)
    {
        Serial.print("SPIKE - ");
        Serial.print(Flexvalue);
        Flexvalue = 255;
    }
    // write the flex sensort value to the BLE characteristic
    characteristic.writeValue(String(Flexvalue).c_str());
    Serial.print(name);
    Serial.print(" Value is ...");
    Serial.println(Flexvalue);
}