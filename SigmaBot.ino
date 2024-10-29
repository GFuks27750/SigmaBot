


#define PWMA 5
#define PWMB 6
#define AIN1 A1
#define AIN2 A0
#define BIN1 A2
#define BIN2 A3

#define ENCA 3
#define ENCB 2

//dostępne komendy
#define F 70//jedź prosto
#define B 66//jedź to do tyłu
#define R 82// skręć w prawo
#define L 76//skręć w lewo
#define S 83//zatrzymaj się 
#define G 71//uruchom silniki kół, nadaj im prędkość i na jaką odległość mają przejechać
#define W 87 //opóźnienie żeby mieć czas postawić robota na ziemię 
int pos[] = {0,0};

const int wheelCircumference = 3.14 * 6.5;
int traveledDistance = 0; 
bool stillRiding;
bool stillTurning;

void setup() {
  Serial.begin(115200);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), countEncoderA, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCB), countEncoderB, RISING);
}

void loop() {
  while(Serial.available() > 0){
  stillRiding = true; 
  stillTurning = true;
  resetData();

  int command = Serial.read();
  int value = Serial.parseInt();

  Serial.print((char) command);
  Serial.print(" ");
  Serial.print(value);
  Serial.println();

  switch(command){
    case F:
    while (stillRiding) {
      Forward(200, value, 1);
    }
    break; 

    case B: 
    while(stillRiding){
      Backward(200,value,1);
    }
    break;
    case R:
    while(stillTurning){
      Right(value);
    }
    break;

    case L: 
    while(stillTurning){
      Left(value);
    }
    break;

    case W:
    delay(value);
    break;
    }
  }
  resetData();
}

void Forward(int targetV, int targetDistance, int encoderNumber){
  Ride(targetV, targetDistance, encoderNumber);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  
}

void Backward(int targetV, int targetDistance, int encoderNumber){
  Ride(targetV, targetDistance, encoderNumber);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN2, HIGH);
  digitalWrite(BIN1, LOW);

}

void Right(int targetDistance) {
  targetDistance = angleConvertion(targetDistance);
  if(calculateRotations(targetDistance) >= pos[0]){
    analogWrite(PWMA, 100);
    analogWrite(PWMB, 100);
    digitalWrite(AIN1, LOW); 
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, HIGH);  
    digitalWrite(BIN2, LOW);
  }
  else{
    Stop();
    stillTurning = false;
    resetData();
  }

}

void Left(int targetDistance){
  targetDistance = angleConvertion(targetDistance);
  if(calculateRotations(targetDistance) >= pos[0]){
  analogWrite(PWMA, 100);
  analogWrite(PWMB, 100);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN2, HIGH);
  digitalWrite(BIN1, LOW);
  }
  else{
    Stop();
    stillTurning = false;
    resetData();
  }

}

void Stop(){
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN2, LOW);
  digitalWrite(BIN1, LOW);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void Ride(int targetV, int targetDistance, int encoderNumber){ //0 -> lewy enkoder, 1 -> prawy enkoder
    // Sprawdzamy, czy przejechano wystarczającą odległość
 if(calculateRotations(targetDistance) >= pos[encoderNumber]){
    float A = 30; 
    int Lp = pos[0];
    int Ll = pos[1];
    int Vp = targetV + A * (Lp - Ll);
    int Vl = targetV - A * (Lp - Ll);
    analogWrite(PWMA, Vp);
    analogWrite(PWMB, Vl);
  }
  else{
    Stop();
    stillRiding = false; 
  }

}

void resetData(){
    pos[0] = 0;              
    pos[1] = 0; 
    traveledDistance = 0; 
}

float calculateRotations(int targetDistance){
  float pulsesPerRotation = 20;
  float rotations = (float)targetDistance / wheelCircumference; 
  return abs(pulsesPerRotation * rotations); 
}

void countEncoderA(){
  int a = digitalRead(ENCA);
  if(a>0){
    pos[1]++;
  }
  else{
    pos[1]--;
  }
}
void countEncoderB(){
  int b = digitalRead(ENCB);
  if(b>0){
    pos[0]++;
  }
  else{
    pos[0]--;
  }
}

int angleConvertion(int angle){
  switch(angle){
    case 45:
      angle = 4;
    break;
    case 90:
      angle = 9;

  }
  return angle; 
}