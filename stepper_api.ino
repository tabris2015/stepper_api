#include <Arduino.h>
#include "motor_api.h"
#define STEP_PIN 9
#define DIR_PIN 3
#define ENABLE_PIN 7

#define TIME 15

Motor motor(DIR_PIN, STEP_PIN, ENABLE_PIN);


uint8_t done = 0;

// serial string parse
// Buffer for the incoming data
char inData[100];
// Buffer for the parsed data chunks
char *inParse[100];

// Storage for data as string
String inString = "";

// Incoming data id
int index = 0;
// Read state of incoming data
boolean stringComplete = false;

// prototipos 

void ParseSerialData();


void moverAngulo(int, int, int, int, int);
void moverTiempo(int, int, int, int, int);
void moverPasos(int, int, int, int, int);


void setup() {

  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
    
  digitalWrite(DIR_PIN, 0);

  // motor.spinSteps(100, 5, 0, 3, 300);

  // motor.spinTime(1000, 4, 1, 4, 500);

  // motor.spinAngle(90, 6, 0, 4, 1000);

  Serial.begin(9600);
}

void loop() {

  if (stringComplete) 
  {
    // Parse the recieved data
    ParseSerialData();
    // Reset inString to empty
    inString = "";    
    // Reset the system for further 
    // input of data   
    stringComplete = false; 
  }  

  motor.update();
}

void ParseSerialData()
{
  // The data to be parsed
  char *p = inData;
  // Temp store for each data chunk
  char *str;   
  // Id ref for each chunk 
  int count = 0;
    
  // Loop through the data and seperate it into
  // chunks at each "," delimeter
  while ((str = strtok_r(p, ",", &p)) != NULL)
  { 
    // Add chunk to array  
    inParse[count] = str;
    // Increment data count
    count++;      
  }
  if(count == 1)
  {
    if(*inParse[0] == 'X')
    {
      motor.stop();
      Serial.println("Deteniendo");
    }
  }
  // If the data has two values then..  
  if(count == 4)
  {
    // Define value 1 as a function identifier
    char *func = inParse[0];
    // Define value 2 as a property value
    int dir = atoi(inParse[1]);
    int vel = atoi(inParse[2]);
    int prop = atoi(inParse[3]);
    
    // Call the relevant identified function  
    switch(*func)
    {
      case 'A': moverAngulo(dir, vel, prop, 1, 0); break;
      case 'T': moverTiempo(dir, vel, prop, 1, 0); break;
      case 'P': moverPasos(dir, vel, prop, 1, 0); break;
    }    
  }
  
  if(count == 6)
  {
    // Define value 1 as a function identifier
    char *func = inParse[0];
    // Define value 2 as a property value
    int dir = atoi(inParse[1]);
    int vel = atoi(inParse[2]);
    int prop = atoi(inParse[3]);
    int rep = atoi(inParse[4]);
    int pause = atoi(inParse[5]);
    
    // Call the relevant identified function  
    switch(*func)
    {
      case 'A': moverAngulo(dir, vel, prop, rep, pause); break;
      case 'T': moverTiempo(dir, vel, prop, rep, pause); break;
      case 'P': moverPasos(dir, vel, prop, rep, pause); break;
    }  
  }
}

void serialEvent() 
{
  // Read while we have data
  while (Serial.available() && stringComplete == false) 
  {
    // Read a character
    char inChar = Serial.read(); 
    // Store it in char array
    inData[index] = inChar; 
    // Increment where to write next
    index++;     
    // Also add it to string storage just
    // in case, not used yet :)
    inString += inChar;
    
    // Check for termination character
    if (inChar == '\n') 
    {
      // Reset the index
      index = 0;
      // Set completion of read to true
      stringComplete = true;
    }
  }
}

// funciones

void moverAngulo(int dir, int vel, int angle, int rep, int pause)
{
  motor.spinAngle(angle, vel, dir, rep, pause);
  Serial.print("Moviendo ");
  Serial.print(angle);
  Serial.println(" grados.");
}
void moverTiempo(int dir, int vel, int time, int rep, int pause)
{
  motor.spinTime(time, vel, dir, rep, pause);
  Serial.print("Moviendo ");
  Serial.print(time);
  Serial.println(" ms.");
}
void moverPasos(int dir, int vel, int steps, int rep, int pause)
{
  motor.spinSteps(steps, vel, dir, rep, pause);
  Serial.print("Moviendo ");
  Serial.print(steps);
  Serial.println(" pasos.");
}