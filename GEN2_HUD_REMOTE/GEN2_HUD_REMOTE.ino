
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

// Create string for next turn
String turn = "null";

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
void nextDirection() {

  // Keep previous turn direction if override/turn now is pressed, just sets direction and time to zero
  if(server.arg("turn") == "NOW") {
  } else {
    turn = server.arg("turn");
  }

  String distance = server.arg("dist");
  String time = server.arg("time");
  
  // UPDATE LCD WITH A FUNCTION PLACED HERE
  String direction = "Turn " + turn + " in " + distance + " ft or " + time + " sec.";
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
  server.on("/nextDirection", nextDirection);
  server.begin();
}

// Run the webserver, handle everything!
void loop(void)
{
  server.handleClient();
}