const int OUTPUT_LED_PIN = 13;
const int sonicLED = 12;
const int OUTPUT_PIEZO_PIN = 1;
const int INPUT_FSR_PIN_1 = A0;
const int INPUT_FSR_PIN_2 = A1;
const int DELAY_MS = 1000; 
const int trigPin = 3;
const int echoPin = 2;
const int SPK = 10;
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>  

#define NOTE_C4  262
LiquidCrystal_I2C lcd(0x27, 16, 2); 
long duration; 
int distance;
int p_data1;
int p_data2;
int u_data;
int p_delta = 10000;
int p_counteralert = 2;
int p_counter;
int u_counteralert = 2;
int u_counter;
int u_delta = 30;
int melody[] = {
  NOTE_C4
};
int noteDurations[] = {
  4
};
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11);
int BTsignal_1;
int BTsginal_2;
String value1; 
String value2;
String value3;
String value4;
char caracter;
String p_data1_p_data2;
int situation_code = 0;
int p_situation = 0;
int u_situation = 0;
 
void setup() {
  pinMode(OUTPUT_LED_PIN, OUTPUT);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);
  pinMode(INPUT_FSR_PIN_1, INPUT);
  pinMode(INPUT_FSR_PIN_2, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  BTSerial.begin(9600);
  Serial.println("Ultrasonic Sensor HC-SR04 Test");
  Serial.println("with Arduino UNO R3");
  Serial.println("Bluetooth Function Activated");
  }


/*void data(int multi){
  multi = 0;
  for (){
    
  }
  u_data = u_sense();
  p_data1 = p_sense1();
  p_data2 = p_sense2();
}
*/
int p_sense1(){
  int fsrVal = analogRead(INPUT_FSR_PIN_1);
  int freq = map(fsrVal, 0, 1023, 0, 1000);
  return freq;
}

int p_sense2(){
  int fsrVal2 = analogRead(INPUT_FSR_PIN_2);
  int freq2 = map(fsrVal2, 0, 1023, 0, 1000);
  return freq2;
}

void feedbackled(){
  //
}

void lcdprintScode(){
  Serial.println("p_situation=" + p_situation);
  Serial.println("u_situation=" + u_situation);
  Serial.println("situation_code=" + situation_code);
}

void PU_check(){
  if (abs(p_data1 - p_data2) >= p_delta){
    p_counter = p_counter+1;
   }
   if (u_data >= u_delta){
    u_counter = u_counter+1;
  }
}

void Case_count_check(){
  if (p_counter == p_counteralert){
    play();
    p_situation = 1;
    p_counter = 0;}

  if (u_counter == u_counteralert){
    play();
    u_situation = 1;
    u_counter = 0;}
}

void Case_define(){
  if (u_situation = 1,p_situation = 0){
    situation_code = 2;
    p_situation = 0;
  } else if (u_situation = 0,p_situation = 1){
    situation_code = 3;
    u_situation = 0;
  }else if (u_situation = 1,p_situation = 1){
    situation_code = 4;
    p_situation = 0;
    u_situation = 0;
  }else{
    situation_code = 1;
  }
  lcdprintScode();
}

void loop() {
  
  data();
  ledprint(p_data1, p_data2, u_data);
  Serial.print("PR=");
  Serial.println(p_data1);
  Serial.print("PL=");
  Serial.println(p_data2);
  Serial.print("U=");                
  Serial.println(u_data);
  
  
  PU_check();
  Case_count_check();
  Case_define();
  delay(300);
  
    /*int BTsignal = 1;

  if (BTsignal_1 = 1){               test
    Serial.println(BTsignal_1 == 1);
   }*/

//if(BTSerial.available()) {
 /*  caracter = BTSerial.read();
  
    if(caracter == 'A'){BTSerial.write(p_data1);}
    if(caracter == 'B'){BTSerial.write(p_data2);} 
    if(caracter == 'C'){ 
    p_data1_p_data2 = (String) p_data1 + "," + (String) p_data2;
    BTSerial.write(p_data1);
    }  */ 
   value1 = p_data1;
   value2 = p_data2;
   value3 = u_data;
   value4 = situation_code;
   String BTData1 = String(value1);
   String BTData2 = String(value2);
   String BTData3 = String(value3);
   String BTData4 = String(value4);
   String BTDatatoSend_P = "RightData = " + BTData1 + ",LeftData = " + BTData2;
   String BTDatatoSend_U = "DistanceData = " + BTData3;
   String BTSituation = "Situation" + BTData4;
   BTSerial.print(BTDatatoSend_P);
   BTSerial.print(BTDatatoSend_U);
   BTSerial.print(BTSituation);
   Serial.print(value1);
   Serial.print(value2);
   Serial.print(value3);
   Serial.print(value4);
   delay(1000);
  /*  BTSerial.write(p_data2);
   Serial.print(value2);*/

  //}

  if (p_data1 + p_data2 == 0){
    int p_delta = 10000;
  }
}


void ledprint(int p_data1, int p_data2, int s_data){
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("pR=");
  lcd.setCursor(3,0);
  lcd.print(p_data1);
  lcd.setCursor(8,0);
  lcd.print("pL=");         
  lcd.setCursor(11,0);
  lcd.print(p_data2);
  lcd.setCursor(0,1);
  lcd.print("distance");
  lcd.setCursor(9,1);
  lcd.print(s_data);
  lcd.setCursor(13,1);
  lcd.print("cm");
}


void data(){
  u_data = u_sense();
  p_data1 = p_sense1();
  p_data2 = p_sense2();
  
  if (p_data1 + p_data2 >= 0){
    int p_delta = (p_data1 + p_data2)/10;
  }
}

void play(){
  for (int thisNote = 0; thisNote < 3; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(SPK, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(SPK);
  }
}

int u_sense(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance; 
}