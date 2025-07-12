#include <Servo.h>

/* ----------------- PIN SETUP ----------------- */
// Motor A
const int enA = 5;
const int in1 = 2;
const int in2 = 3;
// Motor B
const int enB = 6;
const int in3 = 4;
const int in4 = 7;

// Ultrasonik
const int trigPin = 8;
const int echoPin = 9;

// Servo penyerang
const int servoPin = 10;
const int posisiTurun = 120;
const int posisiNaik  = 180;

// Line follower (digital) – LOW saat warna hitam
const int sensorPin = A0;

/* ----------------- VARIABEL GLOBAL ----------------- */
Servo serang;
long duration;
int distance;

/* ----------------- SETUP ----------------- */
void setup() {
  // Motor pins
  pinMode(enA, OUTPUT); pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT); pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);

  // Ultrasonik
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Line follower
  pinMode(sensorPin, INPUT);

  // Servo
  serang.attach(servoPin);
  serang.write(posisiTurun);

  //gerakan
  stopMotor();
  delay(500);

  Serial.begin(9600);
}

/* ----------------- LOOP ----------------- */
void loop() {
  // Cek warna hitam (LOW = garis hitam terdeteksi)
  int sensorValue = digitalRead(sensorPin);

  if (sensorValue == HIGH) {
    Serial.println("Garis HITAM terdeteksi → mundur");
    stopMotor();
    delay(2000);
    mundur(190, 90);
    Serial.println("MUNDUR...");
    delay(500);              // ← delay langsung setelah mundur
    stopMotor();
    delay(2000);
    return;
  }


  // Baca sensor ultrasonik
  digitalWrite(trigPin, LOW);  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Jarak: "); Serial.print(distance); Serial.println(" cm");

  // Logika servo dan kecepatan
  if (distance > 0 && distance <= 8) {
    serang.write(posisiNaik);
    delay(1000);
    maju(255, 255); // Musuh dekat, maju cepat sambil serang
  }
  else if (distance > 8 && distance <= 13) {
    serang.write(posisiTurun);
    maju(255, 255); // Maju pelan karena masih agak jauh
  }
  else if (distance > 13 && distance <= 40) {
    serang.write(posisiTurun);
    maju(100, 70);
    delay(500);
    maju(200,200 ); // Maju pelan karena masih agak jauh
  }
  else {
    serang.write(posisiTurun);
    putarKanan(150, 0); // Cari musuh (putar di tempat)
  }

  delay(10);
}

/* ----------------- FUNGSI MOTOR ----------------- */
void maju(int speedA, int speedB) {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  analogWrite(enA, speedA); analogWrite(enB, speedB);
}

void mundur(int speedA, int speedB) {
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  analogWrite(enA, speedA); analogWrite(enB, speedB);
}

void putarKanan(int speedA, int speedB) {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);  digitalWrite(in4, HIGH);
  analogWrite(enA, speedA); analogWrite(enB, speedB);
}

void stopMotor() {
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
  analogWrite(enA, 0);    analogWrite(enB, 0);
}
