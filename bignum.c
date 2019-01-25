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
    new_bn->_string = NULL;

    return new_bn;
}

BOOL bn_str2bn(BIGNUM * _dest, char * _source) {
    if (!_dest) {
        ERROR("bn_str2bn: _dest is null.");
        return FALSE;
    }
    if (!_source) {
        ERROR("bn_str2bn: _source is null.");
        return FALSE;
    }

    // preprocess
    size_t srclen = strlen(_source); /* it must be null-terminated. */
    size_t offset = 0;
    char current_char = '\0';
    BOOL all_zero = TRUE;
    BOOL valid = FALSE;
    for (int i = 0; i < srclen; ++ i) {
        current_char = _source[i];

        if (current_char != '0') {
            all_zero = FALSE;
            if (!valid) {
                offset = i; /* save offset where first got non-zero number. */
                valid = TRUE;
            }
        }
        if (current_char < '0' || current_char > '9') {
            ERROR("bn_str2bn: _source has non-number character.");
            return FALSE;
        }
    }

    if (all_zero) {
      offset = srclen - 1;
    }

    _dest->_length = srclen - offset;
    _dest->_alloc_size = srclen - offset + 1;
    _dest->_nums = (byte *)realloc(_dest->_nums, _dest->_alloc_size);
    if (! _dest->_nums) {
        ERROR("bn_str2bn: realloc failed.");
        return FALSE;
    }

    for (int i = 0; i < srclen - offset; ++ i) {
        set_nibble_at(_dest->_nums, i, _source[i + offset] - '0');
    }

    return TRUE;
}

char * bn_bn2str(BIGNUM * _source) {
    if (!_source) {
        ERROR("bn_bn2str: _source is null.");
        return NULL;
    }

    if (_source->_string != NULL) {
      return _source->_string;
    }

    char * string = (char *)malloc(_source->_length + 1);
    memset(string, 0, _source->_length + 1);

    foreach_num(byte digit, _source) {
        string[_index] = digit + '0';
    }
    string[_source->_length] = '\0';

    _source->_string = string; /* keep the pointer. */

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

    char * str = bn_bn2str(_num);
    fprintf(_stream, "%s\n", str);
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
    if (!_num) {
        ERROR("bn_free: _num is null.");
        return;
    }

    if (_num->_string != NULL) {
      free(_num->_string);
    }

    if (_num->_nums != NULL) {
      free(_num->_nums);
    }

    free(_num);
}
