
#include <Wire.h> // For i2c communications with the MCP23017 i/o expanders and the HT16K33 LED matrix driver
#include "Adafruit_LEDBackpack.h" // For the HT16K33
#include "Adafruit_GFX.h" // For the HT16K33

// Serial
String command; // Used for debugging to print the chars to the Serial Monitor
int commandValue = 0;
char charIn;


Adafruit_LEDBackpack matrixA = Adafruit_LEDBackpack();
Adafruit_LEDBackpack matrixB = Adafruit_LEDBackpack();
Adafruit_LEDBackpack matrixC = Adafruit_LEDBackpack();
Adafruit_LEDBackpack matrixD = Adafruit_LEDBackpack();
Adafruit_LEDBackpack matrixE = Adafruit_LEDBackpack();

#define matAaddy 0x70
#define matBaddy 0x74
#define matCaddy 0x76
#define matDaddy 0x72
#define matEaddy 0x71

// MCP23017 registers (everything except direction defaults to 0)
#define IODIRA   0x00   // IO direction  (0 = output, 1 = input (Default))
#define IODIRB   0x01
#define IOPOLA   0x02   // IO polarity   (0 = normal, 1 = inverse)
#define IOPOLB   0x03
#define GPINTENA 0x04   // Interrupt on change (0 = disable, 1 = enable)
#define GPINTENB 0x05
#define DEFVALA  0x06   // Default comparison for interrupt on change (interrupts on opposite)
#define DEFVALB  0x07
#define INTCONA  0x08   // Interrupt control (0 = interrupt on change from previous, 1 = interrupt on change from DEFVAL)
#define INTCONB  0x09
#define IOCON    0x0A   // IO Configuration: bank/mirror/seqop/disslw/haen/odr/intpol/notimp
//#define IOCON 0x0B  // same as 0x0A
#define GPPUA    0x0C   // Pull-up resistor (0 = disabled, 1 = enabled)
#define GPPUB    0x0D
#define INFTFA   0x0E   // Interrupt flag (read only) : (0 = no interrupt, 1 = pin caused interrupt)
#define INFTFB   0x0F
#define INTCAPA  0x10   // Interrupt capture (read only) : value of GPIO at time of last interrupt
#define INTCAPB  0x11
#define GPIOA    0x12   // Port value. Write to change, read to obtain value
#define GPIOB    0x13
#define OLLATA   0x14   // Output latch. Write to latch output.
#define OLLATB   0x15

#define expA 0x20
#define expB 0x21
#define expC 0x22
#define expD 0x23

#define barCur   0
#define barVolt  3
#define barOx    6
#define barOhm   9

uint8_t state[64]; // buffer for state of buttons
uint8_t potstate[9]; // buffer for state of potentiometers
uint16_t ABuffer[8]; // buffer for LED matrix A
uint16_t BBuffer[8]; // buffer for LED matrix B
uint16_t CBuffer[8]; // buffer for LED matrix C
uint16_t DBuffer[8]; // buffer for LED matrix D
uint16_t EBuffer[8]; // buffer for LED matrix E

uint8_t MTdaysC[4] = {0,1,2,3};
uint8_t MThoursC[2] = {4,5};
uint8_t MTminutesC[2] = {6,7};
uint8_t MTCat[8] = {0,1,2,3,4,5,6,7};
uint8_t MTAn[8] = {0,1,2,3,4,5,6,7};
uint8_t pitchCat[4] = {0,1,2,3};
uint8_t pitchAn[8] = {8,9,10,11,12,13,14,15};
uint8_t yawCat[4] = {4,5,6,7};
uint8_t yawAn[8] = {8,9,10,11,12,13,14,15};
uint8_t rollCat[4] = {0,1,2,3};
uint8_t rollAn[8] = {0,1,2,3,4,5,6,7};
uint8_t ihrCat[4] = {0,1,2,3};
uint8_t ihrAn[8] = {8,9,10,11,12,13,14,15};
uint8_t abrCat[4] = {4,5,6,7};
uint8_t abrAn[8] = {8,9,10,11,12,13,14,15};
uint8_t ahrCat[4] = {4,5,6,7};
uint8_t ahrAn[8] = {0,1,2,3,4,5,6,7};
uint8_t statCat[6] = {0,1,2,3,4,5};
uint8_t statAn[6] = {0,1,2,3,4,5};


uint8_t grphCats[24] = {0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7};
uint8_t grphAns[24] =  {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2};
uint8_t cryoCats[24] = {0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7};
uint8_t cryoAns[24] =  {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2};

//uint8_t cryoCats[24] = {7,6,5,4,3,2,1,0,7,6,5,4,3,2,1,0,7,6,5,4,3,2,1,0};
//uint8_t cryoAns[24] =  {2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0};

//                   r,g,r,g,r,g,r,g,r,g,r,g,r,g,r,g,r,g,r,g,r,g,r,g
uint8_t gtwelve[] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
uint8_t geleven[] = {0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
uint8_t gten[] =    {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
uint8_t gnine[] =   {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
uint8_t geight[] =  {0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
uint8_t gseven[] =  {0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
uint8_t gsix[] =    {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1};
uint8_t gfive[] =   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1};
uint8_t gfour[] =   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1};
uint8_t gthree[] =  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1};
uint8_t gtwo[] =    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0};
uint8_t gone[] =    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0};

//                   r,g,r,g,r,g,r,g,r,g,r,g,r,g,r,g,r,g,r,g,r,g,r,g
uint8_t ctwelve[] = {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
uint8_t celeven[] = {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0};
uint8_t cten[] =    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0};
uint8_t cnine[] =   {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0};
uint8_t ceight[] =  {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0};
uint8_t cseven[] =  {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0};
uint8_t csix[] =    {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t cfive[] =   {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t cfour[] =   {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t cthree[] =  {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t ctwo[] =    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t cone[] =    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


unsigned long numericTimer = 0;
#define NUMERICREFRESH 2000
uint8_t numericPos = 0;

// Mission Timer
uint8_t days = 0;
uint8_t minutes = 0;
uint8_t hours = 0;
unsigned long clockTimer = 0;

int rxCmnd[5];

void NprocessSerial (void)
{
  uint8_t cmndPos = 0;
  if (Serial.available()) {
    delay(5); // wait for all data to come in
    while(Serial.available() > 0) {
      charIn = Serial.read();
      if (charIn == '\n') {
        decodeCommand();
      } else {
        if (charIn == ',') {
          //rxCmnd[cmndPos] = commandValue;
          //threeDigitDisp(matrixD, DBuffer, yawCat, yawAn, commandValue);
          cmndPos++;
        } else {
          //if( isDigit(charIn) )// is this an ascii digit between 0 and 9?
          //{
            rxCmnd[cmndPos] = (charIn - '0');
            //commandValue = (commandValue * 10) + (charIn - '0'); // yes, accumulate the value
          //}          
        }
      }
    }
  }
}
void processSerial (void)
{
  uint8_t cmndPos = 0;
  commandValue = 0;
  if (Serial.available()) {
    delay(5); // wait for all data to come in
    while(Serial.available() > 0) {
      charIn = Serial.read();
      if (charIn == '\n') {
        decodeCommand();
      } else {
        if (charIn == ',') {
          //rxCmnd[cmndPos] = commandValue;
          //commandValue = 0;
          //threeDigitDisp(matrixD, DBuffer, yawCat, yawAn, commandValue);
          cmndPos++;
        } else {
          //if( isDigit(charIn) )// is this an ascii digit between 0 and 9?
          //{
            rxCmnd[cmndPos] = (charIn - '0');
            //commandValue = (commandValue * 10) + (charIn - '0'); // yes, accumulate the value
          //}          
        }
      }
    }
  }
}

void decodeCommand(void)
{
  if (rxCmnd[0] == 1) {
    switch (rxCmnd[1]) {
      case 4:      
      ledOn(matrixE, EBuffer, rxCmnd[3], rxCmnd[4]);
      break;
    }
  } else if (rxCmnd[0] == 0) {
    switch (rxCmnd[1]) {
      case 4:      
      ledOff(matrixE, EBuffer, rxCmnd[3], rxCmnd[4]);
      break;
    }
    
  } else if (rxCmnd[0] == 2) { // Set bargraphs
      cBarDisp(matrixC, CBuffer, cryoCats, cryoAns, rxCmnd[1],rxCmnd[2]);
  }
  for (uint8_t i=0; i<5; i++) {
    rxCmnd[i] = 0;
  }
}

void allMatrixOff(void)
{
  matrixOff(matrixA, ABuffer);
  matrixOff(matrixB, BBuffer);
  matrixOff(matrixC, CBuffer);
  matrixOff(matrixD, DBuffer);
  matrixOff(matrixE, EBuffer);
}

void matrixOff(Adafruit_LEDBackpack matrix, uint16_t buffer[8])
{
  for (uint8_t i=0; i<8; i++) {
    buffer[i] = 0;
  }
  dispMat(matrix, buffer);
}

void ledOn (Adafruit_LEDBackpack matrix, uint16_t buffer[8], uint8_t cat, uint8_t an)
{
  buffer[cat] |= _BV(an);
  dispMat(matrix, buffer);
}

void ledOff (Adafruit_LEDBackpack matrix, uint16_t buffer[8], uint8_t cat, uint8_t an)
{
  if (buffer[cat] & _BV(an)) {
    buffer[cat] ^= _BV(an);
  }
  dispMat(matrix, buffer);
}

void dispMat (Adafruit_LEDBackpack matrix, uint16_t buffer[8])
{
  for (uint8_t i=0; i<8; i++) {
    matrix.displaybuffer[i] = buffer[i];
  }
  matrix.writeDisplay();
}

void lampTestOn (void)
{
  for (uint8_t cathode=0; cathode<6; cathode++) {
    for (uint8_t anode=0; anode<6; anode++) {
      ledOn(matrixE, EBuffer, statCat[cathode], statAn[anode]);
    }
  }
  ledOn(matrixE, EBuffer, 2, 7);
  ledOn(matrixE, EBuffer, 4, 4);
}

void lampTestOff (void)
{
  for (uint8_t cathode=0; cathode<6; cathode++) {
    for (uint8_t anode=0; anode<6; anode++) {
      ledOff(matrixE, EBuffer, statCat[cathode], statAn[anode]);
    }
  }
  ledOff(matrixE, EBuffer, 2, 7);
  ledOff(matrixE, EBuffer, 4, 4);
}

void threeDigitDisp (Adafruit_LEDBackpack matrix, uint16_t buffer[8], uint8_t cats[], uint8_t ans[], uint16_t number)
{
  uint8_t ones, tens, hundreds;
  hundreds = number/100;
  number = number-hundreds*100;
  tens = number/10;
  ones = number-tens*10;
  digitDisp(matrix, buffer, 1, cats, ans, hundreds);
  digitDisp(matrix, buffer, 2, cats, ans, tens);
  digitDisp(matrix, buffer, 3, cats, ans, ones);
}

void digitDisp(Adafruit_LEDBackpack matrix, uint16_t buffer[8], uint8_t pos, uint8_t cat[], uint8_t an[], uint8_t num)
{
  switch (num) {
    case 11: // all off
    ledOff(matrix, buffer, cat[(0+pos)], an[0]); // A
    ledOff(matrix, buffer, cat[(0+pos)], an[1]); // B
    ledOff(matrix, buffer, cat[(0+pos)], an[2]); // C
    ledOff(matrix, buffer, cat[(0+pos)], an[3]); // D
    ledOff(matrix, buffer, cat[(0+pos)], an[4]); // E
    ledOff(matrix, buffer, cat[(0+pos)], an[5]); // F
    ledOff(matrix, buffer, cat[(0+pos)], an[6]); // G
    ledOff(matrix, buffer, cat[(0+pos)], an[7]); // Decimal Point
    break;

    case 10: // Decimal Point
    ledOff(matrix, buffer, cat[(0+pos)], an[0]); // A
    ledOff(matrix, buffer, cat[(0+pos)], an[1]); // B
    ledOff(matrix, buffer, cat[(0+pos)], an[2]); // C
    ledOff(matrix, buffer, cat[(0+pos)], an[3]); // D
    ledOff(matrix, buffer, cat[(0+pos)], an[4]); // E
    ledOff(matrix, buffer, cat[(0+pos)], an[5]); // F
    ledOff(matrix, buffer, cat[(0+pos)], an[6]); // G
    ledOn(matrix, buffer, cat[(0+pos)], an[7]); // Decimal Point
    break;

    case 0:
    ledOn(matrix, buffer, cat[(0+pos)], an[0]); // A
    ledOn(matrix, buffer, cat[(0+pos)], an[1]); // B
    ledOn(matrix, buffer, cat[(0+pos)], an[2]); // C
    ledOn(matrix, buffer, cat[(0+pos)], an[3]); // D
    ledOn(matrix, buffer, cat[(0+pos)], an[4]); // E
    ledOn(matrix, buffer, cat[(0+pos)], an[5]); // F
    ledOff(matrix, buffer, cat[(0+pos)], an[6]); // G
    //ledOff(matrix, buffer, cat[(0+pos)], an[7]); // Decimal Point
    break;

    case 1:
    ledOff(matrix, buffer, cat[(0+pos)], an[0]); // A
    ledOn(matrix, buffer, cat[(0+pos)], an[1]); // B
    ledOn(matrix, buffer, cat[(0+pos)], an[2]); // C
    ledOff(matrix, buffer, cat[(0+pos)], an[3]); // D
    ledOff(matrix, buffer, cat[(0+pos)], an[4]); // E
    ledOff(matrix, buffer, cat[(0+pos)], an[5]); // F
    ledOff(matrix, buffer, cat[(0+pos)], an[6]); // G
    //ledOff(matrix, buffer, cat[(0+pos)], an[7]); // Decimal Point
    break;
    
    case 2:
    ledOn(matrix, buffer, cat[(0+pos)], an[0]); // A
    ledOn(matrix, buffer, cat[(0+pos)], an[1]); // B
    ledOff(matrix, buffer, cat[(0+pos)], an[2]); // C
    ledOn(matrix, buffer, cat[(0+pos)], an[3]); // D
    ledOn(matrix, buffer, cat[(0+pos)], an[4]); // E
    ledOff(matrix, buffer, cat[(0+pos)], an[5]); // F
    ledOn(matrix, buffer, cat[(0+pos)], an[6]); // G
    //ledOff(matrix, buffer, cat[(0+pos)], an[7]); // Decimal Point
    break;
    
    case 3:
    ledOn(matrix, buffer, cat[(0+pos)], an[0]); // A
    ledOn(matrix, buffer, cat[(0+pos)], an[1]); // B
    ledOn(matrix, buffer, cat[(0+pos)], an[2]); // C
    ledOn(matrix, buffer, cat[(0+pos)], an[3]); // D
    ledOff(matrix, buffer, cat[(0+pos)], an[4]); // E
    ledOff(matrix, buffer, cat[(0+pos)], an[5]); // F
    ledOn(matrix, buffer, cat[(0+pos)], an[6]); // G
    //ledOff(matrix, buffer, cat[(0+pos)], an[7]); // Decimal Point
    break;
    
    case 4:
    ledOff(matrix, buffer, cat[(0+pos)], an[0]); // A
    ledOn(matrix, buffer, cat[(0+pos)], an[1]); // B
    ledOn(matrix, buffer, cat[(0+pos)], an[2]); // C
    ledOff(matrix, buffer, cat[(0+pos)], an[3]); // D
    ledOff(matrix, buffer, cat[(0+pos)], an[4]); // E
    ledOn(matrix, buffer, cat[(0+pos)], an[5]); // F
    ledOn(matrix, buffer, cat[(0+pos)], an[6]); // G
    //ledOff(matrix, buffer, cat[(0+pos)], an[7]); // Decimal Point
    break;
    
    case 5:
    ledOn(matrix, buffer, cat[(0+pos)], an[0]); // A
    ledOff(matrix, buffer, cat[(0+pos)], an[1]); // B
    ledOn(matrix, buffer, cat[(0+pos)], an[2]); // C
    ledOn(matrix, buffer, cat[(0+pos)], an[3]); // D
    ledOff(matrix, buffer, cat[(0+pos)], an[4]); // E
    ledOn(matrix, buffer, cat[(0+pos)], an[5]); // F
    ledOn(matrix, buffer, cat[(0+pos)], an[6]); // G
    //ledOff(matrix, buffer, cat[(0+pos)], an[7]); // Decimal Point
    break;
    
    case 6:
    ledOn(matrix, buffer, cat[(0+pos)], an[0]); // A
    ledOff(matrix, buffer, cat[(0+pos)], an[1]); // B
    ledOn(matrix, buffer, cat[(0+pos)], an[2]); // C
    ledOn(matrix, buffer, cat[(0+pos)], an[3]); // D
    ledOn(matrix, buffer, cat[(0+pos)], an[4]); // E
    ledOn(matrix, buffer, cat[(0+pos)], an[5]); // F
    ledOn(matrix, buffer, cat[(0+pos)], an[6]); // G
    //ledOff(matrix, buffer, cat[(0+pos)], an[7]); // Decimal Point
    break;
    
    case 7:
    ledOn(matrix, buffer, cat[(0+pos)], an[0]); // A
    ledOn(matrix, buffer, cat[(0+pos)], an[1]); // B
    ledOn(matrix, buffer, cat[(0+pos)], an[2]); // C
    ledOff(matrix, buffer, cat[(0+pos)], an[3]); // D
    ledOff(matrix, buffer, cat[(0+pos)], an[4]); // E
    ledOff(matrix, buffer, cat[(0+pos)], an[5]); // F
    ledOff(matrix, buffer, cat[(0+pos)], an[6]); // G
    //ledOff(matrix, buffer, cat[(0+pos)], an[7]); // Decimal Point
    break;
    
    case 8:
    ledOn(matrix, buffer, cat[(0+pos)], an[0]); // A
    ledOn(matrix, buffer, cat[(0+pos)], an[1]); // B
    ledOn(matrix, buffer, cat[(0+pos)], an[2]); // C
    ledOn(matrix, buffer, cat[(0+pos)], an[3]); // D
    ledOn(matrix, buffer, cat[(0+pos)], an[4]); // E
    ledOn(matrix, buffer, cat[(0+pos)], an[5]); // F
    ledOn(matrix, buffer, cat[(0+pos)], an[6]); // G
    //ledOff(matrix, buffer, cat[(0+pos)], an[7]); // Decimal Point
    break;
    
    case 9:
    ledOn(matrix, buffer, cat[(0+pos)], an[0]); // A
    ledOn(matrix, buffer, cat[(0+pos)], an[1]); // B
    ledOn(matrix, buffer, cat[(0+pos)], an[2]); // C
    ledOn(matrix, buffer, cat[(0+pos)], an[3]); // D
    ledOff(matrix, buffer, cat[(0+pos)], an[4]); // E
    ledOn(matrix, buffer, cat[(0+pos)], an[5]); // F
    ledOn(matrix, buffer, cat[(0+pos)], an[6]); // G
    //ledOff(matrix, buffer, cat[(0+pos)], an[7]); // Decimal Point
    break;
    
  } // end switch(num)
} // end function digitDisp()

void expanderWriteBothPort(const byte reg, const byte data, const byte theport)
{
  Wire.beginTransmission (theport);
  Wire.write (reg);
  Wire.write (data);  // port A
  Wire.write (data);  // port B
  Wire.endTransmission ();
} // end of expanderWriteBothPort

// read a byte from the expander
unsigned int expanderReadPort (const byte reg, int theport) 
{
  Wire.beginTransmission (theport);
  Wire.write (reg);
  Wire.endTransmission ();
  Wire.requestFrom (theport, 1);
  return Wire.read();
} // end of expanderRead

void playSound (uint8_t s)
{
  Serial.write(s);
}  


void scanButtons(void) {
  byte current[64];
  uint16_t value = 0;
  value |= expanderReadPort(GPIOA, expA) << 8;
  value |= expanderReadPort(GPIOB, expA);
  for (uint8_t i=0; i<16; i++) {
    if (value & (1 << i)) {
      current[i] = 1;
    } else {
      current[i] = 0;
    }
  }
  value = 0;
  value |= expanderReadPort(GPIOA, expB) << 8;
  value |= expanderReadPort(GPIOB, expB);
  for (uint8_t i=0; i<16; i++) {
    if (value & (1 << i)) {
      current[i+16] = 1;
    } else {
      current[i+16] = 0;
    }
  }
  value = 0;
  value |= expanderReadPort(GPIOA, expC) << 8;
  value |= expanderReadPort(GPIOB, expC);
  for (uint8_t i=0; i<16; i++) {
    if (value & (1 << i)) {
      current[i+32] = 1;
    } else {
      current[i+32] = 0;
    }
  }
  value = 0;
  value |= expanderReadPort(GPIOA, expD) << 8;
  value |= expanderReadPort(GPIOB, expD);
  for (uint8_t i=0; i<16; i++) {
    if (value & (1 << i)) {
      current[i+48] = 1;
    } else {
      current[i+48] = 0;
    }
  }
  for (uint8_t i=0; i<64; i++) {
    if ((current[i] == 0) && (state[i] == 1)) { // Switch was closed, now open
      Serial.write(i+128);
      state[i] = current[i];
      if (i==17) lampTestOff();
    } else if ((current[i] == 1) && (state[i] == 0)) { // Switch was open, now closed
      Serial.write(i);
      state[i] = current[i];
      if (i==17) lampTestOn();
    }
  }  
}

void switchBars (Adafruit_LEDBackpack matrix, uint16_t buffer[8], uint8_t grphCats[], uint8_t grphAns[], uint8_t cmnd[], uint8_t offset)
{
  for (uint8_t i=0; i<24; i++)
  {
    if (cmnd[i] == 1) {
      ledOn(matrix, buffer, grphCats[i], (grphAns[i] + offset));
    } else {
      ledOff(matrix, buffer, grphCats[i], (grphAns[i] + offset));
    }
  }
}

void barDisp (Adafruit_LEDBackpack matrix, uint16_t buffer[8], uint8_t grphCats[], uint8_t grphAns[], uint8_t pos, uint8_t offset)
{
  //uint8_t cmnd[24];
  switch (pos) {
    case 12:    
    switchBars(matrix, buffer, grphCats, grphAns, gtwelve, offset);
    break;
    
    case 11:
    switchBars(matrix, buffer, grphCats, grphAns, geleven, offset);
    break;

    case 10:
    switchBars(matrix, buffer, grphCats, grphAns, gten, offset);
    break;

    case 9:
    switchBars(matrix, buffer, grphCats, grphAns, gnine, offset);
    break;

    case 8:
    switchBars(matrix, buffer, grphCats, grphAns, geight, offset);
    break;

    case 7:
    switchBars(matrix, buffer, grphCats, grphAns, gseven, offset);
    break;

    case 6:
    switchBars(matrix, buffer, grphCats, grphAns, gsix, offset);
    break;

    case 5:
    switchBars(matrix, buffer, grphCats, grphAns, gfive, offset);
    break;

    case 4:
    switchBars(matrix, buffer, grphCats, grphAns, gfour, offset);
    break;

    case 3:
    switchBars(matrix, buffer, grphCats, grphAns, gthree, offset);
    break;

    case 2:
    switchBars(matrix, buffer, grphCats, grphAns, gtwo, offset);
    break;

    case 1:
    switchBars(matrix, buffer, grphCats, grphAns, gone, offset);
    break;
  }
}  

void cBarDisp (Adafruit_LEDBackpack matrix, uint16_t buffer[8], uint8_t grphCats[], uint8_t grphAns[], uint8_t pos, uint8_t offset)
{
  //uint8_t cmnd[24];
  switch (pos) {
    case 12:    
    switchBars(matrix, buffer, grphCats, grphAns, ctwelve, offset);
    break;
    
    case 11:
    switchBars(matrix, buffer, grphCats, grphAns, celeven, offset);
    break;

    case 10:
    switchBars(matrix, buffer, grphCats, grphAns, cten, offset);
    break;

    case 9:
    switchBars(matrix, buffer, grphCats, grphAns, cnine, offset);
    break;

    case 8:
    switchBars(matrix, buffer, grphCats, grphAns, ceight, offset);
    break;

    case 7:
    switchBars(matrix, buffer, grphCats, grphAns, cseven, offset);
    break;

    case 6:
    switchBars(matrix, buffer, grphCats, grphAns, csix, offset);
    break;

    case 5:
    switchBars(matrix, buffer, grphCats, grphAns, cfive, offset);
    break;

    case 4:
    switchBars(matrix, buffer, grphCats, grphAns, cfour, offset);
    break;

    case 3:
    switchBars(matrix, buffer, grphCats, grphAns, cthree, offset);
    break;

    case 2:
    switchBars(matrix, buffer, grphCats, grphAns, ctwo, offset);
    break;

    case 1:
    switchBars(matrix, buffer, grphCats, grphAns, cone, offset);
    break;
  }
}  


void potToGraph (int reading, uint8_t pot)
{
    if (abs(reading - potstate[pot]) > 0) {
      potstate[pot] = reading;
      barDisp(matrixB, BBuffer, grphCats, grphAns, reading, pot);
  }
}

void updateClock (void)
{
  uint8_t ones, tens, hundreds, number;
  minutes++;
  if (minutes > 59) {
    minutes = 0;
    hours++;
    if (hours > 24) {
      hours = 0;
      days++;
      if (days > 999) {
        days = 0;
      }
      hundreds = days/100;
      number = days-hundreds*100;
      tens = days/10;
      ones = days-tens*10;
      digitDisp(matrixD, DBuffer, 0, MTdaysC, MTAn, hundreds);
      digitDisp(matrixD, DBuffer, 1, MTdaysC, MTAn, tens);
      digitDisp(matrixD, DBuffer, 2, MTdaysC, MTAn, ones);
    }
    //hundreds = hours/100;
    //number = hours-hundreds*100;
    tens = hours/10;
    ones = hours-tens*10;
    //digitDisp(matrixD, DBuffer, 1, MTdaysC, MTAn, hundreds);
    digitDisp(matrixD, DBuffer, 0, MThoursC, MTAn, tens);
    digitDisp(matrixD, DBuffer, 1, MThoursC, MTAn, ones);

  }
  tens = minutes/10;
  ones = minutes-tens*10;
  //digitDisp(matrixD, DBuffer, 1, MTdaysC, MTAn, hundreds);
  digitDisp(matrixD, DBuffer, 0, MTminutesC, MTAn, tens);
  digitDisp(matrixD, DBuffer, 1, MTminutesC, MTAn, ones);
}

void initClock (void)
{
  digitDisp(matrixD, DBuffer, 3, MTCat, MTAn, 10);
  digitDisp(matrixD, DBuffer, 5, MTCat, MTAn, 10);
  for (uint8_t i=0; i<8; i++) {
    digitDisp(matrixD, DBuffer, i, MTCat, MTAn, 0);
  }
}

void randomNumerics (void)
{
  switch (numericPos) {
    case 0:  
    threeDigitDisp(matrixD, DBuffer, yawCat, yawAn, random(0,360));
    numericPos = 1;
    break;
    
    case 1:
    threeDigitDisp(matrixD, DBuffer, pitchCat, pitchAn, random(0,360));
    numericPos = 2;
    break;
    
    case 2:
    threeDigitDisp(matrixA, ABuffer, rollCat, rollAn, random(0,360));
    numericPos = 3;
    break;
    
    case 3:
    threeDigitDisp(matrixA, ABuffer, ihrCat, ihrAn, random(0,220));
    numericPos = 4;
    break;
    
    case 4:
    threeDigitDisp(matrixA, ABuffer, abrCat, abrAn, random(0,100));
    numericPos = 5;
    break;
    
    case 5:
    threeDigitDisp(matrixA, ABuffer, ahrCat, ahrAn, random(0,220));
    numericPos = 0;
    break;
  }
}

void scanPots() 
{
  int reading;
  
  reading = map(analogRead(0), 0, 1010, 1, 12);
  potToGraph(reading, barVolt);
  reading = map(analogRead(1), 0, 1010, 1, 12);
  potToGraph(reading, barOhm);
  reading = map(analogRead(2), 0, 1010, 1, 12);
  potToGraph(reading, barCur);
  reading = map(analogRead(3), 0, 1010, 1, 12);
  potToGraph(reading, barOx);

} // end scanPots()

void setup ()
{
  Wire.begin ();  // to communicate with i2c
  Serial.begin (115200);  // for debugging via serial terminal on computer
  expanderWriteBothPort (IOCON, 0b01100000, expA); // mirror interrupts, disable sequential mode 
  expanderWriteBothPort (IOCON, 0b01100000, expB); // mirror interrupts, disable sequential mode 
  expanderWriteBothPort (IOCON, 0b01100000, expC);
  expanderWriteBothPort (IOCON, 0b01100000, expD);
  expanderWriteBothPort (GPPUA, 0xFF, expA);   // pull-up resistor for switch - both ports
  expanderWriteBothPort (GPPUA, 0xFF, expB);   // pull-up resistor for switch - both ports
  expanderWriteBothPort (GPPUA, 0xFF, expC);
  expanderWriteBothPort (GPPUA, 0xFF, expD);
  expanderWriteBothPort (IOPOLA, 0xFF, expA);  // invert polarity of signal - both ports
  expanderWriteBothPort (IOPOLA, 0xFF, expB);  // invert polarity of signal - both ports
  expanderWriteBothPort (IOPOLA, 0xFF, expC);
  expanderWriteBothPort (IOPOLA, 0xFF, expD);
//  expanderWriteBothPort (GPINTENA, 0xFF, expA); // enable interrupts - both ports
//  expanderWriteBothPort (GPINTENA, 0xFF, expB); // enable interrupts - both ports
//  expanderWriteBothPort (GPINTENA, 0xFF, expC); // enable interrupts - both ports
//  expanderWriteBothPort (GPINTENA, 0xFF, expD); // enable interrupts - both ports
//  expanderWriteBothPort (INTCONA, 0x00, expA);
//  expanderWriteBothPort (INTCONA, 0x00, expB);
//  expanderWriteBothPort (INTCONA, 0x00, expC);
//  expanderWriteBothPort (INTCONA, 0x00, expD);  
  matrixA.begin(matAaddy);  // pass in the address of the HT16K33
  matrixB.begin(matBaddy);  // pass in the address of the HT16K33
  matrixC.begin(matCaddy);  // pass in the address of the HT16K33
  matrixD.begin(matDaddy);  // pass in the address of the HT16K33
  matrixE.begin(matEaddy);  // pass in the address of the HT16K33

  matrixA.setBrightness(10);  
  matrixB.setBrightness(10);  
  matrixC.setBrightness(10);  
  matrixD.setBrightness(10);  
  matrixE.setBrightness(10);  


  allMatrixOff();

  cBarDisp(matrixC, CBuffer, cryoCats, cryoAns, 10,0);
  cBarDisp(matrixC, CBuffer, cryoCats, cryoAns, 12,3);
  cBarDisp(matrixC, CBuffer, cryoCats, cryoAns, 10,6);
  cBarDisp(matrixB, BBuffer, cryoCats, cryoAns, 12,12);
  initClock();
  updateClock();
  clockTimer = millis();
  //lampTestOn();
  ledOn(matrixE, EBuffer, 4, 4);

  //Serial.println("Setup Complete");
}  // end of setup


void loop() {
//  scanButtons();
  if (NUMERICREFRESH < (millis() - numericTimer)) {
    numericTimer = millis();
    randomNumerics();
  }
//  scanButtons();
  if (60000 < (millis() - clockTimer)) {
    clockTimer = millis();
    updateClock();
  }
//  scanButtons();
  scanPots();
  scanButtons();
  processSerial();
}
