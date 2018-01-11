#include <TimerOne.h>
#include <Ultrasonic.h>


//This part is for one motor
#define M1A           8                      // motor forward
#define M1B           7                      // motor reverse 
#define PWM1          6                     // PWM motor pin ,Be careful in case of change to put the PWM to a pin able to generate PWM
#define encoder0PinA  2                      // DO NOT CHANGE THIS, FOR INTERRUPT NECESSARY!!

#define M2A           4                      // motor forward
#define M2B           5                      // motor reverse 
#define PWM2          9                      // PWM motor pin, Be careful in case of change to put the PWM to a pin able to generate PWM
#define encoder0PinB  3                      // DO NOT CHANGE THIS, FOR INTERRUPT NECESSARY!!


#define TIMERSTEP       1000000             // Defined as 1 sec (Arduino counts the time in microsecs) in order to set the frequency in a more user friendly way
                                            // If you change the TIMERSTEP, change the velocity equation as well accordingly
#define set_distance      10      //cm


//These are the bit data coming from Raspberry pi
#define Bit3   22   
#define Bit2   23
#define Bit1   24  
#define Bit0   26  


//volatile unsigned long int :if you dont get a desired value or accuracy for encoder readings change float to this


int timercounterA=0;  // counts the rising edges of the encoder reading
float velocityA=0;
float position1A=0;  // stores the previous position in order to calculate the velocity
float position2A=0;  // latest position
int timercounterB=0;  // counts the rising edges of the encoder reading
float velocityB=0;
float position1B=0;  // stores the previous position in order to calculate the velocity
float position2B=0;  // latest position
float distance=0;   // initialize the distance of ultrasound
byte error=0;
int D1;
int D2;



Ultrasonic ultrasonic(12, 13);

//void blabla{
//  error=(digitalRead(Bit2)<<2)|(digitalRead(Bit1)<<1)|digitalRead(Bit0);
//  if (digitalRead(Bit3)==1) error=-error;
//  }

void setup() {
  
  Timer1.initialize(TIMERSTEP);
  
  pinMode(M1A, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2B, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(encoder0PinA, INPUT_PULLUP); // set the encoder 0 so that it is not floating
  pinMode(encoder0PinB, INPUT_PULLUP);

  pinMode(Bit0, INPUT);
  pinMode(Bit1, INPUT);
  pinMode(Bit2, INPUT);
  pinMode(Bit3, INPUT);
  
  Serial.begin(9600);

      digitalWrite(M1A,LOW); //initially stop
      digitalWrite(M1B,LOW);
      digitalWrite(M2A,LOW);
      digitalWrite(M2B,LOW);
    
  Timer1.attachInterrupt(callback);       //start interrupt on main using initialized Timer, helps to mask the variables also, if used somewhere else, the operation is not affected 
  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoderA, RISING); // at each Timer1 period go to interrupt execute doEncoderA
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(digitalPinToInterrupt(encoder0PinB), doEncoderB, RISING);  //similarly, all independent from the "loop"



}



void callback(){
  
  position2A=timercounterA;
  velocityA=(position2A-position1A)/(3*298)*60;  // rpm: in one cycle one encoder reads 6 pulses rising and falling, since we count only rising "3", the gear ratio is 298:1 and for sec to min "60"
  position1A=position2A;

  position2B=timercounterB;
  velocityB=(position2B-position1B)/(3*298)*60;
  position1B=position2B;

  error=(digitalRead(Bit2)<<2)|(digitalRead(Bit1)<<1)|digitalRead(Bit0);
  

    Serial.println(" Veleocity of wheel A: ");
    Serial.print(velocityA);
    Serial.println(" Veleocity of wheel B: ");
    Serial.print(velocityB);
    Serial.println(error); 
  }


// code for the PID control with the detected error.
void pwm_control{
  
  
  }
  
void loop() {
// put your main code here, to run repeatedly:


distance=ultrasonic.distanceRead();

if (distance>set_distance)   // if this is true it will run the motor forward
  {
    if(digitalRead(Bit3==1)){ //ball is on the left frame,turn right command
      pwm_control(); // call pwm function şn order to get D1 D2 variables
      analogWrite(PWM1, D1); //D1 is the higher duty cycle
      analogWrite(PWM2, D2); //D2 is the lower duty cycle
    }
    else{// ball is on the right,turn left command
      pwm_control();
      analogWrite(PWM1, D2);
      analogWrite(PWM2, D1);
      }

      digitalWrite(M1A,HIGH); //now activate the motors forward
      digitalWrite(M2A,HIGH);
    }
   
else {
    analogWrite(PWM1, 0);
    analogWrite(PWM2, 0);
    }
}




void doEncoderA() {
  timercounterA ++;
}

void doEncoderB() {
  timercounterB ++;
}







