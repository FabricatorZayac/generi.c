#ifndef RESULT_H_
#define RESULT_H_

#include "sumtype.h"

enum Result { Ok, Err };
#define Result(T, E) generic_sumtype(Result, (Ok, T), (Err, E))

#define Ok(R, t) build_data(R, Ok, t)
#define Err(R, e) build_data(R, Err, e)

#endif  // RESULT_H_
