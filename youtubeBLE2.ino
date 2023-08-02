/*
  run on Aduino RP2040 NANO Connect

  This example scans for BLE peripherals until one with the advertised service
  "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
  it will remotely control the BLE Peripheral's LED, when the button is pressed or released.

  And also get user input from Serial Monitor, send to BLE Peripheral

  modified from ArduinoBLE > Central > LedControl
*/

#include <ArduinoBLE.h>

// variables for button
const int buttonPin = 2;
int oldButtonState = LOW;

#define MyLocalName "XIAO BLE"
#define FLEX1_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd70"
#define FLEX2_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd71"
#define FLEX3_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd72"
#define FLEX4_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd73"
#define FLEX5_CHARACTERISTIC_UUID "22a28815-c6bd-401b-a0f1-d47c42a0bd74"

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    // configure the button pin as input
    pinMode(buttonPin, INPUT_PULLUP);

    // initialize the BLE hardware
    BLE.begin();

    Serial.println("BLE Central - LED control");

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
    BLECharacteristic ledCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
    BLECharacteristic flex1Characteristic = peripheral.characteristic(FLEX1_CHARACTERISTIC_UUID);
    BLECharacteristic flex2Characteristic = peripheral.characteristic(FLEX2_CHARACTERISTIC_UUID);
    BLECharacteristic flex3Characteristic = peripheral.characteristic(FLEX3_CHARACTERISTIC_UUID);
    BLECharacteristic flex4Characteristic = peripheral.characteristic(FLEX4_CHARACTERISTIC_UUID);
    BLECharacteristic flex5Characteristic = peripheral.characteristic(FLEX5_CHARACTERISTIC_UUID);

    /*
    if (!ledCharacteristic) {
      Serial.println("Peripheral does not have LED characteristic!");
      peripheral.disconnect();
      return;
    } else if (!ledCharacteristic.canWrite()) {
      Serial.println("Peripheral does not have a writable LED characteristic!");
      peripheral.disconnect();
      return;
    }
    */
    int a = 1;
    int step = 10;
    while (peripheral.connected())
    {
        // while the peripheral is connected

        // read the button pin
        int buttonState = !digitalRead(buttonPin);

        if (oldButtonState != buttonState)
        {
            // button changed
            oldButtonState = buttonState;

            if (buttonState)
            {
                Serial.println("button pressed");

                // button is pressed, write 0x01 to turn the LED on
                ledCharacteristic.writeValue((byte)0x01);
            }
            else
            {
                Serial.println("button released");

                // button is released, write 0x00 to turn the LED off
                ledCharacteristic.writeValue((byte)0x00);
            }
        }

        if (Serial.available() > 0)
        {
            String stringIn = Serial.readString();
            Serial.println(stringIn);
            flex1Characteristic.writeValue(stringIn.c_str());
        }
        a = a + step;
        if (a > 100)
        {
            a = 5;
        }
        flex1Characteristic.writeValue(String(a).c_str());
        flex2Characteristic.writeValue(String(a + 1).c_str());
        flex3Characteristic.writeValue(String(a + 2).c_str());
        flex4Characteristic.writeValue(String(a + 3).c_str());
        flex5Characteristic.writeValue(String(a + 4).c_str());
        delay(1000);
    }

    Serial.println("Peripheral disconnected");
}
