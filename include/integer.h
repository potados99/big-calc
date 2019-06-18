//
//  integer.h
//  big_calc
//
//  Created by POTADOS on 05/02/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#ifndef _integer_h
#define _integer_h

#include <stdint.h>
#include <limits.h>
#include <math.h>

/********************************
 * Nibble
 ********************************/

/**
 * Function-like prototype:
 * bn_t NINB(bn_t n);
 *
 * Nibbles in bytes.
 * Get how many bytes are needed to store nibbles.
 *
 * @param n         Number of nibbles.
 *
 * @return          Number of bytes need to store n nibbles.
 */
#define NINB(n)                                                     \
(((n) >> 1) + 1)

/**
 * Function-like prototype:
 * bn_t BIDX(bn_t n);
 *
 * Byte index.
 * Get index of byte where nibble is at.
 *
 * @param n         Index of nibble.
 *
 * @return          Index of byte where nibble is at.
 */
#define BIDX(n)                                                     \
((n) >> 1)

/**
 * Function-like prototype:
 * byte HIGH_NIBBLE(byte b);
 *
 * Get high nibble of byte.
 *
 * @param b         Target byte to extract high nibble.
 *
 * @return          High four bits of b.
 */
#define HIGH_NIBBLE(b)                                              \
(((b) >> 4) & 0x0F)

/**
 * Function-like prototype:
 * byte LOW_NIBBLE(byte b);
 *
 * Get low nibble of byte.
 *
 * @param b         Target byte to extract low nibble.
 *
 * @return          Low four bits of b.
 */
#define LOW_NIBBLE(b)                                               \
((b) & 0x0F)

/**
 * Function-like prototype:
 * byte BYTE(byte high, byte low);
 *
 * Get a byte consists of high nibble and low nibble
 *
 * @param high      A byte which has low four digits to be high nibble.
 * @param low       A byte which has low four digits to be low nibble.
 *
 * @return          A Byte made of each high/low nibble.
 */
#define BYTE(high, low)                                             \
(((high) << 4) | LOW_NIBBLE(low))

/**
 * Function-like prototype:
 * void SET_HIGH_NIBBLE(byte * bp, byte n);
 *
 * Set high nibble of a byte at bp(byte pointer).
 *
 * @param bp        Pointer to target byte.
 * @param n         A byte having source nibble at low four bits.
 */
#define SET_HIGH_NIBBLE(bp, n)                                      \
do {                                                                \
*(bp) = BYTE((n), LOW_NIBBLE(*(bp)));                               \
} while(0)

/**
 * Function-like prototype:
 * void SET_LOW_NIBBLE(byte * bp, byte n);
 *
 * Set low nibble of a byte at bp(byte pointer).
 *
 * @param bp        Pointer to target byte.
 * @param n         A byte having source nibble at low four bits.
 */
#define SET_LOW_NIBBLE(bp, n)                                       \
do {                                                                \
*(bp) = BYTE(HIGH_NIBBLE(*(bp)), (n));                              \
} while(0)

/**
 * Function-like prototype:
 * byte get_nibble_at(byte * arr, bn_t index);
 *
 * Get nibble in array at specific index.
 *
 * @param arr       Pointer of byte array which has nibble.
 * @param index     Index of nibble. NOT an index for byte.
 *
 * @return          Byte which has the found nibble at low four bits.
 */
#define get_nibble_at(arr, index)                                   \
(byte)((index % 2) ?                                                \
LOW_NIBBLE(*((arr) + (BIDX(index)))) :                              \
HIGH_NIBBLE(*((arr) + (BIDX(index)))))

/**
 * Function-like prototype:
 * void set_nibble_at(byte * arr, bn_t index, byte val);
 *
 * @param arr       Pointer of byte array which has nibble.
 * @param index     Index of nibble. NOT an index for byte.
 * @param val       Byte which has the nibble to set at low four bits.
 */
#define set_nibble_at(arr, index, val)                              \
do {                                                                \
if ((index) % 2) {                                                  \
SET_LOW_NIBBLE(((arr) + BIDX(index)), (val));                       \
}                                                                   \
else {                                                              \
SET_HIGH_NIBBLE(((arr) + BIDX(index)), (val));                      \
}                                                                   \
} while(0)


/********************************
 * Integer.
 ********************************/

/**
 * Function-like prototype:
 * int INT_LEN(int _int);
 *
 * Get number of digits of an integer.
 *
 * @param _int              Integer to get number of digits.
 *
 * @return                  Number of digits of integer in decimal.
 *                          1 when got zero.
 */
#define INT_LEN(_int)                                               \
((_int == 0) ? 1 : (floor(log10(llabs(_int))) + 1))

/**
 * Function-like prototype:
 * int INT_DIGIT_AT(int _int, int _index_from_lsb);
 *
 * Get specific digit of integer with index starting from lsb.
 *
 * @param _int              Integer to get specific digit.
 * @param _index_from_lsb   Index indicating specific digit.
 *
 * @return                  Value of digit.
 */
#define INT_DIGIT_AT(_int, _index_from_lsb)                         \
((_int / (long long int)pow(10, _index_from_lsb)) % 10)

/**
 * Get max value of signed integer type.
 *
 * @param _type             Type of integer.
 *
 * @return                  Same as 2 ^ ((sizeof(_type) * 8) - 1) - 1.
 */
#define INT_SIGN_MAX(_type)                                         \
(((_type)(-1) + ((_type)(1) << ((sizeof(_type) << 3) - 2)))         \
+ ((_type)(1) << ((sizeof(_type) << 3) - 2)))


/********************************
 * Iteration.
 ********************************/

/**
 * Iterate array.
 *
 * @param _item_pointer_decl    Declaration for each item.
 * @param _array                Array to iterate.
 *
 * Usage:
 * foreach(int * item, my_int_array) { ... }
 */
#define foreach(_item_pointer_decl, _array) \
for(int keep = 1, count = 0, size = sizeof (_array) / sizeof *(_array); \
keep && count != size; \
keep = !keep, count++) \
for(_item_pointer_decl = (_array) + count; keep; keep = !keep)

/**
 * Iterate every digit of integet from LSB.
 *
 * @param _byte_decl        Declaration of byte variable to store each digit.
 * @param _int              Integer to iterate.
 *
 * Usage:
 * foreach_int_from_lsb(byte digit, 96400) { ... }
 */
#define foreach_int_from_lsb(_byte_decl, _int)                      \
for(int _digit_place = 0, _keep = 1;                                \
_int != 0;                                                          \
_int /= 10, ++_digit_place, _keep = !_keep)                         \
for (_byte_decl = _int % 10; _keep; _keep = !_keep)


#endif /* _integer_h */
