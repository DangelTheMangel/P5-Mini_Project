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
int minimumDelay = 100;
float pi = 3.14;
char sepperatrorChar = ';';
char endchar = '\n';
int in_Signal;
int phase = 1;

bool testsignal = true;
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
    //float signal = amplitude * (sin(CycleCounter*angularFrequency/afDivider)+1);
    //float signal = amplitude * (sin((CycleCounter*2*pi*frequency+phase)+1));
    float signal = amplitude * (sin(CycleCounter * angularFrequency / afDivider) + phase);
    Serial.println(signal);
    analogWrite(analogpin, signal);
    CycleCounter += 1;
  }
}

//Create a place to hold the incoming message
char message[MAX_MESSAGE_LENGTH];

void readData() {
  //Check to see if anything is available in the serial receive buffer
  while (Serial.available() > 0) {
    static unsigned int message_pos = 0;
    //message = char[MAX_MESSAGE_LENGTH]
    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();
    //Message coming in (check not terminating character) and guard for over message size
    if (inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1)) {
      //Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    }
    //Full message received...
    else {
      interpretData();
      message_pos = 0;
    }
  }
}

void interpretData() {
  String input = " ";
  for (char c : message) {
    if (c == endchar) {
      break;
    }
    if (c == 'b') {
      testsignal = !testsignal;
      testVibration();
    }
    input += c;
    //c = '\0';
  }
  in_Signal = input.toFloat();
  //Serial.println(in_Signal);
  analogWrite(analogpin, in_Signal);
  delay(minimumDelay);
  analogWrite(analogpin, 0);
  Serial.println(in_Signal);
  memset(message, '\0', MAX_MESSAGE_LENGTH);
}

