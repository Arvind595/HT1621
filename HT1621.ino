
/*
 * \file HT1621-test.ino
 * \brief Simple test sketch for the HT1621 class. 
 * \date 4 february 2020
 * \version 1.0
 */

#include "HT1621.h"
#define ms 700

HT1621 ht(13,12,8);// cs,wr,data
//const short  val[]={250,96,188,244,102,214,222,112,254,246,126,206,154,236,158,30,218,110,10,232,138,122,204,62,118,26,214,142,234,230};
//bin values for 0-9 and a-y char for 7segment all possible shapes 
	//const short val[10] = {250, 96, 188, 244, 102, 214, 222, 112, 254, 246};
	//const short val[10]={0xfa,0x60,0xbc,0xf4,0x66,0xd6,0xde,0x70,0xfe,0xf6}
	//val for Digits-7,6,9 segment 0-9 hex values , with lsb as special functions to use set lsb =1
	//const short vals5[10]={0x5f,0x50,0x3d,0x79,0x72,0x6f,0x6f,0x51,0x7f,0x73};
    //const short valMSB[10]={0xa0,0x00,0xc0,0x40,0x60,0x60,0xe0,0x00,0xe0,0x60};
	//const short valLSB[10]={0x0f,0x06,0x0b,0x0f,0x06,0x0d,0x0d,0x07,0x0f,0x07};
/*	int modifyBit(int n, int p, int b) 
{ 
    int mask = 1 << p; 
    return (n & ~mask) | ((b << p) & mask); 
} */
void setup() {

  register uint8_t i;
  
  ht.begin();
  
  ht.sendCommand(HT1621::RC256K);
  ht.sendCommand(HT1621::BIAS_THIRD_4_COM);
  ht.sendCommand(HT1621::SYS_EN);
  ht.sendCommand(HT1621::LCD_ON);

  // clear memory
  for(i=0; i<16; i++)
    ht.write(i,0);

  // write all zeroes 0
  for(i=0; i<16; i++)  ht.write(i, 0xBE);
	
	// clear memory
	ht.clearDisp();
	
	//avaliable to public:
	//uint8_t read(uint8_t address);
	
    //ht.inWrite( unsigned short segment, unsigned short number,char play, char pause);
	
	//ht.inWrite( unsigned short segment, unsigned short number);
	
	//ht.specialSeg(uint8_t binten,uint8_t binfiften);
	
	//ht.specialSeg(String s,char en);
	
	//ht.adWrite(unsigned short address, uint8_t value);
}

void loop() {
	
	//writing a number to segment/digit palce
	ht.inWrite(7,6);
	delay(ms);
	
	//to use special segement on digit 6 & 7(play and pause)
	ht.inWrite(7,5,'y','n');	
	delay(ms);
	
	// y to enable and n to null d to disable
	ht.inWrite(7,5,'d','n');
	delay(ms);
	
	ht.inWrite(7,5,'n','n');
	delay(ms);
	
	//to control segments of  address 10&15 which is text segments with 8-bit
	// segment 10 contains ACTION,SCI-FI,DRAMA,CONCERT,--X---,VCD,DVD,CD
	//left most being MSB of the 8-bit value 
	//example to enable ACTION : uint8_t=0b1000 X000 or 0x80  //x is dont care bit
	//
	//similarly segment 15 contanins ALL,REP, : ,A, -B, DD
	// BYTE=0b 000X 000X
	//
	// example to enable drama and rep
	//(10,15);
	ht.specialSeg(0b00100000,0b01000000);
	delay(ms);
	
	//to control segment 5 and dots
	//this function is still in beta (dots)
	ht.specialSeg("wma",'y');
	delay(ms);
	//to print wma and enable using y or n to disable(beta)
	//to control dots at seg 8&15
	ht.specialSeg("d1",'y');
	delay(ms);
	ht.specialSeg("d2",'y');
	delay(ms);
	
	ht.clearDisp();
	delay(ms);
	
	//addWrite address write translates the address/segment byte to natural numbers
	//and can write the bytes direclty to manupilate all coms and segs
	// i/p byte segment-----hardware byte segment---contents to control---8-BIT
	//   1----5---WMA--------------------------------OB  1111 1111 / 0000 0000
	//   2----6---D7[FULL],||------------------------OB  0000 000PU
	//   3----7---D6[FULL],>-------------------------OB  0000 000PL
	//   4----8--- :,D5[FULL]------------------------OB D2000 0000
	//   5----9---D4[FULL],MP3-----------------------OB  0000 000MP
	//   6----10--LINE72^----------------------------OB  0000 0000
	//   7----11--ROCK,JAZZ,DIGITAL,CLASSIC,D3[dcba]-OB  OOOO 0000
	//   8----12--D3[EGF],CHAMP,D2[dcba]-------------OB  000C 0000
	//   9----13--D2[egf],TRACK,D1[dcba]-------------OB  000T 0000
	//   10---14--D1[egf],TITLE,ALB,SHUFF,PROG-------OB  000T OOOX
	//   11---15--ALL,REP, :,A,-B,DD,----------------OB  OOOX OOOX
	ht.adWrite(7,0b10100000);
	delay(ms);
}
/*

Setting a bit
Use the bitwise OR operator (|) to set a bit.

number |= 1UL << n;
That will set the nth bit of number. n should be zero, if you want to set the 1st bit and so on upto n-1, if you want to set the nth bit.

Use 1ULL if number is wider than unsigned long; promotion of 1UL << n doesn't happen until after evaluating 1UL << n where it's undefined behaviour to shift by more than the width of a long. The same applies to all the rest of the examples.

Clearing a bit
Use the bitwise AND operator (&) to clear a bit.

number &= ~(1UL << n);
That will clear the nth bit of number. You must invert the bit string with the bitwise NOT operator (~), then AND it.

Toggling a bit
The XOR operator (^) can be used to toggle a bit.

number ^= 1UL << n;
That will toggle the nth bit of number.

Checking a bit
You didn't ask for this, but I might as well add it.

To check a bit, shift the number n to the right, then bitwise AND it:

bit = (number >> n) & 1U;
That will put the value of the nth bit of number into the variable bit.

Changing the nth bit to x
Setting the nth bit to either 1 or 0 can be achieved with the following on a 2's complement C++ implementation:

number ^= (-x ^ number) & (1UL << n);
Bit n will be set if x is 1, and cleared if x is 0. If x has some other value, you get garbage. x = !!x will booleanize it to 0 or 1.

To make this independent of 2's complement negation behaviour (where -1 has all bits set, unlike on a 1's complement or sign/magnitude C++ implementation), use unsigned negation.

number ^= (-(unsigned long)x ^ number) & (1UL << n);
or

unsigned long newbit = !!x;    // Also booleanize to force 0 or 1
number ^= (-newbit ^ number) & (1UL << n);
It's generally a good idea to use unsigned types for portable bit manipulation.

or

number = (number & ~(1UL << n)) | (x << n);
(number & ~(1UL << n)) will clear the nth bit and (x << n) will set the nth bit to x.


*/