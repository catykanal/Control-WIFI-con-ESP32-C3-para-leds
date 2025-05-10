#include <WiFi.h>
#include <WebServer.h>

// Definición de pines
#define LED_BLANCO_1 0     // LED luz blanca 1
#define LED_BLANCO_2 10    // LED luz blanca 2
#define LED_CALIDO_1 20    // LED luz cálida 1
#define LED_CALIDO_2 21    // LED luz cálida 2

// Configuración del punto de acceso
const char* ssid = "apple lamp";
const char* password = "12345678";

WebServer server(80);  // Crear servidor en el puerto 80

// Variables de estado
String colorActual = "apagado";  // Puede ser: "blanco", "calido", "apagado"

void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Configurar pines de los LEDs como salida
  pinMode(LED_BLANCO_1, OUTPUT);
  pinMode(LED_BLANCO_2, OUTPUT);
  pinMode(LED_CALIDO_1, OUTPUT);
  pinMode(LED_CALIDO_2, OUTPUT);
  
  // Inicializar todos los LEDs apagados
  apagarTodosLosLEDs();
  
  // Crear punto de acceso
  Serial.println("Configurando punto de acceso...");
  WiFi.softAP(ssid, password);
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Dirección IP del AP: ");
  Serial.println(myIP);
  
  // Configurar rutas del servidor web
  server.on("/", handleRoot);
  server.on("/blanco", handleBlanco);
  server.on("/calido", handleCalido);
  server.on("/apagar", handleApagar);
  
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}

void apagarTodosLosLEDs() {
  digitalWrite(LED_BLANCO_1, LOW);
  digitalWrite(LED_BLANCO_2, LOW);
  digitalWrite(LED_CALIDO_1, LOW);
  digitalWrite(LED_CALIDO_2, LOW);
  colorActual = "apagado";
}

void handleRoot() {
  String html = "<!DOCTYPE html>";
  html += "<html lang='es'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Control de Lámpara</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }";
  html += "h1 { color: #333; }";
  html += ".btn { border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 10px 5px; cursor: pointer; border-radius: 5px; }";
  html += ".btn-blanco { background-color: #f5f5f5; color: black; }";
  html += ".btn-calido { background-color: #ffcc80; }";
  html += ".btn-apagar { background-color: #f44336; }";
  html += ".estado { font-size: 20px; margin: 20px; }";
  html += ".container { max-width: 500px; margin: 0 auto; }";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class='container'>";
  html += "<h1>Control de Lámpara</h1>";
  
  // Mostrar estado actual
  html += "<p class='estado'>Estado actual: <strong>";
  if(colorActual == "blanco") {
    html += "<span style='color:#f5f5f5; text-shadow: 1px 1px 1px #000;'>LUZ BLANCA</span>";
  } else if(colorActual == "calido") {
    html += "<span style='color:#ffcc80; text-shadow: 1px 1px 1px #000;'>LUZ CÁLIDA</span>";
  } else {
    html += "<span style='color:#666;'>APAGADA</span>";
  }
  html += "</strong></p>";
  
  // Botones de control
  html += "<a href='/blanco' class='btn btn-blanco'>Luz Blanca</a>";
  html += "<a href='/calido' class='btn btn-calido'>Luz Cálida</a>";
  html += "<a href='/apagar' class='btn btn-apagar'>Apagar Lámpara</a>";
  html += "</div>";
  html += "</body>";
  html += "</html>";
  
  server.send(200, "text/html", html);
}

void handleBlanco() {
  apagarTodosLosLEDs();
  digitalWrite(LED_BLANCO_1, HIGH);
  digitalWrite(LED_BLANCO_2, HIGH);
  colorActual = "blanco";
  Serial.println("Encendido luz blanca");
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleCalido() {
  apagarTodosLosLEDs();
  digitalWrite(LED_CALIDO_1, HIGH);
  digitalWrite(LED_CALIDO_2, HIGH);
  colorActual = "calido";
  Serial.println("Encendido luz cálida");
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleApagar() {
  apagarTodosLosLEDs();
  Serial.println("Todos los LEDs apagados");
  server.sendHeader("Location", "/");
  server.send(303);
}
