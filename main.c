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
    bn_str2bn(bn, "012340567890");

    printf("length of bn is %d\n", (int)bn_len(bn));
    printf("content of bn is ");
    bn_print(stdout, bn);

    return 0;
}
