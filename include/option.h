#ifndef OPTION_H_
#define OPTION_H_

#include "sumtype.h"

enum Option { None, Some };
#define Option(T) generic_sumtype(Option, (None), (Some, T))

#define None(O) build_data(O, None)
#define Some(O, t) build_data(O, Some, t)

#endif  // OPTION_H_
