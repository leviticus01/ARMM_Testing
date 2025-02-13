#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "index.h"

// Define input pins
#define LED1 0
#define LED2 2

// Define output pins
#define BUTTON 16

// Define Wi-Fi network SSID and password
const char* ssid = "ESP8266_AP_Test";
const char* password = "0123456789";

// Create a webserver object
ESP8266WebServer server(80);

// Set up the html webpage
void handleRoot() {
 String s = webpage;
 server.send(200, "text/html", s);
}

// Update nav data
// Nav States: 0 = clear, 1 = left, 2 = right, 3 = straight
void updateNav() {
  String commandedNavState = server.arg("state");
  String displayNavState = commandedNavState;

  // Convert numeric nav state to a readable string
  if(commandedNavState == "1") {
    displayNavState = "TURN LEFT";
  } else if(commandedNavState == "2") {
    displayNavState = "TURN RIGHT";
  } else if(commandedNavState == "3") {
    displayNavState = "GO STRAIGHT";
  } else {
    displayNavState = "OFF";
  }

  // UPDATE LCD WITH A FUNCTION PLACED HERE
  Serial.print("Nav Changed to: ");
  Serial.println(displayNavState);
  server.send(200, "text/plane", displayNavState);
}

// Update blindspot data
// Blindspot States: 0 = clear, 1 = left, 2 = right
void updateBlindSpot() {
  String commandedBlindSpotState = server.arg("state");
  String displayBlindSpotState = commandedBlindSpotState;

  // Convert numeric blindspot state to a readable string
  if(commandedBlindSpotState == "1") {
    displayBlindSpotState = "ALERT LEFT!";
  } else if(commandedBlindSpotState == "2") {
    displayBlindSpotState = "ALERT RIGHT!";
  } else {
    displayBlindSpotState = "OFF";
  }

  // UPDATE LCD WITH A FUNCTION PLACED HERE
  Serial.print("Blind Spot Changed to: ");
  Serial.println(displayBlindSpotState);
  server.send(200, "text/plane", displayBlindSpotState);
}

// Update speed data
// Speed Data: 0 to 20 ("mph")
void updateSpeed() {
  String commandedSpeed = server.arg("state");
  String displaySpeed = commandedSpeed + " mph";
  // UPDATE LCD WITH A FUNCTION PLACED HERE
  Serial.print("Speed changed to: ");
  Serial.println(displaySpeed);
  server.send(200, "text/plane", displaySpeed);
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
  server.on("/set_nav", updateNav);
  server.on("/set_blindspot", updateBlindSpot);
  server.on("/set_speed",updateSpeed);
  server.begin();
}

// Run the webserver, handle everything!
void loop(void)
{
  server.handleClient();
}