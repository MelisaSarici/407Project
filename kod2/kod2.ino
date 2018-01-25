#include <TimerOne.h>
#include "TimerObject.h"

#include <PID_v1.h>

#define PIN_INPUT 0
#define PIN_OUTPUT 3

#define Setpoint  500 //rpm
//Define Variables we'll be connecting to
double  Input, Output;

//Define the aggressive and conservative Tuning Parameters
double aggKp=4, aggKi=0.2, aggKd=1;
double consKp=1, consKi=0.05, consKd=0.25;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

#define PWM1          6                     // PWM motor pin ,Be careful in case of change to put the PWM to a pin able to generate PWM
#define encoder0PinA  2                      // DO NOT CHANGE THIS, FOR INTERRUPT NECESSARY!!

#define TIMERSTEP       40000            // Defined as 1 sec (Arduino counts the time in microsecs) in order to set the frequency in a more user friendly way
 TimerObject *timer1 = new TimerObject(4000);                        // If you change the TIMERSTEP, change the velocity equation as well accordingly

//volatile unsigned long int :if you dont get a desired value or accuracy for encoder readings change float to this


int timercounterA=0;  // counts the rising edges of the encoder reading
float velocityA=0;
float position1A=0;  // stores the previous position in order to calculate the velocity
float position2A=0;  // latest position
static int D=0;
int count=0;
void setup() {
  
  Timer1.initialize(TIMERSTEP);
  
  pinMode(PWM1, OUTPUT);
  pinMode(encoder0PinA, INPUT_PULLUP); // set the encoder 0 so that it is not floating
 

  Serial.begin(115200);
  setPwmFrequency(6, 8); //Sets the pwm output of 5 to around 8 to 9 kHz
  
    timer1->setOnTimer(&duty);
    timer1->Start();
       
  Timer1.attachInterrupt(callback);       //start interrupt on main using initialized Timer, helps to mask the variables also, if used somewhere else, the operation is not affected 
  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoderA, CHANGE); // at each Timer1 period go to interrupt execute doEncoderA
  // encoder pin on interrupt 1 (pin 3)

//initialize the variables we're linked to

  //turn the PID on
  myPID.SetMode(AUTOMATIC);


}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}


void callback(){
    
   position2A=timercounterA;
   velocityA=(position2A-position1A)*30/TIMERSTEP;
    position1A=position2A;
    Input = velocityA;
     Serial.print(D);
    Serial.print(",");
    Serial.println(velocityA);
 
    
  }

 void duty(){

if (D<70){
        D++;
        }

if (D>=70)
      D=70;

  }

  
void loop() {
// put your main code here, to run repeatedly:
       timer1->Update();
      analogWrite(PWM1, D);

      Input = velocityA;

  double gap = abs(Setpoint-Input); //distance away from setpoint
  if (gap < 10)
  {  //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
     //we're far from setpoint, use aggressive tuning parameters
     myPID.SetTunings(aggKp, aggKi, aggKd);
  }

  myPID.Compute();
  analogWrite(PIN_OUTPUT, Output);
      
}

void doEncoderA() {
  timercounterA ++;
}



















