#ifndef OPTION_H_
#define OPTION_H_

#include "sumtype.h"

enum Option { None, Some };
#define Option(T) generic_sumtype(Option, (None), (Some, T))

#define None(O) \
    (O) { .switcher = None }
#define Some(O, t) \
    (O) { .switcher = Some, .body.Some = t }

#endif  // OPTION_H_
