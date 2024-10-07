#include <WiFi.h>
#include <WebServer.h>
#include "esp_wifi.h"

// Configuración de red WiFi
const char* ssid = "Tu_SSID";
const char* password = "Tu_Contraseña";

// Definición de pines para el puente H izquierdo
#define ENA_PIN_izq 17        
#define IN1_PIN_izq 15         
#define IN2_PIN_izq 2        
#define IN3_PIN_izq 0         
#define IN4_PIN_izq 4        
#define ENB_PIN_izq 16   

// Definición de pines para el puente H derecho
#define ENA_PIN_der 23         
#define IN1_PIN_der 22         
#define IN2_PIN_der 21        
#define IN3_PIN_der 19         
#define IN4_PIN_der 18        
#define ENB_PIN_der 5

WebServer server(80); // Servidor web en el puerto 80

void setup() {
  Serial.begin(115200);

  // Inicializar WiFi
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configuración de pines como salida
  pinMode(ENA_PIN_izq, OUTPUT);
  pinMode(IN1_PIN_izq, OUTPUT);
  pinMode(IN2_PIN_izq, OUTPUT);
  pinMode(ENB_PIN_izq, OUTPUT);
  pinMode(IN3_PIN_izq, OUTPUT);
  pinMode(IN4_PIN_izq, OUTPUT);
  pinMode(ENA_PIN_der, OUTPUT);
  pinMode(IN1_PIN_der, OUTPUT);
  pinMode(IN2_PIN_der, OUTPUT);
  pinMode(ENB_PIN_der, OUTPUT);
  pinMode(IN3_PIN_der, OUTPUT);
  pinMode(IN4_PIN_der, OUTPUT);

  // Configurar rutas del servidor web
  server.on("/", handleRoot);
  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/rotateLeft", handleRotateLeft);
  server.on("/rotateRight", handleRotateRight);
  server.on("/stop", handleStop);

  server.begin();
  Serial.println("Servidor web iniciado");
}

void loop() {
  server.handleClient(); // Manejar solicitudes del cliente web
}

// Funciones de manejo de rutas
void handleRoot() {
  String html = "<html><head><title>Control del Vehiculo</title><style>";
  html += "body { font-family: Arial, sans-serif; background-color: #f0f8ff; color: #333; text-align: center; padding: 20px; }";
  html += "h1 { color: #00509e; margin-bottom: 30px; }";
  html += ".button { font-size: 20px; width: 160px; height: 60px; margin: 15px; border-radius: 10px; background-color: #007bff; color: white; border: none; cursor: pointer; transition: background-color 0.3s ease; }";
  html += ".button:hover { background-color: #0056b3; }";
  html += ".button:active { background-color: #003f7f; transform: scale(0.98); }"; // Color más oscuro y efecto de presión
  html += "</style></head><body>";
  html += "<h1>Control del Vehículo</h1>";
  html += "<button class=\"button\" onclick=\"fetch('/forward')\">Adelante</button><br>";
  html += "<button class=\"button\" onclick=\"fetch('/left')\">Izquierda</button>";
  html += "<button class=\"button\" onclick=\"fetch('/stop')\">Detener</button>";
  html += "<button class=\"button\" onclick=\"fetch('/right')\">Derecha</button><br>";
  html += "<button class=\"button\" onclick=\"fetch('/backward')\">Atras</button><br><br>";
  html += "<button class=\"button\" onclick=\"fetch('/rotateLeft')\">Girar Izquierda</button>";
  html += "<button class=\"button\" onclick=\"fetch('/rotateRight')\">Girar Derecha</button>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}


void handleForward() {
  motorAdelante(255);
  server.send(200, "text/plain", "Adelante");
}

void handleBackward() {
  motorAtras(255);
  server.send(200, "text/plain", "Atrás");
}

void handleLeft() {
  motorIzquierda(255);
  server.send(200, "text/plain", "Izquierda");
}

void handleRight() {
  motorDerecha(255);
  server.send(200, "text/plain", "Derecha");
}

void handleRotateLeft() {
  motorGirarIzquierda(255);
  server.send(200, "text/plain", "Girar Izquierda");
}

void handleRotateRight() {
  motorGirarDerecha(255);
  server.send(200, "text/plain", "Girar Derecha");
}

void handleStop() {
  motorDetener();
  server.send(200, "text/plain", "Detenido");
}

// Funciones de control del motor (mismas que antes)
void motorAdelante(int speed) {
  // Motores izquierdo y derecho hacia adelante
  digitalWrite(IN1_PIN_izq, HIGH);
  digitalWrite(IN2_PIN_izq, LOW);
  analogWrite(ENA_PIN_izq, speed);

  digitalWrite(IN3_PIN_izq, LOW);
  digitalWrite(IN4_PIN_izq, HIGH);
  analogWrite(ENB_PIN_izq, speed);

  digitalWrite(IN1_PIN_der, HIGH);
  digitalWrite(IN2_PIN_der, LOW);
  analogWrite(ENA_PIN_der, speed);

  digitalWrite(IN3_PIN_der, LOW);
  digitalWrite(IN4_PIN_der, HIGH);
  analogWrite(ENB_PIN_der, speed);
}

void motorAtras(int speed) {
  // Motores izquierdo y derecho hacia atrás
  digitalWrite(IN1_PIN_izq, LOW);
  digitalWrite(IN2_PIN_izq, HIGH);
  analogWrite(ENA_PIN_izq, speed);

  digitalWrite(IN3_PIN_izq, HIGH);
  digitalWrite(IN4_PIN_izq, LOW);
  analogWrite(ENB_PIN_izq, speed);

  digitalWrite(IN1_PIN_der, LOW);
  digitalWrite(IN2_PIN_der, HIGH);
  analogWrite(ENA_PIN_der, speed);

  digitalWrite(IN3_PIN_der, HIGH);
  digitalWrite(IN4_PIN_der, LOW);
  analogWrite(ENB_PIN_der, speed);
}

  // Movimiento lateral hacia la izquierda
void motorIzquierda(int speed) {
    // Frente Izquierdo hacia Atras
  digitalWrite(IN1_PIN_izq, HIGH);  
  digitalWrite(IN2_PIN_izq, LOW); 
  analogWrite(ENA_PIN_izq, speed);

  // Trasero Izquierdo hacia Adelante
  digitalWrite(IN3_PIN_izq, HIGH);  
  digitalWrite(IN4_PIN_izq, LOW); 
  analogWrite(ENB_PIN_izq, speed);

  // Frente Derecho hacia Adelante
  digitalWrite(IN1_PIN_der, HIGH);  
  digitalWrite(IN2_PIN_der, LOW);
  analogWrite(ENA_PIN_der, speed);

  // Trasero Derecho hacia Atras
  digitalWrite(IN3_PIN_der, HIGH);  
  digitalWrite(IN4_PIN_der, LOW);
  analogWrite(ENB_PIN_der, speed);
}

  // Movimiento lateral hacia la derecha
void motorDerecha(int speed) {
// Frente Izquierdo hacia Adelante
  digitalWrite(IN1_PIN_izq, LOW);  
  digitalWrite(IN2_PIN_izq, HIGH); 
  analogWrite(ENA_PIN_izq, speed);

  // Trasero Izquierdo hacia Atras
  digitalWrite(IN3_PIN_izq, LOW);  
  digitalWrite(IN4_PIN_izq, HIGH); 
  analogWrite(ENB_PIN_izq, speed);

  // Frente Derecho hacia Atras
  digitalWrite(IN1_PIN_der, LOW);  
  digitalWrite(IN2_PIN_der, HIGH);
  analogWrite(ENA_PIN_der, speed);

  // Trasero Derecho hacia Adelante
  digitalWrite(IN3_PIN_der, LOW);  
  digitalWrite(IN4_PIN_der, HIGH);
  analogWrite(ENB_PIN_der, speed);
  
}

void motorGirarIzquierda(int speed) {
  // Giro sobre el eje hacia la izquierda
  digitalWrite(IN1_PIN_izq, LOW);
  digitalWrite(IN2_PIN_izq, HIGH);
  analogWrite(ENA_PIN_izq, speed);

  digitalWrite(IN3_PIN_izq, HIGH);
  digitalWrite(IN4_PIN_izq, LOW);
  analogWrite(ENB_PIN_izq, speed);

  digitalWrite(IN1_PIN_der, HIGH);
  digitalWrite(IN2_PIN_der, LOW);
  analogWrite(ENA_PIN_der, speed);

  digitalWrite(IN3_PIN_der, LOW);
  digitalWrite(IN4_PIN_der, HIGH);
  analogWrite(ENB_PIN_der, speed);
}

void motorGirarDerecha(int speed) {
  // Giro sobre el eje hacia la derecha
  digitalWrite(IN1_PIN_izq, HIGH);
  digitalWrite(IN2_PIN_izq, LOW);
  analogWrite(ENA_PIN_izq, speed);

  digitalWrite(IN3_PIN_izq, LOW);
  digitalWrite(IN4_PIN_izq, HIGH);
  analogWrite(ENB_PIN_izq, speed);

  digitalWrite(IN1_PIN_der, LOW);
  digitalWrite(IN2_PIN_der, HIGH);
  analogWrite(ENA_PIN_der, speed);

  digitalWrite(IN3_PIN_der, HIGH);
  digitalWrite(IN4_PIN_der, LOW);
  analogWrite(ENB_PIN_der, speed);
}

void motorDetener() {
  // Detener todos los motores
  digitalWrite(IN1_PIN_izq, LOW);
  digitalWrite(IN2_PIN_izq, LOW);
  analogWrite(ENA_PIN_izq, 0);

  digitalWrite(IN3_PIN_izq, LOW);
  digitalWrite(IN4_PIN_izq, LOW);
  analogWrite(ENB_PIN_izq, 0);

  digitalWrite(IN1_PIN_der, LOW);
  digitalWrite(IN2_PIN_der, LOW);
  analogWrite(ENA_PIN_der, 0);

  digitalWrite(IN3_PIN_der, LOW);
  digitalWrite(IN4_PIN_der, LOW);
  analogWrite(ENB_PIN_der, 0);
}
