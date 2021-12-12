#include<LiquidCrystal.h>
#include <Adafruit_Fingerprint.h>

LiquidCrystal lcd( 4, 5, 6, 7, 8, 9);
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id, k;
byte Candidate_vote[3], ck, cks;
int ind = 0, N;
#define SA 11
#define SB 12
#define SC 13
#define SV 10
#define SR 1
#define Reg 0

boolean vote, voted_ID[100];
void showResult();
void caculateResult();
void voting();
uint8_t registration();
uint8_t checkThumb();

void setup() {
  lcd.begin(16,2);      //LCD

  pinMode(SA, INPUT);
  pinMode(SB,INPUT);
  pinMode(SC,INPUT);
  pinMode(SV,INPUT);
  pinMode(SR,INPUT);
  pinMode(Reg,INPUT);
  
  digitalWrite(SA, HIGH);
  digitalWrite(SB, HIGH);
  digitalWrite(SC, HIGH);
  digitalWrite(SV, HIGH);
  digitalWrite(SR, HIGH);
  digitalWrite(Reg, HIGH);
  
  lcd.clear();

  //Finger print operation
  finger.begin(57600);
  if (finger.verifyPassword()) {
    lcd.print(" Finger Print");
    lcd.setCursor(0,1);
    lcd.print("   Found");
  } else {
    lcd.print(" Finger Print");
    lcd.setCursor(0,1);
    lcd.print(" Not  Found");
    //Serial.print(" Not  Found");
    while (1) { delay(1); }
  }
  finger.getTemplateCount();
  N = finger.templateCount;
 
  delay(2000);
  lcd.clear();
  lcd.print("Electronic ");
  lcd.setCursor(0,1);
  lcd.print("Voting Machine ");
  
}

void loop() {
  if(digitalRead(Reg) == LOW){
    if(registration() == FINGERPRINT_OK){
      delay(1000);
        lcd.clear();
        lcd.print("Registered!");
        lcd.setCursor(0,1);
        lcd.print("Your ID: ");
        lcd.print(id);
    }
    else{
        delay(1000);
        lcd.clear();
        lcd.print("Failed!");
        lcd.setCursor(0,1);
        lcd.print("Try Again");
        lcd.print(id);
    }
    
  }
  
  if(digitalRead(SV) == LOW){
    voting();
  }
    
  if(digitalRead(SR) == LOW){
    showResult();
    caculateResult();
  }
   
}
uint8_t registration(){
  ck = 4;
  while(ck--){
    lcd.clear();
    lcd.print("Put your Thumb");
    int p = -1;
    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      if(p == FINGERPRINT_OK){
        lcd.clear();
        lcd.print("Image taken");
      }
    } 
    finger.image2Tz(1);
    p = -1;
    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      if(p == FINGERPRINT_OK){
        lcd.setCursor(0,1);       
        lcd.print("Again taken");
      }
    }
    finger.image2Tz(2);
    p = -1;
    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
      lcd.clear();
      lcd.print("Prints matched!");
      id = N + 1;
      ck = 0;
      k = finger.storeModel(id);
      if (k == FINGERPRINT_OK) {
        N++;
        return k;
      }else id--;
   }
  }
  return -1;
}

uint8_t checkThumb(){
  lcd.clear();
  lcd.print("Put your Thumb");
  cks = 4;
  while(cks--){
  int p = -1;
  while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      if(p == FINGERPRINT_OK){
        lcd.clear();
        lcd.print("Image taken");
        lcd.setCursor(0,1);
        lcd.print("Please wait..");
      }
     }
      finger.image2Tz();
    p = -1;
    p = finger.fingerFastSearch(); 
    if (p == FINGERPRINT_OK){
      cks  = 0;
      return p; 
    }
   
 }
  return -1;
}

void voting(){
  int p= -1;
  p = checkThumb();
  
  if(p == FINGERPRINT_OK){
    if(voted_ID[finger.fingerID] == true){
      lcd.clear();
      lcd.print("You already");
      lcd.setCursor(0,1);
      lcd.print("   voted");
    }
    else{
      lcd.clear();
      lcd.print("Select One");
      while(ind == 0){
        if(digitalRead(SA)==LOW)
          ind = 1;
        if(digitalRead(SB)==LOW)
          ind = 2;
        if(digitalRead(SC)==LOW)
          ind = 3;
        }
        Candidate_vote[ind - 1]++;
        ind = 0;
        lcd.clear();
        lcd.print("You voted");
        voted_ID[finger.fingerID] = true;
      }
  }

  else {
    lcd.clear();
    lcd.print("Not matched");
    lcd.setCursor(0,1);
    lcd.print("  Try again");
  }
}
void showResult(){
  lcd.clear();
  for(int i = 0; i < 3; i++){
    lcd.setCursor(0 + 7*i, 0);
    lcd.print((char)('A' + i));
    lcd.setCursor(0 + 7*i, 1);
    lcd.print(Candidate_vote[i]);
  }
  delay(2000);  
}

void caculateResult(){
  if(Candidate_vote[0] >= Candidate_vote[1] && Candidate_vote[0] >= Candidate_vote[2]){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Elected: A");
    lcd.setCursor(0,1);
    lcd.print("Got vote: ");
    lcd.print(Candidate_vote[0]);
  }

  else if(Candidate_vote[1] >= Candidate_vote[2]){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Elected: B");
    lcd.setCursor(0,1);
    lcd.print("Got vote: ");
    lcd.print(Candidate_vote[1]);
  } 

  else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Elected: C");
    lcd.setCursor(0,1);
    lcd.print("Got vote: ");
    lcd.print(Candidate_vote[2]);
  } 
  delay(1000);
}
