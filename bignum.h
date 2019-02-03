//
//  bignum.h
//  big_calc
//
//  Created by POTADOS on 23/01/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#ifndef _bignum_h
#define _bignum_h

#include "includes.h"

/********************************
 * Internel nibble operations.
 ********************************/

/**
 * Function-like prototype:
 * size_t NINB(size_t n);
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
 * size_t BIDX(size_t n);
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


/********************************
 * Userspace nibble operations.
 ********************************/

/**
 * Function-like prototype:
 * byte get_nibble_at(byte * arr, size_t index);
 *
 * Get nibble in array at specific index.
 *
 * @param arr       Pointer of byte array which has nibble.
 * @param index     Index of nibble. NOT an index for byte.
 *
 * @return          Byte which has the found nibble at low four bits.
 */
#define get_nibble_at(arr, index)                                   \
((index % 2) ?                                                      \
LOW_NIBBLE(*((arr) + (BIDX(index)))) :                              \
HIGH_NIBBLE(*((arr) + (BIDX(index)))))

/**
 * Function-like prototype:
 * void set_nibble_at(byte * arr, size_t index, byte val);
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

/**
 * Interate every nibble in a bignum.
 *
 * @param _byte_decl        Declaration of byte variable.
 * @param _bignum_st_ptr    Pointer to struct BIGNUM.
 *
 * Usage:
 * foreach_num(byte item, my_bignum) { ... }
 */
#define foreach_num(_byte_decl, _bignum_st_ptr)                     \
for (size_t _index = 0,                                             \
_keep = 1;                                                          \
_keep && (_index != _bignum_st_ptr->_length);                       \
_keep = !_keep,                                                     \
++ _index)                                                          \
for(_byte_decl = get_nibble_at(_bignum_st_ptr->_nums, _index);      \
_keep;                                                              \
_keep = !_keep)

/**
 * Interate every nibble in a bignum in a reversed order.
 * Index stays still but it starts at the highest and goes to lowest.
 *
 * @param _byte_decl        Declaration of byte variable.
 * @param _bignum_st_ptr    Pointer to struct BIGNUM.
 *
 * Usage:
 * foreach_num(byte item, my_bignum) { ... }
 */
#define foreach_num_r(_byte_decl, _bignum_st_ptr)                   \
for (size_t _index = _bignum_st_ptr->_length - 1,                   \
_keep = 1, _done = 0;                                               \
_keep && !_done;                                                    \
_keep = !_keep, _done = !_index, -- _index)                         \
for(_byte_decl = get_nibble_at(_bignum_st_ptr->_nums, _index);      \
_keep;                                                              \
_keep = !_keep)


/********************************
 * Utility macros.
 ********************************/

/**
 * Iterate every digit of integet from LSB.
 *
 * @param _byte_decl        Declaration of byte variable to store each digit.
 * @param _int              Integer to iterate.
 *
 * Usage:
 * for_each_int_from_lsb(byte digit, 96400) { ... }
 */
#define for_each_int_from_lsb(_byte_decl, _int)                     \
for(int _digit_place = 0, _keep = 1;                                \
_int != 0;                                                          \
_int /= 10, ++_digit_place, _keep = !_keep)                         \
for (_byte_decl = _int % 10; _keep; _keep = !_keep)


typedef uint8_t byte;

/**
 * Big number container.
 */
typedef struct _big_num {
    size_t  _length;        /* Length of number. Same as number of digits in decimal. */
    size_t  _alloc_size;    /* Actually allocated size of byte array. Always 1 bigger than _length.*/
    BOOL    _is_negative;   /* Flag indicating if this number is negative. */
    byte    * _nums;        /* Byte array containing digits. One byte has two digits as nibble. */
    char    * _string_out;  /* Already allocated string representation. For reuse purpose. */
} BIGNUM;

#ifdef __cplusplus
extern "C"
{
#endif

    /***********************************************************
     * Creation, allocation, and deletion.
     ***********************************************************/
    
    /**
     * Create new bignum with variable length.
     *
     * @param _length   Length of new bignum.
     *
     * @return          Pointer of new bignum.
     *                  NULL when _length is zero or allocation is failed.
     */
    BIGNUM * bn_new_length(size_t _length);
    
    /**
     * Create new bignum 0 with length 1.
     *
     * @return          Pointer of allocated bignum struct.
     *                  NULL when allocation is failed.
     */
    BIGNUM * bn_new(void);
    
    /**
     * Increase length of bignum by reallocating it.
     *
     * @param _num      Bignum to reallocate.
     * @param _add      Amount of length to increase.
     *
     * @return          Reallocated bignum.
     *                  NULL when _num is NULL.
     *                  Just return _num when _add is zero.
     */
    BIGNUM * bn_realloc_increase(BIGNUM * _num, size_t _add);
    
    /**
     * Decrease length of bignum by reallocating it.
     *
     * @param _num      Bignum to reallocate.
     * @param _sub      Amount of length to decrease.
     *
     * @return          Reallocated bignum.
     *                  NULL when _num is NULL.
     *                  Just return _num when _sub is zero.
     */
    BIGNUM * bn_realloc_decrease(BIGNUM * _num, size_t _sub);
    
    /**
     * Reallocate bignum with length.
     *
     * @param _num      Bignum to reallocate.
     * @param _length   Destination length(number of digits).
     *
     * @return          Reallocated bignum.
     *                  NULL when _num is NULL or _length is zero or allocation is failed.
     */
    BIGNUM * bn_realloc(BIGNUM * _num, size_t _length);
    
    /**
     * Clear contents of bignum.
     * Memory allocation, length will be preserved but contents will be ereased.
     *
     * @param _num      Bignum to clear.
     *
     * @return          Cleared bignum.
     */
    BIGNUM * bn_clear(BIGNUM * _num);
    
    /**
     * Free every allocated fields of structure, and the structure itself.
     * Call is ignored when parameter is NULL.
     *
     * @param _num      Bignum to free.
     */
    void bn_free(BIGNUM * _num);
    
    
    /***********************************************************
     * Conversion and respresentation.
     ***********************************************************/
    
    /**
     * Create new bignum from string.
     *
     * @param _source   Source string.
     *
     * @return          NULL when parameter is NULL or string is invalid.
     */
    BIGNUM * bn_from_string(char * _source);

    /**
     * Create new bignum from integer.
     *
     * @param _source   Source integer(long long).
     *
     * @return          NULL when parameter is NULL or string is invalid.
     */
    BIGNUM * bn_from_integer(long long int _source);
    
    /**
     * Export bignum to string.
     *
     * @param _source   Source bignum.
     *
     * return           NULL when _source is null.
     */
    char * bn_to_string(BIGNUM * _source);
    
    /**
     * Export bignum to integer(long long).
     *
     * @param _source   Source bignum.
     *
     * @return          0 when _source is null or integer overflows.
     */
    long long int bn_to_integer(BIGNUM * _source);
    
    /**
     * Print bignum to file stream.
     *
     * @param _stream   Target file stream to print.
     * @param _num      Bignum to print.
     *
     * @return          Number of characters printed. Negative when parameter is(are) null
     *                  or failed fprintf.
     */
    int bn_fprint(FILE * _stream, BIGNUM * _num);
    
    /**
     * Print bignum to stdout.
     *
     * @param _num      Bignum to print.
     *
     * @return          Number of characters printed.Negative when parameter is(are) null
     *                  or failed fprintf.
     */
    int bn_print(BIGNUM * _num);
    
    
    /***********************************************************
     * Properties of BIGNUM.
     ***********************************************************/
    
    /**
     * Return length of bignum.
     *
     * @param _source   Source bignum.
     *
     * @return          0 when _source is null.
     */
    size_t bn_length(BIGNUM * _source);
    
    /**
     * Return sign of bignum.
     *
     * @param _source   Source bignum.
     *
     * @return          1 when positive, -1 when negative, 0 when _source is null.
     */
    int bn_sign(BIGNUM * _source);
    
    
    /***********************************************************
     * Decimal arithmatic.
     ***********************************************************/
    
    BIGNUM * bn_add(BIGNUM * _left, BIGNUM * _right);


#ifdef __cplusplus
}
#endif

#endif /* _bignum_h */
