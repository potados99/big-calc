//
//  bignum.c
//  big_calc
//
//  Created by POTADOS on 23/01/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#include "bignum.h"

BIGNUM * bn_new(void) {
    BIGNUM * new_bn = (BIGNUM *)malloc(sizeof(BIGNUM) + 1);
    
    new_bn->_length = 1;
    
    new_bn->_alloc_size = NINB(new_bn->_length) + ALLOC_PADDING; /* one nibble and one space */
    new_bn->_nums = (byte *)malloc(new_bn->_alloc_size);
    
    new_bn->_nums[0] = BYTE(0, 0);
    
    return new_bn;
}

void bn_stob(BIGNUM * _dest, char * _source) {
    if (!_dest) {
        ERROR("bn_stob: _dest is null.");
        return;
    }
    if (!_source) {
        ERROR("bn_stob: _source is null.");
        return;
    }
    
    // preprocess
    size_t srclen = strlen(_source); /* it must be null-terminated. */
    size_t offset = 0;
    BOOL valid = FALSE;
    for (int i = 0; i < srclen; ++ i) {
        if (_source[i] != '0' && !valid) {
            offset = i;
            valid = TRUE;
        }
        if (_source[i] < '0' || _source[i] > '9') {
            ERROR("bn_stob: _source has non-number character.");
            return;
        }
    }
    
    _dest->_length = srclen - offset;
    _dest->_alloc_size = srclen - offset + 1;
    _dest->_nums = (byte *)realloc(_dest->_nums, _dest->_alloc_size);
    if (! _dest->_nums) {
        ERROR("bn_stob: realloc failed.");
        return;
    }
    
    for (int i = 0; i < srclen - offset; ++ i) {
        set_nibble_at(_dest->_nums, i, _source[i + offset] - '0');
    }
}

char * bn_btos(BIGNUM * _source) {
    if (!_source) {
        ERROR("bn_btos: _source is null.");
        return NULL;
    }
    
    char * string = (char *)malloc(_source->_length + 1);
    memset(string, 0, _source->_length + 1);
    
    foreach_num(_source, digit) {
        string[_index] = digit + '0';
    }
    string[_source->_length] = '\0';
    
    return string;
}

size_t bn_len(BIGNUM * _dest) {
    if (!_dest) {
        ERROR("bn_len: _dest is null.");
        return 0;
    }
    
    return _dest->_length;
}

void bn_print(FILE * _stream, BIGNUM * _num) {
    if (!_stream) {
        ERROR("bn_print: _stream is null.");
        return;
    }
    if (!_num) {
        ERROR("bn_print: _num is null.");
        return;
    }
    
    char * str = bn_btos(_num);
    fprintf(_stream, "%s\n", str);
    
    free(str);
}

void bn_add(BIGNUM * _dest, BIGNUM * _source) {
    if (!_dest) {
        ERROR("bn_add: _dest is null.");
        return;
    }
    if (!_source) {
        ERROR("bn_add: _source is null.");
        return;
    }
    
    
    /*
     
     1_2 3_4 5_6 7      A
    +
       5_4 2_3 1_3      B
    ---------------
     1_7 7_6 8_8 0      C
     
     nums[i]
     
     
     NIBBLE_AT(nums, 3) = GLN(nums[3 / 2])
     
     
     
     SET_NIBBLE_AT(nums, index, val) =
     
     do {
     if (index % 2) {
     // low
     
     }
     else {
     // high
     }
     
     
     
     } while(0)
     
     
     
     
     
     500
     499
     
     
     */
    
}

void bn_realloc(BIGNUM * _num, size_t _size) {
    
}
void bn_free(BIGNUM * _num) {
    
}
