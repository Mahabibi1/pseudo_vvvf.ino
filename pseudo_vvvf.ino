#define PLS_RATIO 85
#define AIN1 9
#define AIN2 8
#define AIN3 7
#define AIN4 6
#define AIN5 5
#define AIN6 4
#define VR A0

volatile int V_START = 56, V_END = 80, V_OFF = 50, T = 180;
volatile unsigned long cnt = 0;
volatile int d = 0;
volatile boolean up = true;

unsigned long lastUpdate = 0;
const unsigned long interval = 100; // 100 ms para simular o MsTimer2

void setup() {
  int r;
  int pattern = 5;

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN3, OUTPUT);
  pinMode(AIN4, OUTPUT);
  pinMode(AIN5, OUTPUT);
  pinMode(AIN6, OUTPUT);

  Serial.begin(9600);

  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  digitalWrite(AIN3, HIGH);
  digitalWrite(AIN4, LOW);
  digitalWrite(AIN5, LOW);
  digitalWrite(AIN6, LOW);

  delay(1000); // Estabilização do sistema

  r = analogRead(VR) * pattern / 1024;
  Serial.print("Modo selecionado: ");
  Serial.println(r);

  switch (r) {
    case 0: vvvf209(); break;
    case 1: keikyu_new1000(); break;
    case 2: vvvf373(); break;
    case 3: e231_1000(); break;
    case 4: seibu6000(); break;
    default: Serial.println("Modo inválido."); break;
  }
}

void loop() {
  if (millis() - lastUpdate >= interval) {
    lastUpdate = millis();
    t2isr(); // Simula a interrupção
  }
}

// Simula interrupção periódica
void t2isr() {
  if (up) {
    d = V_START + (V_END - V_START) * cnt / T;
    cnt++;
    if (cnt > T) {
      cnt = T;
    }
  } else if (cnt > 0) {
    d = V_OFF + (V_END - V_OFF) * cnt / T;
    cnt--;
  }
}

// Placeholder das funções dos modos
void vvvf209() {
  Serial.println("Iniciando modo VVVF 209...");
  changeTone(300, 800, 5000);
}

void keikyu_new1000() {
  Serial.println("Iniciando modo Keikyu New 1000...");
  changeTone(400, 1200, 5000);
}

void vvvf373() {
  Serial.println("Iniciando modo VVVF 373...");
  changeTone(350, 950, 5000);
}

void e231_1000() {
  Serial.println("Iniciando modo E231-1000...");
  changeTone(200, 600, 5000);
}

void seibu6000() {
  Serial.println("Iniciando modo Seibu 6000...");
  changeTone(250, 700, 5000);
}

// Função para gerar tons com PWM
void changeTone(int f1, int f2, unsigned long t) {
  long i, n, f;
  n = t / 20;
  for (i = 0; i < n; i++) {
    f = f1 + (f2 - f1) * i / n;
    mytone((int)f, 20000, d);
    Serial.print("Frequência: ");
    Serial.print(f);
    Serial.print(" | Duty: ");
    Serial.println(d);
  }
}

void mytone(int f, unsigned long t, int duty) {
  unsigned long i, n;
  unsigned long th, tl;
  unsigned long tlon, tloff, thon, thoff;

  th = 10000UL * duty / f;
  tl = 10000UL * (100 - duty) / f;

  n = (unsigned long)(t / (tl + th));

  thon = (unsigned int)(th * PLS_RATIO / 100 - 4);
  thoff = (unsigned int)(th * (100 - PLS_RATIO) / 100 - 4);
  tlon = (unsigned int)(tl * PLS_RATIO / 100 - 4);
  tloff = (unsigned int)(tl * (100 - PLS_RATIO) / 100 - 4);

  for (i = 0; i < n; i++) {
    digitalWrite(AIN1, HIGH); delayMicroseconds(thon); digitalWrite(AIN1, LOW); delayMicroseconds(thoff);
    digitalWrite(AIN2, HIGH); delayMicroseconds(tlon); digitalWrite(AIN2, LOW); delayMicroseconds(tloff);
    digitalWrite(AIN3, HIGH); delayMicroseconds(thon); digitalWrite(AIN3, LOW); delayMicroseconds(thoff);
    digitalWrite(AIN4, HIGH); delayMicroseconds(tlon); digitalWrite(AIN4, LOW); delayMicroseconds(tloff);
    digitalWrite(AIN5, HIGH); delayMicroseconds(thon); digitalWrite(AIN5, LOW); delayMicroseconds(thoff);
    digitalWrite(AIN6, HIGH); delayMicroseconds(tlon); digitalWrite(AIN6, LOW); delayMicroseconds(tloff);
  }
}

// Funções auxiliares (se quiser usá-las no futuro)
void speedUp() {
  cnt = 0;
  up = true;
  d = 50;
}
void speedDown() {
  cnt = T;
  up = false;
}

void freeRun(int f) {
  // implementação opcional para modo contínuo
}
