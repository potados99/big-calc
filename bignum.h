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

// Nibbles in bytes.
// Get how many bytes are needed to store nibbles.
// n:               integer.
#define NINB(n)                                                     \
(((n) >> 1) + 1)

// Byte index.
// Get index of byte where nibble is at.
// n:               integer, index of nibble.
#define BIDX(n)                                                     \
((n) >> 1)

// Get high nibble of byte.
// b:               byte, value of a byte.
#define HIGH_NIBBLE(b)                                              \
(((b) >> 4) & 0x0F)

// Get low nibble of byte.
// b:               byte, value of a byte.
#define LOW_NIBBLE(b)                                               \
((b) & 0x0F)

// Get a byte consists of high nibble and low nibble
// high:            nibble, value of a high nibble.
// low:             nibble, value of a low nibble.
#define BYTE(high, low)                                             \
(((high) << 4) | LOW_NIBBLE(low))

// Set high nibble of a byte at bp(byte pointer).
// bp:              pointer to byte, pointer indicating a target byte.
// n:               nibble, value of a high nibble.
#define SET_HIGH_NIBBLE(bp, n)                                      \
do {                                                                \
*(bp) = BYTE((n), LOW_NIBBLE(*(bp)));                               \
} while(0)

// Set low nibble of a byte at bp(byte pointer).
// bp:              pointer to byte, pointer indicating a target byte.
// n:               nibble, value of a low nibble.
#define SET_LOW_NIBBLE(bp, n)                                       \
do {                                                                \
*(bp) = BYTE(HIGH_NIBBLE(*(bp)), (n));                              \
} while(0)


/********************************
 * User level nibble operations.
 ********************************/

// Get nibble in array at specific index.
// arr:             pointer to byte, points to a start of bytes array.
// index:           integer, index of nibble.
#define get_nibble_at(arr, index)                                   \
((index % 2) ?                                                      \
LOW_NIBBLE(*((arr) + (BIDX(index)))) :                              \
HIGH_NIBBLE(*((arr) + (BIDX(index)))))

// Set nibble in array at specific index.
// arr:             pointer to byte, points to a start of bytes array.
// index:           integer, index of nibble.
// val:             nibble, value to be set for nibble in a target byte.
#define set_nibble_at(arr, index, val)                              \
do {                                                                \
if ((index) % 2) {                                                  \
SET_LOW_NIBBLE(((arr) + BIDX(index)), (val));                       \
}                                                                   \
else {                                                              \
SET_HIGH_NIBBLE(((arr) + BIDX(index)), (val));                      \
}                                                                   \
} while(0)

// Interate every nibble in a bignum.
// _byte_decl:      declaration of byte variable.
// _bignum_st_ptr:  pointer to struct big_num (a.k.a BIGNUM *).
// Thank you Johannes Schaub.
#define foreach_num(_byte_decl, _bignum_st_ptr)                     \
for (size_t _index = 0,                                             \
_keep = 1;                                                          \
_keep && (_index != _bignum_st_ptr->_length);                       \
_keep = !_keep,                                                     \
++ _index)                                                          \
for(_byte_decl = get_nibble_at(_bignum_st_ptr->_nums, _index);      \
_keep;                                                              \
_keep = !_keep)

// Interate every nibble in a bignum, in a reversed order.
// _byte_decl:      declaration of byte variable.
// _bignum_st_ptr:  pointer to struct big_num (a.k.a BIGNUM *).
#define foreach_num_r(_byte_decl, _bignum_st_ptr)                   \
for (size_t _index = _bignum_st_ptr->_length - 1,                   \
_keep = 1,                                                          \
_done = 0;                                                          \
_keep && !_done;                                                    \
_keep = !_keep,                                                     \
_done = !_index,                                                    \
-- _index)                                                          \
for(_byte_decl = get_nibble_at(_bignum_st_ptr->_nums, _index);      \
_keep;                                                              \
_keep = !_keep)

typedef uint8_t byte;

typedef struct _big_num {
    size_t  _length; /* max 9,223,372,036,854,775,807 digits. */
    size_t  _alloc_size;
    BOOL    _is_negative;
    byte    * _nums;
    char    * _string_out;
} BIGNUM;

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Create new bignum 0.
     *
     * @return          Pointer of allocated bignum struct.
     */
    BIGNUM * bn_new(void);
    
    /**
     * Create new bignum from string.
     *
     * @param _source   Source string.
     *
     * @return          NULL when parameter is NULL or string is invalid.
     */
    BIGNUM * bn_from_string(char * _source);
    
    /**
     * Create new bignum from int.
     *
     * @param _source   Source integer(int).
     *
     * @return          NULL when parameter is NULL or string is invalid.
     */
    BIGNUM * bn_from_int(int _source);
    
    /**
     * Create new bignum from long.
     *
     * @param _source   Source integer(long).
     *
     * @return          NULL when parameter is NULL or string is invalid.
     */
    BIGNUM * bn_from_long(long _source);
    
    /**
     * Export bignum to string.
     *
     * @param _source   Source bignum.
     *
     * return           NULL when _source is null.
     */
    char * bn_to_string(BIGNUM * _source);
    
    void bn_print(FILE * _stream, BIGNUM * _num);

    void bn_add(BIGNUM * _dest, BIGNUM * _source);

    
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
    
    void bn_realloc(BIGNUM * _num, size_t _size);
    void bn_free(BIGNUM * _num);

    
    
#ifdef __cplusplus
}
#endif

#endif /* _bignum_h */
