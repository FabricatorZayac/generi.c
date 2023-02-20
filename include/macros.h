#ifndef MACROS_H_
#define MACROS_H_

#define _TOKENPASTE(x, y) x##y
#define TOKENPASTE(x, y) _TOKENPASTE(x, y)  // macro expansion trickery
#define ADHOC TOKENPASTE(adhoc_, __LINE__)  // nice to have in type macros

#define DERIVE_TRAIT(trait) trait(void, void, trait)
#define DERIVE(trait, T) trait(T, struct ADHOC)

#define PROXY(trait, S, T) TOKENPASTE(PROXY_, trait)(S, T)
#define PROXY_ASSIGN(trait, S) TOKENPASTE(PROXY_ASSIGN_, trait)(S)

#define DefineGeneric(generic, T, name) TOKENPASTE(Define, generic)(name, T)

#define New(S) TOKENPASTE(S, _new)()

#endif  // MACROS_H_
