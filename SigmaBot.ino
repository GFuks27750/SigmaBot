#define PWMA 10
#define PWMB 11
#define AIN1 A3
#define AIN2 A2
#define BIN1 A1
#define BIN2 A0

#define ENCA 3
#define ENCB 2

//dostępne komendy
#define F 70//jedź prosto
#define B 66//jedź to do tyłu
#define R 82// skręć w prawo
#define L 76//skręć w lewo
#define S 83//zatrzymaj się 
#define G 71//uruchom silniki kół, nadaj im prędkość i na jaką odległość mają przejechać

int pos[] = {0,0};

const int wheelCircumference = 3.14 * 6.5;
int traveledDistance = 0; 
bool stillRiding = true; 
bool stillTurning = true; 

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
  String input = Serial.readString();
  int inputLenght = input.length();
  int command = input.charAt(0);
  int value = input.substring(1, inputLenght).toInt();
  Serial.print((char) command);
  Serial.print(" ");
  Serial.print(value);
  Serial.println();

  switch(command){
    case F:
    while (stillRiding) {
      Forward(200, value, 0);
    }
    break; 

    case R:
    while(stillTurning){
      Right(200,value,1);
    }
    break;
    case B: 
    while(stillRiding){
      Backward(200,value,1);
    }
    break;
    case L: 
    while(stillRiding){
      Left(200,value,0);
    }
    break;
    }
  }
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

void Right(int targetV, int targetDistance, int encoderNumber) {
  targetDistance = angleConvertion(targetDistance);
  Ride(targetV, targetDistance, encoderNumber);
  digitalWrite(AIN1, LOW); 
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);  
  digitalWrite(BIN2, LOW);
}

void Left(int targetV, int targetDistance, int encoderNumber){
  targetDistance = angleConvertion(targetDistance);
  Ride(targetV, targetDistance, encoderNumber);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN2, HIGH);
  digitalWrite(BIN1, LOW);
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
    float A = 0.5; 
    int Lp = pos[0];
    int Ll = pos[1];
    int Vp = targetV - A * (Lp - Ll);
    int Vl = targetV + A * (Lp - Ll);
    analogWrite(PWMA, Vp);
    analogWrite(PWMB, Vl);

    // Zapewniamy, że prędkość nie przekracza wartości docelowej
    if(Vp > targetV || Vl > targetV){
      analogWrite(PWMA, targetV);
      analogWrite(PWMB, targetV);
    }
  }
  else{
    Stop();
    stillRiding = false; 
    stillTurning = false;
  }

}

void resetData(){
    pos[0] = 0;              
    pos[1] = 0; 
    traveledDistance = 0; 
}

float calculateRotations(int targetDistance){
  float pulsesPerRotation = 1200;
  float rotations = (float)targetDistance / wheelCircumference; 
  return pulsesPerRotation * rotations; 
}

void countEncoderA(){
  int a = digitalRead(ENCA);
  if(a>0){
    pos[0]++;
  }
  else{
    pos[0]--;
  }
}
void countEncoderB(){
  int b = digitalRead(ENCB);
  if(b>0){
    pos[1]++;
  }
  else{
    pos[1]--;
  }
}

int angleConvertion(int angle){
  switch(angle){
    case 45:
      angle = 6;
    break;
    case 90:
      angle = 13;
    break;
    case 135:
      angle = 19;
    break;
    case 180:
      angle = 25;
    break;
    case 225:
      angle = 31;
    break;
    case 270:
      angle = 37;
    break;
    case 315:
      angle = 43;
    break;
    case 360:
      angle = 49;
    break;
  }
  return angle; 
}