#ifndef RESULT_H_
#define RESULT_H_

#include "sumtype.h"

enum Result { Ok, Err };
#define Result(T, E) generic_sumtype(Result, (Ok, T), (Err, E))

#define Ok(R, t) \
    (R) { .switcher = Ok, .body.Ok = t }
#define Err(R, e) \
    (R) { .switcher = Err, .body.Err = e }

#endif  // RESULT_H_
