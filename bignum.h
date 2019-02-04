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
 * Iteration for bignum.
 ********************************/

/**
 * Interate every nibble in a bignum.
 *
 * @param _byte_decl        Declaration of byte variable.
 * @param _bignum_st_ptr    Pointer to struct BIGNUM.
 *
 * Usage:
 * foreach_bn(byte item, my_bignum) { ... }
 */
#define foreach_bn(_byte_decl, _bignum_st_ptr)                      \
for (bn_t _index = 0,                                               \
_keep = 1;                                                          \
_keep && (_index != _bignum_st_ptr->_length);                       \
_keep = !_keep, ++_index)                                           \
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
 * foreach_bn(byte item, my_bignum) { ... }
 */
#define foreach_bn_r(_byte_decl, _bignum_st_ptr)                    \
for (bn_t _index = _bignum_st_ptr->_length - 1,                     \
_keep = 1, _done = 0;                                               \
_keep && !_done;                                                    \
_keep = !_keep, _done = !_index, --_index)                          \
for(_byte_decl = get_nibble_at(_bignum_st_ptr->_nums, _index);      \
_keep;                                                              \
_keep = !_keep)


/********************************
 * Type definitions.
 ********************************/

typedef uint8_t byte;
typedef long bn_t;

#define BN_T_FMT "%ld"
#define BN_T_MAX INT_SIGN_MAX(bn_t)

/**
 * Big number container.
 * Use little endian.
 * Index zero is LSB.
 */
typedef struct _big_num {
    bn_t    _length;        /* Length of number. Same as number of digits in decimal. */
    bn_t    _alloc_size;    /* Actually allocated size of byte array. Always 1 bigger than _length.*/
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
    BIGNUM * bn_new_length(bn_t _length);
    
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
    BIGNUM * bn_realloc_increase(BIGNUM * _num, bn_t _add);
    
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
    BIGNUM * bn_realloc_decrease(BIGNUM * _num, bn_t _sub);
    
    /**
     * Reallocate bignum with length.
     *
     * @param _num      Bignum to reallocate.
     * @param _length   Destination length(number of digits).
     *
     * @return          Reallocated bignum.
     *                  NULL when _num is NULL or _length is zero or allocation is failed.
     */
    BIGNUM * bn_realloc(BIGNUM * _num, bn_t _length);
    
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
     * NULL will propagate to caller.
     *
     * @param _num      Bignum to free.
     */
    BOOL bn_free(BIGNUM ** _num);

    
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
     * @return          Number of characters printed.
     *                  Negative when parameter is(are) null or failed fprintf.
     */
    int bn_fprint(FILE * _stream, BIGNUM * _num);
    
    /**
     * Print bignum to stdout.
     *
     * @param _num      Bignum to print.
     *
     * @return          Number of characters printed.
     *                  Negative when parameter is(are) null or failed fprintf.
     */
    int bn_print(BIGNUM * _num);
    
    
    /***********************************************************
     * Properties of BIGNUM.
     ***********************************************************/
    
    /*
     * Get validity of bignum.
     * Check itself, its length, and its nums array.
     *
     * @param _num      Bignum to check.
     * @return          Zero when any of condition is not met.
     */
    BOOL bn_valid(BIGNUM * _num);
    
    /**
     * Return length of bignum.
     *
     * @param _source   Source bignum.
     *
     * @return          0 when _source is null.
     */
    bn_t bn_length(BIGNUM * _source);
    
    /**
     * Return sign of bignum.
     *
     * @param _source   Source bignum.
     *
     * @return          1 when positive, -1 when negative, 0 when _source is null.
     */
    int bn_sign(BIGNUM * _source);
    
    
    /***********************************************************
     * Decimal arithmetic.
     ***********************************************************/
    
    /**
     * Add two bignums.
     *
     * @param _left     Bignum at left.
     * @param _right    Bignum at right.
     *
     * @return          New bignum from addition.
     */
    BIGNUM * bn_add(BIGNUM * _left, BIGNUM * _right);
    
    /**
     * Subtraction of two bignums.
     *
     * @param _left     Bignum at left.
     * @param _right    Bignum at right.
     *
     * @return          New bignum from subtraction of _right from _left (_left - _right).
     */
    BIGNUM * bn_sub(BIGNUM * _left, BIGNUM * _right);

    /**
     * Compare two bignums.
     *
     * @param _left     Bignum at left.
     * @param _right    Bignum at right.
     *
     * @return          1 when _left is bigger, -1 when _right is bigger, 0 when same.
     */
    int bn_comp(BIGNUM * _left, BIGNUM * _right);
    
#ifdef __cplusplus
}
#endif

#endif /* _bignum_h */
