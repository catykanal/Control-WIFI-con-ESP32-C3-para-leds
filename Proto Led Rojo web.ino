#include <WiFi.h>
#include <WebServer.h>

// Configuración del punto de acceso
const char* ssid = "apple lamp";
const char* password = "12345678";

// Pin del LED
const int ledPin = 10;

// Crear instancia del servidor web en el puerto 80
WebServer server(80);

// Página HTML
const char* htmlPage = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { 
      font-family: Arial, sans-serif; 
      text-align: center; 
      margin: 0 auto; 
      padding-top: 50px;
    }
    .button {
      display: inline-block;
      padding: 10px 20px;
      font-size: 20px;
      cursor: pointer;
      text-align: center;
      text-decoration: none;
      outline: none;
      color: #fff;
      background-color: #4CAF50;
      border: none;
      border-radius: 15px;
      box-shadow: 0 9px #999;
      margin: 10px;
    }
    .button:hover {background-color: #3e8e41}
    .button:active {
      background-color: #3e8e41;
      box-shadow: 0 5px #666;
      transform: translateY(4px);
    }
    .off {
      background-color: #f44336;
    }
    .off:hover {background-color: #d32f2f}
  </style>
</head>
<body>
  <h1>Control de Lámpara</h1>
  <a href="/on"><button class="button">ENCENDER LÁMPARA</button></a>
  <a href="/off"><button class="button off">APAGAR LÁMPARA</button></a>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Configurar el ESP32 como punto de acceso
  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  // Configurar las rutas del servidor web
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  
  // Iniciar el servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleOn() {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED encendido");
  server.send(200, "text/html", htmlPage);
}

void handleOff() {
  digitalWrite(ledPin, LOW);
  Serial.println("LED apagado");
  server.send(200, "text/html", htmlPage);
}
