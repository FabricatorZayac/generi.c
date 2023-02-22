#ifndef OPTION_H_
#define OPTION_H_

#define Option(T) ADT(Some(T), None)

#define DefineOption(NewType, T) typedef Option(T) NewType;

#define Some(T)
#define None

#endif  // OPTION_H_
