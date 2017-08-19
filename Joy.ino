// ATtyny2313 pins   {PA1,PA0,PD2,PD3,PD4,PD5,PD6,PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7};
static const uint8_t pins[] =  {2,3,4,5,6,7,8,9,10,11,12,A0,A1,A2,A3}; // Arduino Uno|Mini Pro, Button pins


typedef struct
 {
     uint8_t st;
     uint8_t nd;
     uint8_t st_o;
     uint8_t nd_o;
 }  gamepad;

gamepad gp;

void setup() {
  gp.st = 0;
  gp.nd = 0;
  gp.st_o = 0;
  gp.nd_o = 0;
  for (uint8_t p = 0; p < sizeof(pins); p++) {
     pinMode(  pins[p] , INPUT_PULLUP);
     digitalWrite(  pins[p] , HIGH);
  }
  Serial.begin(115200);
  delay(500); //Delay
}

void loop() {

  for(uint8_t p = 0; p < sizeof(pins); p++) {
    if (digitalRead(  pins[p] ) == LOW) {
      press( p + 1 );
    }
    else {
      release( p + 1 );
    }
  }
 
  if ( gp.st_o != gp.st || gp.nd_o != gp.nd    ) {
    gp.st = gp.st_o;
    gp.nd = gp.nd_o;
    JoyState( (char) gp.st , (char) gp.nd );
  }
  delay(20);
}

void JoyState(byte ST_BTN, byte ND_BTN) {
  Serial.write((byte)0xFD);   // HID raw report descriptor
  Serial.write((byte)0x6);    // Length
  Serial.write((byte)0x00);   // Right Y Axis 
  Serial.write((byte)0x00);   // Right X Axis 
  Serial.write((byte)0x00);   // Left Y Axis 
  Serial.write((byte)0x00);   // Left X Axis
  Serial.write((byte)ST_BTN); // fisrt 8 buttons
  Serial.write((byte)ND_BTN); //  second 8 buttons
}

void press(uint8_t b){ 
  if (b > 7) { // second byte
    b = b - 7;
    gp.nd_o |= (int)1 << (b - 1); 
  }
  else { // fisrt byte
    gp.st_o |= (int)1 << (b - 1); 
  }
}
void release(uint8_t b){ 
  if (b > 7) { // second byte
    b = b - 7;
    gp.nd_o &= ~((uint8_t)1 << (b - 1)); 
  }
  else { // first byte
    gp.st_o &= ~((uint8_t)1 << (b - 1)); 
  }
}
