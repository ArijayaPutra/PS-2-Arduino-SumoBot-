#include <PS2X_lib.h>
#define PS2_DAT        12
#define PS2_CMD        11
#define PS2_SEL        10
#define PS2_CLK        13
#define pressures   false
#define rumble      true
PS2X ps2x;
int error = 0;
byte type = 0;
byte vibrate = 50;

#define BRAKE   0
#define CW      1
#define CCW     2
#define DEC_1   50
#define DEC_2   50
#define DEF_1   100 
#define DEF_2   100
#define INC_1   150
#define INC_2   150
#define MAX_1   200
#define MAX_2   200
#define MAX     255
#define MOTOR_1 0
#define MOTOR_2 1
short usSpeed_1;
short usSpeed_2;
unsigned short usMotor_Status_1;
unsigned short usMotor_Status_2;

#define RPWM_1  6
#define LPWM_1  9
#define R_EN_1  8
#define L_EN_1  7
#define RPWM_2  3
#define LPWM_2  5
#define R_EN_2  4
#define L_EN_2  2

void setPWMfrequency(int freq) {
  TCCR0B = TCCR2B & 0b11111000 | freq ;
  TCCR1B = TCCR2B & 0b11111000 | freq ;
  TCCR2B = TCCR2B & 0b11111000 | freq ;
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm) {
  if(motor == MOTOR_1){
    if(direct == CW){
      digitalWrite(L_EN_1, HIGH);
      digitalWrite(R_EN_1, HIGH);
      analogWrite (LPWM_1, pwm);
      analogWrite (RPWM_1, 0);
    }
    else if(direct == CCW){
      digitalWrite(L_EN_1, HIGH);
      digitalWrite(R_EN_1, HIGH);
      analogWrite (LPWM_1, 0);
      analogWrite (RPWM_1, pwm);   
    }
    else{
      digitalWrite(L_EN_1, LOW);
      digitalWrite(R_EN_1, LOW);
      analogWrite (LPWM_1, 0);
      analogWrite (RPWM_1, 0);           
    } 
  }
  else if(motor == MOTOR_2){
    if(direct == CW){
      digitalWrite(L_EN_2, HIGH);
      digitalWrite(R_EN_2, HIGH);
      analogWrite (LPWM_2, pwm);
      analogWrite (RPWM_2, 0);
    }
    else if(direct == CCW){
      digitalWrite(L_EN_2, HIGH);
      digitalWrite(R_EN_2, HIGH); 
      analogWrite (LPWM_2, 0);
      analogWrite (RPWM_2, pwm);   
    }
    else{
      digitalWrite(L_EN_2, LOW);
      digitalWrite(R_EN_2, LOW);
      analogWrite (LPWM_2, 0);
      analogWrite (RPWM_2, 0);          
    }
  }
}

void setPWM(){
  usSpeed_1 = DEF_1;
  usSpeed_2 = DEF_2;
  if(ps2x.Button(PSB_PAD_DOWN)){
    usSpeed_1 = DEC_1;
    usSpeed_2 = DEC_2;
  }
  if(ps2x.Button(PSB_PAD_UP)){
    usSpeed_1 = INC_1;
    usSpeed_2 = INC_2;
  }
  if(ps2x.Button(PSB_R2)){
    usSpeed_1 = MAX_1;
    usSpeed_2 = MAX_2;
    if(ps2x.Button(PSB_L2)){
      usSpeed_1 = MAX;
      usSpeed_2 = MAX;
    }
  }
}

void Stop() {
  Serial.println("Stop");
  usMotor_Status_1 = BRAKE;
  usMotor_Status_2 = BRAKE;
  motorGo(MOTOR_1, usMotor_Status_1, 0);
  motorGo(MOTOR_2, usMotor_Status_2, 0);
}

void Forward() {
  Serial.println("Forward");
  usMotor_Status_1 = CCW;
  usMotor_Status_2 = CW;
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);
}

void Reverse() {
  Serial.println("Reverse");
  usMotor_Status_1 = CW;
  usMotor_Status_2 = CCW;
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);
}

void Left(){
  Serial.println("Left");
  usMotor_Status_1 = CCW;
  usMotor_Status_2 = CW;
  motorGo(MOTOR_1, usMotor_Status_1, BRAKE);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);
}

void Right(){
  Serial.println("Right");
  usMotor_Status_1 = CCW; //berlawan arah
  usMotor_Status_2 = CW; //searah jarum jam;
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, BRAKE);
}

void reverseLeft(){
  Serial.println("Reverse Left");
  usMotor_Status_1 = CW;
  usMotor_Status_2 = CCW;
  motorGo(MOTOR_1, usMotor_Status_1, BRAKE);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);
}

void reverseRight(){
  Serial.println("Reverse Right");
  usMotor_Status_1 = CW; //berlawan arah
  usMotor_Status_2 = CCW; //searah jarum jam;
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, BRAKE);
}

void rotateLeft(){
  Serial.println("Rotate Left");
  usMotor_Status_1 = CW; //berlawan arah
  usMotor_Status_2 = CW; //searah jarum jam
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);
}

void rotateRight(){
  Serial.println("Rotate Right");
  usMotor_Status_1 = CCW; //berlawan arah
  usMotor_Status_2 = CCW; //searah jarum jam
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);  
}

void Standing(){
  if(ps2x.ButtonReleased(PSB_L3)) Stop();
  usSpeed_1 = MAX;
  usSpeed_2 = MAX;
  Reverse();
  delay(1000);
  if(ps2x.ButtonPressed(PSB_L3))  Forward();
}

void Racing(){
  if(ps2x.ButtonReleased(PSB_R3))   Stop();
  usSpeed_1 = MAX;
  usSpeed_2 = MAX;
  Forward();
  delay(1500);
  Reverse();
  delay(4000);
}

void IncreaseSpeed(){
  usSpeed_1 = usSpeed_1 + 10;
  usSpeed_2 = usSpeed_1 + 10;
  if(usSpeed_1 > 255 || usSpeed_2 > 255){
    usSpeed_1 = 255; 
    usSpeed_2 = 255; 
  }
  Serial.print("Speed -: ");
  Serial.print(usSpeed_1);
  Serial.print(" ");
  Serial.println(usSpeed_2);
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);  
}

void DecreaseSpeed(){
  usSpeed_1 = usSpeed_1 - 10;
  usSpeed_2 = usSpeed_2 - 10;
  if(usSpeed_1 < 0 || usSpeed_2 < 0){
    usSpeed_1 = 0; 
    usSpeed_2 = 0; 
  }
  Serial.print("Speed -: ");
  Serial.print(usSpeed_1);
  Serial.print(" ");
  Serial.println(usSpeed_2);
  motorGo(MOTOR_1, usMotor_Status_1, usSpeed_1);
  motorGo(MOTOR_2, usMotor_Status_2, usSpeed_2);  
}

void setup() {
  delay(300);
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  if (error == 0) {
    Serial.println("Sukses");
    Serial.print("pressures = ");
    if (pressures)  Serial.println("true ");
    else            Serial.println("false");
    Serial.print("rumble = ");
    if (rumble) Serial.println("true)");
    else        Serial.println("false");
  }
  else Serial.println("Check Wiring");

  pinMode(RPWM_1, OUTPUT);
  pinMode(LPWM_1, OUTPUT);
  pinMode(R_EN_1, OUTPUT);
  pinMode(L_EN_1, OUTPUT);
  pinMode(RPWM_2, OUTPUT);
  pinMode(LPWM_2, OUTPUT);
  pinMode(R_EN_2, OUTPUT);
  pinMode(L_EN_2, OUTPUT);
  digitalWrite(RPWM_1, LOW);
  digitalWrite(LPWM_1, LOW);
  digitalWrite(RPWM_2, LOW);
  digitalWrite(LPWM_2, LOW);
  digitalWrite(R_EN_1, HIGH);
  digitalWrite(L_EN_1, HIGH);
  digitalWrite(R_EN_2, HIGH);
  digitalWrite(L_EN_2, HIGH);
  Serial.begin(57600);
}

void loop() {
  setPWMfrequency(0x02);
  if(error == 1) return; 
  
  else {
    // PSB_START PSB_SELECT PSB_PAD_UP PSB_PAD_RIGHT PSB_PAD_LEFT PSB_PAD_DOWN
    // PSB_CROSS PSB_SQUARE PSB_TRIANGLE PSB_CIRCLE PSB_L1 PSB_L2 PSB_L3 PSB_R1 PSB_R2 PSB_R3
    // PSS_LX PSS_LY PSS_RX PSS_RY PSAB_PAD_UP PSAB_PAD_RIGHT PSAB_PAD_LEFT PSAB_PAD_DOWN
    ps2x.read_gamepad(false, vibrate);
    vibrate = ps2x.Analog(PSAB_CROSS);
    if(ps2x.Button(PSB_CROSS)){
      setPWM();
      if      (ps2x.Button(PSB_PAD_LEFT))   Left();
      else if (ps2x.Button(PSB_PAD_RIGHT))  Right();
      else    Forward();
    }
    else if(ps2x.Button(PSB_SQUARE)){
      setPWM();
      if      (ps2x.Button(PSB_PAD_LEFT))   reverseLeft();
      else if (ps2x.Button(PSB_PAD_RIGHT))  reverseRight();
      else    Reverse();
    }
    else if(ps2x.Button(PSB_PAD_LEFT)){
      setPWM();
      rotateLeft();
    }
    else if(ps2x.Button(PSB_PAD_RIGHT)){
      setPWM();
      rotateRight();
    }
    else if(ps2x.Button(PSB_L1)){
      setPWM();
      Left();
    } 
    else if(ps2x.Button(PSB_R1)){
      setPWM();
      Right();
    }
    else if(ps2x.Button(PSB_L3)){
      setPWM();
      Standing();
    }
    else if(ps2x.Button(PSB_R3)){
      setPWM();
      Racing();
    }
    else  Stop();     
  }

  //Uncomment Di Bawah Ini
  char user_input;
  while(Serial.available())
  {
    user_input = Serial.read();
    if (user_input =='1')
    {
      Forward();
      delay(1000);
      Stop();
    }
    else if (user_input =='2')
    {
      Reverse();
      delay(1000);
      Stop();
    }
    else if (user_input =='3')
    {
      Left();
      delay(1000);
      Stop();
    }
    else if (user_input =='4')
    {
      Right();
      delay(300);
      Stop();
    }
    else if (user_input =='5')
    {
      reverseLeft();
      delay(300);
      Stop();
    }
    else if (user_input =='6')
    {
      reverseRight();
      delay(300);
      Stop();
    }
    else if (user_input =='7')
    {
      rotateLeft();
      delay(300);
      Stop();
    }
    else if (user_input =='8')
    {
      rotateRight();
      delay(300);
      Stop();
    }
  }
  delay(50);
}
