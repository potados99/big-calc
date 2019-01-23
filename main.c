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
    bn_stob(bn, "000000999999998765432345678987654322345678987654323456710000099");
    
    char * bn_in_string = bn_btos(bn);
    
    printf("length of bn is %ld\n", bn_len(bn));
    printf("content of bn is %s\n", bn_in_string);
    
    free(bn_in_string);
    
    bn_print(stdout, bn);

    return 0;
}
