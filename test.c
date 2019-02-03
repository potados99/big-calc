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
    char * origin_string = "-00001234356789123456789";
    BIGNUM * from_string = bn_from_string(origin_string);
    
    printf("Bignum from string \"%s\": %s in string.\n",
           origin_string, bn_to_string(from_string));
    printf("Bignum from string \"%s\": %lld in long long.\n",
           origin_string, bn_to_integer(from_string));
    
    long long int origin_long_long = -987654321987654321; /* signed */
    BIGNUM * from_long_long = bn_from_integer(origin_long_long);
    
    printf("Bignum from long long %lld: %s in string.\n",
           origin_long_long, bn_to_string(from_long_long));
    printf("Bignum from long long %lld: %lld in long long.\n",
           origin_long_long, bn_to_integer(from_long_long));
    
    bn_free(from_string);
    bn_free(from_long_long);
    
    return TRUE;
}

BOOL int_test() {
    long long int n = 123456789123456789;
    printf("Test number n is %lld.\n", n);
    
    int len = INT_LEN(n);
    assert(len == strlen("123456789123456789"));
    
    printf("len: %d\n\n", len);
    
    puts("Iterate with for statement from LSB.");
    for (int i = 0; i < len; ++i) {
        printf("%d", (int)INT_DIGIT_AT(n, i));
    } puts("\n");
    
    puts("Iterate with for_each_int_from_lsb statement from LSB.");
    for_each_int_from_lsb(byte digit, n) {
        printf("%d", digit);
    } puts("\n");
    
    return TRUE;
}

BOOL add_test() {
    
    BIGNUM * a = bn_from_integer(23);
    BIGNUM * b = bn_from_integer(99);
    
    BIGNUM * c = bn_add(a, b);
    
    printf("%lld + %lld = %lld\n", bn_to_integer(a), bn_to_integer(b), bn_to_integer(c));
    
    bn_free(a);
    bn_free(b);
    bn_free(c);

    return TRUE;
}

BOOL sub_test() {
    
    BIGNUM * a = bn_from_integer(11);
    BIGNUM * b = bn_from_integer(12);
    
    BIGNUM * c = bn_sub(a, b);
    
    printf("%lld - %lld = %lld\n", bn_to_integer(a), bn_to_integer(b), bn_to_integer(c));
    
    bn_free(a);
    bn_free(b);
    bn_free(c);
    
    return TRUE;
}

BOOL comp_test(void) {
    BIGNUM * left = bn_from_integer(11);
    BIGNUM * right = bn_from_integer(12);
    
    int comp = bn_comp(left, right);
    
    char * result = NULL;
    
    switch (comp) {
        case 1:
            result = "bigger than";
            break;
        case 0:
            result = "same as";
            break;
        case -1:
            result = "smaller than";
            break;
        default:
            break;
    }
    
    printf("%lld is %s %lld\n", bn_to_integer(left), result, bn_to_integer(right));
    
    bn_free(left);
    bn_free(right);
    
    return TRUE;
}
