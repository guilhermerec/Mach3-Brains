//Pin Definition IDC OUTPUTS FROM ARDUINO
int A=A0; //IN5 
int B=A1; //IN6 
int C=A2; //IN7 
int D=A3; //IN8 
int JOGMODE=12;//IN11 (CONT/STEP) -OEM245
int JOGSTEP=13; //IN12 (Cycle JOG)-OEM171

//Pin Definition ARDUINO INPUTS FROM ARDUINO
int MPGA=2;//Encoder Canal A
int MPGB=3;//Encoder Canal B
int SELX=4; //Select Channel X
int SELY=5; //Select Channel Y
int SELZ=6; //Select Channel Z
int SELM=7; //Select MODE LINKED TO FEATURE
int PotX=A4; //Potentiometer X and Z(if click to change)
int PotY=A5; //Potentiometer Y


//Variables
volatile int posi = 0; // specify posi as volatile
volatile int posiprev=0; //
int MPGstop=0;
int rot = 0; //0=POSITIVE MPG SPIN; 1=NEGATIVE MPG SPIN
int CHAvar=0;
int CHBvar=0;
int MODE=0;
unsigned long SELMPressedAt = 0;
bool SELMReleased = false;
int SELMPrevState = 0;
long diff=0;
int JOY=0;
int CHANGEJOG=0;
int CASE=0;
int FEATURE=0; //HOW TO WORK JOYSTICK, MPG...LINKED TO SELM
int X =0;
int Y =0;
int Z =0;
int firstQ=0; //___FLAG IF ITS FIRST TIME OR NOTE JOYSTICK BUTTON WAS PRESSED QUICK
int firstS=0; //___FLAG IF ITS FIRST TIME OR NOTE JOYSTICK BUTTON WAS PRESSED SHORT
int firstL=0; //___FLAG IF ITS FIRST TIME OR NOTE JOYSTICK BUTTON WAS PRESSED LONG
int ValueX=0;
int ValueY=0;


void setup() {
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
  pinMode(JOGMODE,OUTPUT);
  pinMode(JOGSTEP,OUTPUT);
  pinMode(PotX, INPUT);
  pinMode(PotY, INPUT);

  pinMode(MPGA,INPUT);
  pinMode(MPGB,INPUT);
  pinMode(SELX,INPUT_PULLUP);
  pinMode(SELY,INPUT_PULLUP);
  pinMode(SELZ,INPUT_PULLUP);
  pinMode(SELM,INPUT_PULLUP);
   
  attachInterrupt(digitalPinToInterrupt(MPGB),readEncoder,RISING);

digitalWrite(A,HIGH); 
digitalWrite(B,HIGH);
digitalWrite(C,HIGH); 
digitalWrite(D,HIGH);
digitalWrite(JOGMODE,HIGH); 
digitalWrite(JOGSTEP,HIGH);

Serial.begin(9600);
}


//____START MAIN ROUTINE__________________________
void loop() {

digitalWrite(A,HIGH); 
digitalWrite(B,HIGH);
digitalWrite(C,HIGH); 
digitalWrite(D,HIGH);

INPUTMODE(); //___________JOY BUTTON FEATURES (QUICK,SHORT and LONG PRESS)

if (JOY==1) //VALUE COMES FROM INPUTMODE
 {JOYSTICK();}
else if (JOY==0)
 {MPG();}

  Serial.print("CASE=");
 Serial.print(CASE);
  Serial.print("; posi=");
 Serial.print(posi);
   Serial.print("; rot=");
 Serial.print(rot);
    Serial.print("; MPGstop=");
  Serial.print(MPGstop);
    Serial.print("; JOY=");
  Serial.print(JOY);
  Serial.println();
 
} //____END MAIN ROUTINE__________________________

//_______________________________________________SUB ROUTINES STARTS HERE__________________________________________________________________________________

//READ THE ENCODER ROUTINE
void readEncoder(){
  rot = digitalRead(MPGA);
  if(rot > 0){
    rot=0; //POSITIVE MPG SPIN
    posi++;
  }
  else{
    rot=1; //NEGATIVE MPG SPIN
    posi--;
  }
}
//END READ THE ENCODER ROUTINE

void INPUTMODE()//___________JOY BUTTON FEATURES (QUICK=<200ms,SHORT=200-600ms and LONG PRESS=>1200ms)
 {FEATURE=digitalRead(SELM); 

  if (FEATURE != SELMPrevState) {
    if (FEATURE == LOW) {
      SELMReleased = false;
      SELMPressedAt = millis();
    }
    else {
      SELMReleased = true;}

  SELMPrevState=FEATURE;
  }

  if (SELMReleased == true) {
          diff = millis()-SELMPressedAt;

          if (diff<200) { //________CASE FOR QUICK PRESS CHANGE STEP RESOLUTION - OEM171
          digitalWrite(A,HIGH); // BIN 4
          digitalWrite(B,HIGH);
          digitalWrite(C,LOW);
          digitalWrite(D,HIGH);
          delay(100);
          digitalWrite(A,HIGH); // BIN 0
          digitalWrite(B,HIGH);
          digitalWrite(C,HIGH);
          digitalWrite(D,HIGH);
          delay(100);}
         
          if (diff>300&&diff<800&&firstS==0)  ////________CASE FOR SHORT PRESS CHANGE JOG CONTINUOS TO INCREMENTAL
          {  digitalWrite(A,LOW); // BIN 1 - OEM204
          digitalWrite(B,HIGH);
          digitalWrite(C,HIGH);
          digitalWrite(D,HIGH);
          delay(100);
          firstS=1;}
          else if (diff>300&&diff<800&&firstS==1)
           {
          digitalWrite(A,HIGH); // BIN 2 - OEM205
          digitalWrite(B,LOW);
          digitalWrite(C,HIGH);
          digitalWrite(D,HIGH);
          delay(100);
          firstS=0;
           }
          
          if (diff>1000&&JOY==0) { //___________CASE FOR LONG PRESS ACTIVATE JOY
            JOY=1;}
          else if (diff>1000&&JOY==1) {
            JOY=0;}

          SELMReleased=false;
          SELMPressedAt = 0;
                            }
  }
//END READ GOLDENBUTTON ROUTINE

void JOYSTICK () //_____SELECT JOYSTICK MODE
 {

ValueX=analogRead(PotX); //MOVE X AXIS
      if(ValueX>1000)
        { digitalWrite(A,LOW); // BIN 15
          digitalWrite(B,LOW);
          digitalWrite(C,LOW);
          digitalWrite(D,LOW); 
          delay(100);
          }
      else if(ValueX<200)
        { digitalWrite(A,HIGH); // BIN 14
          digitalWrite(B,LOW);
          digitalWrite(C,LOW);
          digitalWrite(D,LOW);
          delay(100);} 

      if(ValueX>200&&ValueX<1000)
        {
          digitalWrite(A,HIGH); // BIN 0
          digitalWrite(B,HIGH);
          digitalWrite(C,HIGH);
          digitalWrite(D,HIGH); 
        } 




ValueY=analogRead(PotY); //MOVE Y AXIS
      if(ValueY>1000)
         { digitalWrite(A,LOW); // BIN 13
          digitalWrite(B,HIGH);
          digitalWrite(C,LOW);
          digitalWrite(D,LOW); 
          delay(100);
          }
      else if(ValueY<200)
        { digitalWrite(A,HIGH); // BIN 12
          digitalWrite(B,HIGH);
          digitalWrite(C,LOW);
          digitalWrite(D,LOW);
          delay(100);} 

      if(ValueY>200&&ValueY<1000)
        {
          digitalWrite(A,HIGH); // BIN 0
          digitalWrite(B,HIGH);
          digitalWrite(C,HIGH);
          digitalWrite(D,HIGH);
        }


    Serial.print("; ValueY=");
  Serial.print(ValueX);
    Serial.print("; ValueY=");
  Serial.print(ValueY);
  Serial.println();



 }
//END JOYSTICK ROUTINE

void MPG() //_____SELECT MPG MODE
{
if(posi==posiprev) //IF ELSE determines MPG stopped
   {MPGstop=0;} //MPG Stopped
else
   {MPGstop=1;} //MPG still spining
posiprev=posi;

//3 position switch decides which axis will move on mpgmode
X = digitalRead(SELX)*3; //PULLUP 0V=SELECTED (0+2+1)=3;
Y = digitalRead(SELY)*2; //PULLUP 0V=SELECTED (3+0+1)=4;
Z = digitalRead(SELZ)*1; //PULLUP 0V=SELECTED (3+2+0)=5;
CASE=X+Y+Z; //____DEFINES WHICH AXIS WILL BE SELECTED 3=X, 4=Y, 5=Z;

//rot and MPGstop decides direction ++, -- or stopped. Cases come from above.
switch(CASE){
  case 3: // MOVE X AXIS
      if(rot==0&&MPGstop==1)
        { digitalWrite(A,LOW); // BIN 15
          digitalWrite(B,LOW);
          digitalWrite(C,LOW);
          digitalWrite(D,LOW); 
          delay(100);
          }
      else if(rot==1&&MPGstop==1)
        { digitalWrite(A,HIGH); // BIN 14
          digitalWrite(B,LOW);
          digitalWrite(C,LOW);
          digitalWrite(D,LOW);
          delay(100);} 

      if(MPGstop==0)
        {
          digitalWrite(A,HIGH); // BIN 0
          digitalWrite(B,HIGH);
          digitalWrite(C,HIGH);
          digitalWrite(D,HIGH); 
        } 
  break;

  case 4: // MOVE Y AXIS
      if(rot==0&&MPGstop==1)
         { digitalWrite(A,LOW); // BIN 13
          digitalWrite(B,HIGH);
          digitalWrite(C,LOW);
          digitalWrite(D,LOW); 
          delay(100);
          }
      else if(rot==1&&MPGstop==1)
        { digitalWrite(A,HIGH); // BIN 12
          digitalWrite(B,HIGH);
          digitalWrite(C,LOW);
          digitalWrite(D,LOW);
          delay(100);} 

      if(MPGstop==0)
        {
          digitalWrite(A,HIGH); // BIN 0
          digitalWrite(B,HIGH);
          digitalWrite(C,HIGH);
          digitalWrite(D,HIGH);
        }
  break;

  case 5: // MOVE Z AXIS
      if(rot==0&&MPGstop==1)
        { digitalWrite(A,LOW); // BIN 11
          digitalWrite(B,LOW);
          digitalWrite(C,HIGH);
          digitalWrite(D,LOW); 
          delay(100);
          }
      else if(rot==1&&MPGstop==1)
        { digitalWrite(A,HIGH); // BIN 10
          digitalWrite(B,LOW);
          digitalWrite(C,HIGH);
          digitalWrite(D,LOW);
          delay(100);} 

      if(MPGstop==0)
        {
          digitalWrite(A,HIGH); // BIN 0
          digitalWrite(B,HIGH);
          digitalWrite(C,HIGH);
          digitalWrite(D,HIGH);
        }
  break;
    }//END SWITCH CASE


}//END MPG ROUTINE
