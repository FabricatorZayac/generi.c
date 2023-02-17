#ifndef MACROS_H_
#define MACROS_H_

#define _TOKENPASTE(x, y) x ## y
#define TOKENPASTE(x, y) _TOKENPASTE(x, y)
#define ADHOC TOKENPASTE(adhoc_, __LINE__)

#endif // MACROS_H_
