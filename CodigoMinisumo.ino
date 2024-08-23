// Pines de los sensores TCRT5000
#define TCRT_LEFT_PIN A0
#define TCRT_RIGHT_PIN A2
#define TCRT_BACK_PIN A1

// Pines del sensor ultrasónico HC-SR04
#define TRIG_PIN 10
#define ECHO_PIN 11

// Pines de los motores
#define MOTOR_LEFT_FORWARD 3
#define MOTOR_LEFT_BACKWARD 4
#define MOTOR_RIGHT_FORWARD 5
#define MOTOR_RIGHT_BACKWARD 6

// Variables para almacenar los valores de los sensores
int tcrtLeft;
int tcrtRight;
int tcrtBack;
long duration;
int distance;

void setup() {
  pinMode(TCRT_LEFT_PIN, INPUT);
  pinMode(TCRT_RIGHT_PIN, INPUT);
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
  tcrtRight = analogRead(TCRT_RIGHT_PIN);
  tcrtBack = analogRead(TCRT_BACK_PIN);

  // Leer el sensor ultrasónico
  distance = readUltrasonicDistance();

  
  // Control del movimiento según los sensores TCRT5000
  // if (isOnWhite(tcrtLeft) || isOnWhite(tcrtRight)) {
  if (isOnWhite(tcrtLeft)) {
    // Si cualquiera de los sensores detecta blanco, retroceder
    moveBackward();
  } else if (isOnWhite(tcrtBack)){
    // Si el sensor detecta blanco en la parte tracera acelera
    moveForward();
  } else {
    // Si ambos sensores detectan negro, proceder a evaluar el sensor ultrasónico
    if (distance <= 50) { // Aumentar el umbral de 20 cm a 50 cm
      // Si detecta un objeto a menos de 50 cm, avanzar hacia él
      moveForward();
    } else {
      // Si no detecta ningún objeto, buscar girando
      searchForObject();
    }
  }

  delay(100); // Retardo para estabilidad
}

void mostrarTcrt(){
  // Serial.print("Left: ");
  // Serial.println(tcrtLeft );
  // Serial.print("Right: ");
  // Serial.println(tcrtRight );
  Serial.print("Back: ");
  Serial.print(tcrtBack );
  Serial.print(" ");
  Serial.println(isOnWhite(tcrtBack));

}


bool isOnWhite(int sensorValue) {
  // Asumiendo que el valor leído es bajo para blanco y alto para negro
  return sensorValue < 500; // Ajustar según sea necesario
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
  return distance;
}

void moveForward() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void moveBackward() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
}

void searchForObject() {
  // Avanzar una distancia fija
  moveForward();
  delay(1000); // Ajustar el tiempo según la distancia
  
  // Girar para buscar
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
  delay(500); // Ajustar el tiempo según el giro necesario
  
  // Detener para reevaluar
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}