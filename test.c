//
//  test.c
//  big_calc
//
//  Created by POTADOS on 25/01/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#include "test.h"

BOOL input_creation_test() {
  puts("input_creation_test.");
  puts("type q to quit test.");

  int bn_arr_ptr = 0;
  BOOL test_successful = FALSE;
  BIGNUM *bn_arr[255];
  memset(bn_arr, 0, sizeof(bn_arr));

  char ibuf[255];

  for(;;) {
    memset(ibuf, 0, sizeof(ibuf));
    printf("input number > ");
    if (fgets(ibuf, sizeof(ibuf) - 1, stdin) == NULL) {
      ERROR("input_creation_test: fgets failed.");
      test_successful = FALSE;
      goto free_and_quit;
    }
    ibuf[strlen(ibuf) - 1] = '\0'; /* kill newline */

    if (strlen(ibuf) == 0) {
      // got empty string.
      continue;
    }
    if (strcmp(ibuf, "q") == 0) {
      // got "q".
      printf("Free and quit.\n");
      test_successful = TRUE;
      goto free_and_quit;
    }

    bn_arr[bn_arr_ptr] = bn_new();
    if (! bn_str2bn(bn_arr[bn_arr_ptr], ibuf)) {
      continue;
    }

    printf("Added ");
    bn_print(stdout, bn_arr[bn_arr_ptr]);

    printf("Whole list:\n");
    for(int i = 0; i <= bn_arr_ptr; ++ i) {
      bn_print(stdout, bn_arr[i]);
    }

    bn_arr_ptr ++;
  }

  free_and_quit: {
    for(int i = 0; i < bn_arr_ptr; ++ i) {
      bn_free(bn_arr[i]);
    }
    return test_successful;
  }
}

BOOL single_num_str_test() {
  BIGNUM * bn = bn_new();
  bn_str2bn(bn, "12345");

  bn_print(stdout, bn);
  bn_print(stdout, bn);
  bn_print(stdout, bn);
  bn_print(stdout, bn);

  return TRUE;
}
