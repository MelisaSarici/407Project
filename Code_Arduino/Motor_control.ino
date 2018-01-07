#include <TimerOne.h>
#include <Ultrasonic.h>


//This part is for one motor
#define M1A           8                      // motor forward
#define M2A           7                      // motor reverse 
#define PWM1          6                     // PWM motor pin ,Be careful in case of change to put the PWM to a pin able to generate PWM
#define encoder0PinA  2                      // DO NOT CHANGE THIS, FOR INTERRUPT NECESSARY!!

#define M1B           4                      // motor forward
#define M2B           5                      // motor reverse 
#define PWM2          9                      // PWM motor pin, Be careful in case of change to put the PWM to a pin able to generate PWM
#define encoder0PinB  3                      // DO NOT CHANGE THIS, FOR INTERRUPT NECESSARY!!


#define TIMERSTEP       1000000             // Defined as 1 sec (Arduino counts the time in microsecs) in order to set the frequency in a more user friendly way
                                            // If you change the TIMERSTEP, change the velocity equation as well accordingly

#define set_distance      10      //cm


#define Left    10  
#define Right   11



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
int ComRight=0;
int ComLeft=0;


Ultrasonic ultrasonic(12, 13);



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

  pinMode(Right, INPUT);
  pinMode(Left, INPUT);
  
  Serial.begin(9600);
  
    
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


    Serial.println(" Veleocity of wheel A: ");
    Serial.print(velocityA);
    Serial.println(" Veleocity of wheel B: ");
    Serial.print(velocityB);
    Serial.println("************************"); 
  }


  
void loop() {
// put your main code here, to run repeatedly:

ComRight=digitalRead(Right);
ComLeft=digitalRead(Left);
distance=ultrasonic.distanceRead();

if (distance>set_distance)   // if this is true it will run the motor forward
  {
    if(ComLeft!=LOW && ComRight==LOW)
    {
      digitalWrite(M1A,HIGH); //you can define these in setup I guess, give it a try for setting them LOW at the begining
      digitalWrite(M1B,HIGH);
      digitalWrite(M2A,LOW);
      digitalWrite(M2B,LOW);
      
      analogWrite(PWM1, 0);
      analogWrite(PWM2, 30);
  
    }
    
     if(ComRight!=LOW && ComLeft==LOW)
    {
      digitalWrite(M1A,HIGH); //you can define these in setup I guess, give it a try for setting them LOW at the begining
      digitalWrite(M1B,HIGH);
      digitalWrite(M2A,LOW);
      digitalWrite(M2B,LOW);
      
      analogWrite(PWM1, 30);
      analogWrite(PWM2, 0);
  
    }

     if(ComRight!=LOW && ComLeft!=LOW)
    {
      digitalWrite(M1A,HIGH); //you can define these in setup I guess, give it a try for setting them LOW at the begining
      digitalWrite(M1B,HIGH);
      digitalWrite(M2A,LOW);
      digitalWrite(M2B,LOW);
      
      analogWrite(PWM1, 30);
      analogWrite(PWM2, 30); 
    
  }

   if(ComRight==LOW && ComLeft==LOW)
    {
      
      analogWrite(PWM1, 0);
      analogWrite(PWM2, 0); 
    
  }
    

 }
  
else {
    analogWrite(PWM1, 0);
    analogWrite(PWM2, 0);
    }



    // BURADA MOTORLARIN DÖNÜŞ İŞLEMİNİ GERÇEKLEŞTİREN BİR KOD OLACAK. BU KOD GRIPPER TOPU YAKALADIKTAN  SONRA BİR VARIABLE I DEĞİŞTİRİP CALL EDİLMESİYLE AKTİF OLACAK VE BİR 
    // RETURN DEĞERİ OLACAK. BU RETURN DEĞERİ RESET EDİLENE DEK SENSÖRLER INAKTIF KALMALI. SONRA ULTRASON TEKRAR DATA OKUMAYA BAŞLAYABILIR VE GRIPPER BIR SONRAKI GÖREVINE GEÇEBİLİR
}




void doEncoderA() {
  timercounterA ++;
}

void doEncoderB() {
  timercounterB ++;
}







