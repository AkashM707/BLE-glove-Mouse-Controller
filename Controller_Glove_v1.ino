#include <BleMouse.h>                                         // BLE Communication librairy
#include <MPU6050_tockn.h>                                    // MPU6050 Data recuperation librairy
#include <Wire.h>             

BleMouse bleMouse;                                            // Mouse decalaration

int strainGauge = 15;                                         // Pin declaration for the strain gauge
int sensorValue=0;                                            // Value of the dividing bridge
int threshold = 1900;                                         // Threshold value for the click

MPU6050 mpu6050(Wire);
int X,Y,Z;                                                    // Data Variables for mouse co-ordinates
int OX,OY,OZ;                                                 // Angle Variables for calucating gyroscope zero error
int ValueX,ValueY;                                            // Quantified values
int mouse=0;                                                  // State of the mouse (click or no click)

void setup() {
   
  Serial.begin(115200);                                       // Initialise Serial communication
  Wire.begin();                                               // Initialise I2C communication
  mpu6050.begin();                                            // Initialise Gyro communication
  //Serial.println("STRTM");                                  // Identifier "STARTM" for start mouse
  mpu6050.calcGyroOffsets(true);                              // Setting Gyro offsets
  mpu6050.update();                                           // Command to calculate the sensor data before using the get command
  OX = mpu6050.getAngleX();                                   // Getting angle X Y Z
  OY = mpu6050.getAngleY();
  OZ = mpu6050.getAngleZ();
  bleMouse.begin();                                           //Initialise BLE communication

  if(OX < 0){                                                 // Inverting the sign of all the three offset values for zero error correction
    OX = OX *(-1);}
  else{
    OX = (OX-OX)-OX;}

  if(OY < 0){
    OY = (OY *(-1));}
  else{
    OY = ((OY-OY)-OY)+10;}

  if(OZ < 0){
    OZ = OZ *(-1);}
  else{
    OZ = (OZ-OZ)-OZ;}
}

void loop() {
  mpu6050.update();                                                 //Update the values of the mpu6050
  sensorValue = analogRead(strainGauge);                            //Update the value of dividing bridge
  X = OX + mpu6050.getAngleX();                                     // Getting current angle for X Y Z and correcting the zero error
  Y = OY + mpu6050.getAngleY();
  Z = OZ + mpu6050.getAngleZ();
  if( X > 28)                                                       //Quantification of the values (7 states per axis)
  {
    ValueX = -3;
  }
  else
  {
    if(X > 17)
    {
      ValueX = -2;
    }
    else
    {
      if(X > 6)
      {
        ValueX = -1;
      }
      else
      {
        if(X > -5)
        {
          ValueX= 0;
        }
        else
        {
          if(X > -15)
          {
            ValueX = 1;
          }
          else
          {
            if( X > -26)
            {
              ValueX = 2;
            }
            else
            {
              ValueX = 3;
            }
          }
        }
      }
    }
  }
  if( Y > 28)
  {
    ValueY = 3;
  }
  else
  {
    if(Y > 17)
    {
      ValueY = 2;
    }
    else
    {
      if(Y > 5)
      {
        ValueY = 1;
      }
      else
      {
        if(Y > -6)
        {
          ValueY= 0;
        }
        else
        {
          if(Y > -16)
          {
            ValueY = -1;
          }
          else
          {
            if( Y > -26)
            {
              ValueY = -2;
            }
            else
            {
              ValueY = -3;
            }
          }
        }
      }
    }
  }
  if(bleMouse.isConnected()){                                          //Check if the mouse is connected
    delay(5);                                                          //Mouse movement resolution delay
    //Serial.println("DATAL,"+String(X)+','+String(Y));                //Debug line
    if(!((ValueX==0)&&(ValueY==0))){                                   //Check if the mouse move
      bleMouse.move(-ValueY,ValueX,0);                                 //Move the mouse according to the quantified value
    }
    //Serial.println("DATAC,"+String(mpu6050.getAngleX())+','+String(mpu6050.getAngleY())+','+String(mpu6050.getAngleZ())); //Debug Line
    //Serial.println(sensorValue);                                     //Debug Line 
    if((mouse==0)&&(sensorValue < threshold )&&(sensorValue != 0)){    //Checks if Left Mouse Button is pushed 
      delay(100);                                                      //Debounce delay
      //Serial.println("DATAB,L");                                     //Debug Line 
      bleMouse.press(1);                                               //Press the Left-click button
      mouse=1;                                                         //State of the click : pressed
    }
    else
    { 
      if((mouse==1)&&(sensorValue > threshold)){                       //Check if the user realesed the Left-click button
        //Serial.println("DATAB,R");
        bleMouse.release(1);                                           //Release the click
        mouse=0;                                                       //State of the click : non pressed
      }
    }
  }
}
