/*------------------------------------------------------------------------------
  The MIT License (MIT)

  Copyright (c) 2013 Nick Iaconis et al.
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software, schematic, layout and associated documentaion (the
  "System"), to deal in the System without restriction, including without
  limitation the rights to use, copy, modify, merge, publish, distribute,
  sublicense, and/or sell copies of the System, and to permit persons to whom
  the System is furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the System.
  
  THE SYSTEM IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SYSTEM OR THE USE OR OTHER DEALINGS IN THE
  SYSTEM.
------------------------------------------------------------------------------*/

/*------------------
  #### ATtiny84 ####
  ------------------
         ____       
  VCC  <|*   |> GND 
  IO10 <|    |> IO 0
  IO 9 <|    |> IO 1
  RST  <|    |> IO 2
  IO 8 <|    |> IO 3
  IO 7 <|    |> SCK 
  MOSI <|____|> MISO

  ------------------*/

/*----------------------------
  #### Swap Motor Drivers ####
  ----------------------------
  
  Regex:   ^( *)(.*tb6612fng)
  Replace: $1//$2
  
  Regex:   ^( *)//(.*drv8833)
  Replace: $1$2
  
  ----------------------------*/

#include <SoftwareSerial.h>
#define uSecCyc 870

// Motor control A
//#define mcPin1A 1 // tb6612fng
//#define mcPin2A 2 // tb6612fng
//#define pwmPinA 3 // tb6612fng
#define mcPin1A 10 // drv8833
#define mcPin2A 9 // drv8833
char pwmPinA = mcPin1A; // drv8833
#define maxA 28
char levelA = 0;
int pwmA = 0;

// Motor control B
//#define mcPin1B 0 // tb6612fng
//#define mcPin2B 8 // tb6612fng
//#define pwmPinB 7 // tb6612fng
#define mcPin1B 7 // drv8833
#define mcPin2B 8 // drv8833
char pwmPinB = mcPin1B; // drv8833
#define maxB 28
char levelB = 0;
int pwmB = 0;

// Channel A commands
#define maskA 83
#define upA 65    // A up         = A
#define downA 66  // A down       = B
#define brakeA 67 // A brake      = C
#define floatA 80 // A float      = P
#define ffwdA 81  // A full fwd   = Q
#define frevA 82  // A full rev   = R

// Channel B commands
#define maskB 108
#define upB 68    // B up         = D
#define downB 72  // B down       = H
#define brakeB 76 // B brake      = L
#define floatB 96 // B float      = `
#define ffwdB 100 // B full fwd   = d
#define frevB 104 // B full rev   = h

// Combine Channel A command with Channel B command using logical OR

// Software serial
//#define rxPin 10 // tb6612fng
//#define txPin 9 // tb6612fng
#define rxPin 0 // drv8833
#define txPin 1 // drv8833
#define baudRate 9600
SoftwareSerial softSerial(rxPin, txPin);

// Miscellanious
#define btResetPin 3
char incomingByte = 0;
char maskedByte = 0;
char serialRateLimiter = 0;
// int pwmLevels[8] = {0, 218, 326, 435, 544, 653, 761, 870};
int pwmLevels[29] = {0, 141, 168, 195, 222, 249, 276, 303, \
                      330, 357, 384, 411, 438, 465, 492, \
                      519, 546, 573, 600, 627, 654, 681, \
                      708, 735, 762, 789, 816, 843, 870};

// Function definitions
void comboMode(const char, char*, const char, const char, \
                const char, const char, const char, const char, \
                const char, const char, const char, const char);
void bitBangPwm(char, int, char, int);



/*----------------------------------------------------------------------------*/



void setup()
{
  // bluetooth control init
  pinMode(btResetPin, OUTPUT);
  // digitalWrite(btResetPin, LOW);  // reset Bluetooth module
  // delay(3); // verified minimum   // hold long enough to register reset
  // digitalWrite(btResetPin, HIGH); // end Bluetooth reset
  
  // channel A init
  pinMode(mcPin1A, OUTPUT);
  pinMode(mcPin2A, OUTPUT);
  pinMode(pwmPinA, OUTPUT);
  digitalWrite(mcPin1A, LOW);
  digitalWrite(mcPin2A, LOW);
  digitalWrite(pwmPinA, LOW);
  
  // channel B init
  pinMode(mcPin1B, OUTPUT);
  pinMode(mcPin2B, OUTPUT);
  pinMode(pwmPinB, OUTPUT);
  digitalWrite(mcPin1B, LOW);
  digitalWrite(mcPin2B, LOW);
  digitalWrite(pwmPinB, LOW);
  
  // bitbanged PWM init
  pwmA = pwmLevels[min(abs(levelA), maxA)];
  pwmB = pwmLevels[min(abs(levelB), maxB)];
  
  // serial init
  softSerial.begin(baudRate);
}



void loop()
{
  // Handle serial communication
  //   - software serial interferes with PWM
  //   - limit to once every 8(?) loops
  if (!serialRateLimiter && softSerial.available() > 0)
  {
    // read incoming byte
    incomingByte = softSerial.read();
    

    // Direct PWM mode (fine resolution)
    if (incomingByte & 128) // 1xxx xxxx
    {
      // select proper output
      char* levelX = incomingByte & 64 ? &levelB : &levelA;
      if ((incomingByte & 63) < 29 || (incomingByte & 63) > 35)
      {
        // set the level
        *levelX = (incomingByte & 31) | (incomingByte & 32 ? 224 : 0);
        // set the motor control wires
        digitalWrite(incomingByte & 64 ? mcPin1B : mcPin1A, (*levelX > 0 ? HIGH : LOW));
        digitalWrite(incomingByte & 64 ? mcPin2B : mcPin2A, (*levelX < 0 ? HIGH : LOW));
      }
      else if ((incomingByte & 63) == 32) // xx10 0000 -> brake
      {
        // set the level
        *levelX = 0;
        // set the motor control wires
        digitalWrite(incomingByte & 64 ? mcPin1B : mcPin1A, HIGH);
        digitalWrite(incomingByte & 64 ? mcPin2B : mcPin2A, HIGH);
      }
      // notify sender
      softSerial.write(*levelX);
    }

    // Combo output mode
    else if (incomingByte & 64) // 01xx xxxx
    {
      // set channel A
      comboMode(incomingByte, &levelA, maxA, maskA, brakeA, floatA, \
                  upA, downA, ffwdA, frevA, mcPin1A, mcPin2A);
      // set channel B
      comboMode(incomingByte, &levelB, maxB, maskB, brakeB, floatB, \
                  upB, downB, ffwdB, frevB, mcPin1B, mcPin2B);
      // notify sender
      maskedByte = abs(levelB >> 2) + (levelB < 0 ? 8 : 0);
      maskedByte = (maskedByte << 4) + abs(levelA >> 2) + (levelA < 0 ? 8 : 0);
      softSerial.write(maskedByte);
    }
    
    // set the pwm
    pwmA = pwmLevels[min(abs(levelA), maxA)];
    pwmPinA = levelA < 0 ? mcPin2A : mcPin1A; // drv8833
    pwmB = pwmLevels[min(abs(levelB), maxB)];
    pwmPinB = levelB < 0 ? mcPin2B : mcPin1B; // drv8833
    
    // rate limit
    serialRateLimiter = 8;
  }
  
  // Bitbang the PWM signals
  bitBangPwm(pwmPinA, pwmA, pwmPinB, pwmB);
  
  // Decrement rate limiter
  if (serialRateLimiter)
    --serialRateLimiter;
  
}



/*----------------------------------------------------------------------------*/




void comboMode(const char incomingByte, char* levelX, const char maxX, \
                const char maskX, const char brakeX, const char floatX, \
                const char upX, const char downX, const char ffwdX, \
                const char frevX, const char mcPin1X, const char mcPin2X)
{
  maskedByte = incomingByte & maskX;
  // decode command
  if (maskedByte == brakeX || maskedByte == floatX)
  {
    *levelX = 0;
  }
  else if (maskedByte == upX)
  {
    if (*levelX >= 0)
    {
      *levelX = min((*levelX + 4) & ~3, maxX);
    }
    else
    {
      if (abs(*levelX) & 3)
        *levelX = -(abs(*levelX) & ~3);
      else
        *levelX += 4;
    }
  }
  else if (maskedByte == downX)
  {
    if (*levelX <= 0)
    {
      *levelX = -min((abs(*levelX) + 4) & ~3, maxX);
    }
    else
    {
      if (*levelX & 3)
        *levelX &= ~3;
      else
        *levelX -= 4;
    }
  }
  else if (maskedByte == ffwdX)
  {
    *levelX = maxX;
  }
  else if (maskedByte == frevX)
  {
    *levelX = -maxX;
  }

  // set the motor control wires
  digitalWrite(mcPin1X, (*levelX > 0 || maskedByte == brakeX ? HIGH : LOW));
  digitalWrite(mcPin2X, (*levelX < 0 || maskedByte == brakeX ? HIGH : LOW));
}



void bitBangPwm(char thePwmPinA, int pwmA, char thePwmPinB, int pwmB)
{
  // Bitbang the PWM signals
  // turn on both channels
  if (pwmA > 0)
    digitalWrite(thePwmPinA, HIGH);
  if (pwmB > 0)
    digitalWrite(thePwmPinB, HIGH);
  if (pwmA <= pwmB)
  {
    // pwmA is shorter
    if (pwmA > 0)
    {
      delayMicroseconds(pwmA);
      // channelA off
      digitalWrite(thePwmPinA, LOW);
    }
    // wait the difference
    if (pwmA != pwmB)
    {
      delayMicroseconds(pwmB - pwmA);
    }
    if (pwmB > 0)
    {
      // channelB off
      digitalWrite(thePwmPinB, LOW);
    }
    // finish out the cycle
    if (pwmB < uSecCyc)
      delayMicroseconds(uSecCyc - pwmB);
  }
  else
  {
    // pwmB is shorter
    if (pwmB > 0)
    {
      delayMicroseconds(pwmB);
      // channelB off
      digitalWrite(thePwmPinB, LOW);
    }
    // wait the difference
    if (pwmA != pwmB)
    {
      delayMicroseconds(pwmA - pwmB);
    }
    if (pwmA > 0)
    {
      // channelA off
      digitalWrite(thePwmPinA, LOW);
    }
    // finish out the cycle
    if (pwmA < uSecCyc)
      delayMicroseconds(uSecCyc - pwmA);
  }
}
