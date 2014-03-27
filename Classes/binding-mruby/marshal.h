
#ifndef MARSHAL_H
#define MARSHAL_H

#include <mruby.h>

void marshalDumpInt(mrb_state *, char *, mrb_value);
mrb_value marshalLoadInt(mrb_state *, char *);
mrb_value marshalLoadInt(mrb_state *, char *,unsigned long len);

#endif // MARSHAL_H
