#ifndef ADT_H_
#define ADT_H_

#include "cursed_macros.h"
#include "macros.h"

#define FGET(x, ...) x
#define GET_ENUM(x, ...) FGET x __VA_OPT__(, )
#define _GET_FIELD(x, ...) __VA_OPT__(__VA_ARGS__ x;)
#define GET_FIELD(x, ...) _GET_FIELD x

#define data(Name, ...)                                   \
    struct Name {                                         \
        union {                                           \
            FOREACH(GET_FIELD, __VA_ARGS__)               \
        } body;                                           \
        enum { FOREACH(GET_ENUM, __VA_ARGS__) } switcher; \
    }

#define generic_sumtype(enum_name, ...)     \
    struct {                                \
        union {                             \
            FOREACH(GET_FIELD, __VA_ARGS__) \
        } body;                             \
        enum enum_name switcher;            \
    }

#define build_data(T, label, ...) \
    (T) { .switcher = label, __VA_OPT__(.body.label = __VA_ARGS__) }

#define match(expr, ...)                                                     \
    {                                                                        \
        typeof(expr) ___self = expr;                                         \
        switch (___self.switcher) { FOREACH(CONCAT_SEMICOLON, __VA_ARGS__) } \
    }

#define of(E, ...) \
    break;         \
    case E:;       \
        __VA_OPT__(__VA_ARGS__ = ___self.body.E;)

#endif  // ADT_H_
