//
//  main.c
//  big_calc
//
//  Created by POTADOS on 23/01/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#include "bignum.h"
#include "test.h"

int main(int argc, const char * argv[]) {
    
    // bignum_stack_test();
    // bignum_conversion_test();

    // int_test();
    
     add_test();
    
     sub_test();
    
    // comp_test();
    
     big_arith_test();

     // printf(BN_T_FMT "\n", BN_T_MAX);
    
    // printf("%lld\n", ((long long)((long long)1 << 63) - 1));
    return 0;
}
