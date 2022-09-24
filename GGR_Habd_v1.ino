#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Wire.h>                                                 // Used by I2C and HMC5883L compass
#include <HMC5883L.h>      
//******************************************************************************************************                                                                  
// GPS Variables & Setup

int GPS_Course;                                                    // variable to hold the gps's determined course to destination
int Number_of_SATS;                                                // variable to hold the number of satellites acquired
TinyGPSPlus gps;                                                   // gps = instance of TinyGPS 
                                                                   // pin 17 (blue)   is connected to the TX on the GPS
                                                                   // pin 16 (yellow) is connected to the RX on the GPS
//******************************************************************************************************                                                                  
//// Setup Drive Motors using the Adafruit Motor Controller version 1.0 Library
//
//AF_DCMotor motor1(1, MOTOR12_64KHZ);                               // create motor #1, 64KHz pwm
//AF_DCMotor motor2(2, MOTOR12_64KHZ);                               // create motor #2, 64KHz pwm
//AF_DCMotor motor3(3, MOTOR12_64KHZ);                               // create motor #3, 64KHz pwm
//AF_DCMotor motor4(4, MOTOR12_64KHZ);                               // create motor #4, 64KHz pwm

#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15          // L298N in1 motors Rightx          GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  0           // L298N in4 motors Left            GPIO0(D3)

int turn_Speed = 500;                                              // motor speed when using the compass to turn left and right
int mtr_Spd = 700;                                                 // motor speed when moving forward and reverse

//******************************************************************************************************
// Compass Variables & Setup

HMC5883L compass;                                                  // HMC5883L compass(HMC5883L)
int16_t mx, my, mz;                                                // variables to store x,y,z axis from compass (HMC5883L)
int desired_heading;                                               // initialize variable - stores value for the new desired heading
int compass_heading;                                               // initialize variable - stores value calculated from compass readings
int compass_dev = 5;                                               // the amount of deviation that is allowed in the compass heading - Adjust as Needed
                                                                   // setting this variable too low will cause the robot to continuously pivot left and right
                                                                   // setting this variable too high will cause the robot to veer off course

int Heading_A;                                                     // variable to store compass heading
int Heading_B;                                                     // variable to store compass heading in Opposite direction
int pass = 0;                                                      // variable to store which pass the robot is on
 
//*************************************************************
// Bluetooth/wifi Variables & Setup

String str;                                                        // raw string received from android to arduino
String blueToothVal;                                                  // stores the last value sent over via bluetooth

const char* ssid = "DIY HAL";
ESP8266WebServer server(80);
//*************************************************************
// GPS Locations

unsigned long Distance_To_Home;                                    // variable for storing the distance to destination

int ac =0;                                                         // GPS array counter
int wpCount = 0;                                                   // GPS waypoint counter
double Home_LATarray[50];                                          // variable for storing the destination Latitude - Only Programmed for 5 waypoint
double Home_LONarray[50];                                          // variable for storing the destination Longitude - up to 50 waypoints


int increment = 0;
//===============================================================

void setup() 
{  

#define ENA   14          //GPIO14(D5)        Enable/speed motors Right 
#define ENB   12          //GPIO12(D6)          Enable/speed motors Left
#define IN_4  0            //in-4   GPIO15(D3)           L298N in1 motors Rightx
#define IN_3  2            //in-3   GPIO13(D4)           L298N in2 motors Right
#define IN_2  15           //in-2   GPIO2(D7)            L298N in3 motors Left
#define IN_1  13           //in-1   GPIO0(D8)            L298N in4 motors Left

 
  Serial.begin(115200);                                            // Serial 0 is for communication with the computer
  Serial1.begin(9600);                                             // Serial 1 is for Bluetooth communication - DO NOT MODIFY - JY-MCU HC-06 v1.40
  Serial.begin(9600);                                             // Serial 2 is for GPS communication at 9600 baud - DO NOT MODIFY - Ublox Neo 6m   

// Connecting WiFi

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
 // Starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
 
  // Compass
  Wire.begin();                                                    // Join I2C bus used for the HMC5883L compass
  compass.begin();                                                 // initialize the compass (HMC5883L)
  compass.setRange(HMC5883L_RANGE_1_3GA);                          // Set measurement range  
  compass.setMeasurementMode(HMC5883L_CONTINOUS);                  // Set measurement mode  
  compass.setDataRate(HMC5883L_DATARATE_30HZ);                     // Set data rate  
  compass.setSamples(HMC5883L_SAMPLES_8);                          // Set number of samples averaged  
  compass.setOffset(-40,41);                                          // Set calibration offset 

  Startup();                                                       // Run the Startup procedure on power-up one time
}

//=================================================================
// Main Loop
void loop()
{ 
  
  bluetooth();                                                     // Run the Bluetooth procedure to see if there is any data being sent via BT                                                    
  getGPS();                                                        // Update the GPS location
  getCompass();                                                    // Update the Compass Heading

}

void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
