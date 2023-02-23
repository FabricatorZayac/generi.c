#ifndef OPTION_H_
#define OPTION_H_

#include "sumtype.h"

#define _None
#define _Some(T) T Some

enum Option { None, Some };
#define Option(T) sumtype(Option, _None, _Some(T))

#define None(O) \
    (O) { .switcher = None }
#define Some(O, t) \
    (O) { .switcher = Some, .body.Some = t }

#endif  // OPTION_H_
