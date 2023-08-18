// This file is a part of A³Pandemic. License is GPLv3: https://github.com/ambisonics-audio-association/Ambijockey/blob/main/COPYING
// © Copyright 2021 Jendrik Bradaczek

/*

TODO: software-side button debouncing

Bounce pushbutton = Bounce(PIN_BUTTON_ENCODER, 10);
pinMode(PIN_BUTTON_ENCODER, INPUT_PULLUP);
  if (pushbutton.update())
  {
    if (pushbutton.fallingEdge())
    {
        // ...
    }
  }
*/

#include <Arduino.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>

int constexpr numButtons = 3;
int pinsButtonPress [numButtons] = {41, 35, 38};
int pinsButtonLED [numButtons] = {36, 39, 13};
bool buttonsNew [numButtons] = {false, false, false};
bool buttonsOld [numButtons] = {false, false, false};

// Multiplexer In/Out Pin's
#define muxInBtnMx1 20 //Butten Matrix
#define muxInBtnMx2 19 //Butten Matrix
#define muxInPot 18    //Poti in
#define muxInBtnEncoder 8 //Encoder Buttons in

// Multiplexer address Pin's (s0/s1/s2)
#define s0 23 // low-order bit
#define s1 22
#define s2 21 // high-order bit

int constexpr numEncoders = 4;
int pinsEncoderDT [numEncoders] = {0, 2, 4, 6};
int pinsEncoderCLK [numEncoders] = {1, 3, 5, 7};
Encoder encoders [numEncoders] = {
  Encoder(pinsEncoderDT[0], pinsEncoderCLK[0]),
  Encoder(pinsEncoderDT[1], pinsEncoderCLK[1]),
  Encoder(pinsEncoderDT[2], pinsEncoderCLK[2]),
  Encoder(pinsEncoderDT[3], pinsEncoderCLK[3])
};

// NeoPixel
#define npxl_pin 9
#define npxl_leds 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(npxl_leds, npxl_pin, NEO_GRB + NEO_KHZ800);

// multipexer adrress Bit
bool addrBit0 = 0;
bool addrBit1 = 0;
bool addrBit2 = 0;

// btnMatrx arry
bool btnMxNew[16];
bool btnMxOld[16];

// poti arry
int potiNew[8];
int potiOld[8];

// BtnEncoder
bool btnEncoderNew[8];
bool btnEncoderOld[8];

long posEnc0 = 0;
long posEnc1 = 0;
long posEnc2 = 0;
long posEnc3 = 0;

long newEnc0 = 0;
long newEnc1 = 0;
long newEnc2 = 0;
long newEnc3 = 0;

void initButtons()
{
  for(auto button = 0u; button < numButtons; ++button) {
    pinMode(pinsButtonPress[button], INPUT_PULLUP);
    pinMode(pinsButtonLED[button], OUTPUT);
  }
}

void initBtnMatrix()
{
  // btnMatrix init
  for (byte i = 0; i < 16; i++)
  {
    btnMxNew[i] = 0;
    btnMxOld[i] = 0;
  }
}

void initBtnEncoder()
{
  for (byte i = 0; i < 4; i++)
  {
    btnEncoderNew[i] = 0;
    btnEncoderOld[i] = 0;
  }
}

void initPoti()
{
  for (byte i = 0; i < 8; i++)
  {
    potiNew[i] = 0;
    potiOld[i] = 0;
  }
}

void initLEDs()
{
    strip.clear();
    strip.show();
}

void readButtons()
{
  for(auto button = 0u; button < numButtons; ++button)
    buttonsNew[button] = 1 - digitalRead(pinsButtonPress[button]);
}

void readMux()
{
  for (byte i = 0; i < 8; i++)
  {
    addrBit0 = bitRead(i, 0);
    addrBit1 = bitRead(i, 1);
    addrBit2 = bitRead(i, 2);
    digitalWrite(s0, addrBit0);
    digitalWrite(s1, addrBit1);
    digitalWrite(s2, addrBit2);
    delayMicroseconds(50); // Damit der 4051 zeit hat die Adresse um zu schalten

    // read Butten Matrix
    btnMxNew[i] = digitalRead(muxInBtnMx1);
    btnMxNew[i + 8] = digitalRead(muxInBtnMx2);

    // read Butten Encoder
    btnEncoderNew[i] = digitalRead(muxInBtnEncoder);

    // read the Poti's
    potiNew[i] = analogRead(muxInPot);
  }
}

void sendButtons()
{
  for(auto button = 0u; button < numButtons; ++button)
  {
    if(buttonsNew[button] != buttonsOld[button])
    {
      Serial.print("B");
      Serial.print(button+16);
      Serial.print(":");
      Serial.println(buttonsNew[button]);

      buttonsOld[button] = buttonsNew[button];
    }
  }
}

void sendBtnMx()
{
  for (byte i = 0; i < 16; i++)
  {
    if (btnMxNew[i] != btnMxOld[i])
    {
      Serial.print("B");
      Serial.print(i);
      Serial.print(":");
      Serial.println(btnMxNew[i]);
      btnMxOld[i] = btnMxNew[i];
    }
  }
}

void sendPoti()
{
  for (byte i = 0; i < 8; i++)
  {
    if (abs(potiNew[i] - potiOld[i]) > 2)
    {
      Serial.print("P");
      Serial.print(i);
      Serial.print(":");
      Serial.println(potiNew[i]);
      potiOld[i] = potiNew[i];
    }
  }
}


void sendBtnEncoder()
{
  for (byte i = 0; i < 4; i++)
  {
    if (btnEncoderNew[i] != btnEncoderOld[i])
    {
      Serial.print("EB");
      Serial.print(i);
      Serial.print(":");
      Serial.println(btnEncoderNew[i]);
      btnEncoderOld[i] = btnEncoderNew[i];
    }
  }
}

void sendEncoder()
{
  for(auto encoder = 0u ; encoder < numEncoders ; ++encoder) {
    auto increment = encoders[encoder].read();
    if(abs(increment) >= 4) {
      Serial.print("Enc");
      Serial.print(encoder);
      Serial.print(":");
      Serial.println(increment / 4);
      encoders[encoder].write(0);
    }
  }
}

void receivePixels(){
  if (Serial.available()) {
    String command = Serial.readStringUntil(',');
    if(command.startsWith("L")) {
      int pixNum  = Serial.readStringUntil(',').toInt();
      int red = Serial.readStringUntil(',').toInt();
      int green = Serial.readStringUntil(',').toInt();
      int blue  = Serial.readStringUntil('\n').toInt();

      strip.setPixelColor(pixNum, strip.Color(green, red, blue));
      strip.show(); // This sends the updated pixel color to the hardware.
    }
  }
}

void receiveButtonLEDS()
{
  if (Serial.available()) {
    String command = Serial.readStringUntil(',');
    if(command.startsWith("BL")) {
      int button  = Serial.readStringUntil(',').toInt();
      int on = Serial.readStringUntil('\n').toInt();
      digitalWrite(pinsButtonLED[button], on);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(10);

  while (!Serial)
  {
    ; // wait for serial conaction
  }

  Serial.println("#######################################");
  Serial.println("          controller connected");
  Serial.println("#######################################");

  initButtons();

  pinMode(muxInPot, INPUT_PULLDOWN);
  pinMode(muxInBtnMx1, INPUT_PULLDOWN);
  pinMode(muxInBtnMx2, INPUT_PULLDOWN);

  initBtnMatrix();
  initBtnEncoder();
  initPoti();
  initLEDs();

  strip.begin();
}

void loop()
{
  readButtons();
  readMux();

  sendButtons();
  sendBtnMx();
  sendBtnEncoder();
  sendEncoder();
  sendPoti();

  receiveButtonLEDS();
  receivePixels();
}
