void bluetooth()
{
    server.handleClient();
    blueToothVal = server.arg("State");                              //  convert the string 'str' into an integer and assign it to blueToothVal
    Serial.println(blueToothVal);    

// .................................................................................................................................................


 if(blueToothVal=="1"){Serial1.println("Forward");Forward();}
 else if(blueToothVal=="2"){Serial1.println("Reverse");Reverse();} 
 else if(blueToothVal=="3"){Serial1.println("Left");LeftTurn();StopCar();}
 else if(blueToothVal=="4"){Serial1.println("Right");RightTurn();StopCar();}
 else if(blueToothVal=="5"){Serial1.println("Stop Car");StopCar();}
 else if(blueToothVal=="6"){setWaypoint();}
 else if(blueToothVal=="7"){goWaypoint();} 
 else if(blueToothVal=="5"){Serial1.println("Turn Around");turnAround();}
 else if(blueToothVal=="9"){Serial1.println("Compass Forward"); setHeading();Compass_Forward();}
 else if(blueToothVal=="10"){setHeading();}
 else if(blueToothVal=="11"){gpsInfo();}
 else if(blueToothVal=="12"){Serial1.println("Compass Turn Right");CompassTurnRight();}
 else if(blueToothVal=="13"){Serial1.println("Compass Turn Left");CompassTurnLeft();}
 else if(blueToothVal=="16"){clearWaypoints();}
 else if(blueToothVal=="17"){ac = 0; Serial1.print("Waypoints Complete");}

 // end of nisted if condetions

// ..................................................................................................................................................  
// Slider Value for Speed

if (blueToothVal)                                    
  {    
   //Serial.println(blueToothVal);
   
      if (blueToothVal == "20") turn_Speed = 400;
      else if (blueToothVal == "21") turn_Speed = 470;
      else if (blueToothVal == "22") turn_Speed = 540;
      else if (blueToothVal == "23") turn_Speed = 610;
      else if (blueToothVal == "24") turn_Speed = 680;
      else if (blueToothVal == "25") turn_Speed = 750;
      else if (blueToothVal == "26") turn_Speed = 820;
      else if (blueToothVal == "27") turn_Speed = 890;
      else if (blueToothVal == "28") turn_Speed = 960;
      else if (blueToothVal == "29") turn_Speed = 1023;
  
Serial1.print("Speed set To:  ");
Serial1.println(turn_Speed);
  }  
  
} // end of bluetooth procedure
