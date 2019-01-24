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
    bn_str2bn(bn, "100101010100101000");

    printf("length of bn is %ld\n", bn_len(bn));
    printf("content of bn is ");
    bn_print(stdout, bn);

    return 0;
}

