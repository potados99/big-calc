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
    bn_str2bn(bn, "45678765456776543456789876543456789098723456");
    
    printf("length of bn is %ld\n", bn_len(bn));
    printf("content of bn is" );
    bn_print(stdout, bn);
    
    foreach_num(bn, _myd) {
        printf("digit at %zu is %d.\n", _index, _myd);
    }
    
    return 0;
}

