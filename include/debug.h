//
//  debug.h
//  big_calc
//
//  Created by POTADOS on 04/02/2019.
//  Copyright © 2019 potados. All rights reserved.
//

#ifndef _debug_h
#define _debug_h

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/********************************
 * No condition.
 ********************************/

/**
 * Function-like prototype:
 * void fatal(int _exit_code, const char * _fmt, ...);
 *
 * Print fatal error and exit.
 *
 * @param _exit_code        Int value returned to system.
 * @param _fmt              Formated message.
 * @param ...               Args.
 */
#define fatal(_exit_code, _fmt, ...)                                        \
do {                                                                        \
_print_error("Fatal", _fmt, ##__VA_ARGS__);                                 \
exit(_exit_code);                                                           \
} while(0)

/**
 * Function-like prototype:
 * void error(T _return, const char * _fmt, ...);
 *
 * Print error and return.
 *
 * @param _return           Any type of value returned to caller.
 * @param _fmt              Formated message.
 * @param ...               Args.
 */
#define error(_return, _fmt, ...)                                           \
do {                                                                        \
_print_error("Error", _fmt, ##__VA_ARGS__);                                 \
return _return;                                                             \
} while(0)


/********************************
 * Conditional.
 ********************************/

/**
 * Function-like prototype:
 * void verify(int _x, T _return, const char * _fmt, ...);
 *
 * Check condition.
 * if condition is false, print error and return.
 *
 * @param _x                Condition. Error occures when it is zero(false).
 * @param _return           Any type of value returned to caller.
 * @param _fmt              Formated message.
 * @param ...               Args.
 */
#define verify(_x, _return, _fmt, ...)                                      \
do {                                                                        \
if (!(_x)) {                                                                \
_print_error("Error", _fmt, ##__VA_ARGS__);                                 \
return _return;                                                             \
}                                                                           \
} while(0)

/**
 * Function-like prototype:
 * void check(int _x, const char * _fmt, ...);
 *
 * Check condition.
 * if condition is false, print error.
 *
 * @param _x                Condition. Error occures when it is zero(false).
 * @param _fmt              Formated message.
 * @param ...               Args.
 */
#define check(_x, _fmt, ...)                                                \
do {                                                                        \
if (!(_x)) {                                                                \
_print_error("Warning", _fmt, ##__VA_ARGS__);                               \
}                                                                           \
} while(0)


/********************************
 * Output.
 ********************************/

/**
 * Function-like prototype:
 * int _print_error(const char * _level, const char * _fmt, ...);
 *
 * @param _level            Level of error.
 * @param _fmt              Formated message.
 *
 * @return                  Number of characters printed.
 */
#define _print_error(_level, _fmt, ...)                                     \
fprintf(stderr,                                                             \
"<" _level "> at file \"%s\", function \"%s\", line %d: " _fmt "\n",        \
__FILE__, __func__, __LINE__, ##__VA_ARGS__);

#endif /* _debug_h */

