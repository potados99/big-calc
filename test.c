//
//  test.c
//  big_calc
//
//  Created by POTADOS on 25/01/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#include "test.h"

BOOL input_creation_test() {
  int bn_arr_ptr = 0;
  BIGNUM *bn_arr[255];
  memset(bn_arr, 0, sizeof(bn_arr));

  char ibuf[255];

  for(;;) {
    memset(ibuf, 0, sizeof(ibuf));
    if (fgets(ibuf, sizeof(ibuf) - 1, stdin) == NULL) {
      return FALSE;
    }
    ibuf[strlen(ibuf) - 1] = '\0'; /* kill newline */

    if (strcmp(ibuf, "quit") == 0) {
      printf("Free and quit.\n");
      for(int i = 0; i < bn_arr_ptr; ++ i) {
        bn_free(bn_arr[i]);
      }
      return TRUE;
    }
    if (strlen(ibuf) == 0) {
      continue;
    }

    bn_arr[bn_arr_ptr] = bn_new();
    bn_str2bn(bn_arr[bn_arr_ptr], ibuf);

    printf("Added %s\n", ibuf);
    printf("Whole list:\n");
    for(int i = 0; i <= bn_arr_ptr; ++ i) {
      bn_print(stdout, bn_arr[i]);
    }
    printf("\n");

    bn_arr_ptr ++;
  }
}
