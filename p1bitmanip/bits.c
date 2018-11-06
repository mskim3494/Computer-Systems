/* 
 * CS:APP Data Lab 
 * 
 * Stephen Huh (stephenhuh)
 * Minsu Kim (mskim3494)

 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//#include "absVal.c"
//#include "addOK.c"
//#include "allEvenBits.c"
//#include "allOddBits.c"
//#include "anyEvenBit.c"





/* 
 * anyOddBit - return 1 if any odd-numbered bit in word set to 1
 *   Examples anyOddBit(0x5) = 0, anyOddBit(0x7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyOddBit(int x) {
       int y;
       y= (170<<24)+(170<<16)+(170<<8)+170;
       // y has all odd numbered bits set to 1
       return !!(x & y); // test for equality
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  //if x == 0, then 0, otherwise a number
  int y = ~x+1;
  // if x == 0, then 0|0=0; Otherwise, a number
  int z = x|y;
  // if x == 0, then z>>31=0; otherwise, z>>31=-1
  return (z>>31) + 1;
}
//#include "bitAnd.c"
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  // variable declarations
  int mask, sum, nibbles;
    //first generate a mask to keep track of 1's in every nibbl
    mask= 0x11 + (0x11<<8);
    mask += mask<<16; // mask= 0001 0001 0001 0001 0001 0001 0001 0001
    // systematically shift the bits of x to match the mask and
    // test whether there is a 0 or a 1
    sum= x&mask;
    sum+= (x>>1)&mask;
    sum+= (x>>2)&mask;
    sum+= (x>>3)&mask;
    // now each nibble has the sum of ones of its corresponding
    // nibble in x; make it into a single sum for every byte
    nibbles= 0x0F + (0x0F<<8);
    nibbles += nibbles<<16;
    sum += sum>>4;
    sum &= nibbles;
    // similarly, get the sums of each byte
    sum += sum>>8;
    sum += sum>>16;
    // & with 0x3F because that's the max number of ones there can be
    return sum & 0x3F;
}
/* 
 * bitMask - Generate a mask consisting of all 1's 
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
  //mask all bits greater than highbit (+1 for indexing)
  //mask bits less than lowbit
  //flip to return
  return ~(((~0 << highbit) & ~(1 << highbit)) | (~(~0<<lowbit)));
}
//#include "bitNor.c"
//#include "bitOr.c"
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
    // parity of the whole is decided by
    // parity of A and B = parity of (parity of A and parity of B)
    x ^= x >> 16;
    // keep repeating to get the lower bits
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    // last bit represents the parity
    return x &= 1;
}
//#include "bitXor.c"
//#include "byteSwap.c"
//#include "conditional.c"
//#include "copyLSB.c"
//#include "divpwr2.c"
//#include "evenBits.c"
//#include "ezThreeFourths.c"
//#include "fitsBits.c"
//#include "fitsShort.c"
//#include "float_abs.c"
//#include "float_f2i.c"
//#include "float_half.c"
//#include "float_i2f.c"
//#include "float_neg.c"
//#include "float_twice.c"
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  // Utilize shifting to find nth byte and mask to return only that byte.
  return (x >> (n<<3) & 0xff);
}
//#include "greatestBitPos.c"
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  // variable declaration
  int bits;
  // make number positive
  // xor with sign of x; if negative, -1; 0 otherwise
   x= (x>>31)^x;
  // calculate number of bits using log base two of x + 2
  bits = (!!(x>>16))<<4;
  // check to see if there are bits systematically reducing
  // to the LSB
   bits += (!!(x>>(bits+8)))<<3;
   bits += (!!(x>>(bits+4)))<<2;
   bits += (!!(x>>(bits+2)))<<1;
   bits += (!!(x>>(bits+1)));
  // if x=0 or x=-1, then bits=1
   bits += 1 + (!(1^x)) + (!!bits);
  return bits;
}
//#include "ilog2.c"
//#include "implication.c"
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  // variable declaration
  int low_b, high_b;
    // check lower boundary
    // if low_b = 1, then x >= 48 is false
    // if low_b = 0, then x >= 48 is true
    low_b= (x + (~48+1))>>31;
    //check upper boundary
    // if high_b = 1, then x <= 57 is false
    //else true
    high_b= (57+(~x+1))>>31;
    // if either of them is false (1), then return 0; otherwise return 1
    return !(low_b|high_b);
}
/* 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {
  //use XOR to return 0 when equal, bang to return value needed
  return !(x^y);
}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
   // Find sign of x and y, 1 = negative; 0 = non-negative
    int xsign = x >> 31;
    int ysign = y >> 31;
    
    // Need to consider two cases: when signs are same and otherwise
    
    // Same sign --> subtract y from x + 1
    // if x is larger, first bit of (x + (~y+1)) = 0 (positive)
    // otherwise 1
    int same_sign = !(xsign^ysign) & ((x+(~y)) >> 31);
    
    // Different sign
    // if x>=0 and y<0, then 0; otherwise, 1
    // for same sign, 0;
    int diff_sign = xsign & !ysign;
    
    // if either of them is false (1), then return 0; otherwise return 1
    return !(same_sign|diff_sign);
}
//#include "isLess.c"
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
    // find the difference sign between the two
    // if y >= x, then 0; otherwise 1
    int diff_sign= (y+(~x+1))>>31;
    // get the signs of the numbers
    // if same, then 0; otherwise 1
    int xysign= (x>>31)^(y>>31);
    //check for overflow in subtraction
    int over= xysign&(x>>31);
    return !((diff_sign|xysign)^over);
}
//#include "isNegative.c"
/* 
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
int isNonNegative(int x) {
  int y= 1<<31;
  return !(x&y);
}
//#include "isNonZero.c"
/* 
 * isNotEqual - return 0 if x == y, and 1 otherwise 
 *   Examples: isNotEqual(5,5) = 0, isNotEqual(4,5) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNotEqual(int x, int y) {
  //test equality with XOR use !! to return boolean
  return !!(x^y);
}
//#include "isPositive.c"
//#include "isPower2.c"
//#include "isTmax.c"
//#include "isTmin.c"
//#include "isZero.c"
//#include "leastBitPos.c"
/*
 * leftBitCount - returns count of number of consective 1's in
 *     left-hand (most significant) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Rating: 4
 */
int leftBitCount(int x) {
  return 2;
}
//#include "logicalNeg.c"
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  int m= 31+(~n+1);
  int y= ~0<<1;
  return (x>>n) & ~(y<<m);
  // shift by n and mask
}
//#include "minusOne.c"
//#include "multFiveEighths.c"
//#include "negate.c"
//#include "oddBits.c"
/* 
 * rempwr2 - Compute x%(2^n), for 0 <= n <= 30
 *   Negative arguments should yield negative remainders
 *   Examples: rempwr2(15,2) = 3, rempwr2(-35,3) = -3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int rempwr2(int x, int n) {
  //negpower = -2^n
  int negpower = (~0) << n;
  int rem = x & (~(~0 << n));
  int negrem = (~rem + 1);
  //to consider for both positive & negative cases 
  //x&negrem == 0 for positive, -1 otherwise
  return rem + (((x& negrem) >> 0x1F) & negpower);  
}
/* 
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
  int N = n<<3;
  int y = ~(255<<N) & x;
  y |= c<<N;
  return y;
}
//#include "reverseBytes.c"
//#include "rotateLeft.c"
/* 
 * rotateRight - Rotate x to the right by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateRight(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateRight(int x, int n) {
    int m= 32 + (~n+1); // ~n+1 negates the number for subtraction, get 32-n
    int y= (x>>n) & ~((1<<31)>>(n+(~1+1)));
    // first shift the number to the right by n
    // then mask the possibly resulting 1's from the shift
    return y | (x<<m); // add the lost bits to the front
}
//#include "satAdd.c"
//#include "satMul2.c"
/*
 * satMul3 - multiplies by 3, saturating to Tmin or Tmax if overflow
 *  Examples: satMul3(0x10000000) = 0x30000000
 *            satMul3(0x30000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0x70000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0xD0000000) = 0x80000000 (Saturate to TMin)
 *            satMul3(0xA0000000) = 0x80000000 (Saturate to TMin)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 3
 */
int satMul3(int x) {
    int Tmax= ~(1<<31);
    // compute x*3
    int twox= x + x;
    int threex= twox + x;
    // -1 if negative, 0 otherwise
    int sign= x>>31;
    // check for overflow. xor multiplications with x (first bit is 1 if diff signs)
    // and check if either is 1. if no overflow, then 0
    int overflow_check= ((twox^x)|(threex^x)) >>31;
    // find which saturation value it should be (either Tmax or Tmin), 0 otherwise
    int sat_val= overflow_check & (sign ^ Tmax);
    // return sat_val if threex>Tmax or threex<Tmin, otherwise return threex
    return sat_val | (~overflow_check & threex);
}
//#include "sign.c"
//#include "sm2tc.c"
/* 
 * subOK - Determine if can compute x-y without overflow
 *   Example: subOK(0x80000000,0x80000000) = 1,
 *            subOK(0x80000000,0x70000000) = 0, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int subOK(int x, int y) {
  // overflow happens only if x<0 and y>0 (different signs)
  int signs= (x>>31) ^ (y>>31);
  // and only if sign of x-y is different with that of x
  int diff= x + (~y+1);
  int diff_sign= (diff>>31)^(x>>31);
  return !(signs & diff_sign);
}
//#include "tc2sm.c"
//#include "thirdBits.c"
//#include "tmax.c"
//#include "tmin.c"
//#include "trueFiveEighths.c"
//#include "trueThreeFourths.c"
//#include "upperBits.c"
