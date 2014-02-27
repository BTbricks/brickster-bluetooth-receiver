# Bill of Materials

Listed here is a complete bill of materials for assembling one bluetooth receiver.

## Example listing

+ **Identifier** Device Package (ManufacturerPartNumber) _comment_

## 8884bt Bluetooth Receiver

+ **U1** ATtiny84 SOIC14 (ATTINY84-20SSUR)
+ **U2** MotorDriver SOP24 (TB6612FNG(O,C,8,EL)
+ **U3** Regulator3.3V SOT223 (RT9164A-33GG)
+ **U4** Transceiver ? (RN42-I/RM)

+ **B1/B2** BridgeRectifier MBS-2 (CDBHM240L-HF)
+ **B3/B4** BridgeDiode Micro-DIP (MDB8S)

+ **D1** Blue/OrangeLED 0605 (LTST-C195TBKFKT)
+ **D2** SmallSigDiode SOD80 (LL4148) _can be excluded if programming is not necessary_

+ **C1/C3/C4** CapCer0.1uF 0402 (CL05F104ZO5NNNC)
+ **C2/C5** CapTant10uF 1206 (F931D106MAA)

+ **R1** Res10kOhm 0402 (RMCF0402JT10K0)
+ **R2** Res?kOhm 0402 (?) _device value not yet known_

+ **J1** ProgrammerICSP N/A (N/A) _used to program the ATtiny84, use of pogo pins is advised_
+ **J2/J3/J4** WirePF N/A ([8886](http://shop.lego.com/en-US/LEGO-Power-Functions-Extension-Wire-8886 "Power Functions Extension Wire") _or_ [8871](http://shop.lego.com/en-US/LEGO-Power-Functions-Extension-Wire-20-8871 "Power Functions Extension Wire 20""))
