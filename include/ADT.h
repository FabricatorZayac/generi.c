#ifndef ADT_H_
#define ADT_H_

#define ADT(...)     \
    struct {         \
        union {      \
        } value;     \
        enum {} tag; \
    }

#define DefineADT(NewType, ...) typedef ADT(__VA_ARGS__) NewType;

#define ADT_TAG(Name, T)       \
    struct {                   \
        T value;               \
        enum { Name } tagname; \
    }

#endif  // ADT_H_
