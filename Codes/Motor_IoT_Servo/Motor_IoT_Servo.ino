/*
                            *
                            * Project Name:   Kisan Agricultural Robot
                            * Author List:    Abhishek Gore, Hrishikesh Chowkwale, Piyush Malpure
                            * Filename:     Motor_IoT_Servo.ino
                            * Functions:    servo_setup(),setServoPulse(uint8_t, double),servo_loop, setup(),move_forward(),dist_moved(),stop_rover(),sensors(),loop()
                            * Global Variables: servonum,En1,En2,En3,En4,En5,En6,m1a,m1b,m2a,m2b,m3a,m3b,m4a,m4b,m5a,m5b,m6a,m6b,a
                            *
*/
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//Create objects for each Servo motor
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x42);
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(&Wire, 0x40);

/*Depending on your servo make, the pulse width min and max may vary, you 
 want these to be as small/large as possible without hitting the hard stop
 for max range. You'll have to tweak them as necessary to match the servos you
 have!*/
#define SERVOMIN  96 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  470 // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
uint8_t servonum = 0; //Servo numbering for sending pulses to respective motor
//DC motor pins
int En1 = 52; //Enable DC motor 1
int En2 = 53; //Enable DC motor 2
int En3 = 51; //Enable DC motor 3
int En4 =50; //Enable DC motor 4
int En5=49; //Enable DC motor 5
int En6=48; //Enable DC motor 6
int En7 = 46; //Enable Suction Pump 
int m1a = 2; //Select direction and speed of DC motor 1
int m1b = 3; //Select direction and speed of DC motor 1
int m2a = 4; //Select direction and speed of DC motor 2
int m2b = 5; //Select direction and speed of DC motor 2 
int m3a = 6; //Select direction and speed of DC motor 3
int m3b = 7; //Select direction and speed of DC motor 3
int m4a = 8; //Select direction and speed of DC motor 4
int m4b = 9; //Select direction and speed of DC motor 4
int m5a=10; //Select direction and speed of DC motor 5
int m5b=11; //Select direction and speed of DC motor 5
int m6a=12; //Select direction and speed of DC motor 6
int m6b=13; //Select direction and speed of DC motor 6
int m7a = 2; //Select direction and speed of Suction Pump
int m7b = 3; //Select direction and speed of Suction Pump
int a; //Encoder pulse count

  /*
                    * Function Name:  servo_setup
                    * Input:    NONE
                    * Output:   NONE
                    * Logic:    The next element of the Fibonacci series is given by
                    *     next = current_element + prev_element. The code loops for num_elements and
                    *     prints out the next element
                    * Example Call:   servo_setup();
                    *
                    */
void servo_setup() {

  pwm.begin();
  pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz updates
  delay(10);

  pwm1.begin();
  pwm1.setPWMFreq(50);
  //delay(10);
  
  pwm2.begin();
  pwm2.setPWMFreq(50);
  delay(10);
}

    /*
                    * Function Name:  setServoPulse
                    * Input:    n -> unsigned integer which stores servo number, pulse->stores pulse width
                    * Output:   NONE
                    * Logic:    Initializing pulse count and getting motors to reset position
                    * Example Call:   setServoPulse(0, 0.001);
                    *
                    */
// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {

  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 50;   // 50 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;  // convert to us
  pulse /= pulselength;
  Serial.println(pulse);
  
  pwm.setPWM(n, 0, pulse);
  delay(10);
  pwm1.setPWM(n, 0, pulse);
  delay(10);
  pwm2.setPWM(n, 0, pulse);
  delay(10);
}
   /*
                    * Function Name:  servo_moisture_check
                    * Input:    NONE
                    * Output:   NONE
                    * Logic: Used to drive the arm for checking soil moisture     
                    * Example Call:   servo_moisture_check();
                    *
                    */


void servo_moisture_check() {
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm1.setPWM(servonum, 0, pulselen);
    delay(5);
    pwm2.setPWM(servonum, 0, pulselen);
    delay(5);
  }
  delay(500);
  
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
    delay(5);
    pwm1.setPWM(servonum, 0, pulselen);
    delay(5);
    pwm2.setPWM(servonum, 0, pulselen);
    delay(5);
  }

  delay(250);

  servonum ++;
  if (servonum > 411) servonum = 0;


}

   /*
                    * Function Name:  servo_loop
                    * Input:    NONE
                    * Output:   NONE
                    * Logic: Used to drive the arm for completing the seeding process     
                    * Example Call:   servo_loop();
                    *
                    */
void servo_loop() {
  
  // Picking up seeds
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servonum, 283, pulselen);
    delay(5);
  }
  delay(500);
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX/2; pulselen++) {
    pwm1.setPWM(servonum, 0, pulselen);
    delay(5);
    pwm2.setPWM(servonum, 0, pulselen);
    delay(5);
  }
  delay(500);
  //Turn on suction
  digitalWrite(En7,HIGH);
  analogWrite(m7a,150);  //6.35V
  analogWrite(m7b,0);
    
  delay(1000);
  //Retracing servos
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm2.setPWM(servonum, 0, pulselen);
    delay(5);
    pwm1.setPWM(servonum, 0, pulselen);
    delay(5);
    pwm.setPWM(servonum, 283, pulselen);
    delay(5);
  }
  delay(500);
  //Proceeding for seeding
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm1.setPWM(servonum, 0, pulselen);
    delay(5);
    pwm2.setPWM(servonum, 0, pulselen);
    delay(5);
  }
  delay(500);
  //Turn off Suction
  analogWrite(m7a,0);
  delay(1000);
  //Retracing Servos
  
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
    delay(5);
    pwm1.setPWM(servonum, 0, pulselen);
    delay(5);
    pwm2.setPWM(servonum, 0, pulselen);
    delay(5);
  }

  delay(250);

  servonum ++;
  if (servonum > 411) servonum = 0;

} 

     /*
                    * Function Name:  setup
                    * Input:    NONE
                    * Output:   NONE
                    * Logic:    This function is used to set pins as Input and Output. This code executes only once initially. 
                    * Example Call:   setup();
                    *
                    */
void setup() {

  //All motor control pins are set as output
  pinMode(En1,OUTPUT); 
  pinMode(En2,OUTPUT);
  pinMode(En3,OUTPUT);
  pinMode(En4,OUTPUT);
  pinMode(m1a,OUTPUT);
  pinMode(m1b,OUTPUT);
  pinMode(m2a,OUTPUT);
  pinMode(m2b,OUTPUT);
  pinMode(m3a,OUTPUT);
  pinMode(m3b,OUTPUT);
  pinMode(m4a,OUTPUT);
  pinMode(m4b,OUTPUT); 
  pinMode(m5a,OUTPUT);
  pinMode(m5b,OUTPUT);
  pinMode(m6a,OUTPUT);
  pinMode(m6b,OUTPUT);
  pinMode(42,INPUT);  //Read encoder pulses 
  servo_setup(); //Setup all servo motors
  Serial.begin(9600); //Initialise serial monitor for Arduino IDE
}

       /*
                    * Function Name:  move_forward
                    * Input:    NONE
                    * Output:   NONE
                    * Logic:    This function is used for moving the rover in forward direction. 
                    * The PWM values are set so that RPM of all motors is equal.
                    * This will prevent deviation of rover from its path.
                    * Example Call:   move_forward();
                    *
                    */
void move_forward()
{

   digitalWrite(En1,HIGH);
   digitalWrite(En2,HIGH);
   digitalWrite(En3,HIGH);
   digitalWrite(En4,HIGH);
   digitalWrite(En5,HIGH);
   digitalWrite(En6,HIGH);
   analogWrite(m1a,0);  
   analogWrite(m1b,192);
   analogWrite(m2a,192);
   analogWrite(m2b,0);  
   analogWrite(m3a,192);
   analogWrite(m3b,0);  
   analogWrite(m4a,0);
   analogWrite(m4b,192);  

   //Encoder Motors
   analogWrite(m5a,77);
   analogWrite(m5b,0);   
   analogWrite(m6a,77);
   analogWrite(m6b,0);
}

       /*
                    * Function Name:  dist_moved
                    * Input:    None
                    * Output:   None
                    * Logic:    This function is used to calculate encoder pulses.
                    * Once the distance is covered for each seed, the rover will stop.
                    * Example Call:   dist_moved();
                    *
                    */
void dist_moved()
{

//Integer a is used to store value of pulses.
  while(a<=540)
  {
    //This loop executes when pulse is HIGH
    while(digitalRead(42));
    //Increment 'a' when the pulse is LOW
    a++;
    //This loop executes when pulse is LOW
    while(!digitalRead(42));
    Serial.println(a);
  }
  //Stop the rover once required pulses are reached
  stop_rover();
  delay(1000);
}

       /*
                    * Function Name:  stop_rover()
                    * Input:    NONE
                    * Output:   NONE
                    * Logic:    This function is used to stop the rover. 
                    * All PWM pins that are HIGH when the rover moves in a direction are set LOW for the motors to stop.
                    * Example Call:   stop_rover();
                    *
                    */
void stop_rover()
{  
   digitalWrite(En1,LOW);
   digitalWrite(En2,LOW);
   digitalWrite(En3,LOW);
   digitalWrite(En4,LOW);
   digitalWrite(En5,LOW);
   digitalWrite(En6,LOW);
   analogWrite(m1b,0);
   analogWrite(m2a,0);
   analogWrite(m3a,0);
   analogWrite(m4b,0);
   analogWrite(m5a,0);
   analogWrite(m6a,0);
}

       /*
                    * Function Name:  sensors()
                    * Input:    None
                    * Output:   Output values will be between 0 to 255 as 10 bit ADC
                    * Logic:    Read Values on ADC output pins. 
                    * Example Call:   sensors();
                    *
                    */
void sensors()
{
  int soilmoisture;
  int humidity;
  int tempreture;
  int Vt=0,Vh=0,Vm=0;
  tempreture = analogRead(A0);//connect sensor to Analog 0
  humidity = analogRead(A1);//connect sensor to Analog 1
  soilmoisture = analogRead(A2); //connect sensor to Analog 2
  Serial.print(Vt); //Sending these values on serial port so that Rpi connected to USB can read it
  Serial.print("\t");
  Serial.print(Vh);
  Serial.print("\t");
  Serial.print(Vm);
  Serial.print("\t");
   
 
}

       /*
                    * Function Name:  loop()
                    * Input:    None
                    * Output:   None
                    * Logic:    
                    * Calls functions for arm mechanism, rover movement and reading sensor values.
                    * Example Call:   loop();
                    *
                    */
void loop() 
{
  //servo_moisture_check(); //check soil moisture
  move_forward(); //Rover moves in forward direction
  dist_moved(); //Calculate distance moved so that it can stop at specific distance
  delay(35000);
  a=0; //Reset a every time rover stops
  //servo_loop(); //Arm mechanism for sowing seeds
}
