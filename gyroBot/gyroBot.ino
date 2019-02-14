//#include "WiFi.h"
#include <MFRC522.h>
#include <SPI.h>
#include "ESPAsyncWebServer.h"
#include "FS.h"   //Include File System Headers
#include "SSD1306Spi.h"

//Defines for H bridge
#define enA 5
#define in1 4   //GPIO 1 blocks TX Serial Port
#define in2 14   //GPIO 3 blocks RX Serial Port
#define in3 12
#define in4 13
#define enB 15

// Replace with your network credentials
const char* ssid     = "ESP2";
const char* password = "";

bool accessPointMode = true;       //Use as a hotspot or connect to a local WiFi network

const char* filename = "/index.html";
int angleX = 0;
int angleY = 0;
bool deviceIsConnected = false;
AsyncWebServer server(80);

void setMotorSpeed(int leftMotor, int rightMotor) {
  if (leftMotor < 800) {   //Turn left
    analogWrite(enA, 1023 - leftMotor); // Send PWM signal to L298N Enable pin
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else if (leftMotor > 1040) {
    leftMotor -= 1200;
    analogWrite(enA, leftMotor); // Send PWM signal to L298N Enable pin
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }

  if (rightMotor < 800) {   //Turn left
    analogWrite(enB, 1023 - rightMotor); // Send PWM signal to L298N Enable pin
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else if (rightMotor > 1040) {
    rightMotor -= 1200;
    analogWrite(enB, rightMotor); // Send PWM signal to L298N Enable pin
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
}

int angleToMotorSpeed (int angle) {
  long motorSpeed = 0;
  motorSpeed = angle * (1023 / 90);
  return motorSpeed;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Setup Begin");
  //Initialize H bridge
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in4, OUTPUT);
  // Set initial rotation direction


  digitalWrite(in1, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in4, LOW);


  //Initialize File System
  if (SPIFFS.begin())
  {
    Serial.println("SPIFFS Initialize....ok");
  }
  else
  {
    Serial.println("SPIFFS Initialization...failed");
  }

  File f = SPIFFS.open(filename, "r");

  if (!f) {
    Serial.println("file open failed");
  }
  else
  {
    //Write html site to file
    Serial.println("Writing Data to File");
  }


  //Initialize WiFi Settings
  WiFi.setAutoConnect(0);                    //Enables or Disaple WiFi Quick Connect

  if (!accessPointMode) {
    WiFi.begin(ssid, password);               //Connect to a local WiFi network
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }
    Serial.println(WiFi.localIP().toString()); //Print local IP-Address*/
  } else {
    WiFi.softAP(ssid, password, 1, false, 1);              //Create WiFi hotspot WiFi.softAP(ssid, password, channel, hidden, max_connection)
    Serial.print("Access Point \"");
    Serial.print(ssid);
    Serial.println("\" started");
    Serial.print("IP address:\t");
    Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    deviceIsConnected = true;
    int paramsNr = request->params();
    //Serial.println(paramsNr);
    for (int i = 0; i < paramsNr; i++) {
      AsyncWebParameter* p = request->getParam(i);
      if (p->name() == "angleX") {
        angleX = p->value().toInt();
        Serial.print("Angle x: " + String(angleX));
      } else if (p->name() == "angleY") {
        angleY = p->value().toInt();
        Serial.println("Angle y: " + String(angleY));
      } else {
        Serial.print("unknown name: ");
        Serial.print(p->name());
        Serial.print(", value to int: ");
        Serial.println(p->value().toInt());
      }
    }
    request->send(SPIFFS, "/index.html");
    //drawDirection();
  });
  server.begin();
 // delay(5000);

  //TEST

  Serial.println("Setup Finished");
}

void loop() {
  //Serial.println("Angle: " + String(angleX) + " " + String(angleY) + " ");
  int newSpeed = angleToMotorSpeed(angleX);
  setMotorSpeed(newSpeed, newSpeed);
}
