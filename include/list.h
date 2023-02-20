#ifndef LIST_H_
#define LIST_H_

#include <stddef.h>

#include "macros.h"

#define LIST(T, S, ...)                                                        \
    T *(*get)(S * self, size_t index);                                         \
    void (*put)(S * self,                                                      \
                size_t index,                                                  \
                T __VA_OPT__(*) value##__VA_OPT__(_address));                  \
    void (*push_back)(S * self, T __VA_OPT__(*) value##__VA_OPT__(_address));  \
    void (*push_front)(S * self, T __VA_OPT__(*) value##__VA_OPT__(_address)); \
    T *(*pop_back)(S * self);                                                  \
    T *(*pop_front)(S * self);                                                 \
    void (*append)(S * self, void *);                                          \
    void (*insert)(S * self,                                                   \
                   size_t index,                                               \
                   T __VA_OPT__(*) value##__VA_OPT__(_address));               \
    T *(*remove)(S * self, size_t index);

#define DERIVE_TRAIT(trait) trait(void, void, trait)
#define DERIVE(trait, T) trait(T, struct ADHOC)

#endif  // LIST_H_
