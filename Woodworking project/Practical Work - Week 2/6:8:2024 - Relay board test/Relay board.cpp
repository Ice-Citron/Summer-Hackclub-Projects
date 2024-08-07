//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 15;

//the time when the sensor outputs a low impulse
long unsigned int lowIn;        

//the amount of milliseconds the sensor has to be low
//before we assume all motion has stopped
long unsigned int pause = 100;

boolean lockLow = true;
boolean takeLowTime;  

int R_EN = 2;
int R_PWM = 3;
int L_EN = 4;

int pirPin1 = 5;    //the digital pin connected to the PIR sensor's output
int pirPin2 = 6;
int pirPin3 = 7;
int pirPin4 = 8;
int relayPin1 = 9;
int relayPin2 = 10;
int relayPin3 = 11;
int relayPin4 = 12;
int relayPin5 = 13;

//int ledPin = 13;

/////////////////////////////
//SETUP
/////////////////////////////
void setup(){
  Serial.begin(1000);

  pinMode(pirPin1, INPUT);
  pinMode(pirPin2, INPUT);
  pinMode(pirPin3, INPUT);
  pinMode(pirPin4, INPUT);
  digitalWrite(pirPin1, LOW);
  digitalWrite(pirPin2, LOW);
  digitalWrite(pirPin3, LOW);
  digitalWrite(pirPin4, LOW);

  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  pinMode(relayPin4, OUTPUT);
  pinMode(relayPin5, OUTPUT);

  pinMode(R_EN, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(L_EN, OUTPUT);

  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor /n");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
}

////////////////////////////
//LOOP
////////////////////////////
void loop(){

  if(digitalRead(pirPin1) == HIGH || digitalRead(pirPin2) == HIGH || digitalRead(pirPin3) == HIGH || digitalRead(pirPin4) == HIGH){
    //digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
    analogWrite(R_PWM, 250); //spins motor -- for 1 hour 80
    digitalWrite(relayPin1, HIGH);
    digitalWrite(relayPin2, HIGH);
    digitalWrite(relayPin3, HIGH);
    digitalWrite(relayPin4, HIGH);
    digitalWrite(relayPin5, HIGH);

    //digitalWrite(ledPin, HIGH);

    if(lockLow){ 
      //makes sure we wait for a transition to LOW before any further output is made:
      lockLow = false;             
      Serial.println("---");
      Serial.print("motion detected at ");
      Serial.print(millis()/1000);
      Serial.println(" sec");
      millis();
    }           
    takeLowTime = true;
  }

  if(digitalRead(pirPin1) == LOW || digitalRead(pirPin2) == LOW || digitalRead(pirPin3) == LOW || digitalRead(pirPin4) == LOW){       
    //digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state
    analogWrite(R_PWM, 0); //spins motor -- for 1 hour 
    digitalWrite(relayPin1, LOW);
    digitalWrite(relayPin2, LOW);
    digitalWrite(relayPin3, LOW);
    digitalWrite(relayPin4, LOW);
    digitalWrite(relayPin5, LOW);

    //digitalWrite(ledPin, LOW);

    if(takeLowTime){
      lowIn = millis();          //save the time of the transition from high to LOW
      takeLowTime = false;       //make sure this is only done at the start of a LOW phase
    }
    //if the sensor is low for more than the given pause, 
    //we assume that no more motion is going to happen
    if(!lockLow && millis() - lowIn > pause){ 
      //makes sure this block of code is only executed again after 
      //a new motion sequence has been detected
      lockLow = true;                       
      Serial.print("motion ended at ");      //output
      Serial.print((millis() - pause)/1000);
      Serial.println(" sec");
      millis();
    }
  }
}
