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

#include <SoftwareSerial.h>
#define uSecCyc 870

// Motor control A
#define mcPin1A 1
#define mcPin2A 2
#define pwmPinA 3
#define maxA 7
int levelA = 0;
int pwmA = 0;

// Motor control B
#define mcPin1B 0
#define mcPin2B 8
#define pwmPinB 7
#define maxB 7
int levelB = 0;
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
#define rxPin 10
#define txPin 9
#define baudRate 9600
SoftwareSerial softSerial(rxPin, txPin);

// Miscellanious
char incomingByte = 0;
char maskedByte = 0;
char serialRateLimiter = 0;
int pwmMin = 0;
int pwmMax = 0;
int pwmLevels[8] = {0, 218, 326, 435, 544, 653, 761, 870};

void setup()
{
  // serial init
  softSerial.begin(baudRate);
  
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
    
    // channel A
    maskedByte = incomingByte & maskA;
    // decode command
    if (maskedByte == brakeA || maskedByte == floatA)
    {
      levelA = 0;
    }
    else if (maskedByte == upA)
    {
      levelA = min(levelA + 1, maxA);
    }
    else if (maskedByte == downA)
    {
      levelA = max(levelA - 1, -maxA);
    }
    else if (maskedByte == ffwdA)
    {
      levelA = maxA;
    }
    else if (maskedByte == frevA)
    {
      levelA = -maxA;
    }
    // set outputs
    digitalWrite(mcPin1A, (levelA > 0 || maskedByte == brakeA ? HIGH : LOW));
    digitalWrite(mcPin2A, (levelA < 0 || maskedByte == brakeA ? HIGH : LOW));
    pwmA = pwmLevels[min(abs(levelA), maxA)];
    
    // channel B
    maskedByte = incomingByte & maskB;
    // decode command
    if (maskedByte == brakeB || maskedByte == floatB)
    {
      levelB = 0;
    }
    else if (maskedByte == upB)
    {
      levelB = min(levelB + 1, maxB);
    }
    else if (maskedByte == downB)
    {
      levelB = max(levelB - 1, -maxB);
    }
    else if (maskedByte == ffwdB)
    {
      levelB = maxB;
    }
    else if (maskedByte == frevB)
    {
      levelB = -maxB;
    }
    // set outputs
    digitalWrite(mcPin1B, (levelB > 0 || maskedByte == brakeB ? HIGH : LOW));
    digitalWrite(mcPin2B, (levelB < 0 || maskedByte == brakeB ? HIGH : LOW));
    pwmB = pwmLevels[min(abs(levelB), maxB)];
    
    // notify sender
    maskedByte = abs(levelB) + (levelB < 0 ? 8 : 0);
    maskedByte = (maskedByte << 4) + abs(levelA) + (levelA < 0 ? 8 : 0);
    softSerial.print(maskedByte, DEC);
    
    // rate limit
    serialRateLimiter = 8;
  }
  
  // Bitbang the PWM signals
  // turn on both channels
  if (pwmA > 0)
    digitalWrite(pwmPinA, HIGH);
  if (pwmB > 0)
    digitalWrite(pwmPinB, HIGH);
  if (pwmA <= pwmB)
  {
    // pwmA is shorter
    if (pwmA > 0)
      delayMicroseconds(pwmA);
    // channelA off
    digitalWrite(pwmPinA, LOW);
    // wait the difference
    if (pwmA != pwmB)
      delayMicroseconds(pwmB - pwmA);
    // channelB off
    digitalWrite(pwmPinB, LOW);
    // finish out the cycle
    if (pwmB < uSecCyc)
      delayMicroseconds(uSecCyc - pwmB);
  }
  else
  {
    // pwmB is shorter
    if (pwmB > 0)
      delayMicroseconds(pwmB);
    // channelB off
    digitalWrite(pwmPinB, LOW);
    // wait the difference
    if (pwmA != pwmB)
      delayMicroseconds(pwmA - pwmB);
    // channelA off
    digitalWrite(pwmPinA, LOW);
    // finish out the cycle
    if (pwmA < uSecCyc)
      delayMicroseconds(uSecCyc - pwmA);
  }
  
  // Decrement rate limiter
  if (serialRateLimiter)
    --serialRateLimiter;
  
}
