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

// Checks the button status
void checkButtonState() {

  // Configure text that will be displayed on the webpage for button state
  String button_displayState = "OFF!";
  if(digitalRead(BUTTON)) {
    button_displayState = "ON!";
  }

  // Send the button state string to the server (this will update the webpage)
  server.send(200, "text/plane", button_displayState);

  /*// zzzLT Debugging
  Serial.print("Button State: ");
  Serial.print(digitalRead(BUTTON));
  Serial.print(" || ");
  Serial.print("Display Status: ");
  Serial.println(button_displayState);
  */
}

// Controls the LEDs
void controlLEDs() {
    // Configure text that will be displayed on the webpage for LED state
  String state = "OFF";

  // This string is pulled from the webpage and determines the physical LED state
  String act_state = server.arg("state");

  // Turn on or off the LED based on act_state string from webpage
  if(act_state == "1") {
    digitalWrite(LED1,HIGH); //LED ON
    state = "ON";
  }
  else {
    digitalWrite(LED1,LOW); //LED OFF
    state = "OFF";
  }

  // Update the webpage with the current LED state
  server.send(200, "text/plane", state);
}

// Runs once
void setup(void)
{
  Serial.begin(115200);

  // Assign pin IO modes
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT); 
  pinMode(BUTTON,INPUT); 

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
  server.on("/led_set", controlLEDs);
  server.on("/buttonread", checkButtonState);
  server.begin();
}

// Run the webserver
void loop(void)
{
  server.handleClient();
}
