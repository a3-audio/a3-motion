/*

  A3 Motion Firmware

  Copyright (C) 2021 Jendrik Bradaczek
  Copyright (C) 2022-2023 Patric Schmitz

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <chrono>

#include <Arduino.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>

// tap button is treated separately via interrupt, see below
int constexpr numButtons = 2;
int pinsButtonPress [numButtons] = {41, 35};
int constexpr numButtonLEDs = 3;
int pinsButtonLED [numButtonLEDs] = {36, 39, 13};
bool buttonsNew [numButtons] = {false, false};
bool buttonsOld [numButtons] = {false, false};

// tap button
auto constexpr pinTap = 38;
auto constexpr debounceMicros = 10000;
volatile uint64_t timeTapMicros;
volatile uint64_t timeLastChangeMicros;
volatile bool firstEdgeDetected = false;
auto numTapPress = 0;
auto numTapRelease = 0;
void isr_tap();

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
int constexpr pinNeoPixels = 9;
int constexpr numPixels = 16;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, pinNeoPixels, NEO_GRB + NEO_KHZ800);

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

uint64_t timeStartMicros;

void initButtons()
{
  for(auto button = 0u; button < numButtons; ++button) {
    pinMode(pinsButtonPress[button], INPUT_PULLUP);
  }
  for(auto led = 0u; led < numButtonLEDs; ++led) {
    pinMode(pinsButtonLED[led], OUTPUT);
  }

  pinMode(pinTap, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinTap), isr_tap, CHANGE);
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

void isr_tap()
{
  auto const time = std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::high_resolution_clock::now().time_since_epoch()).count();

  if(!firstEdgeDetected)
  {
    timeTapMicros = time;
  }

  firstEdgeDetected = true;
  timeLastChangeMicros = time;
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

void sendTapButton()
{
  uint64_t timeMicros = std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  uint64_t timeTapMicrosLocal = 0;

  enum class SendEvent {
    Press,
    Release,
    None
  } sendEvent = SendEvent::None;

  cli();
  if(firstEdgeDetected &&
    timeMicros - timeLastChangeMicros > debounceMicros)
  {
    firstEdgeDetected = false;

    if(digitalRead(pinTap) == LOW) {
      ++numTapPress;
      sendEvent = SendEvent::Press;
      timeTapMicrosLocal = timeTapMicros;
    }
    else if(numTapPress > numTapRelease) {
      // we only count releases if a press was detected beforehand,
      // otherwise very short press/release events will only be seen
      // as a release.
      ++numTapRelease;
      sendEvent = SendEvent::Release;
    }
  }
  sei();

  switch(sendEvent)
  {
  case SendEvent::Press:
    // Serial.print(numTapPress);
    // Serial.print(" : ");
    // Serial.println(numTapRelease);
    Serial.print("B18:1:");
    Serial.println(timeTapMicrosLocal - timeStartMicros);
    break;
  case SendEvent::Release:
    Serial.println("B18:0");
    break;
  case SendEvent::None:
    break;
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

  sendTapButton();
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

  // while (!Serial); // wait for serial connection

  initButtons();

  pinMode(muxInPot, INPUT_PULLDOWN);
  pinMode(muxInBtnMx1, INPUT_PULLDOWN);
  pinMode(muxInBtnMx2, INPUT_PULLDOWN);

  initBtnMatrix();
  initBtnEncoder();
  initPoti();
  initLEDs();

  strip.begin();
  for(int pixel = 0; pixel < numPixels ; ++pixel) {
    strip.setPixelColor(1, strip.Color(0, 0, 0));
  }
  strip.show();

  timeStartMicros = std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::high_resolution_clock::now().time_since_epoch()).count();
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
