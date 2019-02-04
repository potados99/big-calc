//
//  bignum.c
//  big_calc
//
//  Created by POTADOS on 23/01/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#include "bignum.h"

BIGNUM * bn_new_length(size_t _length) {
    verify(_length != 0, NULL, "Cannot create bignum with length 0.");

    BIGNUM * new_bn;
    verify((new_bn = (BIGNUM *)malloc(sizeof(BIGNUM) + 1)), NULL, "Allocation for new_bn failed.");
    
    // Initialize
    new_bn->_length         = 0;
    new_bn->_alloc_size     = 0;
    new_bn->_is_negative    = 0;
    new_bn->_nums           = 0;
    new_bn->_string_out     = 0;
    
    // Allocate
    new_bn->_length = _length;
    new_bn->_alloc_size = sizeof(byte) * (NINB(new_bn->_length) + (size_t)ALLOC_PADDING);
    verify((new_bn->_nums = (byte *)malloc(new_bn->_alloc_size)), NULL, "Allocation for bn_new->_nums failed.");
    
    // Clear
    verify(bn_valid(bn_clear(new_bn)), NULL, "bn_clear failed.");

    return new_bn;
}

BIGNUM * bn_new(void) {
    return bn_new_length(1);
}

BIGNUM * bn_realloc_increase(BIGNUM * _num, size_t _add) {
    verify(bn_valid(_num), NULL, "_num is invalid.");
    
    return _add ? bn_realloc(_num, _num->_length + _add) : _num;
}

BIGNUM * bn_realloc_decrease(BIGNUM * _num, size_t _sub) {
    verify(bn_valid(_num), NULL, "_num is invalid.");
    
    return _sub ? bn_realloc(_num, _num->_length - _sub) : _num;
}

BIGNUM * bn_realloc(BIGNUM * _num, size_t _length) {
    verify(_length != 0, NULL, "_length is zero.");
    verify(bn_valid(_num), NULL, "_num is invalid.");
    
    _num->_length = _length;
    _num->_alloc_size = sizeof(byte) * (NINB(_num->_length) + (size_t)ALLOC_PADDING);
    _num->_nums = (byte *)realloc(_num->_nums, _num->_alloc_size);
    verify(bn_valid(_num), NULL, "Failed to reallocate _num->_nums.");

    return _num;
}

BIGNUM * bn_clear(BIGNUM * _num) {
    verify(bn_valid(_num), NULL, "_num is invalid.");
    
    memset(_num->_nums, 0, _num->_alloc_size);  /* _nums */
    
    _num->_is_negative = FALSE;                 /* _is_negative */
    
    if (_num->_string_out != NULL) {
        printf("ostr: %p\n", _num->_string_out);
    }
    _num->_string_out = NULL;                   /* _string_out */
    
    return _num;
}

BOOL bn_free(BIGNUM ** _num) {
    verify(bn_valid((*_num)), FALSE, "_num is invalid.");
    
    if ((*_num)->_string_out != NULL) {
        free((*_num)->_string_out); /* optional */
    }
    
    free((*_num)->_nums);
    free((*_num));
    
    *_num = NULL;
    
    return TRUE;
}


BIGNUM * bn_from_string(char * _source) {
    verify(_source, FALSE, "_source is null.");
    
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
                error(FALSE, "_source has wrong negative sign character.");
            }
        }
        
        // sign check: positive
        else if (current_char == '+') {
            if (i == 0) {
                negative = FALSE;
            }
            else {
                error(FALSE, "_source has wrong positive sign character.");
            }
        }
        
        // non-number check
        else if (current_char < '0' || current_char > '9') {
            error(FALSE, "_source has non-number character.");
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
    
    for (size_t i = 0; i < srclen - offset; ++ i) {
        set_nibble_at(dest->_nums, i, _source[srclen - 1 - i] - '0'); /* little endian */
    }
    
    return dest;
}

BIGNUM * bn_from_integer(long long int _source) {
    if (_source == 0) {
        return bn_new();
    }
    
    size_t source_digits = INT_LEN(_source);
    
    BIGNUM * dest;
    dest = bn_new_length(source_digits);
    
    BOOL source_is_negative = (_source < 0);
    _source = (source_is_negative ? -_source : _source); /* abs */
    dest->_is_negative = source_is_negative;

    foreach_int_from_lsb(byte digit, _source) {
        set_nibble_at(dest->_nums, _digit_place, digit); /* little endian */
    }
    
    return dest;
}


char * bn_to_string(BIGNUM * _source) {
    verify(bn_valid(_source), NULL, "_source is invalid.");
    
    if (_source->_string_out != NULL) {
        return _source->_string_out;
    }
    
    BOOL negative       = _source->_is_negative;
    int n_padding       = negative ? 1 : 0;
    size_t len          = _source->_length;
    size_t alloc_size   = len + n_padding + 1;
    
    char * string       = (char *)malloc(alloc_size);
    memset(string, 0, alloc_size);
    
    if (negative) {
        string[0] = '-';
    }
    
    foreach_bn(byte digit, _source) {
        string[(len - 1 - _index) + n_padding] = digit + '0'; /* little endian */
    }
    string[_source->_length + n_padding] = '\0';
    
    _source->_string_out = string; /* keep the pointer. */
    
    return string;
}

long long int bn_to_integer(BIGNUM * _source) {
    verify(bn_valid(_source), 0, "_source is invalid.");
    
    int long_long_max_digits = floor(log10(llabs(LLONG_MAX))) + 1;
    
    // Todo: check range precisely.
    verify(_source->_length <= long_long_max_digits, 0, "Bignum exceeds range of long long int");

    long long int number = 0;
    long long int power = 1;
    
    foreach_bn(byte digit, _source) { /* little endian */
        number += digit * power;
        
        verify(number >= 0, 0, "Overflow!");
        
        power *= 10;
    }
    
    return number * (_source->_is_negative ? -1 : 1);
}

int bn_fprint(FILE * _stream, BIGNUM * _num) {
    verify(_stream, -1, "_stream is null.");
    verify(bn_valid(_num), -1, "_num is invalid.");
    
    return fputs(bn_to_string(_num), _stream);
}

int bn_print(BIGNUM * _num) {
    return bn_fprint(stdout, _num);
}


size_t bn_length(BIGNUM * _source) {
    verify(bn_valid(_source), 0, "_source is invalid.");
    
    return _source->_length;
}

int bn_sign(BIGNUM * _source) {
    verify(bn_valid(_source), 0, "_source is invalid.");

    return (_source->_is_negative ? -1 : 1);
}


static BIGNUM * _bn_abs_add(BIGNUM * _positive_left, BIGNUM * _positive_right) {
    BIGNUM * _left = _positive_left;
    BIGNUM * _right = _positive_right;
    
    BOOL left_is_bigger = (_left->_length > _right->_length);
    size_t big_len = (left_is_bigger) ? _left->_length : _right->_length;
    size_t small_len = (left_is_bigger) ? _right->_length : _left->_length;
    
    BIGNUM * result = bn_new_length(big_len);

    byte left_nibble    = 0;
    byte right_nibble   = 0;
    byte addition       = 0;
    BOOL carry          = 0;
    
    for (size_t i = 0; i < big_len; ++i) {
        left_nibble     = 0;
        right_nibble    = 0;
        addition        = 0;
        
        if (i < small_len) {
            // together
            left_nibble = get_nibble_at(_left->_nums, i);
            right_nibble = get_nibble_at(_right->_nums, i);
        }
        else {
            // alone
            if (left_is_bigger) {
                left_nibble = get_nibble_at(_left->_nums, i);
            }
            else {
                right_nibble = get_nibble_at(_right->_nums, i);
            }
        }
        
        addition = left_nibble + right_nibble + (carry ? 1 : 0);
        if (addition > 9) {
            addition -= 10;
            carry = TRUE;
        }
        else {
            carry = FALSE;
        }
        
        set_nibble_at(result->_nums, i, addition);
        
        if (carry && (i == big_len - 1)) {
            // got carry on last digit.
            bn_realloc_increase(result, 1);
            
            set_nibble_at(result->_nums, i + 1 , 1);
        }
        
    } /* end of for */
    
    return result;
}

static BIGNUM * _bn_abs_sub(BIGNUM * _positive_bigger_left, BIGNUM * _positive_right) {
    BIGNUM * _left = _positive_bigger_left; /* every caller calls this function with left always bigger. */
    BIGNUM * _right = _positive_right;
    
    BIGNUM * result = bn_new_length(_left->_length);
    
    byte left_nibble    = 0;
    byte right_nibble   = 0;
    byte subtraction    = 0;
    BOOL borrow         = 0;
    BOOL borrow_over    = 0;
    
    for (size_t i = 0; i < _left->_length; ++i) {
        left_nibble     = 0;
        right_nibble    = 0;
        subtraction     = 0;
        
        if (i < _right->_length) {
            // together
            left_nibble = get_nibble_at(_left->_nums, i);
            right_nibble = get_nibble_at(_right->_nums, i);
        }
        else {
            // alone
            left_nibble = get_nibble_at(_left->_nums, i);
        }
        
        if (borrow_over) {
            // this digit got a borrow from next digit.
            
            if (left_nibble == 0) {
                left_nibble = 9;
                borrow_over = TRUE;
            }
            else {
                --left_nibble;
                borrow_over = FALSE;
                }
        }
        
        if (borrow) {
            // previous digit got a borrow from this digit.
            
            if (left_nibble == 0) {
                left_nibble = 9;
                borrow_over = TRUE;
            }
            else {
                --left_nibble;
                borrow_over = FALSE;
            }
        }
        
        borrow = (left_nibble < right_nibble);
        subtraction = (borrow ? 10 : 0) + left_nibble - right_nibble;
        
        set_nibble_at(result->_nums, i, subtraction);
        
    } /* end of for */
    
    size_t actual_len = 0;
    foreach_bn(byte d, result) { /* little endian. from LSB. */
        if (d != 0) { /* crop zeros */
            actual_len = _index + 1;
        }
    }
    
    bn_realloc(result, actual_len); /* decrease. */
    
    return result;
}

static int _bn_abs_comp(BIGNUM * _positive_left, BIGNUM * _positive_right) {
    BIGNUM * _left = _positive_left;
    BIGNUM * _right = _positive_right;
    
    long long int length_dif = _left->_length - _right->_length; /* sign, be careful! */
    
    if (length_dif > 0) {
        return 1;
    }
    else if (length_dif < 0) {
        return -1;
    }
    
    // same length
    foreach_bn_r(byte left_digit, _left) { /* from MSB */
        int digit_dif = left_digit - get_nibble_at(_right->_nums, _index); /* Be careful for that sign! */
        if (digit_dif > 0) {
            // left bigger
            return 1;
        }
        else if (digit_dif < 0) {
            // right bigger
            return -1;
        }
        else {
            // do nothing
        }
    }
    
    return 0;
}

BIGNUM * bn_add(BIGNUM * _left, BIGNUM * _right) {
    verify(bn_valid(_left), 0, "_left is invalid.");
    verify(bn_valid(_right), 0, "_right is invalid.");
    
    BIGNUM * result = NULL;
    
    if (_left->_is_negative == _right->_is_negative) {
        // same sign.
        
        if (_left->_is_negative) {
            // - -
            result = _bn_abs_add(_left, _right);
            result->_is_negative = TRUE; /* obvious. */
        }
        else {
            // + +
            result = _bn_abs_add(_left, _right);
            result->_is_negative = FALSE; /* obvious. */
        }
    }
    else {
        // different sign.
        
        int comp = _bn_abs_comp(_left, _right);
        if (comp == 0) {
            return bn_new_length(1); /* result is zero. */
        }
        else if (comp != 1 && comp != -1) {
            error(NULL, "Comparison failed.");
        }
        
        if (_left->_is_negative) {
            // - +
            result = _bn_abs_sub(_right, _left);
            
            if (comp == 1) {
                // when left is bigger.
                // (-bigger + smaller) = negative.
                result->_is_negative = TRUE;
            }
            else if (comp == -1) {
                // when left is smaller.
                // (-smaller + bigger) = positive.
                result->_is_negative = FALSE;
            }
            else {
                fatal(1, "Control flow error!"); /* THIS CANNOT HAPPEN. */
            }
        }
        else {
            // + -
            result = _bn_abs_sub(_left, _right);
            
            if (comp == 1) {
                // when left is bigger.
                // (bigger - smaller) = positive.
                result->_is_negative = FALSE;
            }
            else if (comp == -1) {
                // when left is smaller.
                // (smaller - bigger) = negative.
                result->_is_negative = TRUE;
            }
            else {
                fatal(1, "Control flow error!"); /* THIS CANNOT HAPPEN. */
            }
        }
    }
    
    return result;
}

BIGNUM * bn_sub(BIGNUM * _left, BIGNUM * _right) {
    verify(bn_valid(_left), 0, "_left is invalid.");
    verify(bn_valid(_right), 0, "_right is invalid.");
    
    BIGNUM * result = NULL;
    
    if (_left->_is_negative == _right->_is_negative) {
        // same sign.
        
        int comp = _bn_abs_comp(_left, _right);
        if (comp == 0) {
            return bn_new_length(1); /* result is zero. */
        }
        else if (comp != 1 && comp != -1) {
            error(NULL, "Comparison failed.");
        }
        
        if (_left->_is_negative) {
            // - -
            
            if (comp == 1) {
                // when abs left is bigger.
                // (-bigger + smaller) = negative.
                result = _bn_abs_sub(_left, _left);
                result->_is_negative = TRUE;
            }
            else if (comp == -1){
                // when abs left is smaller.
                // (-smaller + bigger) = positive.
                result = _bn_abs_sub(_right, _left);
                result->_is_negative = FALSE;
            }
            else {
                fatal(1, "Control flow error!"); /* THIS CANNOT HAPPEN. */
            }
        }
        else {
            // + +
            
            if (comp == 1) {
                // when abs left is bigger.
                // bigger - smaller is positive.
                result = _bn_abs_sub(_left, _right);
                result->_is_negative = FALSE;
            }
            else if (comp == -1){
                // when abs left is smaller.
                // smaller - bigger is negative.
                result = _bn_abs_sub(_right, _left);
                result->_is_negative = TRUE;
            }
            else {
                fatal(1, "Control flow error!"); /* THIS CANNOT HAPPEN. */
            }
        }
    }
    else {
        // different sign.
        
        if (_left->_is_negative) {
            // - +
            result = _bn_abs_add(_left, _right);
            result->_is_negative = TRUE; /* obvious. */
        }
        else {
            // + -
            result = _bn_abs_add(_left, _right);
            result->_is_negative = FALSE; /* obvious. */
        }
    }
    
    return result;
}

int bn_comp(BIGNUM * _left, BIGNUM * _right) {
    verify(bn_valid(_left), 0, "_left is invalid.");
    verify(bn_valid(_right), 0, "_right is invalid.");
    
    if (_left->_is_negative == _right->_is_negative) {
        // same sign.
        
        if (_left->_is_negative) {
            // - -
            return _bn_abs_comp(_left, _right) * -1; /* reverse comparison result of abs. */
        }
        else {
            // + +
            return _bn_abs_comp(_left, _right);
        }
    }
    else {
        // different sign.
        
        if (_left->_is_negative) {
            // - +
            return -1;
        }
        else {
            // + -
            return 1;
        }
    }
    
    return 0;
}
