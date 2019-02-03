//
//  bignum.c
//  big_calc
//
//  Created by POTADOS on 23/01/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#include "bignum.h"

BIGNUM * bn_new_length(size_t _length) {
    if (_length == 0) {
        ERROR("bn_new_length: Cannot create bignum with length 0.");
        return NULL;
    }
    
    BIGNUM * new_bn;
    if ((new_bn = (BIGNUM *)malloc(sizeof(BIGNUM) + 1)) == NULL) {
        ERROR("bn_new_length: allocation for new_bn failed.");
        return NULL;
    }
    
    // Initialize
    new_bn->_length         = 0;
    new_bn->_alloc_size     = 0;
    new_bn->_is_negative    = 0;
    new_bn->_nums           = 0;
    new_bn->_string_out     = 0;
    
    // Allocate
    new_bn->_length = _length;
    new_bn->_alloc_size = sizeof(byte) * (NINB(new_bn->_length) + (size_t)ALLOC_PADDING);
    if ((new_bn->_nums = (byte *)malloc(new_bn->_alloc_size)) == NULL) {
        ERROR("bn_new_length: allocation for bn_new->_nums failed.");
        return NULL;
    }
    
    // Clear
    if (bn_clear(new_bn) == NULL) {
        ERROR("bn_new_length: bn_clear failed.");
        return NULL;
    }

    return new_bn;
}

BIGNUM * bn_new(void) {
    return bn_new_length(1);
}

BIGNUM * bn_realloc_increase(BIGNUM * _num, size_t _add) {
    if (_num == NULL) {
        ERROR("bn_realloc_increase: _num is null.");
        return NULL;
    }
    
    if (_add == 0) {
        return _num;
    }
    
    return bn_realloc(_num, _num->_length + _add);
}

BIGNUM * bn_realloc_decrease(BIGNUM * _num, size_t _sub) {
    if (_num == NULL) {
        ERROR("bn_realloc_decrease: _num is null.");
        return NULL;
    }
    
    if (_sub == 0) {
        return _num;
    }
    
    return bn_realloc(_num, _num->_length - _sub);
}

BIGNUM * bn_realloc(BIGNUM * _num, size_t _length) {
    if (_num == NULL) {
        ERROR("bn_realloc: _num is null.");
        return NULL;
    }
    if (_num->_nums == NULL) {
        ERROR("bn_realloc: _num->_nums is null.");
        return NULL;
    }
    if (_length == 0) {
        ERROR("bn_realloc: Cannot reallocate bignum with length 0.");
        return NULL;
    }
    
    if (_num->_length == _length) {
        return _num;
    }
    
    _num->_length = _length;
    _num->_alloc_size = _length + 1;
    _num->_nums = (byte *)realloc(_num->_nums, _num->_alloc_size);
    
    if (_num->_nums == NULL) {
        ERROR("bn_realloc: Failed to reallocate _num->_nums.");
        return NULL;
    }
    
    if (bn_clear(_num) == NULL) {
        ERROR("bn_realloc: bn_clear failed.");
        return NULL;
    }
    
    return _num;
}

BIGNUM * bn_clear(BIGNUM * _num) {
    if (_num == NULL) {
        ERROR("bn_clear: _num is NULL.");
        return NULL;
    }
    if (_num->_nums == NULL) {
        ERROR("bn_clear: _num->_nums is NULL.");
        return NULL;
    }
    
    memset(_num->_nums, 0, _num->_alloc_size);  /* _nums */
    
    _num->_is_negative = FALSE;                 /* _is_negative */
    
    if (_num->_string_out != NULL) {
        printf("ostr: %p\n", _num->_string_out);
    }
    _num->_string_out = NULL;                   /* _string_out */
    
    return _num;
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
    
    BIGNUM * dest;
    dest = bn_new_length(srclen - offset);
    dest->_is_negative = negative;

    for (int i = 0; i < srclen - offset; ++ i) {
        set_nibble_at(dest->_nums, i, _source[i + offset] - '0');
    }
    
    return dest;
}

BIGNUM * bn_from_integer(long long int _source) {
    size_t source_digits = floor(log10(llabs(_source))) + 1;
    
    BIGNUM * dest;
    dest = bn_new_length(source_digits);
    
    BOOL source_is_negative = (_source < 0);
    _source = (source_is_negative ? -_source : _source); /* abs */
    dest->_is_negative = source_is_negative;

    for_each_int_from_lsb(byte digit, _source) {
        set_nibble_at(dest->_nums, dest->_length - 1 - _digit_place, digit);
    }
    
    return dest;
}


char * bn_to_string(BIGNUM * _source) {
    if (_source == NULL) {
        ERROR("bn_bn2str: _source is null.");
        return NULL;
    }
    if (_source->_nums == NULL) {
        ERROR("bn_bn2str: _source->_nums is null.");
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
    if (_source->_nums == NULL) {
        ERROR("bn_to_integer: _source->_nums is null.");
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
    
    number *= (_source->_is_negative ? -1 : 1);
    
    return number;
}

int bn_fprint(FILE * _stream, BIGNUM * _num) {
    if (_stream == NULL) {
        ERROR("bn_fprint: _stream is null.");
        return -1;
    }
    if (_num == NULL) {
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


BIGNUM * bn_add(BIGNUM * _left, BIGNUM * _right) {
    if (_left == NULL) {
        ERROR("bn_add: _left is null.");
        return NULL;
    }
    if (_left->_nums == NULL) {
        ERROR("bn_add: _left->_nums is null.");
        return NULL;
    }
    if (_right == NULL) {
        ERROR("bn_add: _right is null.");
        return NULL;
    }
    if (_right->_nums == NULL) {
        ERROR("bn_add: _right->_nums is null.");
        return NULL;
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
    return NULL;
}

