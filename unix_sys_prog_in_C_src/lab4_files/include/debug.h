#ifndef __DEBUG_H_
#define __DEBUG_H_

#define debug_print(fmt, ...) \
    do { if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)

#endif // __DEBUG_H_
