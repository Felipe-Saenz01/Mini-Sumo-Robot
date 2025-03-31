// Pines de los sensores TCRT5000
// #define TCRT_LEFT_PIN A0
// #define TCRT_RIGHT_PIN 1
// #define TCRT_BACK_PIN A1

// Pines del sensor ultrasónico HC-SR04
#define TRIG_PIN 10
#define ECHO_PIN 11

// Pines de los motores
#define MOTOR_LEFT_FORWARD 5
#define MOTOR_LEFT_BACKWARD 6
#define MOTOR_RIGHT_FORWARD 3
#define MOTOR_RIGHT_BACKWARD 9

// Pines del DIP switch (configuración pull-up interno)
#define DIP_SWITCH_ONE_PIN 4   // Pin para controlar giro izquierdo
#define DIP_SWITCH_TWO_PIN 2  // Pin para controlar giro derecho

int velocidad = 150;

// Variables para almacenar los valores de los sensores
int tcrtLeft;
// int tcrtRight;
int tcrtBack;
long duration;
int distance;

void setup() {
  // pinMode(TCRT_LEFT_PIN, INPUT);
  // pinMode(TCRT_RIGHT_PIN, INPUT);
  // pinMode(TCRT_BACK_PIN, INPUT);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  // Configura pines del DIP switch como entradas con pull-up
  pinMode(DIP_SWITCH_ONE_PIN, INPUT_PULLUP);
  pinMode(DIP_SWITCH_TWO_PIN, INPUT_PULLUP);
  
  Serial.begin(9600);
  delay(4500); // Ajustar el tiempo según la distancia
}

void loop() {
  // Leer los sensores TCRT5000
  // tcrtLeft = analogRead(TCRT_LEFT_PIN);
  tcrtLeft = 40;
  // tcrtRight = digitalRead(TCRT_RIGHT_PIN);
  // tcrtBack = analogRead(TCRT_BACK_PIN);
  tcrtBack = 40;

  // Leer el sensor ultrasónico
  distance = readUltrasonicDistance();

  // mostrarTcrt();
  // showDipSwitch();  

  
  // Control del movimiento según los sensores TCRT5000
  if (isOnWhite(tcrtLeft)) {
    // Si cualquiera de los sensores detecta blanco, retroceder
    Serial.println("Retroceder");
    // moveBackward();
  } else if (isOnWhite(tcrtBack)){
    // Si el sensor detecta blanco en la parte tracera acelera
    Serial.println("Avanzar");
    // moveForward();
  } else {
    // Si ambos sensores detectan negro, proceder a evaluar el sensor ultrasónico
    if (distance <= 20) { // Aumentar el umbral de 20 cm a 50 cm
      // Si detecta un objeto a menos de 50 cm, avanzar hacia él
      moveForward();
      Serial.println("Atacar");
    } else {
      // Si no detecta ningún objeto, buscar girando
      searchForObject();
      Serial.println("Buscar");

    }
  }

  delay(200); // Retardo para estabilidad
}

void mostrarTcrt(){
  Serial.print("Back: ");
  Serial.print(tcrtBack );
  Serial.print(" - ");
  Serial.println(isOnWhite(tcrtBack));
  Serial.print("Left: ");
  Serial.print(tcrtLeft);
  Serial.print(" - ");
  Serial.println(isOnWhite(tcrtLeft));

}

void showDipSwitch(){
// Mostrar estado de los switches
  Serial.print("DIP Status - 1: ");
  Serial.print(isOneDipEnabled());
  Serial.print(" | 2: ");
  Serial.println(isTwoDipEnabled());
}

// Función para leer el estado del DIP switch (invertido por pull-up)
bool isOneDipEnabled() {
  return !digitalRead(DIP_SWITCH_ONE_PIN); // LOW cuando activado
}

bool isTwoDipEnabled() {
  return !digitalRead(DIP_SWITCH_TWO_PIN); // LOW cuando activado
}


bool isOnWhite(int sensorValue) {
  if(isTwoDipEnabled() ){
    // Asumiendo que el valor leído es bajo para blanco y alto para negro
    return sensorValue < 40; // Ajustar según sea necesario
  }else {
    // Asumiendo que el valor leído es bajo para blanco y alto para negro
    return sensorValue < 40; // Ajustar según sea necesario

  }
}

long readUltrasonicDistance() {
  // Enviar un pulso de 10 microsegundos para iniciar la medición
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Leer el tiempo de respuesta del eco
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Convertir el tiempo en distancia
  distance = (duration / 2) / 29.1; // Convertir a cm
  // Serial.println("---");
  // Serial.print(distance);
  // Serial.println(" cm");
  return distance;
}

void moveForward() {
  // analogWrite(MOTOR_LEFT_FORWARD, 255);
  // analogWrite(MOTOR_RIGHT_FORWARD, 255);
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, velocidad);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, velocidad);
}

void moveBackward() {
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  // analogWrite(MOTOR_LEFT_BACKWARD, 255);
  // analogWrite(MOTOR_RIGHT_BACKWARD, 255);
  analogWrite(MOTOR_LEFT_BACKWARD, 175);
  analogWrite(MOTOR_RIGHT_BACKWARD, 175);
  delay(250); // Ajustar el tiempo según la distancia

}

void searchForObject() {
  // Avanzar una distancia fija
  moveForward();
  delay(10); // Ajustar el tiempo según la distancia
  if (isOneDipEnabled()){
    // Girar para buscar
    analogWrite(MOTOR_LEFT_FORWARD, velocidad);
    analogWrite(MOTOR_RIGHT_BACKWARD, 0);
    analogWrite(MOTOR_RIGHT_FORWARD, velocidad);
    analogWrite(MOTOR_LEFT_BACKWARD, 0);
  } else {
    analogWrite(MOTOR_LEFT_FORWARD, 0);
    analogWrite(MOTOR_LEFT_BACKWARD, velocidad);
    analogWrite(MOTOR_RIGHT_FORWARD, 0);
    analogWrite(MOTOR_RIGHT_BACKWARD, velocidad);
  }
  delay(300); // Ajustar el tiempo según el giro necesario
  
  // Detener para reevaluar
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}