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

    if (bignum_stack_test()) {
      printf("# bignum_stack_test succedded.\n");
    }
    else {
      printf("# bignum_stack_test failed.\n");
    }

    return 0;
}
