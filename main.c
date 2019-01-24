//
//  main.c
//  big_calc
//
//  Created by POTADOS on 23/01/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#include "bignum.h"

int main(int argc, const char * argv[]) {
    BIGNUM *bn = bn_new();
    bn_str2bn(bn, "1234");
    
    char * bn_in_string = bn_bn2str(bn);
    printf("length of bn is %ld\n", bn_len(bn));
    printf("content of bn is %s\n", bn_in_string);
    free(bn_in_string);
    
    foreach_num(bn, _myd) {
        printf("digit at %zu is %d.\n", _index, _myd);
    }
    
    return 0;
}

