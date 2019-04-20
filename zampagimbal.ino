#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver dx = Adafruit_PWMServoDriver();
Adafruit_PWMServoDriver rx = Adafruit_PWMServoDriver(0x41);

//all arms on 0,3,6
//all forearms on 1,4,7
//all feet on 2,5,

int arm[]={0,3,6};
int forearm[]={1,4,7};
int foot[]={2,5,8};

#define MAX  120
#define MIN  560
#define DEG90 250
#define DEG180 120
#define DEG0 420
#define DEGM84 560


float getangle(int, bool inverted = false);
int getvalue(float, bool inverted = false);
int getSyncedValue(int,int valueoff=0);

const int MID = (MAX + MIN) / 2;

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");

  dx.begin();
  rx.begin();

  dx.setPWMFreq(60);
  rx.setPWMFreq(60);
   // Analog servos run at ~60 Hz updates

  delay(10);
}



void loop() {
}
void serialEvent()
{
  String message = Serial.readStringUntil('\n');
  char* p = &message[0];
  int val = atoi(p);
  Serial.println(val);
  setAllrx(val,forearm);
  setAlldx(def(val),forearm);
  int newrx=getSyncedValue(val,-100);
  int newdx=uniform(val,-100);
  setAllrx(newrx,foot);
  setAlldx(newdx,foot);

}



float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float getangle(int pulse , bool inverted = false) {
  if (!inverted) {

    float angle = mapfloat((float)pulse, MIN, MAX, -84, 180);
    return angle;
  }
  else {
    float angle = mapfloat((float)pulse, MIN, MAX, 180, -84);
    return angle;
  }
}


int getvalue(float angle, bool inverted = false) {
  if (!inverted) {
    int value = mapfloat(angle, -84, 180, MIN, MAX);
    return value;
  }
  else {
    int value = mapfloat(angle, -84, 180, MAX, MIN);
    return value;
  }
}

int getSyncedValue(int armValue,int valueoff =0) {
  float current = getangle(armValue);
  int sync = getvalue(90 - current,1);
  return sync+valueoff;


}
int def(int val){
   return mapfloat(val,MIN,MAX,MAX,MIN);
}

int uniform(int value,int offset){
  offset=-offset;
  value=mapfloat(value,MIN,MAX,MAX,MIN);
  int nw=getSyncedValue(value,offset);
  value=mapfloat(nw,MIN,MAX,MAX,MIN);
  return nw;
}

void setAlldx(int value,int* arr){
  for(int i=0;i<3;i++){
    int pin=arr[i];
    dx.setPWM(pin,0,value);
  }
}

void setAllrx(int value,int* arr){
  for(int i=0;i<3;i++){
    int pin=arr[i];
    rx.setPWM(pin,0,value);
  }
}


void standup(){
  
}
