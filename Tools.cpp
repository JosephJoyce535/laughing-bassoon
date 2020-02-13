/*
    Utility functions (e.g., for bit/byte manipulations) used in Y86 simulator.
    
    Author:    
    version:    
*/
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <cstdint>
#include "Tools.h"

namespace Tools {
    
/*-----------------------------------------------------------------------------------------------
    getBits
	Extracts bits 'low' to 'high' (inclusive) from 'source' and returns the result as
	an 64-bit integer. 
	Example:
	
	getBits(12,15,0x0123456789abcdef);
	
	would return the value:
	
	0x00000000000000c;
	
	(Bits 12-15 of the 64-bit word, counting from the right correspond to hex digit 'c' in the
	source word. Note that any number of bits from 1 to 64 could be specified depending on the values
	of 'low' and 'high'.
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t getBits(unsigned low, unsigned high, uint64_t source)
{
    assert((high < 64) && (high >= 0) && (low <= high));
    int diff = 63 - high;
    uint64_t mask = source << diff;
    source = mask >> (diff + low);
    return source;
}
/*-----------------------------------------------------------------------------------------------
    setBits
	Sets (to 1) the bits from bit numbers 'low' to 'high' (inclusive) in 
	the source word and returns the resulting word.
	
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t setBits(unsigned low, unsigned high, uint64_t source)
{   
    assert((high < 64) && (high >= 0) && (low <= high));
    uint64_t mask = 0;
    uint64_t one = 1;
    for (uint64_t i = low; i <= high; i++) {
        uint64_t newmask = one << i;
        mask = mask + newmask;
    }
    return (source | mask);
}
/*-----------------------------------------------------------------------------------------------
    clearBits
	
	Clears (to 0) the bits from bit numbers 'low' to 'high' (inclusive) in 
	the source word and returns the resulting word.
	
	The value of high must be greater than or equal to low and high must be in the range 0 to 63. 
------------------------------------------------------------------------------------------------*/
uint64_t clearBits(unsigned low, unsigned high, uint64_t source)
{
    assert((high < 64) && (high >= 0) && (low <= high));
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    uint64_t one = 1;
    for (uint64_t i = low; i <= high; i++) {
        uint64_t newmask = one << i;
        mask = mask - newmask;
    }
    return (source & mask);
}

/*-----------------------------------------------------------------------------------------------
    assignOneBit
	Sets the bit number 'bitNum' to the binary value (0 or 1) specified by 'bitVal' in
	the source word and returns the resulting word.
	
	'bitNum' must be in the range 0 to 63 (inclusive) and 'bitVal' must be 0 or 1.
------------------------------------------------------------------------------------------------*/
uint64_t assignOneBit(unsigned bitNum, unsigned bitVal, uint64_t source)
{
    assert((bitNum < 64) && (bitNum >= 0) && (bitVal == 0 || bitVal == 1));
    uint64_t one = 1;
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    if (bitVal == 1) {
        source |= (one << bitNum);
    }
    else {
        uint64_t newmask = one << bitNum;
        mask = mask - newmask;
        source &= mask;
    }
    return source;
}

/*-----------------------------------------------------------------------------------------------
    getByteNumber
	Returns the specified byte number from the source word.
	
	Example:
	getByteNumber(3,0x0011223344556677);
	
	will return the single byte value:
	
	0x44
	
	(Note that the '3' refers to byte 3 of a 8-byte unsigned integer (bytes are numbered from
	the right starting at 0 when looking at a word value in hex). 
------------------------------------------------------------------------------------------------*/
uint8_t getByteNumber(unsigned byteNum, uint64_t source)
{
  assert(byteNum >= 0 && byteNum < 8);
  byteNum = byteNum * 8;
  int x = ((source>>byteNum) & 0xFF);   
  return x;
}
/*-----------------------------------------------------------------------------------------------
    putByteNumber
	Returns the source word after replacing the specified byte (byteNum) with a given 
	byte value (byteVal).
	
	Example:
	putByteNumber(3,0x00,0x0011223344556677);
	
	will return the value:
	
	0x0011223300556677
	
	(Note that the '3' refers to byte 3 of a 8-byte unsigned integer (bytes are numbered from
	the right starting at 0 when looking at a word value in hex). 
------------------------------------------------------------------------------------------------*/
uint64_t putByteNumber(unsigned byteNum, uint8_t byteVal, uint64_t source)
{
    assert(byteNum >= 0 && byteNum < 8);
    uint64_t one = 1;
    uint64_t mask = ((one << 8) - one) << (byteNum * 8);
    uint64_t value = source & ~mask;
    uint64_t newmask = (uint64_t) byteVal << (byteNum * 8);
    value |= newmask;
    return value;                  
}
/*-----------------------------------------------------------------------------------------------
    buildWord
	
	Returns a 64 bit word consisting of the 8 parameter bytes, where b0 is the least significant
	and b7 is the most significant byte.
------------------------------------------------------------------------------------------------*/
uint64_t buildWord(unsigned char b0, unsigned char b1,unsigned char b2, unsigned char b3,
                          unsigned char b4, unsigned char b5,unsigned char b6, unsigned char b7)
{
    uint64_t word = ((uint64_t) b7 << 56) + ((uint64_t) b6 << 48) + ((uint64_t) b5 << 40) + ((uint64_t) b4 << 32) + ((uint64_t) b3 << 24) + (b2 << 16) + (b1 << 8) + (b0); 
    return word;                     
}
/*-----------------------------------------------------------------------------------------------
    isNegative

	Returns true if the input parameter is negative when interpreted as a signed value.
------------------------------------------------------------------------------------------------*/
bool isNegative(uint64_t source)
{
    int x = source >> 63;
    //printf("%d\n", x);
    if (x == 1)
        return true;
    else
        return false;
}
/*-----------------------------------------------------------------------------------------------
    expandBits
	
	Reads the source and converts it to a character string consisting of 72 bytes--71 characters
	followed by a '\0' character (string terminator).
	Example usage: 
		char buf[72];
		expandBits(0xAABBCCDDEE001122UL,buf);

		Will cause the following string to be written to the buf[] array (not including quotes):
	
	"10101010 10111011 11001100 11011101 11101110 00000000 00010001 00100010"
	
	Note the space after every group of 8 digits except the last. The last byte of
	the string is the '\0' byte, that is not printable but used to indicate the end
	of a C string.
------------------------------------------------------------------------------------------------*/
void expandBits(uint64_t source, char *bits)
{
   assert(bits != __null);
   int k = 0;
   for (int i = 0; i < 71; i++) {
      if (i == 8 || i == 17  || i == 26 || i == 35 || i == 44 || i == 53 || i == 62) {
        bits[i] = ' ';
      }
      else {
         uint64_t x = source << k;
         k++;
         int t = x >> 63;
         if (t == 1)
             bits[i] = '1';
         else
             bits[i] = '0';
      }
   }
   bits[71] = '\0';
   return;
    
}
/*-----------------------------------------------------------------------------------------------
    clearBuffer
	Treats pbuf as a pointer to an array of 'size' bytes and clears them all to zero.
------------------------------------------------------------------------------------------------*/
void clearBuffer(char * pbuf, int size)
{
    assert(pbuf != __null && size >= 0 && size <= 2147483647);
    for (int i = 0; i<size; i++) {
        pbuf[i] = 0;
    } 
    return;
}

} // end namespace Tools
