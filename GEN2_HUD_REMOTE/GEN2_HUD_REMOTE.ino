
// For ESP32
// #include <WiFi.h>
// #include <WebServer.h>

// For ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Use regardless of controller
#include <WiFiClient.h>
#include "index.h"

// Define Wi-Fi network SSID and password
const char* ssid = "ARMM_WiFi";
const char* password = "ARMM_WiFi2025!";

// Create strings for instructions
String turnInstruction = "null"; // LEFT, RIGHT, STRAIGHT, TURN NOW, or END TEST (from the server)
String timeInstruction = "null"; // time to next turn in seconds (from the server)
String distInstruction = "null"; // distance to next turn in ft (from the server)
String testStatus = "null"; // "running" or "ended" or "arrived" idk
String direction = "null"; // string to send to serial for testing

// Create a webserver object (ESP32)
// WebServer server(80);// REPLACE WITH WebServer server(80); FOR ESP 32

// Create a webserver object (ESP8266)
ESP8266WebServer server(80);// REPLACE WITH WebServer server(80); FOR ESP 32

// Set up the html webpage
void handleRoot() {
 String s = webpage;
 server.send(200, "text/html", s);
}

// Load next direction when commanded
void recieveInstruction() {

  if(server.arg("turn") == "END TEST") {
    turnInstruction = "You will arrive";
    distInstruction = server.arg("dist");
    timeInstruction = server.arg("time");
  } else if (server.arg("turn") == "TURN NOW") {
    distInstruction = "0";
    timeInstruction = "0";
  } else {
    turnInstruction = server.arg("turn");
    distInstruction = server.arg("dist");
    timeInstruction = server.arg("time");
  }

  // Parse data and place display updates here!

  direction = turnInstruction + " in " + distInstruction + " ft or " + timeInstruction + " sec.";
  Serial.println(direction);
  server.send(200, "text/plane", direction);

}

// Runs once
void setup(void) {
  Serial.begin(115200);

  // Create & broadcast a WiFi network using the SSID and Password
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();

  /*// zzzLT Debugging
  Serial.println("WiFi Ready.");
  Serial.print("IP address: ");
  Serial.println(IP);
  */
  
  // Run functions when the client makes specific URL/index requests 
  server.on("/", handleRoot);
  server.on("/instruction", recieveInstruction);
  server.begin();
}

// Run the webserver, handle everything!
void loop(void)
{
  server.handleClient();
}