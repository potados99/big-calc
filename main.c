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

    if (input_creation_test()) {
      printf("# input_creation_test succedded.\n");
    }
    else {
      printf("# input_creation_test failed.\n");
    }



    //single_num_str_test();


    return 0;
}
