//###[Pins]###
int MoPin = 2;        // Digital pin for the vibration motor
int analogpin = 3;    // Analog pin for vibration control
//###[The signal]###
int in_Signal; // the signal rescivede from the other script
bool readyToRecvieSignal = false; //Boolean that say if the script is ready to recvive a new signal
//###[Bumps settings]###
int minimumDelay = 10; //The minimum delay between bumps
bool useBumps = true;  // Use bumps in signal or continuous signal
//###[The test signal]###
bool testsignal = false; //If the test signa lshould be play
int CycleCounter = 0; //The time to contine the test signal
int amplitude = 1.5;  // Amplitude of the test signal
int frequency = 50;   // Frequency of the test signal
int phase = 1; // phase for test signal
int angularFrequency = 20; // The angaularFrequency of test signal
int afDivider = 1000; //devider for the test signal
float pi = 3.14; //pi... but not so accorate

//Run when the ardino starts. It set up pins and serial and then start a test vibarations 
void setup() {
  pinMode(MoPin, OUTPUT);
  pinMode(analogpin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Serial Ready to take commands");
  startVibaration();
}

//This is a startvibaration that runs when the ardino runs starts and it just vibrate and then the ardino is ready to get signals
void startVibaration() {
  Serial.println("Test signal ready");
  digitalWrite(MoPin, HIGH);
  delay(1000);
  digitalWrite(MoPin, LOW);
  delay(1000);
  Serial.println("Test signal done");
  readyToRecvieSignal = true;
}

//if testsignal are true it send out testsignals else it look at the incomming data and create bumps
void loop() {
  if (testsignal) {
    testSignal();
  }else{
    readData();
  }
}

//test signal 
void testSignal() {
  float signal = amplitude * (sin(CycleCounter * angularFrequency / afDivider) + phase);
  Serial.println(signal);
  analogWrite(analogpin, signal);
  CycleCounter += 1;
}

//Read the incomming singal form serial
void readData() {
  while (Serial.available() > 0) {
    if (useBumps)
      interpretDataFlick();
    else
      interpretDataContinues();
  }
  if (readyToRecvieSignal) {
    Serial.println("r");
  }
}

//Read the data and send a burst signal to the speaker
//Inspred by https://projecthub.arduino.cc/ansh2919/serial-communication-between-python-and-arduino-663756
void interpretDataFlick() {
  readyToRecvieSignal = false;
  in_Signal = Serial.readString().toInt();
  analogWrite(analogpin, in_Signal);
  delay(minimumDelay);
  analogWrite(analogpin, 0);
  Serial.flush();
  readyToRecvieSignal = true;
}
//Read the data and send a continues signal to the speaker
//Inspred by https://projecthub.arduino.cc/ansh2919/serial-communication-between-python-and-arduino-663756
void interpretDataContinues() {
  readyToRecvieSignal = false;
  in_Signal = Serial.readString().toInt();
  analogWrite(analogpin, in_Signal);
  Serial.flush();
  readyToRecvieSignal = true;
}
