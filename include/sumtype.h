#ifndef ADT_H_
#define ADT_H_

#include "cursed_macros.h"
#include "macros.h"

#define sumtype(name, ...)                         \
    struct {                                       \
        union {                                    \
            FOREACH(CONCAT_SEMICOLON, __VA_ARGS__) \
        } body;                                    \
        enum name switcher;                        \
    }

#define match(S, ...)                                                  \
    {                                                                  \
        typeof(S) self = S;                                            \
        switch (S.switcher) { FOREACH(CONCAT_SEMICOLON, __VA_ARGS__) } \
    }

#define of(E, ...) \
    break;         \
    case E:;       \
        __VA_OPT__(__VA_ARGS__ = self.body.E;)

#endif  // ADT_H_
