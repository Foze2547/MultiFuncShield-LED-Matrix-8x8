#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

const int r = 8;  // กำหนดขนาดของเมทริกซ์ LED เป็น 8x8
const int c = 8; 
const int rows[8] = {22, 23, 24, 25, 26, 27, 28, 29}; // Anode + แถวของเมทริกซ์ LED
const int cols[8] = {A8, A9, A10, A11, A12, A13, A14, A15}; // cathod - คอลัมน์ของเมทริกซ์ LED
#define buzzer 3

void setup()
{ 
  Serial.begin(9600); 
  pinMode(buzzer, OUTPUT);
  Timer1.initialize(); 
  MFS.initialize(&Timer1); // initialize multi-function shield library

  MFS.write("inst");
  delay(2000);
  pinMode(buzzer, OUTPUT);
  for (int i = 0; i < 8; i++) {
    pinMode(rows[i], OUTPUT);
    pinMode(cols[i], OUTPUT);
  }
}

int x = 0;
int cnt = 0;
bool buzzerOn = false; // สร้างตัวแปรเก็บสถานะของ Buzzer

void loop()
{
  byte btn = MFS.getButton();
  if (btn)
  {
    byte buttonNumber = btn & B00111111; 
    byte buttonAction = btn & B11000000;
    if (buttonAction == BUTTON_PRESSED_IND)
    {
      switch(buttonNumber)
      {
        case 1:
          cnt++;
          if (cnt >= 11) {
            x = 1;}
          break;
        case 2: 
          if (cnt > 10) {
            cnt = 10; 
          } else {
            cnt--; 
          }
          break;
        case 3: 
          cnt = 0; 
          break;
      }
      if(cnt > 10){
        MFS.write(9999);
      }else{
      MFS.write(cnt);}
    }
  }
  if (x == 1){
    digitalWrite(buzzer, LOW);
    x = 0;
  }
  if(cnt > 10){
    x = 1;
  }else{
    digitalWrite(buzzer, HIGH);
  }
  

  if (cnt % 2 == 0) {
        E();
      } else {
        O();
      }
}


void E() {
  bool pattern[r][c] = {
    {1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 0}
  };
  displayPattern(pattern);
}

void O() {
  bool pattern[r][c] = {
    {0,0,0,1,1,0,0,0},
    {0,0,1,0,0,1,0,0},
    {0,0,1,0,0,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,0,1,0,0,1,0,0},
    {0,0,1,0,0,1,0,0},
    {0,0,0,1,1,0,0,0}
  };
  displayPattern(pattern);
}

void displayPattern(bool pattern[r][c]) {
  for (int count = 0; count <= cnt; count++) { // เปลี่ยน while loop เป็น for loop ที่ทำงานตามเงื่อนไขของ cnt
    for (int col = 0; col < 8; col++) {
      digitalWrite(cols[0] + col, LOW);
      for (int row = 0; row < 8; row++) {
        digitalWrite(rows[0] + row, pattern[row][col] ? HIGH : LOW);
      }
      delay(1);
      digitalWrite(cols[0] + col, HIGH);
    }
  }
}