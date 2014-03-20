# Bill of Materials

Listed here is a complete bill of materials for assembling one bluetooth receiver.

## Example listing

+ **Identifier** Device Package (ManufacturerPartNumber) _comment_

## 8884bt Bluetooth Receiver

+ **U1** ATtiny84 SOIC14 (ATTINY84-20SSUR)
+ **U2** MotorDriver SOP24 (TB6612FNG(O,C,8,EL)
+ **U3** Regulator3.3V SOT223 (RT9164A-33GG)
+ **U4** BluetoothModule SMD35 (RN42-I/RM)

+ **Q1/Q2** MOSFET2N+2P SOIC8 (DMHC3025LSD-13) _populate Q1 or Q2 not both!, Q1 for PF 9V & GND, Q2 for PF C1 & C2_

+ **B3/B4** BridgeRectifier MBS-2 (CDBHM240L-HF)

+ **D1** Blue/OrangeLED 0605 (LTST-C195TBKFKT)

+ **C1/C3** CapCer0.1uF 0805 (CC0805ZRY5V9BB104)
+ **C2/C5** CapTant10uF 1206 (T491A106K006AT7280)

+ **R1** Res10kOhm 0805 (RMCF0805JT10K0)
+ **R2** Res3.3kOhm 0805 (RMCF0805JT3K30)

+ **J1** ProgrammerICSP N/A (N/A) _used to program the ATtiny84, use of pogo pins is advised, no external power!_
+ **J2/J3/J4** WirePF N/A ([8886](http://shop.lego.com/en-US/LEGO-Power-Functions-Extension-Wire-8886 "Power Functions Extension Wire") _or_ [8871](http://shop.lego.com/en-US/LEGO-Power-Functions-Extension-Wire-20-8871 "Power Functions Extension Wire 20"")) _square pads denote PF 9V channel_
