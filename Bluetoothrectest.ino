/*
 Test  receiving flex data over bluetooth


*/
// include bluetooth library
#include <ArduinoBLE.h>
// include servo library
#include <Servo.h>
// include Serial library

// Set the output pins that the servos are connected to

int fullbent[5] = {10, 10, 10, 10, 10};
int straight[5] = {170, 170, 170, 170, 170};

// Set input mode to WIRED(0) or BLUETOOTH (1)

bool USE_BLUETOOTH = 1;

const String GLOVESERVICENAME = "Glove";
const char *GloveServiceUUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char *Flex1CharacteristicUUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char *Flex2CharacteristicUUID = "19B10002-E8F2-537E-4F6C-D104768A1214";
const char *Flex3CharacteristicUUID = "19B10003-E8F2-537E-4F6C-D104768A1214";
const char *Flex4CharacteristicUUID = "19B10004-E8F2-537E-4F6C-D104768A1214";
const char *Flex5CharacteristicUUID = "19B10005-E8F2-537E-4F6C-D104768A1214";

void setup()
{

  // begin bluetooth initialization
  if (USE_BLUETOOTH)
  {
    if (!BLE.begin())
    {

      while (1)
        ;
    }

    // start scanning for Glove service by name
    BLE.scanForName(GLOVESERVICENAME);
  }
}

// Start the main programme loop
void loop()
{

  if (USE_BLUETOOTH)
  {
    // check if a peripheral has been discovered
    BLEDevice peripheral = BLE.available();
    if (peripheral)
    {
      // stop scan seeems to stop connection being dropped
      BLE.stopScan();
      // connect to the peripheral

      // discover the attributes of the service
      peripheral.discoverAttributes();

      BLEService Glove_Service = peripheral.service(GloveServiceUUID);

      BLEShortCharacteristic Flex1_characteristic = Glove_Service.characteristic(Flex1CharacteristicUUID);
      BLEShortCharacteristic Flex2_characteristic = Glove_Service.characteristic(Flex2CharacteristicUUID);
      BLEShortCharacteristic Flex3_characteristic = Glove_Service.characteristic(Flex3CharacteristicUUID);
      BLEShortCharacteristic Flex4_characteristic = Glove_Service.characteristic(Flex4CharacteristicUUID);
      BLEShortCharacteristic Flex5_characteristic = Glove_Service.characteristic(Flex5CharacteristicUUID);

      while (peripheral.connected())
      {

        // reads the current value of the characteristics from bluetooth
        Flex1_characteristic.read();
        Flex2_characteristic.read();
        Flex3_characteristic.read();
        Flex4_characteristic.read();
        Flex5_characteristic.read();

        int position[5]; // array to hold the current positions ( percentage bent ofeach finger)

        position[0] = Flex1_characteristic.value();
        position[1] = Flex2_characteristic.value();
        position[2] = Flex3_characteristic.value();
        position[3] = Flex4_characteristic.value();
        position[4] = Flex5_characteristic.value();

        MoveServo(0, position[0]);
        MoveServo(1, position[1]);
        MoveServo(2, position[2]);
        MoveServo(3, position[3]);
        MoveServo(4, position[4]);

        // wait a bit - not sure why?
        delay(1000);
      }
      // when the central disconnects, print it out:

      // then loop back round and listen for a connection from a BLE central device
    }
  }
  else
  {
    // get data direct from input pins
  }
}

// Other functions called by the setup and loop

// move a given servo to a specific position
void MoveServo(int finger, int position)
{
  Serial.println("received " + position + " for flex" + finger);
}
