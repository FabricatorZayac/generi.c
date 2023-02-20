#ifndef MACROS_H_
#define MACROS_H_

#define _TOKENPASTE(x, y) x##y
#define TOKENPASTE(x, y) _TOKENPASTE(x, y)  // macro expansion trickery
#define ADHOC TOKENPASTE(adhoc_, __LINE__)  // nice to have in type macros

#endif  // MACROS_H_
