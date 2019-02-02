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
    new_bn->_is_negative = FALSE;
    new_bn->_nums = (byte *)malloc(new_bn->_alloc_size);
    new_bn->_nums[0] = BYTE(0, 0);
    new_bn->_string_out = NULL;
    
    return new_bn;
}

BIGNUM * bn_from_string(char * _source) {
    if (_source == NULL) {
        ERROR("bn_from_string: _source is null.");
        return FALSE;
    }
    
    // Preprocess
    size_t srclen       = strlen(_source); /* must be null-terminated. */
    size_t offset       = 0;
    char current_char   = '\0';
    
    BOOL all_zero       = TRUE;
    BOOL valid          = FALSE;
    BOOL negative       = FALSE;
    
    for (int i = 0; i < srclen; ++ i) {
        current_char = _source[i];
        
        // sign check: negative
        if (current_char == '-') {
            if (i == 0) {
                negative = TRUE;
            }
            else {
                ERROR("bn_from_string: _source has wrong negative sign character.");
                return FALSE; /* got '-' but not the first character. */
            }
        }
        
        // sign check: positive
        else if (current_char == '+') {
            if (i == 0) {
                negative = FALSE;
            }
            else {
                ERROR("bn_from_string: _source has wrong positive sign character.");
                return FALSE; /* got '+' but not the first character. */
            }
        }
        
        // non-number check
        else if (current_char < '0' || current_char > '9') {
            ERROR("bn_from_string: _source has non-number character.");
            return FALSE;
        }
        
        // all-zero check
        else if (current_char != '0') {
            all_zero = FALSE;
            if (!valid) {
                offset = i; /* save offset where first got non-zero number. */
                valid = TRUE;
            }
        }
    }
    
    // Postprocess
    if (all_zero) {
        offset = srclen - 1;
        negative = FALSE; /* no -0. */
    }
    
    BIGNUM * dest = bn_new();
    
    dest->_length = srclen - offset;
    dest->_alloc_size = srclen - offset + 1;
    dest->_is_negative = negative;
    dest->_nums = (byte *)realloc(dest->_nums, dest->_alloc_size);
    if (dest->_nums == NULL) {
        ERROR("bn_str2bn: realloc failed.");
        return FALSE;
    }
    
    for (int i = 0; i < srclen - offset; ++ i) {
        set_nibble_at(dest->_nums, i, _source[i + offset] - '0');
    }
    
    return dest;
}

BIGNUM * bn_from_integer(long long int _source) {
    return NULL;
}

char * bn_to_string(BIGNUM * _source) {
    if (_source == NULL) {
        ERROR("bn_bn2str: _source is null.");
        return NULL;
    }
    
    if (_source->_string_out != NULL) {
        return _source->_string_out;
    }
    
    BOOL negative       = _source->_is_negative;
    int n_padding       = negative ? 1 : 0;
    size_t alloc_size   = _source->_length + n_padding + 1;
    
    char * string       = (char *)malloc(alloc_size);
    memset(string, 0, alloc_size);
    
    if (negative) {
        string[0] = '-';
    }
    
    foreach_num(byte digit, _source) {
        string[_index + n_padding] = digit + '0';
    }
    string[_source->_length + n_padding] = '\0';
    
    _source->_string_out = string; /* keep the pointer. */
    
    return string;
}

long long int bn_to_integer(BIGNUM * _source) {
    if (_source == NULL) {
        ERROR("bn_to_integer: _source is null.");
        return 0;
    }
    
    int long_long_max_digits = floor(log10(llabs(LONG_LONG_MAX))) + 1;
    
    // Todo: check range precisely.
    if (_source->_length > long_long_max_digits) {
        ERROR("bn_to_integer: bignum exceeds range of long long int");
        return 0;
    }

    long long int number = 0;
    long long int power = 1;
    
    foreach_num_r(byte digit, _source) {
        number += digit * power;
        
        if (number < 0) {
            ERROR("bn_to_integer: overflow.");
            return 0;
        }
        
        power *= 10;
    }
    
    return number;
}

int bn_fprint(FILE * _stream, BIGNUM * _num) {
    if (!_stream) {
        ERROR("bn_fprint: _stream is null.");
        return -1;
    }
    if (!_num) {
        ERROR("bn_fprint: _num is null.");
        return -1;
    }
    
    char * str = bn_to_string(_num);
    
    return fprintf(_stream, "%s\n", str);
}

int bn_print(BIGNUM * _num) {
    return bn_fprint(stdout, _num);
}

size_t bn_length(BIGNUM * _source) {
    if (_source == NULL) {
        ERROR("bn_len: _source is null.");
        return 0;
    }
    
    return _source->_length;
}

int bn_sign(BIGNUM * _source) {
    if (_source == NULL) {
        ERROR("bn_is_negative: _source is null.");
        return 0;
    }
    
    return (_source->_is_negative ? -1 : 1);
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

void bn_realloc_increase(BIGNUM * _num, size_t _size) {
    
}

void bn_realloc_decrease(BIGNUM * _num, size_t _size) {
    
}

void bn_realloc(BIGNUM * _num, size_t _size) {
    
}

void bn_free(BIGNUM * _num) {
    if (!_num) {
        ERROR("bn_free: _num is null.");
        return;
    }
    
    if (_num->_string_out != NULL) {
        free(_num->_string_out);
    }
    
    if (_num->_nums != NULL) {
        free(_num->_nums);
    }
    
    free(_num);
}

