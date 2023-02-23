#ifndef RESULT_H_
#define RESULT_H_

#include "sumtype.h"

#define _Ok(T) T Ok
#define _Err(E) E Err

enum Result { Ok, Err };
#define Result(T, E) sumtype(Result, _Ok(T), _Err(E))

#define Ok(R, t) \
    (R) { .switcher = Ok, .body.Ok = t }
#define Err(R, e) \
    (R) { .switcher = Err, .body.Err = e }

#endif  // RESULT_H_
