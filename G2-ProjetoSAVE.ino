// S.A.V.E- Sistemas Autonomo de Vagas em Estacionamento
#include <Servo.h>



// ===== PINOS =====

// Motores

#define dir1PinL  2

#define dir2PinL  4

#define speedPinL 6    // PWM

#define dir1PinR  7

#define dir2PinR  8

#define speedPinR 5    // PWM



// Sensores IR (5 sensores)

#define LFSensor_0  3   // Mais à esquerda

#define LFSensor_1  A1

#define LFSensor_2  A2  // Central

#define LFSensor_3  A3

#define LFSensor_4  10  // Mais à direita



// Velocidades

#define LF_SPEED  150  // Velocidade normal

#define TURN_SPEED 150 // Velocidade de curva (reduzida para mais precisão)

#define TURN_90_DEGREES_TIME 500 // Tempo estimado para girar 90 graus



// Estados

enum State { FOLLOWING_LINE, TURNING_90_LEFT, TURNING_90_RIGHT, SEARCHING_LINE };

State currentState = FOLLOWING_LINE;

unsigned long turnStartTime;



// Variáveis

int sensor[5];  // LOW = linha preta, HIGH = fundo branco



// ===== FUNÇÕES DOS MOTORES =====

void go_Advance() {

  digitalWrite(dir1PinL, HIGH);

  digitalWrite(dir2PinL, LOW);

  digitalWrite(dir1PinR, HIGH);

  digitalWrite(dir2PinR, LOW);

  analogWrite(speedPinL, LF_SPEED);

  analogWrite(speedPinR, LF_SPEED);

}



void turn_Left() {  // Roda direita ativa (esquerda desliga)

  digitalWrite(dir1PinL, LOW);

  digitalWrite(dir2PinL, LOW);

  digitalWrite(dir1PinR, HIGH);

  digitalWrite(dir2PinR, LOW);

  analogWrite(speedPinL, 0);

  analogWrite(speedPinR, TURN_SPEED);

}



void turn_Right() {  // Roda esquerda ativa (direita desliga)

  digitalWrite(dir1PinL, HIGH);

  digitalWrite(dir2PinL, LOW);

  digitalWrite(dir1PinR, LOW);

  digitalWrite(dir2PinR, LOW);

  analogWrite(speedPinL, TURN_SPEED);

  analogWrite(speedPinR, 0);

}



void stop_Stop() {

  digitalWrite(dir1PinL, LOW);

  digitalWrite(dir2PinL, LOW);

  digitalWrite(dir1PinR, LOW);

  digitalWrite(dir2PinR, LOW);

}



void rotate_Left_90() {

  digitalWrite(dir1PinL, LOW);

  digitalWrite(dir2PinL, HIGH);

  digitalWrite(dir1PinR, HIGH);

  digitalWrite(dir2PinR, LOW);

  analogWrite(speedPinL, TURN_SPEED);

  analogWrite(speedPinR, TURN_SPEED);

}



void rotate_Right_90() {

  digitalWrite(dir1PinL, HIGH);

  digitalWrite(dir2PinL, LOW);

  digitalWrite(dir1PinR, LOW);

  digitalWrite(dir2PinR, HIGH);

  analogWrite(speedPinL, TURN_SPEED);

  analogWrite(speedPinR, TURN_SPEED);

}



void search_Line() {

  digitalWrite(dir1PinL, HIGH);

  digitalWrite(dir2PinL, LOW);

  digitalWrite(dir1PinR, LOW);

  digitalWrite(dir2PinR, HIGH);

  analogWrite(speedPinL, TURN_SPEED);

  analogWrite(speedPinR, TURN_SPEED);

}



// ===== LEITURA DOS SENSORES =====

void read_Sensors() {

  sensor[0] = digitalRead(LFSensor_0);

  sensor[1] = digitalRead(LFSensor_1);

  sensor[2] = digitalRead(LFSensor_2);

  sensor[3] = digitalRead(LFSensor_3);

  sensor[4] = digitalRead(LFSensor_4);

  

  // Debug: imprimir valores dos sensores

  Serial.print("Sensores: ");

  for(int i=0; i<5; i++) {

    Serial.print(sensor[i]);

    Serial.print(" ");

  }

  Serial.println();

}



bool detect_90_Left_Turn() {

  return (sensor[0] == LOW && sensor[1] == LOW && sensor[3] == HIGH && sensor[4] == HIGH);

}



bool detect_90_Right_Turn() {

  return (sensor[3] == LOW && sensor[4] == LOW && sensor[0] == HIGH && sensor[1] == HIGH);

}



// ===== LÓGICA PRINCIPAL =====

void auto_Tracking() {

  read_Sensors();



  switch(currentState) {

    case FOLLOWING_LINE:

      if (detect_90_Left_Turn()) {

        currentState = TURNING_90_LEFT;

        turnStartTime = millis();

        rotate_Left_90();

      } 

      else if (detect_90_Right_Turn()) {

        currentState = TURNING_90_RIGHT;

        turnStartTime = millis();

        rotate_Right_90();

      }

      else if (sensor[2] == LOW) {

        if (sensor[1] == HIGH && sensor[3] == HIGH) {

          go_Advance();

        }

        else if (sensor[0] == LOW || sensor[1] == LOW) {

          turn_Left();

        }

        else if (sensor[3] == LOW || sensor[4] == LOW) {

          turn_Right();

        }

      } else {

        currentState = SEARCHING_LINE;

        search_Line();

      }

      break;

      

    case TURNING_90_LEFT:

      if (millis() - turnStartTime >= TURN_90_DEGREES_TIME) {

        currentState = FOLLOWING_LINE;

        stop_Stop();

      }

      break;

      

    case TURNING_90_RIGHT:

      if (millis() - turnStartTime >= TURN_90_DEGREES_TIME) {

        currentState = FOLLOWING_LINE;

        stop_Stop();

      }

      break;

      

    case SEARCHING_LINE:

      if (sensor[2] == LOW) {  // Se encontrou a linha central

        currentState = FOLLOWING_LINE;

        stop_Stop();

      }

      break;

  }

}



// ===== SETUP E LOOP =====

void setup() {

  // Configura motores

  pinMode(dir1PinL, OUTPUT);

  pinMode(dir2PinL, OUTPUT);

  pinMode(speedPinL, OUTPUT);

  pinMode(dir1PinR, OUTPUT);

  pinMode(dir2PinR, OUTPUT);

  pinMode(speedPinR, OUTPUT);



  // Configura sensores

  pinMode(LFSensor_0, INPUT);

  pinMode(LFSensor_1, INPUT);

  pinMode(LFSensor_2, INPUT);

  pinMode(LFSensor_3, INPUT);

  pinMode(LFSensor_4, INPUT);



  Serial.begin(9600);  // Inicializa comunicação serial

  stop_Stop();

}



void loop() {

  auto_Tracking();

  delay(10);  // Pequeno delay para evitar oscilações

}


