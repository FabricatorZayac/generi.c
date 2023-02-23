#ifndef MACROS_H_
#define MACROS_H_

#define _TOKENPASTE(x, y) x##y
#define TOKENPASTE(x, y) _TOKENPASTE(x, y)  // macro expansion trickery
#define ADHOC TOKENPASTE(adhoc_, __LINE__)  // nice to have in type macros

// Max number of types in a template is currently 6
#define DERIVE_TRAIT(trait) trait(void, void, void, void, void, void, void)
#define DERIVE(trait, ...) trait(__VA_ARGS__)

#define PROXY(trait, S, ...) TOKENPASTE(PROXY_, trait)(S, __VA_ARGS__)
#define PROXY_ASSIGN(trait, S) TOKENPASTE(PROXY_ASSIGN_, trait)(S)

#define CONCAT_SEMICOLON(x, ...) x;
#define OF_EXPAND(x, ...) _##x __VA_OPT__(, )

#endif  // MACROS_H_
