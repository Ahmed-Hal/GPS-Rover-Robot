void Forward()
{
  Serial.println("Forward");
      digitalWrite(IN_4, HIGH);
      digitalWrite(IN_3, LOW);
     analogWrite(ENA, mtr_Spd);
      digitalWrite(IN_2, LOW);
      digitalWrite(IN_1, HIGH);
      analogWrite(ENB, mtr_Spd);            
 
}

// **********************************************************************************************************************************************************************

void Forward_Meter()
{
      digitalWrite(IN_4, HIGH);
      digitalWrite(IN_3, LOW);
     analogWrite(ENA, mtr_Spd);
      digitalWrite(IN_2, LOW);
      digitalWrite(IN_1, HIGH);
      analogWrite(ENB, mtr_Spd);
      
  delay(500);
}

// **********************************************************************************************************************************************************************

void Reverse()
{
      digitalWrite(IN_4, LOW);
      digitalWrite(IN_3,HIGH );
      analogWrite(ENA, mtr_Spd);

      digitalWrite(IN_2, HIGH);
      digitalWrite(IN_1,LOW );
      analogWrite(ENB, mtr_Spd);
}

// **********************************************************************************************************************************************************************

void LeftTurn()
{
      digitalWrite(IN_4, HIGH);
      digitalWrite(IN_3, LOW);
      analogWrite(ENA, mtr_Spd);

      digitalWrite(IN_2,HIGH);
      digitalWrite(IN_1,LOW );
      analogWrite(ENB, mtr_Spd);
  delay(100);    
}

// **********************************************************************************************************************************************************************

void RightTurn()
{
      digitalWrite(IN_4, LOW);
      digitalWrite(IN_3,HIGH );
      analogWrite(ENA, mtr_Spd);

      digitalWrite(IN_2, LOW);
      digitalWrite(IN_1, HIGH);
      analogWrite(ENB, mtr_Spd);
  delay(100);                                                                       //delay for 100ms more responsive turn per push on bluetooth  
}

// **********************************************************************************************************************************************************************


void SlowLeftTurn()
{
   
      digitalWrite(IN_4, HIGH);
      digitalWrite(IN_3, LOW);
      analogWrite(ENA, turn_Speed);

       digitalWrite(IN_2,HIGH);
      digitalWrite(IN_1,LOW );
      analogWrite(ENB, turn_Speed);
}

// **********************************************************************************************************************************************************************

// Turning too fast will over-shoot the compass and GPS course


void SlowRightTurn()
{
   
      digitalWrite(IN_4, LOW);
      digitalWrite(IN_3,HIGH );
      analogWrite(ENA, turn_Speed);

      digitalWrite(IN_2, LOW);
      digitalWrite(IN_1, HIGH);
      analogWrite(ENB, turn_Speed);
}

// **********************************************************************************************************************************************************************

void StopCar()
{
   
  
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2,LOW );

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
   
}


// Compass Drive Section
// This Portion of code steers the Vehicle based on the compass heading
// **********************************************************************************************************************************************************************

void CompassTurnRight()                                                          // This Function Turns the car 90 degrees to the right based on the current desired heading
{
  StopCar();    
  getCompass();                                                                  // get current compass heading      

  desired_heading = (desired_heading + 90);                                      // set desired_heading to plus 90 degrees 
  if (desired_heading >= 360) {desired_heading = (desired_heading - 360);}       // if the desired heading is greater than 360 then subtract 360 from it

  while ( abs(desired_heading - compass_heading) >= compass_dev)                 // If the desired heading is more than Compass Deviation in degrees from the actual compass heading then
      {                                                                          // correct direction by turning left or right

    getCompass();                                                                // Update compass heading during While Loop
    bluetooth();                                                                 // if new bluetooth value received break from loop        
    if (blueToothVal == "5"){break;}                                               // If a Stop Bluetooth command ('5') is received then break from the Loop
        
    if (desired_heading >= 360) {desired_heading = (desired_heading - 360);}     // if the desired heading is greater than 360 then subtract 360 from it                                            
                                                                
    int x = (desired_heading - 359);                                             // x = the GPS desired heading - 360    
    int y = (compass_heading - (x));                                             // y = the Compass heading - x
    int z = (y - 360);                                                           // z = y - 360
            
        if ((z <= 180) && (z >= 0))                                              // if z is less than 180 and not a negative value then turn left 
            {                                                                    // otherwise turn right
              SlowLeftTurn();                            
            } 
            else
            {
              SlowRightTurn();        
            }  
        }    
    {
      StopCar();                                                                  // Stop the Car when desired heading and compass heading match
    }
 }    


// **********************************************************************************************************************************************************************

void CompassTurnLeft()                                                           // This procedure turns the car 90 degrees to the left based on the current desired heading
{
  StopCar();    
  getCompass();                                                                  // get current compass heading                                                                                  
  //desired_heading = (compass_heading - 90);                                    // set desired_heading to compass value minus 90 degrees

  desired_heading = (desired_heading - 90);                                      // set desired_heading to minus 90 degrees
  if (desired_heading <= 0) {desired_heading = (desired_heading + 360);}         // if the desired heading is greater than 360 then add 360 to it
  while ( abs(desired_heading - compass_heading) >= compass_dev)                 // If the desired heading is more than Compass Deviation in degrees from the actual compass heading then
      {                                                                          // correct direction by turning left or right
    getCompass();                                                                // Get compass heading again during While Loop
    bluetooth();                                                                 // if new bluetooth value received break from loop              
    if (blueToothVal == "5"){break;}                                               // If a 'Stop' Bluetooth command is received then break from the Loop
    
    if (desired_heading >= 360) {desired_heading = (desired_heading - 360);}     // if the desired heading is greater than 360 then subtract 360 from it                                            
                                                                
    int x = (desired_heading - 359);                                             // x = the desired heading - 360    
    int y = (compass_heading - (x));                                             // y = the Compass heading - x
    int z = (y - 360);                                                           // z = y - 360
        if (z <= 180)                                                            // if z is less than 180 and not a negative value then turn left         
       // if ((z <= 180) && (z >= 0))                                             
            {                                                                    // otherwise turn right
              SlowLeftTurn();                             
            } 
            else
            {
              SlowRightTurn();              
            }  
        }    
    {
      StopCar();                                                                 // Stop the Car when desired heading and compass heading match
    }
 }   

// **********************************************************************************************************************************************************************

void Compass_Forward()                                               
{            
  while (blueToothVal == "9")                                           // Go forward until Bluetooth 'Stop' command is sent

  //while (true)                                                        
   {  
    getCompass();                                                     // Update Compass Heading
    bluetooth();                                                      // Check to see if any Bluetooth commands have been sent
    if (blueToothVal == "5") {break;}                                   // If a Stop Bluetooth command ('5') is received then break from the Loop
    
    if ( abs(desired_heading - compass_heading) <= compass_dev )      // If the Desired Heading and the Compass Heading are within the compass deviation, X degrees of each other then Go Forward
                                                                      // otherwise find the shortest turn radius and turn left or right  
       {
         Forward(); 
      } else 
         {    
            int x = (desired_heading - 359);                          // x = the GPS desired heading - 360
            int y = (compass_heading - (x));                          // y = the Compass heading - x
            int z = (y - 360);                                        // z = y - 360
                     
            if ((z <= 180) && (z >= 0))                               // if z is less than 180 and not a negative value then turn left 
            {                                                         // otherwise turn right
              SlowLeftTurn();
            }
            else
            {
              SlowRightTurn();
            }              
        } 
 }                                                                   // End While Loop
}                                                                    // End Compass_Forward

// **********************************************************************************************************************************************************************

void turnAround()                                                   // This procedure turns the Car around 180 degrees, every time the "Turn Around" button is pressed
 {                                                                  // the car alternates whether the next turn will be to the left or right - this is determined by the 'pass' variable
                                                                    // Imagine you are cutting the grass, when you get to the end of the row - the first pass you are turning one way and on the next pass you turn the opposite   
    if (pass == 0) { CompassTurnRight(); }                          // If this is the first pass then turn right
    
    else { CompassTurnLeft(); }                                     // If this is the second pass then turn left
      
    //Forward_Meter();                                              // Go forward one meter (approximately)
    StopCar();                                                      // Stop the car
    
       
    if (pass == 0)                                                  // If this is the first pass then Turn Right
      {       
        CompassTurnRight();                                         // Turn right
        pass = 1 ;                                                  // Change the pass value to '1' so that the next turn will be to the left
      }
      
    else 
      {     
         
    if (desired_heading == Heading_A)                               // This section of code Alternates the desired heading 180 degrees
     {                                                              // for the Compass drive forward
      desired_heading = Heading_B;
     }
    else if (desired_heading == Heading_B)
     {
      desired_heading = Heading_A;
     }        
          
        CompassTurnLeft();                                          // If this is the second pass then Turn Left
        pass = 0;                                                   // Change the pass value to '0' so that the next turn will be to the right

      }
      
  Compass_Forward();                                                // Maintain the 'desired heading' and drive forward
}
   
