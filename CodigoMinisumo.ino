// Pines de los sensores TCRT5000
#define TCRT_LEFT_PIN A0
// #define TCRT_RIGHT_PIN 1
#define TCRT_BACK_PIN A1

// Pines del sensor ultrasónico HC-SR04
#define TRIG_PIN 10
#define ECHO_PIN 11

// Pines de los motores
#define MOTOR_LEFT_FORWARD 3
#define MOTOR_LEFT_BACKWARD 9
#define MOTOR_RIGHT_FORWARD 5
#define MOTOR_RIGHT_BACKWARD 6

// Variables para almacenar los valores de los sensores
int tcrtLeft;
// int tcrtRight;
int tcrtBack;
long duration;
int distance;

void setup() {
  pinMode(TCRT_LEFT_PIN, INPUT);
  // pinMode(TCRT_RIGHT_PIN, INPUT);
  pinMode(TCRT_BACK_PIN, INPUT);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  // Leer los sensores TCRT5000
  tcrtLeft = analogRead(TCRT_LEFT_PIN);
  // tcrtRight = digitalRead(TCRT_RIGHT_PIN);
  tcrtBack = analogRead(TCRT_BACK_PIN);

  // Leer el sensor ultrasónico
  distance = readUltrasonicDistance();

  // mostrarTcrt();

  
  // Control del movimiento según los sensores TCRT5000
  // if (isOnWhite(tcrtLeft) || isOnWhite(tcrtRight)) {
  if (isOnWhite(tcrtLeft)) {
    // Si cualquiera de los sensores detecta blanco, retroceder
    // Serial.println("Retroceder");
    moveBackward();
  } else if (isOnWhite(tcrtBack)){
    // Si el sensor detecta blanco en la parte tracera acelera
    // Serial.println("Avanzar");
    moveForward();
  } else {
    // Si ambos sensores detectan negro, proceder a evaluar el sensor ultrasónico
    if (distance <= 20) { // Aumentar el umbral de 20 cm a 50 cm
      // Si detecta un objeto a menos de 50 cm, avanzar hacia él
      moveForward();
      // Serial.println("Atacar");
    } else {
      // Si no detecta ningún objeto, buscar girando
      searchForObject();
      // Serial.println("Buscar");

    }
  }

  delay(300); // Retardo para estabilidad
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


bool isOnWhite(int sensorValue) {
  // Asumiendo que el valor leído es bajo para blanco y alto para negro
  return sensorValue > 500; // Ajustar según sea necesario
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
  analogWrite(MOTOR_LEFT_FORWARD, 255);
  analogWrite(MOTOR_RIGHT_FORWARD, 255);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void moveBackward() {
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, 255);
  analogWrite(MOTOR_RIGHT_BACKWARD, 255);
}

void searchForObject() {
  // Avanzar una distancia fija
  moveForward();
  delay(500); // Ajustar el tiempo según la distancia
  
  // Girar para buscar
  analogWrite(MOTOR_LEFT_FORWARD, 255);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 255);
  delay(250); // Ajustar el tiempo según el giro necesario
  
  // Detener para reevaluar
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}