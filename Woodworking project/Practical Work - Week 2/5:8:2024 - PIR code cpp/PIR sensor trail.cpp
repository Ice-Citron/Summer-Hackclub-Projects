// The time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;

// The time when the sensor outputs a low impulse
long unsigned int lowIn;

// The amount of milliseconds the sensor has to be low
// Before we assume all motion has stopped
long unsigned int pause = 5000;

boolean lockLow = true;
boolean takeLowTime;

int pirPin = 3; // The digital pin connected to the PIR sensor's output
int ledPin = 13; // LED pin

// SETUP
void setup(){
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  // Give the sensor some time to calibrate
  Serial.print("Calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
}

// LOOP
void loop(){
  if(digitalRead(pirPin) == HIGH){
    digitalWrite(ledPin, HIGH); // The LED visualizes the sensors output pin state
    if(lockLow){
      // Makes sure we wait for a transition to LOW before any further output is made
      lockLow = false;
    }
    Serial.print("motion detected at ");
    Serial.print(millis()/1000);
    Serial.println(" sec");
    delay(50);
    takeLowTime = true;
  }

  if(digitalRead(pirPin) == LOW){
    digitalWrite(ledPin, LOW); // The LED visualizes the sensors output pin state
    
    if(takeLowTime){
      lowIn = millis(); // Save the time of the transition from high to LOW
      takeLowTime = false; // Make sure this is only done at the start of a LOW phase
    }
    
    if(!lockLow && millis() - lowIn > pause){
      // Makes sure this block of code is only executed again after
      // A new motion sequence has been detected
      lockLow = true;
      Serial.print("motion ended at ");
      Serial.print((millis() - pause)/1000);
      Serial.println(" sec");
      delay(50);
    }
  }
}
