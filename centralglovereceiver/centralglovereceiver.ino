/*
  Central Glovve Receiver 2023

  To control the servos in a robot hand using flex sensors in a glove

  Trasnission via BLE so the set up can be wireless


*/
// include bluetooth library
#include <ArduinoBLE.h>
// include servo library
#include <Servo.h>

// Set the output pins that the servos are connected to
int Finger1Pin = A4;
int Finger2Pin = A0;
int Finger3Pin = A1;
int Finger4Pin = A2;
int Finger5pin = A3;

int fullbent[5] = {10, 10, 10, 10, 10};
int straight[5] = {170, 170, 170, 170, 170};
// set the imput pins the flex sensors are connected to ( in case of direct connection)

int Flex1pin = A5;
int Flex2pin = A0;
int Flex3pin = A1;
int Flex4pin = A2;
int Flex5pin = A4;

// Set input mode to WIRED(0) or BLUETOOTH (1)

bool USE_BLUETOOTH = 1;

const String GLOVESERVICENAME = "Glove";
const char *GloveServiceUUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char *Flex1CharacteristicUUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char *Flex2CharacteristicUUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// create servo objects and put in an array
Servo fingerServos[5] = {
    Servo Finger1Servo,
    Servo Finger2Servo,
    Servo Finger3Servo,
    Servo Finger4Servo,
    Servo Finger5Servo};

void setup()
{

  // initiate serial connection for feedback information
  Serial.begin(9600);
  while (!Serial)
    ;

  // attach servos

  Finger1Servo.attach(Finger1Pin);

  // check fingers are working
  MoveServo(Finger1Servo, 10);
  MoveServo(Finger1Servo, 170);
  MoveServo(Finger1Servo, 10);

  // begin bluetooth initialization
  if (USE_BLUETOOTH)
  {
    if (!BLE.begin())
    {
      Serial.println("starting Bluetooth® Low Energy module failed!");

      while (1)
        ;
    }

    Serial.println("Bluetooth® Low Energy Central - Glove Receiver");

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
      Serial.println("Connecting to ...");
      Serial.println(peripheral.localName());

      if (peripheral.connect())
      {
        Serial.println("Connected");
      }
      else
      {
        Serial.println("Failed to connect!");
        return;
      }
      // discover the attributes of the service
      peripheral.discoverAttributes();

      BLEService Glove_Service = peripheral.service(GloveServiceUUID);

      BLECharacteristic Flex1_characteristic = Glove_Service.characteristic(Flex1CharacteristicUUID);
      BLECharacteristic Flex2_characteristic = Glove_Service.characteristic(Flex2CharacteristicUUID);

      while (peripheral.connected())
      {

        // reads the current value of the characteristics from bluetooth
        Flex1_characteristic.read();

        int positions[5]; // array to hold the current positions ( percentage bent ofeach finger)

        position[0] = hextoint(Flex1_characteristic.value(), Flex1_characteristic.valueLength());

        MoveServo(Finger1Servo, position);
        Serial.println();
        // wait a bit - not sure why?
        delay(2);
      }
      // when the central disconnects, print it out:
      Serial.print(F("Disconnected from peripheral: "));
      Serial.println(peripheral.localName());
      // then loop back round and listen for a connection from a BLE central device
    }
  }
  else
  {
    // get data direct from input pins
  }
}

// Other functions called by the setup and loop

// for printing out the details from the blluetooth byte value
int printData(const unsigned char data[], int length)
{
  for (int i = 0; i < length; i++)
  {
    unsigned char b = data[i];

    if (b < 16)
    {
      Serial.print("0");
    }

    Serial.print(b, HEX);
    Serial.println();
  }
}

// to convert the first charater of the bluetooth value to and integer - must be a better way
int hextoint(const unsigned char data2[], int length)
{
  int answer = data2[0];
  Serial.print("As int = ");
  Serial.println(answer);
  return answer;
}

// move a given servo to a specific position
void MoveServo(Servo moveservo, int position)
{
  position = map(position, 215, 65, 10, 170);
  moveservo.write(position);
  Serial.print("Move to...");
  Serial.println(position); // sets the servo position according to the scaled value
  delay(1);
}

// bend a finger to a given percentage
void bendFinger(int fingerNum, int percentBent)
{
  position = map(percentBent, 0, 100, straight(fingerNum), fullbent(fingerNum));
  fingerServos[fingerNum].write(position)
}