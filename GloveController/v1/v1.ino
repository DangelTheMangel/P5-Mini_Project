int MoPin = 2;  // vibrator Grove connected to digital pin 9
int minimumDelay = 100;
void setup() {
  Serial.begin(9600);
  pinMode(MoPin, OUTPUT);
  Serial.println("serial Ready to take comands");
  testVibration();
}

void testVibration(){
  Serial.println("testsignal ready");
  digitalWrite(MoPin, HIGH);
  delay(1000);

  digitalWrite(MoPin, LOW);
  delay(1000);
  Serial.println("testsignal done");

}
void loop() {
  while (Serial.available()) {
    delay(minimumDelay);
    char c = Serial.read();
    Serial.println(c);
    if (c == '1'){
      digitalWrite(MoPin, HIGH);
    }else{
      digitalWrite(MoPin, LOW);
    }
  }

  
}
