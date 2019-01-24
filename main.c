//
//  main.c
//  big_calc
//
//  Created by POTADOS on 23/01/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#include "bignum.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    BIGNUM *bn = bn_new();
    bn_stob(bn, "1234");

    printf("yeah %d\n", get_nibble_at(bn->_nums, 1));
    
    set_nibble_at(bn->_nums, 1, 9);
    
    printf("yeah now %d\n", get_nibble_at(bn->_nums, 1));

    
    char * bn_in_string = bn_btos(bn);
    
    printf("length of bn is %ld\n", bn_len(bn));
    printf("content of bn is %s\n", bn_in_string);
    
    free(bn_in_string);
    
    foreach_num(bn, _myd) {
        printf("digit at %zu is %d.\n", _index, _myd);
    }
    
    puts("");
    
    foreach_num_r(bn, _mydr) {
        printf("digit at %zu is %d.\n", _index, _mydr);
    }
    
    return 0;
}

