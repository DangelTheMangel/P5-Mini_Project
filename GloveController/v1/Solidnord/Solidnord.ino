////grøn er minus
////Blå er plus
////Lilla er R
////ORange er ground
//// pink og sort er + - for den bevælgige del
int MoPin = 2;  // Vibration motor connected to digital pin 2
int analogpin = 3;
int amplitude = 1.5;
int frequency = 50;
int angularFrequency = 20;
int afDivider = 1000;
int minimumDelay = 10;
float pi = 3.14;
char sepperatrorChar = ';';
char endchar = '\n';
int in_Signal;
int phase = 1;
bool testsignal = false;
bool readyToRecvieSignal = false;
void setup() {
  pinMode(MoPin, OUTPUT);
  pinMode(analogpin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Serial Ready to take commands");
  testVibration();
}

void testVibration() {
  Serial.println("Test signal ready");
  digitalWrite(MoPin, HIGH);
  delay(1000);
  digitalWrite(MoPin, LOW);
  delay(1000);
  Serial.println("Test signal done");
}
const unsigned int MAX_MESSAGE_LENGTH = 20;
int CycleCounter = 0;
//https://www.programmingelectronics.com/serial-read/
void loop() {
  readData();
  if (testsignal) {
    float signal = amplitude * (sin(CycleCounter * angularFrequency / afDivider) + phase);
    Serial.println(signal);
    analogWrite(analogpin, signal);
    CycleCounter += 1;
  }
}

void readData() {
  while (Serial.available() > 0) {
    interpretDataFlick();
  }
  if(readyToRecvieSignal){
    Serial.println("r");
  }
}

void interpretDataFlick() {
    readyToRecvieSignal = false;
    in_Signal = Serial.readString().toInt();
    analogWrite(analogpin, in_Signal);
    delay(minimumDelay);
    analogWrite(analogpin, 0);
    Serial.flush();
    readyToRecvieSignal = true;
}

void interpretDataContinues() {
    readyToRecvieSignal = false;
    in_Signal = Serial.readString().toInt();
    analogWrite(analogpin, in_Signal);
    Serial.flush();
    readyToRecvieSignal = true;
}
