//
//  test.c
//  big_calc
//
//  Created by POTADOS on 25/01/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#include "test.h"

BOOL bignum_stack_test() {
  puts("bignum_stack_test.");
  puts("  h       help");
  puts("  q       quit");
  puts("  v       view stack");
  puts("  <num>   push bignum to stack");

  const int stack_size = 256;
  int bn_stack_ptr = stack_size - 1;

  BOOL stack_empty = TRUE;
  BOOL stack_full = FALSE;
  BOOL test_successful = FALSE;

  BIGNUM * bn_stack[stack_size];
  memset(bn_stack, 0, sizeof(bn_stack));

  char ibuf[512];

  for(;;) {
    memset(ibuf, 0, sizeof(ibuf));
    printf("input number > ");

    rewind(stdin);
    if (fgets(ibuf, sizeof(ibuf) - 1, stdin) == NULL) {
      printf("input_creation_test: fgets failed.\n");
      test_successful = FALSE;
      goto free_and_quit;
    }
    rewind(stdin);

    ibuf[strlen(ibuf) - 1] = '\0'; /* kill newline */
    puts("");

    if (strlen(ibuf) == 0) {
      // got empty string.
      continue;
    }
    else if (strcmp(ibuf, "h") == 0) {
      puts("bignum_stack_test.");
      puts("  h       help");
      puts("  q       quit");
      puts("  v       view stack");
      puts("  <num>   push bignum to stack");

      continue;
    }
    else if (strcmp(ibuf, "q") == 0) {
      // got "q".
      test_successful = TRUE;
      if (stack_empty) {
        printf("quit.\n");
        return test_successful;
      }
      else {
        printf("Free and quit.\n");
        goto free_and_quit;
      }
    }
    else if (strcmp(ibuf, "v") == 0) {
      // got "v".
      if (stack_empty) {
        printf("stack is empty.\n");
        continue;
      }
      printf("<stack origin>\n");
      for(int i = stack_size - 1; i > bn_stack_ptr; -- i) {
        printf("<+%d>  %s\n", i, bn_to_string(bn_stack[i]));
      }
      continue;
    }

    if (stack_full) {
      printf("stack is full.\n");
      test_successful = FALSE;
      goto free_and_quit;
    }

    if (! (bn_stack[bn_stack_ptr] = bn_from_string(ibuf))) {
      continue;
    }
    stack_empty = FALSE;

    // stack view
    printf("<stack origin>\n");
    for(int i = stack_size - 1; i >= bn_stack_ptr; -- i) {
      printf("<+%d>  %s ", i, bn_to_string(bn_stack[i]));
      if (i == bn_stack_ptr) {
        printf("(added)");
      }
      printf("\n");
    }

    bn_stack_ptr --;

    // stack_empty = (bn_stack_ptr == stack_size - 1); /* no need here. */
    stack_full = (bn_stack_ptr == -1); /* means pointer was 0 and it is full now. */
  }

  free_and_quit: {
    for(int i = stack_size - 1; i > bn_stack_ptr; -- i) {
        bn_free(bn_stack[i]);
    }
    return test_successful;
  }
}

BOOL single_num_str_test() {
    BIGNUM * bn = bn_from_string("12345");
    
    bn_print(bn);
    bn_print(bn);
    bn_print(bn);
    bn_print(bn);
    
    return TRUE;
}

BOOL bignum_conversion_test(void) {
    BIGNUM * a = bn_from_string("9309990999999999999");
    
    long long int a_long_long = bn_to_integer(a);
    
    printf("a_long_long is %lld\n", a_long_long);
    
    return TRUE;
}
