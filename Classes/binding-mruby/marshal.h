#ifndef _MARSHAL_H
#define _MARSHAL_H

extern "C"
void mrb_mruby_marshal_gem_init(mrb_state* M);

mrb_value mrb_marshal_load(mrb_state* M, char* str,long len);

#endif