/*

  Central Glove TRansmitter 2023

  To send flex sensor (finger bend) values to a robot hand via BLE

  this is the code for the glove

*/

// include bluetooth library
#include <ArduinoBLE.h>

// Set the input pins the flex sensors are connected to

int finger_pins[5] = {A0, A1, A2, A3, A4};
int fullbent[5] = {10, 10, 10, 10, 10};
int straight[5] = {170, 170, 170, 170, 170};

// set the UUID of the BLE service
BLEService FlexService("19B10000-E8F2-537E-4F6C-D104768A1214");

// Set the UUID of the BLE characteristics for the flex sensor values

BLEByteCharacteristic Flex1("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic Flex2("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic Flex3("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic Flex4("19B10004-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic Flex5("19B10005-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// run the setup actions

void setup()
{

  // initiate serial connection for feedback information
  Serial.begin(9600);
  while (!Serial)
    ;

  // begin BLE initialization
  if (!BLE.begin())
  {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1)
      ;
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Glove");
  BLE.setAdvertisedService(FlexService);

  // add the characteristic to the service
  FlexService.addCharacteristic(Flex1);
  FlexService.addCharacteristic(Flex2);
  FlexService.addCharacteristic(Flex3);
  FlexService.addCharacteristic(Flex4);
  FlexService.addCharacteristic(Flex5);

  // add service
  BLE.addService(FlexService);

  // set the initial value for the characeristic:
  Flex1.writeValue(0);
  Flex2.writeValue(0);
  Flex3.writeValue(0);
  Flex4.writeValue(0);
  Flex5.writeValue(0);
  // start advertising
  BLE.advertise();

  Serial.print("Advertising BLE Glove Peripheral as .....");
  Serial.println("Glove");
}

void loop()
{
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central)
  {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected())
    {
      sendflexvalue(0, Flex1, "Flex1", 700, 400);
      sendflexvalue(1, Flex2, "Flex2", 700, 400);
      sendflexvalue(2, Flex3, "Flex3", 700, 400);
      sendflexvalue(3, Flex4, "Flex4", 700, 400);
      sendflexvalue(4, Flex5, "Flex pinky", 700, 400);
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    // then loop back round and listen for a connection from a BLE central device
  }
}

// Other functions called by the setup and loop

// send the value of the amount you want the finger to fliex
void sendflexvalue(int finger, BLEByteCharacteristic characteristic, String name, int straight, int bent)
{

  int Flexvalue = mapflex(finger, analogRead(fingerpin[finger]));
  // BLE can only take a value up to 255 so check if its too high
  if (Flexvalue > 255)
  {
    Serial.print("SPIKE - ");
    Serial.print(Flexvalue);
    Flexvalue = 255;
  }
  // write the flex sensort value to the BLE characteristic
  characteristic.writeValue(Flexvalue);
  Serial.print(name);
  Serial.print(" Value is ...");
  Serial.println(Flexvalue);
  delay(10);
}

int mapflex(int finger, int value)
{
  long flexPercent = map(value, straight[finger], fullbent[finger], 0, 100);
  return flexPercent;
}