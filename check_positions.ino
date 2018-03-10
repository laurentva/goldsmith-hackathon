#include <Pozyx.h>
#include <Pozyx_definitions.h>
#include <Wire.h>

void setup(){
  Serial.begin(115200);
  if(Pozyx.begin() == POZYX_FAILURE){
    Serial.println(F("ERROR: Unable to connect to POZYX shield"));
    Serial.println(F("Reset required"));
    delay(100);
    abort();
  }
}

void loop(){
  coordinates_t position;
  int status = checkLocalNewPosition(&position);
  if (status == POZYX_SUCCESS){
    printCoordinates(position);
  }else{
    printErrorCode("positioning");
  }
}

int checkLocalNewPosition(coordinates_t *position){
  assert(position != NULL);
  int status;
  uint8_t int_status = 0;
  // now wait for the positioning to finish or generate an error
  if (Pozyx.waitForFlag_safe(POZYX_INT_STATUS_POS | POZYX_INT_STATUS_ERR, 2*POZYX_DELAY_INTERRUPT, &int_status)){
    if((int_status & POZYX_INT_STATUS_ERR) == POZYX_INT_STATUS_ERR)
    {
      // An error occured during positioning.
      // Please read out the register POZYX_ERRORCODE to obtain more information about the error
      return POZYX_FAILURE;
    }else{
      status = Pozyx.getCoordinates(position);
      return status;
    }
  }else{
    return POZYX_TIMEOUT;
  }
}

// prints the coordinates for either humans or for processing
void printCoordinates(coordinates_t coor){
  Serial.print("POS");
  Serial.print(", x(mm): ");
  Serial.print(coor.x);
  Serial.print(", y(mm): ");
  Serial.print(coor.y);
  Serial.print(", z(mm): ");
  Serial.println(coor.z);
}

// error printing function for debugging
void printErrorCode(String operation){
  uint8_t error_code;
  Pozyx.getErrorCode(&error_code);
  Serial.print("ERROR ");
  Serial.print(operation);
  Serial.print(", local error code: 0x");
  Serial.println(error_code, HEX);
}
