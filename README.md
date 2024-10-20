# SigmaBot

## Purpouse
This is actually project for my class, we work on AlphaBots, but I wanted to build my own robot from scratch, I used an old arduino clone that was lying in my closet for years, ofcourse dc motors with encoders and TB6612FNG as a motor driver.

## Tips for future students
If you are working on the same project as me using AlphaBot (ISS class), you have to change few things in the code, especialy this fragment: 
```
float calculateRotations(int targetDistance){
  float pulsesPerRotation = 1200; // you have to check how many pulses is sending your encoder, for it to actually work.
  float rotations = (float)targetDistance / wheelCircumference; 
  return pulsesPerRotation * rotations; 
}
```
To make your life easier, you can use this function to count pulses per rotation of the wheel (just manually rotate the wheel and display data on serial monitor: 

```
void countEncoderA(){
  int a = digitalRead(ENCA);
  if(a>0){
    pos[0]++;
  }
  else{
    pos[0]--;
  }
}
```
