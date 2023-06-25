

// Define the number of samples to keep track of. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.
const int numReadings = 3;
int tolerance = 10;

int readings1[numReadings];  // the readings from the analog input
int readings2[numReadings];  // the readings from the analog input
int readings3[numReadings];  // the readings from the analog input
int readings4[numReadings];  // the readings from the analog input
int readings5[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading

int total1 = 0;              // the running total
int average1 = 0;            // the average
int prev_average1 = 0;
int inputPin1 = A0;
int print1 = 0;

int total2 = 0;              // the running total
int average2 = 0;            // the average
int prev_average2 = 0;
int inputPin2 = A1;
int print2 = 0;

int total3 = 0;              // the running total
int average3 = 0;            // the average
int prev_average3 = 0;
int inputPin3 = A2;
int print3 = 0;

int total4 = 0;              // the running total
int average4 = 0;            // the average
int prev_average4 = 0;
int inputPin4 = A4;
int print4 = 0;

int total5 = 0;              // the running total
int average5 = 0;            // the average
int prev_average5 = 0;
int inputPin5 = A5;
int print5 = 0;

void setup() {
  // initialize serial communication with computer:
  Serial.begin(9600);
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings1[thisReading] = 0;
    readings2[thisReading] = 0;
    readings3[thisReading] = 0;
    readings4[thisReading] = 0;
    readings5[thisReading] = 0;
  }
}

void loop() {
  // subtract the last reading:
  total1 = total1 - readings1[readIndex];
  total2 = total2 - readings2[readIndex];
  total3 = total3 - readings3[readIndex];
  total4 = total4 - readings4[readIndex];
  total5 = total5 - readings5[readIndex];


  // read from the sensor:
  readings1[readIndex] = analogRead(inputPin1);
  readings2[readIndex] = analogRead(inputPin2);
  readings3[readIndex] = analogRead(inputPin3);
  readings4[readIndex] = analogRead(inputPin4);
  readings5[readIndex] = analogRead(inputPin5);

  // add the reading to the total:
  total1 = total1 + readings1[readIndex];
  total2 = total2 + readings2[readIndex];
  total3 = total3 + readings3[readIndex];
  total4 = total4 + readings4[readIndex];
  total5 = total5 + readings5[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average1 = total1 / numReadings;
  if (abs(average1 - prev_average1)>tolerance){
    print1 = (average1);
    prev_average1 = average1;
  }
  else {
    print1 = (prev_average1);
  }
 
  // calculate the average:
  average2 = total2 / numReadings;
  if (abs(average2 - prev_average2)>tolerance){
    print2 = (average2);
    prev_average2 = average2;
  }
  else {
    print2 = (prev_average2);
  }

   // calculate the average:
  average3 = total3 / numReadings;
  if (abs(average3 - prev_average3)>tolerance){
    print3 = (average3);
    prev_average3 = average3;
  }
  else {
    print3 = (prev_average3);
  }

   // calculate the average:
  average4 = total4 / numReadings;
  if (abs(average4 - prev_average4)>tolerance){
    print4 = (average4);
    prev_average4 = average4;
  }
  else {
    print4 = (prev_average4);
  }

   // calculate the average:
  average5 = total5 / numReadings;
  if (abs(average5 - prev_average5)>tolerance){
    print5 = (average5);
    prev_average5 = average5;
  }
  else {
    print5 = (prev_average5);
  }

  Serial.print(print1);
  Serial.print(" ");
  Serial.print(print2);
  Serial.print(" ");
  Serial.print(print3);
  Serial.print(" ");
  Serial.print(print4);
  Serial.print(" ");
  Serial.println(print5);
  delay(10);  // delay in between reads for stability
  
}
