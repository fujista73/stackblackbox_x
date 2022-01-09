
/*
****************************************
  Function processCommand
  parses data commands contained in receivedChars[]
  receivedChars[] has not been checked for errors

  passed:

  global:
        receivedChars[]
        newData

  Returns:

  Sets:
        receivedChars[]
        newData

*/
void processCommand()
{

  if (strcmp ("SBB01", receivedChars) == 0)
  {
    mueveMotor(1, 1, sentido);
    
  }

  else if (strcmp ("SBB02", receivedChars) == 0)
  {
    mueveMotor(1, 1, sentido);
    
  }

  else if (strcmp ("SBB03", receivedChars) == 0)
  {
    mueveMotor(1, 1, !sentido);
    
  }

  else if (strcmp ("SBB04", receivedChars) == 0)
  {
    mueveMotor(1, 1, !sentido);
    
  }

  receivedChars[0] = '\0';
  newData = false;
}




// function recvWithStartEndMarkers by Robin2 of the Arduino forums
// See  http://forum.arduino.cc/index.php?topic=288234.0
/*
****************************************
  Function recvWithStartEndMarkers
  reads serial data and returns the content between a start marker and an end marker.

  passed:

  global:
        receivedChars[]
        newData

  Returns:

  Sets:
        newData
        receivedChars

*/
void recvWithStartEndMarkers()
{

  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;
  if (Serial3.available() > 0)
  {
    rc = Serial3.read();
    if (recvInProgress == true)
    {
      if (rc != endMarker)
      {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx > maxDataLength) {
          ndx = maxDataLength;
        }
      }
      else
      {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }

}


void verMonitorSerie()
{

    // Si llega algo por el puerto bluetooth, lo escribe en pantalla

    if (Serial3.available()) {
      Serial.write(Serial3.read());    // Escribe por el puerto serie lo que recibe por el puerto bluetooth
    }

    if (Serial.available()) {
      char c = Serial.read() ;
      Serial.print(c);
      Serial3.print(c);
    }
   

    // Si llega algo por el puerto 433MHz, lo escribe en pantalla

    if (Serial1.available()) {
      Serial.write(Serial1.read());    // Escribe por el puerto serie lo que recibe por el puerto HC-12
    }

    if (Serial.available()) {
      char c = Serial.read() ;
      Serial.print(c);
      Serial1.print(c);
    }
    

}
