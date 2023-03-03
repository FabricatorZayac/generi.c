#ifndef RESULT_H_
#define RESULT_H_

#include "sumtype.h"

enum Result { Ok, Err };
#define Result(T, E) generic_sumtype(Result, (Ok, T), (Err, E))

#define Ok(R, t) build_data(R, Ok, t)
#define Err(R, e) build_data(R, Err, e)

#define TRY(ret, expr, ...)                 \
    match(expr,                             \
          of(Ok __VA_OPT__(, __VA_ARGS__)), \
          of(Err, Error e) return (Err(ret, e)))

// NOTE: this is a crutch. When I nest macros too deeply shit just seems to break
// The problem is most likely to do with how I implement `match()`
// Either that or the problem is with my scuffed FOREACH macro
#define _TRY(ret, expr, ...)                    \
    if (expr.switcher == Ok) {                  \
        __VA_OPT__(__VA_ARGS__ = expr.body.Ok); \
    } else if (expr.switcher == Err) {          \
        Error e = expr.body.Err;                \
        return (Err(ret, e));                   \
    }

#endif  // RESULT_H_
