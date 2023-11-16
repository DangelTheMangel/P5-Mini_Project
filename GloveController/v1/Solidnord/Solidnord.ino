////grøn er minus
////Blå er plus
////Lilla er R
////ORange er ground
//// pink og sort er + - for den bevælgige del
int MoPin = 2;  // Vibration motor connected to digital pin 2
int analogpin = 3;
int amplitude = 5;
int frequency = 10;
int angularFrequency = 10;
int afDivider = 1000;
int minimumDelay = 1000;
char sepperatrorChar = ';';
char endchar = '\n';
int in_Signal;
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
  //Check to see if anything is available in the serial receive buffer
  while (Serial.available() > 0) {
    //Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;
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
      String input = " ";
      for (char c : message) {
        if (c == sepperatrorChar) {
          break;
        }
        if(c == 'b'){
          testVibration();
        }
        input += c;
      }
      in_Signal = input.toInt();
      Serial.println(in_Signal);
      analogWrite(analogpin, in_Signal);
      message_pos = 0;
    }
  }
  float signal = amplitude * (sin(CycleCounter*angularFrequency/afDivider)+1);
  Serial.println(signal);
  analogWrite(analogpin, signal);
  CycleCounter += 1;
}
