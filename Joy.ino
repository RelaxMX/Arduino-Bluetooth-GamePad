
static const uint8_t pins[] = {2,3,4,5,6,7,8,9,10,11,12,A0,A1,A2,A3};

typedef struct
 {
     uint32_t st;
     uint32_t nd;
     uint32_t st_o;
     uint32_t nd_o;
 }  gamepad;

gamepad gp;

void setup() {
  gp.st = 0;
  gp.nd = 0;
  gp.st_o = 0;
  gp.nd_o = 0;
  for (int p = 0; p < 15; p++) {
     pinMode(  pins[p] , INPUT_PULLUP);
     digitalWrite(  pins[p] , HIGH);
  }
  Serial.begin(115200);
  delay(500); //Delay
}

void loop() {

  for(int p = 0; p <15; p++) {
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
    gp.nd_o |= (uint32_t)1 << (b - 1); 
  }
  else { // fisrt byte
    gp.st_o |= (uint32_t)1 << (b - 1); 
  }
}
void release(uint8_t b){ 
  if (b > 7) { // second byte
    b = b - 7;
    gp.nd_o &= ~((uint32_t)1 << (b - 1)); 
  }
  else { // first byte
    gp.st_o &= ~((uint32_t)1 << (b - 1)); 
  }
}
