/**
 * \file HT1621.cpp
 * \brief Implementation of a class for dealing with the Holtek HT1621 chip.
 * \date 31 january 2020
 * \version 1.0
 */

#include "HT1621.h"

void HT1621::begin()
{
    pinMode(_DATA_pin, OUTPUT);
    pinMode(_RW_pin, OUTPUT);
    pinMode(_CS_pin, OUTPUT);
 
    digitalWrite(_CS_pin, HIGH);
    digitalWrite(_RW_pin, HIGH);
    digitalWrite(_DATA_pin, HIGH);

#ifndef __HT1621_READ
    register uint8_t i;
    
    for(i=0; i<16; i++)
        ram[i] = 0;
#endif
    
}
//delay 20 microseconds...
void HT1621::writeBits(uint8_t data, uint8_t cnt)
{
    register uint8_t i;

    for(i=0;i<cnt;i++,data <<=1)
    {
        digitalWrite(_RW_pin, LOW);
        delayMicroseconds(20);
        digitalWrite(_DATA_pin, data&0x80 ? HIGH : LOW);
        delayMicroseconds(20);
        digitalWrite(_RW_pin, HIGH);
        delayMicroseconds(20);
    }
}

#ifdef __HT1621_READ

uint8_t HT1621::readBits(uint8_t cnt)
{
    uint8_t data, i, state;
    
    
    pinMode(_DATA_pin, INPUT);
    
    for(i=0, data=0; i<cnt; data <<= 1, i++) {
        digitalWrite(_RW_pin, LOW);
        delayMicroseconds(20);
        data |= (digitalRead(_DATA_pin) == HIGH);
        digitalWrite(_RW_pin, HIGH);
        delayMicroseconds(20);
    }
    
    pinMode(_DATA_pin, OUTPUT);
    
    return data;
}

#endif

void HT1621::sendCommand(uint8_t cmd, bool first, bool last)
{
    if (first) {
        TAKE_CS();
        writeBits(COMMAND_MODE, 4);
    }
    
    writeBits(cmd, 8);
    
    if (last)
        RELEASE_CS();
}

void HT1621::write(uint8_t address, uint8_t data)
{
    TAKE_CS();
    
    writeBits(WRITE_MODE, 3);
    writeBits(address<<3, 6); // 6 is because max address is 128
    writeBits(data, 8);
#ifndef __HT1621_READ
    ram[address] = data;
#endif
    
    RELEASE_CS();
}

void HT1621::write(uint8_t address, uint8_t data, uint8_t cnt)
{
    register uint8_t i;
    
    TAKE_CS();
    
    writeBits(WRITE_MODE, 3);
    writeBits(address<<3, 6);
    for (i = 0; i < cnt; i++) {
        writeBits(data, 8);
#ifndef __HT1621_READ
        ram[i] = data;
#endif
    }
    
    RELEASE_CS();
}

void HT1621::write(uint8_t address, uint8_t *data, uint8_t cnt)
{
    register uint8_t i;
    
    TAKE_CS();
    
    writeBits(WRITE_MODE, 3);
    writeBits(address<<3, 6);
    for (i = 0; i < cnt; i++) {
        writeBits(data[i], 8);
#ifndef __HT1621_READ
        ram[i] = data[i];
#endif
    }
    
    RELEASE_CS();
}

#ifdef __HT1621_READ

uint8_t HT1621::read(uint8_t address)
{
    uint8_t data;
    
    TAKE_CS();
    
    writeBits(READ_MODE, 3);
    writeBits(address<<3, 6);
    data = readBits(8);
    
    RELEASE_CS();
    
    return data;
}

void HT1621::read(uint8_t address, uint8_t *data, uint8_t cnt)
{
    register uint8_t i;
    
    TAKE_CS();
    
    writeBits(READ_MODE, 3);
    writeBits(address<<3, 6);
    for (i = 0; i < cnt; i++)
        data[i] = readBits(8);
    
    RELEASE_CS();
}

#else

uint8_t HT1621::read(uint8_t address)
{
    return ram[address];
}

void HT1621::read(uint8_t address, uint8_t *data, uint8_t cnt)
{
    register uint8_t i;
    
    for (i = 0; i < cnt; i++)
        data[i] = ram[address+i];
}

 
   


void HT1621::clearDisp(){
    for (int i = 0; i < 127; i++)
    {
        write(i, 0);
    }
}
	const short val[10] = {250, 96, 188, 244, 102, 214, 222, 112, 254, 246};
	const short vals5[10]={0x5f,0x50,0x3d,0x79,0x72,0x6f,0x6f,0x51,0x7f,0x73};
    const short valMSB[10]={0xa0,0x00,0xc0,0x40,0x60,0x60,0xe0,0x00,0xe0,0x60};
	const short valLSB[10]={0x0f,0x06,0x0b,0x0f,0x06,0x0d,0x0d,0x07,0x0f,0x07};
	
void HT1621::inWrite(unsigned short segment, unsigned short number){
    
     if((segment<=7 && segment >=1)&&(number<=9 && number >=0) )
     {		
		 
        switch (segment)
    {
        case 1: write(14,valMSB[number]);
				write(13,valLSB[number]);
				break;
				
        case 2: write(12,valLSB[number]);
				write(13,valMSB[number]);
				break;
				
        case 3:write(11,valLSB[number]);
				write(12,valMSB[number]);
				break;

        case 4: write(9,val[number]);
				break;

        case 5:write(8,vals5[number]);
				break;

        case 6:  write(7,val[number]);
				break;
        
        case 7:write(6,val[number]);
				break;
    }

    }
}

void HT1621::inWrite(unsigned short segment, unsigned short number, char play, char pause){
     
     if((segment<=7 && segment >=1)&&(number<=9 && number >=0) )
     {		
		 
        switch (segment)
    {
        case 1: write(14,valMSB[number]);
				write(13,valLSB[number]);
				break;
				
        case 2: write(12,valLSB[number]);
				write(13,valMSB[number]);
				break;
				
        case 3:write(11,valLSB[number]);
				write(12,valMSB[number]);
				break;

        case 4: write(9,val[number]);
				break;

        case 5:write(8,vals5[number]);
				break;

        case 6: if (play=='e' | play=='E'){
				short mask = 1 << 0; 
				write(7, (((val[number]) & ~mask) | ((1 << 0) & mask)));
				}
				
			    if (play=='n'| play=='N'){
				write(7,0x01);
				}
				
				if(play=='d'| play=='D') write(7,val[number]);
				break;
        
        case 7:if(pause=='e' | pause=='E'){
				short mask = 1 << 0;      
				write(6,((val[number]) & ~mask) | ((1 << 0) & mask));
		        }	
				
			    if (pause=='n'| pause=='N'){
				write(6,0x01);
				}
				
				if(pause=='d'| pause=='D') write(6,val[number]);
				break;
    }

    }
}

void HT1621::specialSeg(uint8_t binn,uint8_t bins){
	write(10,binn);	
	write(15,bins);	
}

void HT1621::specialSeg(String s, char en){
	uint8_t statusq=read(15);
	uint8_t statusw=read(8);
	if (en=='y'| en=='Y'){
		if(s=="WMA"| s=="wma"){
			write(5,0xff);
		}
		else if(s=="D1"|s=="d1"){
			
		write (15,(statusq & ~(1 << 5)) | ((1 << 5) & (1 << 5)));
		}
		
		else if(s=="D2"|s=="d2"){
			
		write (8,(statusw & ~(1 << 7)) | ((1 << 7) & (1 << 7)));
		}
		
		
	}
	if(en=='n'|en=='N') {
		write (15,(statusq & ~(1 << 5)) | ((0 << 5) & (1 << 5)));

		write (8,(statusw & ~(1 << 7)) | ((0 << 7) & (1 << 7)));
		
		write(5,0x00);
	}
		
}
	
	void HT1621::adWrite(unsigned short address, uint8_t value){
		switch(address){
		case 1: write(5,value); break;
		case 2:write(6,value);break;
		case 3:write(7,value);break;
		case 4:write(8,value);break;
		case 5:write(9,value);break;
		case 6:write(10,value);break;
		case 7:write(11,value);break;
		case 8:write(12,value);break;
		case 9:write(13,value);break;
		case 10:write(14,value);break;
		case 11:write(15,value);break;
		
	}
	
}
	/*void HT1621::aplWrite(unsigned short segment, char letter){
		const short sym = {126, 206, 154, 236, 158, 30, 218, 110, 10, 232, 138, 122, 204, 62, 118, 26, 214, 142, 234, 230};
		unsigned short seg=0;
	
		switch(segment){	
		case 1: seg=0;break;
		case 2: seg=0;break;
		case 3: seg=0;break;
		case 4: seg=9;break;
		case 5: seg=0;break;
		case 6: seg=7;break;
		case 7: seg=6;break;
		
		}	
		switch (letter){
		case a:	writebreak;
		case b:break;
		case c:break;
		case d:break;
		case e:break;
		case f:break;
		case g:break;
		case h:break;
		case i:break;
		case j:break;
		case k:break;
		case l:break;
		case n:break;
		case o:break;
		case p:break;
		case q:break;
		case r:break;
		case s:break;
		case t:break;
		case u:break;
		case y:break;
		
		case A:break;
		case B:break;
		case C:break;
		case D:break;
		case E:break;
		case F:break;
		case G:break;
		case H:break;
		case I:break;
		case J:break;
		case K:break;
		case L:break;
		case N:break;
		case O:break;
		case P:break;
		case Q:break;
		case R:break;
		case S:break;
		case T:break;
		case U:break;
		case Y:break;
		
}} */

#endif