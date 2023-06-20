#include <MsTimer2.h>

volatile int V_START = 56, V_END = 80, V_OFF = 50, T = 180;

#define PLS_RATIO 85
#define AIN1 9
#define AIN2 8
#define AIN3 7
#define AIN4 6
#define AIN5 5
#define AIN6 4
#define VR A0

volatile unsigned long cnt = 0;
volatile int d = 0;
volatile boolean up = true;

void t2isr(){
  if(up){
    d = V_START + (V_END - V_START) * cnt / T;
    cnt++;
    if(cnt > T){
      cnt = T;
    }
  }
  else if(cnt > 0){
    d = V_OFF + (V_END - V_OFF) * cnt / T;
    cnt--;
  }
}

void speedUp(){
  cnt = 0;
  up = true;
  MsTimer2::start();
  d = 50;
}
void speedDown(){
  cnt = T;
  up = false;
}

void setup(){

  int r;
  int pattern = 5;
  
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinmode(AIN3, OUTPUT);
  pinmode(AIN4, OUTPUT);
  pinmode(AIN5, OUTPUT);
  pinmode(AIN6, OUTPUT);
  Serial.begin(9600);
  MsTimer2::set(100, t2isr);

  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  digitalwrite(AIN3, HIGH);
  digitalwrite(AIN4, LOW);
  digitalwrite(AIN5, LOW);
  digitalwrite(AIN6, LOW);
  r = analogRead(VR) * pattern / 1024;
  Serial.println(r);
  delay(1000);
  switch(r){
    case 0:
      vvvf209();
      break;
    case 1:
      keikyu_new1000();
      break;
    case 2:
      vvvf373();
      break;
    case 3:
      e231_1000();
      break;
    case 4:
      seibu6000();
      break;
  }
  //vvvf209();
  //keikyu_new1000();
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  digitalwrite(AIN3, HIGH);
  digitalwrite(AIN4, LOW);
  digitalwrite(AIN5, LOW);
  digitalwrite(AIN6, LOW);
}


void changeTone(int f1, int f2, unsigned long t);

void loop(){
}

void seibu6000(){
  speedUp();
  changeTone(784, 784, 3720);
  changeTone(659, 988, 2270);
  changeTone(587, 784, 1920);
  changeTone(277, 415, 10070);
  freeRun(415);
  changeTone(622, 415, 9150);
  speedDown();
  changeTone(659, 587, 1750);
  changeTone(988, 831, 2970);
  changeTone(1319, 698, 6020);
  changeTone(988, 311, 5640);
}

void e231_1000(){
  speedUp();
  changeTone(1046, 1046, 4500);
  changeTone(1046, 784, 9400);
  changeTone(784, 1865, 3420);
  changeTone(659, 740, 2300);
  changeTone(340, 466, 11370);
  freeRun(466);
  changeTone(466, 340, 10000);
  changeTone(740, 659, 2300);
  speedDown();
  changeTone(1865, 784, 3420);
  changeTone(784, 1046, 8400);
  changeTone(1046, 1046, 4500);

}

void vvvf373(){
  speedUp();
  changeTone(740, 740, 3000);
  changeTone(392, 932, 6270);
  changeTone(554, 831, 5000);
  changeTone(246, 370, 7320);
  freeRun(370);
  changeTone(370, 247, 10150);
  speedDown();
  changeTone(831, 523, 6840);
  changeTone(880, 294, 8590);

}

void keikyu_new1000(){
  speedUp();
  changeTone(311, 311, 200);
  changeTone(349, 349, 220);
  changeTone(392, 392, 220);
  changeTone(440, 440, 220);
  changeTone(466, 466, 220);
  changeTone(523, 523, 220);
  changeTone(587, 587, 220);
  changeTone(622, 622, 220);
  changeTone(698, 698, 220);
  changeTone(784, 784, 4700);
  changeTone(784, 988, 550);
  changeTone(784, 932, 890);
  changeTone(740, 880, 1200);
  //changeTone(622, 740, 1750);
  //changeTone(494, 554, 2100);
  changeTone(247, 466, 10500);

  freeRun(466);
  speedDown();
  changeTone(1109, 831, 5550);
  changeTone(1047, 698, 4670);
  changeTone(880, 740, 1150);
  changeTone(932, 784, 770);
  changeTone(784, 784, 3840);

}

void vvvf209(){
  speedUp();
  changeTone(233, 233, 1250);
  
  changeTone(293, 932, 1500);
  changeTone(554, 1245, 2100);
  changeTone(740, 1175, 2200);
  changeTone(370, 587, 3400);
  changeTone(220, 294, 2700);
  changeTone(232, 492, 14000);

  freeRun(492);
  changeTone(246, 138, 9600);
  speedDown();
  changeTone(466, 415, 2500);
  changeTone(740, 523, 4700);
  changeTone(987, 659, 3200);
  changeTone(1244, 784, 2500);
  changeTone(1244, 440, 2500);

}
void changeTone(int f1, int f2, unsigned long t){
  long i, n, r, f;
  n = t / 20;
  for(i = 0; i < n; i++){
    f = f1 + (f2 - f1) * i / n;
    //Serial.println(f);
    
    mytone((int)f, 20000, d);
    Serial.println(d);
  }
}


void mytone(int f, unsigned long t, int duty){
  unsigned long i,n;
  unsigned long th, tl;
  unsigned long tlon, tloff, thon, thoff;
  th = 10000UL * duty / f;
  tl = 10000UL * (100 - duty) / f;
  
  n = (unsigned long)(t / (tl + th));
  // PB0 = D8, PB1 = D9
  // Subtract 4uS because executing digitalWrite() takes approximately 4uS
  thon = (unsigned int)(th * PLS_RATIO / 100 - 4);
  thoff = (unsigned int)(th * (100 - PLS_RATIO) / 100 - 4);
  tlon = (unsigned int)(tl * PLS_RATIO / 100 - 4);
  tloff = (unsigned int)(tl * (100 - PLS_RATIO) / 100 - 4);
  
  for(i = 0; i < n; i++){
    digitalWrite(AIN1, HIGH);
    delayMicroseconds(thon);
    digitalWrite(AIN1, LOW);
    delayMicroseconds(thoff);
    digitalWrite(AIN2, HIGH);
    delayMicroseconds(tlon);
    digitalWrite(AIN2, LOW);
    delayMicroseconds(tloff);
    digitalWrite(AIN3, HIGH);
    delayMicroseconds(thon);
    digitalWrite(AIN3, LOW);
    delayMicroseconds(thoff);
    digitalWrite(AIN4, HIGH);
    delayMicroseconds(tlon);
    digitalWrite(AIN4, LOW);
    delayMicroseconds(tloff);
    digitalWrite(AIN5, HIGH);
    delayMicroseconds(thon);
    digitalWrite(AIN5, LOW);
    delayMicroseconds(thoff);
    digitalWrite(AIN6, HIGH);
    delayMicroseconds(tlon);
    digitalWrite(AIN6, LOW);
    delayMicroseconds(tloff);
  }
}

void freeRun(int f){/*
  unsigned long th, tl;
  th = 800000UL / f;
  tl = 200000UL / f;*/
}
