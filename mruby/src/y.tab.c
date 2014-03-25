
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 7 "src\\parse.y"

#undef PARSER_DEBUG

#define YYDEBUG 1
#define YYERROR_VERBOSE 1
/*
 * Force yacc to use our memory management.  This is a little evil because
 * the macros assume that "parser_state *p" is in scope
 */
#define YYMALLOC(n)    mrb_malloc(p->mrb, (n))
#define YYFREE(o)      mrb_free(p->mrb, (o))
#define YYSTACK_USE_ALLOCA 0

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "mruby.h"
#include "mruby/compile.h"
#include "mruby/proc.h"
#include "node.h"

#define YYLEX_PARAM p

typedef mrb_ast_node node;
typedef struct mrb_parser_state parser_state;
typedef struct mrb_parser_heredoc_info parser_heredoc_info;

static int yyparse(parser_state *p);
static int yylex(void *lval, parser_state *p);
static void yyerror(parser_state *p, const char *s);
static void yywarn(parser_state *p, const char *s);
static void yywarning(parser_state *p, const char *s);
static void backref_error(parser_state *p, node *n);
static void tokadd(parser_state *p, int c);

#ifndef isascii
#define isascii(c) (((c) & ~0x7f) == 0)
#endif

#define identchar(c) (isalnum(c) || (c) == '_' || !isascii(c))

typedef unsigned int stack_type;

#define BITSTACK_PUSH(stack, n) ((stack) = ((stack)<<1)|((n)&1))
#define BITSTACK_POP(stack)     ((stack) = (stack) >> 1)
#define BITSTACK_LEXPOP(stack)  ((stack) = ((stack) >> 1) | ((stack) & 1))
#define BITSTACK_SET_P(stack)   ((stack)&1)

#define COND_PUSH(n)    BITSTACK_PUSH(p->cond_stack, (n))
#define COND_POP()      BITSTACK_POP(p->cond_stack)
#define COND_LEXPOP()   BITSTACK_LEXPOP(p->cond_stack)
#define COND_P()        BITSTACK_SET_P(p->cond_stack)

#define CMDARG_PUSH(n)  BITSTACK_PUSH(p->cmdarg_stack, (n))
#define CMDARG_POP()    BITSTACK_POP(p->cmdarg_stack)
#define CMDARG_LEXPOP() BITSTACK_LEXPOP(p->cmdarg_stack)
#define CMDARG_P()      BITSTACK_SET_P(p->cmdarg_stack)

#define SET_LINENO(c,n) ((c)->lineno = (n))

#define sym(x) ((mrb_sym)(intptr_t)(x))
#define nsym(x) ((node*)(intptr_t)(x))

static inline mrb_sym
intern_cstr_gen(parser_state *p, const char *s)
{
  return mrb_intern_cstr(p->mrb, s);
}
#define intern_cstr(s) intern_cstr_gen(p,(s))

static inline mrb_sym
intern_gen(parser_state *p, const char *s, size_t len)
{
  return mrb_intern(p->mrb, s, len);
}
#define intern(s,len) intern_gen(p,(s),(len))

static inline mrb_sym
intern_gen_c(parser_state *p, const char c)
{
  return mrb_intern(p->mrb, &c, 1);
}
#define intern_c(c) intern_gen_c(p,(c))

static void
cons_free_gen(parser_state *p, node *cons)
{
  cons->cdr = p->cells;
  p->cells = cons;
}
#define cons_free(c) cons_free_gen(p, (c))

static void*
parser_palloc(parser_state *p, size_t size)
{
  void *m = mrb_pool_alloc(p->pool, size);

  if (!m) {
    longjmp(p->jmp, 1);
  }
  return m;
}

static node*
cons_gen(parser_state *p, node *car, node *cdr)
{
  node *c;

  if (p->cells) {
    c = p->cells;
    p->cells = p->cells->cdr;
  }
  else {
    c = (node *)parser_palloc(p, sizeof(mrb_ast_node));
  }

  c->car = car;
  c->cdr = cdr;
  c->lineno = p->lineno;
  c->filename_index = p->current_filename_index;
  return c;
}
#define cons(a,b) cons_gen(p,(a),(b))

static node*
list1_gen(parser_state *p, node *a)
{
  return cons(a, 0);
}
#define list1(a) list1_gen(p, (a))

static node*
list2_gen(parser_state *p, node *a, node *b)
{
  return cons(a, cons(b,0));
}
#define list2(a,b) list2_gen(p, (a),(b))

static node*
list3_gen(parser_state *p, node *a, node *b, node *c)
{
  return cons(a, cons(b, cons(c,0)));
}
#define list3(a,b,c) list3_gen(p, (a),(b),(c))

static node*
list4_gen(parser_state *p, node *a, node *b, node *c, node *d)
{
  return cons(a, cons(b, cons(c, cons(d, 0))));
}
#define list4(a,b,c,d) list4_gen(p, (a),(b),(c),(d))

static node*
list5_gen(parser_state *p, node *a, node *b, node *c, node *d, node *e)
{
  return cons(a, cons(b, cons(c, cons(d, cons(e, 0)))));
}
#define list5(a,b,c,d,e) list5_gen(p, (a),(b),(c),(d),(e))

static node*
list6_gen(parser_state *p, node *a, node *b, node *c, node *d, node *e, node *f)
{
  return cons(a, cons(b, cons(c, cons(d, cons(e, cons(f, 0))))));
}
#define list6(a,b,c,d,e,f) list6_gen(p, (a),(b),(c),(d),(e),(f))

static node*
append_gen(parser_state *p, node *a, node *b)
{
  node *c = a;

  if (!a) return b;
  while (c->cdr) {
    c = c->cdr;
  }
  if (b) {
    c->cdr = b;
  }
  return a;
}
#define append(a,b) append_gen(p,(a),(b))
#define push(a,b) append_gen(p,(a),list1(b))

static char*
parser_strndup(parser_state *p, const char *s, size_t len)
{
  char *b = (char *)parser_palloc(p, len+1);

  memcpy(b, s, len);
  b[len] = '\0';
  return b;
}
#define strndup(s,len) parser_strndup(p, s, len)

static char*
parser_strdup(parser_state *p, const char *s)
{
  return parser_strndup(p, s, strlen(s));
}
#undef strdup
#define strdup(s) parser_strdup(p, s)

// xxx -----------------------------

static node*
local_switch(parser_state *p)
{
  node *prev = p->locals;

  p->locals = cons(0, 0);
  return prev;
}

static void
local_resume(parser_state *p, node *prev)
{
  p->locals = prev;
}

static void
local_nest(parser_state *p)
{
  p->locals = cons(0, p->locals);
}

static void
local_unnest(parser_state *p)
{
  p->locals = p->locals->cdr;
}

static int
local_var_p(parser_state *p, mrb_sym sym)
{
  node *l = p->locals;

  while (l) {
    node *n = l->car;
    while (n) {
      if (sym(n->car) == sym) return 1;
      n = n->cdr;
    }
    l = l->cdr;
  }
  return 0;
}

static void
local_add_f(parser_state *p, mrb_sym sym)
{
  p->locals->car = push(p->locals->car, nsym(sym));
}

static void
local_add(parser_state *p, mrb_sym sym)
{
  if (!local_var_p(p, sym)) {
    local_add_f(p, sym);
  }
}

// (:scope (vars..) (prog...))
static node*
new_scope(parser_state *p, node *body)
{
  return cons((node*)NODE_SCOPE, cons(p->locals->car, body));
}

// (:begin prog...)
static node*
new_begin(parser_state *p, node *body)
{
  if (body)
    return list2((node*)NODE_BEGIN, body);
  return cons((node*)NODE_BEGIN, 0);
}

#define newline_node(n) (n)

// (:rescue body rescue else)
static node*
new_rescue(parser_state *p, node *body, node *resq, node *els)
{
  return list4((node*)NODE_RESCUE, body, resq, els);
}

// (:ensure body ensure)
static node*
new_ensure(parser_state *p, node *a, node *b)
{
  return cons((node*)NODE_ENSURE, cons(a, cons(0, b)));
}

// (:nil)
static node*
new_nil(parser_state *p)
{
  return list1((node*)NODE_NIL);
}

// (:true)
static node*
new_true(parser_state *p)
{
  return list1((node*)NODE_TRUE);
}

// (:false)
static node*
new_false(parser_state *p)
{
  return list1((node*)NODE_FALSE);
}

// (:alias new old)
static node*
new_alias(parser_state *p, mrb_sym a, mrb_sym b)
{
  return cons((node*)NODE_ALIAS, cons(nsym(a), nsym(b)));
}

// (:if cond then else)
static node*
new_if(parser_state *p, node *a, node *b, node *c)
{
  return list4((node*)NODE_IF, a, b, c);
}

// (:unless cond then else)
static node*
new_unless(parser_state *p, node *a, node *b, node *c)
{
  return list4((node*)NODE_IF, a, c, b);
}

// (:while cond body)
static node*
new_while(parser_state *p, node *a, node *b)
{
  return cons((node*)NODE_WHILE, cons(a, b));
}

// (:until cond body)
static node*
new_until(parser_state *p, node *a, node *b)
{
  return cons((node*)NODE_UNTIL, cons(a, b));
}

// (:for var obj body)
static node*
new_for(parser_state *p, node *v, node *o, node *b)
{
  return list4((node*)NODE_FOR, v, o, b);
}

// (:case a ((when ...) body) ((when...) body))
static node*
new_case(parser_state *p, node *a, node *b)
{
  node *n = list2((node*)NODE_CASE, a);
  node *n2 = n;

  while (n2->cdr) {
    n2 = n2->cdr;
  }
  n2->cdr = b;
  return n;
}

// (:postexe a)
static node*
new_postexe(parser_state *p, node *a)
{
  return cons((node*)NODE_POSTEXE, a);
}

// (:self)
static node*
new_self(parser_state *p)
{
  return list1((node*)NODE_SELF);
}

// (:call a b c)
static node*
new_call(parser_state *p, node *a, mrb_sym b, node *c)
{
  return list4((node*)NODE_CALL, a, nsym(b), c);
}

// (:fcall self mid args)
static node*
new_fcall(parser_state *p, mrb_sym b, node *c)
{
  return list4((node*)NODE_FCALL, new_self(p), nsym(b), c);
}

// (:super . c)
static node*
new_super(parser_state *p, node *c)
{
  return cons((node*)NODE_SUPER, c);
}

// (:zsuper)
static node*
new_zsuper(parser_state *p)
{
  return list1((node*)NODE_ZSUPER);
}

// (:yield . c)
static node*
new_yield(parser_state *p, node *c)
{
  if (c) {
    if (c->cdr) {
      yyerror(p, "both block arg and actual block given");
    }
    return cons((node*)NODE_YIELD, c->car);
  }
  return cons((node*)NODE_YIELD, 0);
}

// (:return . c)
static node*
new_return(parser_state *p, node *c)
{
  return cons((node*)NODE_RETURN, c);
}

// (:break . c)
static node*
new_break(parser_state *p, node *c)
{
  return cons((node*)NODE_BREAK, c);
}

// (:next . c)
static node*
new_next(parser_state *p, node *c)
{
  return cons((node*)NODE_NEXT, c);
}

// (:redo)
static node*
new_redo(parser_state *p)
{
  return list1((node*)NODE_REDO);
}

// (:retry)
static node*
new_retry(parser_state *p)
{
  return list1((node*)NODE_RETRY);
}

// (:dot2 a b)
static node*
new_dot2(parser_state *p, node *a, node *b)
{
  return cons((node*)NODE_DOT2, cons(a, b));
}

// (:dot3 a b)
static node*
new_dot3(parser_state *p, node *a, node *b)
{
  return cons((node*)NODE_DOT3, cons(a, b));
}

// (:colon2 b c)
static node*
new_colon2(parser_state *p, node *b, mrb_sym c)
{
  return cons((node*)NODE_COLON2, cons(b, nsym(c)));
}

// (:colon3 . c)
static node*
new_colon3(parser_state *p, mrb_sym c)
{
  return cons((node*)NODE_COLON3, nsym(c));
}

// (:and a b)
static node*
new_and(parser_state *p, node *a, node *b)
{
  return cons((node*)NODE_AND, cons(a, b));
}

// (:or a b)
static node*
new_or(parser_state *p, node *a, node *b)
{
  return cons((node*)NODE_OR, cons(a, b));
}

// (:array a...)
static node*
new_array(parser_state *p, node *a)
{
  return cons((node*)NODE_ARRAY, a);
}

// (:splat . a)
static node*
new_splat(parser_state *p, node *a)
{
  return cons((node*)NODE_SPLAT, a);
}

// (:hash (k . v) (k . v)...)
static node*
new_hash(parser_state *p, node *a)
{
  return cons((node*)NODE_HASH, a);
}

// (:sym . a)
static node*
new_sym(parser_state *p, mrb_sym sym)
{
  return cons((node*)NODE_SYM, nsym(sym));
}

static mrb_sym
new_strsym(parser_state *p, node* str)
{
  const char *s = (const char*)str->cdr->car;
  size_t len = (size_t)str->cdr->cdr;

  return mrb_intern(p->mrb, s, len);
}

// (:lvar . a)
static node*
new_lvar(parser_state *p, mrb_sym sym)
{
  return cons((node*)NODE_LVAR, nsym(sym));
}

// (:gvar . a)
static node*
new_gvar(parser_state *p, mrb_sym sym)
{
  return cons((node*)NODE_GVAR, nsym(sym));
}

// (:ivar . a)
static node*
new_ivar(parser_state *p, mrb_sym sym)
{
  return cons((node*)NODE_IVAR, nsym(sym));
}

// (:cvar . a)
static node*
new_cvar(parser_state *p, mrb_sym sym)
{
  return cons((node*)NODE_CVAR, nsym(sym));
}

// (:const . a)
static node*
new_const(parser_state *p, mrb_sym sym)
{
  return cons((node*)NODE_CONST, nsym(sym));
}

// (:undef a...)
static node*
new_undef(parser_state *p, mrb_sym sym)
{
  return list2((node*)NODE_UNDEF, nsym(sym));
}

// (:class class super body)
static node*
new_class(parser_state *p, node *c, node *s, node *b)
{
  return list4((node*)NODE_CLASS, c, s, cons(p->locals->car, b));
}

// (:sclass obj body)
static node*
new_sclass(parser_state *p, node *o, node *b)
{
  return list3((node*)NODE_SCLASS, o, cons(p->locals->car, b));
}

// (:module module body)
static node*
new_module(parser_state *p, node *m, node *b)
{
  return list3((node*)NODE_MODULE, m, cons(p->locals->car, b));
}

// (:def m lv (arg . body))
static node*
new_def(parser_state *p, mrb_sym m, node *a, node *b)
{
  return list5((node*)NODE_DEF, nsym(m), p->locals->car, a, b);
}

// (:sdef obj m lv (arg . body))
static node*
new_sdef(parser_state *p, node *o, mrb_sym m, node *a, node *b)
{
  return list6((node*)NODE_SDEF, o, nsym(m), p->locals->car, a, b);
}

// (:arg . sym)
static node*
new_arg(parser_state *p, mrb_sym sym)
{
  return cons((node*)NODE_ARG, nsym(sym));
}

// (m o r m2 b)
// m: (a b c)
// o: ((a . e1) (b . e2))
// r: a
// m2: (a b c)
// b: a
static node*
new_args(parser_state *p, node *m, node *opt, mrb_sym rest, node *m2, mrb_sym blk)
{
  node *n;

  n = cons(m2, nsym(blk));
  n = cons(nsym(rest), n);
  n = cons(opt, n);
  return cons(m, n);
}

// (:block_arg . a)
static node*
new_block_arg(parser_state *p, node *a)
{
  return cons((node*)NODE_BLOCK_ARG, a);
}

// (:block arg body)
static node*
new_block(parser_state *p, node *a, node *b)
{
  return list4((node*)NODE_BLOCK, p->locals->car, a, b);
}

// (:lambda arg body)
static node*
new_lambda(parser_state *p, node *a, node *b)
{
  return list4((node*)NODE_LAMBDA, p->locals->car, a, b);
}

// (:asgn lhs rhs)
static node*
new_asgn(parser_state *p, node *a, node *b)
{
  return cons((node*)NODE_ASGN, cons(a, b));
}

// (:masgn mlhs=(pre rest post)  mrhs)
static node*
new_masgn(parser_state *p, node *a, node *b)
{
  return cons((node*)NODE_MASGN, cons(a, b));
}

// (:asgn lhs rhs)
static node*
new_op_asgn(parser_state *p, node *a, mrb_sym op, node *b)
{
  return list4((node*)NODE_OP_ASGN, a, nsym(op), b);
}

// (:int . i)
static node*
new_int(parser_state *p, const char *s, int base)
{
  return list3((node*)NODE_INT, (node*)strdup(s), (node*)(intptr_t)base);
}

// (:float . i)
static node*
new_float(parser_state *p, const char *s)
{
  return cons((node*)NODE_FLOAT, (node*)strdup(s));
}

// (:str . (s . len))
static node*
new_str(parser_state *p, const char *s, int len)
{
  return cons((node*)NODE_STR, cons((node*)strndup(s, len), (node*)(intptr_t)len));
}

// (:dstr . a)
static node*
new_dstr(parser_state *p, node *a)
{
  return cons((node*)NODE_DSTR, a);
}

// (:str . (s . len))
static node*
new_xstr(parser_state *p, const char *s, int len)
{
  return cons((node*)NODE_XSTR, cons((node*)strndup(s, len), (node*)(intptr_t)len));
}

// (:xstr . a)
static node*
new_dxstr(parser_state *p, node *a)
{
  return cons((node*)NODE_DXSTR, a);
}

// (:dsym . a)
static node*
new_dsym(parser_state *p, node *a)
{
  return cons((node*)NODE_DSYM, new_dstr(p, a));
}

// (:str . (a . a))
static node*
new_regx(parser_state *p, const char *p1, const char* p2)
{
  return cons((node*)NODE_REGX, cons((node*)p1, (node*)p2));
}

// (:dregx . a)
static node*
new_dregx(parser_state *p, node *a, node *b)
{
  return cons((node*)NODE_DREGX, cons(a, b));
}

// (:backref . n)
static node*
new_back_ref(parser_state *p, int n)
{
  return cons((node*)NODE_BACK_REF, (node*)(intptr_t)n);
}

// (:nthref . n)
static node*
new_nth_ref(parser_state *p, int n)
{
  return cons((node*)NODE_NTH_REF, (node*)(intptr_t)n);
}

// (:heredoc . a)
static node*
new_heredoc(parser_state *p)
{
  parser_heredoc_info *inf = (parser_heredoc_info *)parser_palloc(p, sizeof(parser_heredoc_info));
  return cons((node*)NODE_HEREDOC, (node*)inf);
}

static void
new_bv(parser_state *p, mrb_sym id)
{
}

static node*
new_literal_delim(parser_state *p)
{
  return cons((node*)NODE_LITERAL_DELIM, 0);
}

// (:words . a)
static node*
new_words(parser_state *p, node *a)
{
  return cons((node*)NODE_WORDS, a);
}

// (:symbols . a)
static node*
new_symbols(parser_state *p, node *a)
{
  return cons((node*)NODE_SYMBOLS, a);
}

// xxx -----------------------------

// (:call a op)
static node*
call_uni_op(parser_state *p, node *recv, char *m)
{
  return new_call(p, recv, intern_cstr(m), 0);
}

// (:call a op b)
static node*
call_bin_op(parser_state *p, node *recv, char *m, node *arg1)
{
  return new_call(p, recv, intern_cstr(m), list1(list1(arg1)));
}

static void
args_with_block(parser_state *p, node *a, node *b)
{
  if (b) {
    if (a->cdr) {
      yyerror(p, "both block arg and actual block given");
    }
    a->cdr = b;
  }
}

static void
call_with_block(parser_state *p, node *a, node *b)
{
  node *n;

  if (a->car == (node*)NODE_SUPER ||
      a->car == (node*)NODE_ZSUPER) {
    if (!a->cdr) a->cdr = cons(0, b);
    else {
      args_with_block(p, a->cdr, b);
    }
  }
  else {
    n = a->cdr->cdr->cdr;
    if (!n->car) n->car = cons(0, b);
    else {
      args_with_block(p, n->car, b);
    }
  }
}

static node*
negate_lit(parser_state *p, node *n)
{
  return cons((node*)NODE_NEGATE, n);
}

static node*
cond(node *n)
{
  return n;
}

static node*
ret_args(parser_state *p, node *n)
{
  if (n->cdr) {
    yyerror(p, "block argument should not be given");
    return NULL;
  }
  if (!n->car->cdr) return n->car->car;
  return new_array(p, n->car);
}

static void
assignable(parser_state *p, node *lhs)
{
  if ((int)(intptr_t)lhs->car == NODE_LVAR) {
    local_add(p, sym(lhs->cdr));
  }
}

static node*
var_reference(parser_state *p, node *lhs)
{
  node *n;

  if ((int)(intptr_t)lhs->car == NODE_LVAR) {
    if (!local_var_p(p, sym(lhs->cdr))) {
      n = new_fcall(p, sym(lhs->cdr), 0);
      cons_free(lhs);
      return n;
    }
  }

  return lhs;
}

typedef enum mrb_string_type  string_type;

static node*
new_strterm(parser_state *p, string_type type, int term, int paren)
{
  return cons((node*)(intptr_t)type, cons((node*)0, cons((node*)(intptr_t)paren, (node*)(intptr_t)term)));
}

static void
end_strterm(parser_state *p)
{
  cons_free(p->lex_strterm->cdr->cdr);
  cons_free(p->lex_strterm->cdr);
  cons_free(p->lex_strterm);
  p->lex_strterm = NULL;
}

static parser_heredoc_info *
parsing_heredoc_inf(parser_state *p)
{
  node *nd = p->parsing_heredoc;
  if (nd == NULL)
    return NULL;
  /* mrb_assert(nd->car->car == NODE_HEREDOC); */
  return (parser_heredoc_info*)nd->car->cdr;
}

static void
heredoc_treat_nextline(parser_state *p)
{
  if (p->heredocs_from_nextline == NULL)
    return;
  if (p->parsing_heredoc == NULL) {
    node *n;
    p->parsing_heredoc = p->heredocs_from_nextline;
    p->lex_strterm_before_heredoc = p->lex_strterm;
    p->lex_strterm = new_strterm(p, parsing_heredoc_inf(p)->type, 0, 0);
    n = p->all_heredocs;
    if (n) {
      while (n->cdr)
        n = n->cdr;
      n->cdr = p->parsing_heredoc;
    } else {
      p->all_heredocs = p->parsing_heredoc;
    }
  } else {
    node *n, *m;
    m = p->heredocs_from_nextline;
    while (m->cdr)
      m = m->cdr;
    n = p->all_heredocs;
    mrb_assert(n != NULL);
    if (n == p->parsing_heredoc) {
      m->cdr = n;
      p->all_heredocs = p->heredocs_from_nextline;
      p->parsing_heredoc = p->heredocs_from_nextline;
    } else {
      while (n->cdr != p->parsing_heredoc) {
        n = n->cdr;
        mrb_assert(n != NULL);
      }
      m->cdr = n->cdr;
      n->cdr = p->heredocs_from_nextline;
      p->parsing_heredoc = p->heredocs_from_nextline;
    }
  }
  p->heredocs_from_nextline = NULL;
}

static void
heredoc_end(parser_state *p)
{
  p->parsing_heredoc = p->parsing_heredoc->cdr;
  if (p->parsing_heredoc == NULL) {
    p->lstate = EXPR_BEG;
    p->cmd_start = TRUE;
    end_strterm(p);
    p->lex_strterm = p->lex_strterm_before_heredoc;
    p->lex_strterm_before_heredoc = NULL;
    p->heredoc_end_now = TRUE;
  } else {
    /* next heredoc */
    p->lex_strterm->car = (node*)(intptr_t)parsing_heredoc_inf(p)->type;
  }
}
#define is_strterm_type(p,str_func) ((int)(intptr_t)((p)->lex_strterm->car) & (str_func))

// xxx -----------------------------



/* Line 189 of yacc.c  */
#line 1053 "E:\\workspace\\cocos2d-x-2.2.1\\projects\\RMXP_runner\\mrubysrc\\mruby\\build\\host\\src\\y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     keyword_class = 258,
     keyword_module = 259,
     keyword_def = 260,
     keyword_undef = 261,
     keyword_begin = 262,
     keyword_rescue = 263,
     keyword_ensure = 264,
     keyword_end = 265,
     keyword_if = 266,
     keyword_unless = 267,
     keyword_then = 268,
     keyword_elsif = 269,
     keyword_else = 270,
     keyword_case = 271,
     keyword_when = 272,
     keyword_while = 273,
     keyword_until = 274,
     keyword_for = 275,
     keyword_break = 276,
     keyword_next = 277,
     keyword_redo = 278,
     keyword_retry = 279,
     keyword_in = 280,
     keyword_do = 281,
     keyword_do_cond = 282,
     keyword_do_block = 283,
     keyword_do_LAMBDA = 284,
     keyword_return = 285,
     keyword_yield = 286,
     keyword_super = 287,
     keyword_self = 288,
     keyword_nil = 289,
     keyword_true = 290,
     keyword_false = 291,
     keyword_and = 292,
     keyword_or = 293,
     keyword_not = 294,
     modifier_if = 295,
     modifier_unless = 296,
     modifier_while = 297,
     modifier_until = 298,
     modifier_rescue = 299,
     keyword_alias = 300,
     keyword_BEGIN = 301,
     keyword_END = 302,
     keyword__LINE__ = 303,
     keyword__FILE__ = 304,
     keyword__ENCODING__ = 305,
     tIDENTIFIER = 306,
     tFID = 307,
     tGVAR = 308,
     tIVAR = 309,
     tCONSTANT = 310,
     tCVAR = 311,
     tLABEL = 312,
     tINTEGER = 313,
     tFLOAT = 314,
     tCHAR = 315,
     tXSTRING = 316,
     tREGEXP = 317,
     tSTRING = 318,
     tSTRING_PART = 319,
     tSTRING_MID = 320,
     tNTH_REF = 321,
     tBACK_REF = 322,
     tREGEXP_END = 323,
     tUPLUS = 324,
     tUMINUS = 325,
     tPOW = 326,
     tCMP = 327,
     tEQ = 328,
     tEQQ = 329,
     tNEQ = 330,
     tGEQ = 331,
     tLEQ = 332,
     tANDOP = 333,
     tOROP = 334,
     tMATCH = 335,
     tNMATCH = 336,
     tDOT2 = 337,
     tDOT3 = 338,
     tAREF = 339,
     tASET = 340,
     tLSHFT = 341,
     tRSHFT = 342,
     tCOLON2 = 343,
     tCOLON3 = 344,
     tOP_ASGN = 345,
     tASSOC = 346,
     tLPAREN = 347,
     tLPAREN_ARG = 348,
     tRPAREN = 349,
     tLBRACK = 350,
     tLBRACE = 351,
     tLBRACE_ARG = 352,
     tSTAR = 353,
     tAMPER = 354,
     tLAMBDA = 355,
     tSYMBEG = 356,
     tREGEXP_BEG = 357,
     tWORDS_BEG = 358,
     tSYMBOLS_BEG = 359,
     tSTRING_BEG = 360,
     tXSTRING_BEG = 361,
     tSTRING_DVAR = 362,
     tLAMBEG = 363,
     tHEREDOC_BEG = 364,
     tHEREDOC_END = 365,
     tLITERAL_DELIM = 366,
     tHD_LITERAL_DELIM = 367,
     tHD_STRING_PART = 368,
     tHD_STRING_MID = 369,
     tLOWEST = 370,
     tUMINUS_NUM = 371,
     idNULL = 372,
     idRespond_to = 373,
     idIFUNC = 374,
     idCFUNC = 375,
     id_core_set_method_alias = 376,
     id_core_set_variable_alias = 377,
     id_core_undef_method = 378,
     id_core_define_method = 379,
     id_core_define_singleton_method = 380,
     id_core_set_postexe = 381,
     tLAST_TOKEN = 382
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 990 "src\\parse.y"

    node *nd;
    mrb_sym id;
    int num;
    unsigned int stack;
    const struct vtable *vars;



/* Line 214 of yacc.c  */
#line 1226 "E:\\workspace\\cocos2d-x-2.2.1\\projects\\RMXP_runner\\mrubysrc\\mruby\\build\\host\\src\\y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 1238 "E:\\workspace\\cocos2d-x-2.2.1\\projects\\RMXP_runner\\mrubysrc\\mruby\\build\\host\\src\\y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   10997

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  154
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  160
/* YYNRULES -- Number of rules.  */
#define YYNRULES  555
/* YYNRULES -- Number of states.  */
#define YYNSTATES  971

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   382

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     153,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   130,     2,     2,     2,   128,   123,     2,
     149,   150,   126,   124,   147,   125,   146,   127,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   118,   152,
     120,   116,   119,   117,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   145,     2,   151,   122,     2,   148,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   143,   121,   144,   131,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   129,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    12,    14,    18,    21,
      23,    24,    30,    35,    38,    40,    42,    46,    49,    50,
      55,    58,    62,    66,    70,    74,    78,    83,    85,    89,
      93,   100,   106,   112,   118,   124,   128,   132,   136,   140,
     142,   146,   150,   152,   156,   160,   164,   167,   169,   171,
     173,   175,   177,   182,   183,   189,   192,   196,   201,   207,
     212,   218,   221,   224,   227,   230,   233,   235,   239,   241,
     245,   247,   250,   254,   260,   263,   268,   271,   276,   278,
     282,   284,   288,   291,   295,   297,   300,   302,   307,   311,
     315,   319,   323,   326,   328,   330,   335,   339,   343,   347,
     351,   354,   356,   358,   360,   363,   365,   369,   371,   373,
     375,   377,   379,   381,   383,   385,   386,   391,   393,   395,
     397,   399,   401,   403,   405,   407,   409,   411,   413,   415,
     417,   419,   421,   423,   425,   427,   429,   431,   433,   435,
     437,   439,   441,   443,   445,   447,   449,   451,   453,   455,
     457,   459,   461,   463,   465,   467,   469,   471,   473,   475,
     477,   479,   481,   483,   485,   487,   489,   491,   493,   495,
     497,   499,   501,   503,   505,   507,   509,   511,   513,   515,
     517,   519,   521,   523,   525,   527,   529,   533,   539,   543,
     549,   556,   562,   568,   574,   580,   585,   589,   593,   597,
     601,   605,   609,   613,   617,   621,   626,   631,   634,   637,
     641,   645,   649,   653,   657,   661,   665,   669,   673,   677,
     681,   685,   689,   692,   695,   699,   703,   707,   711,   718,
     720,   722,   724,   727,   732,   735,   739,   741,   743,   745,
     747,   750,   755,   758,   760,   763,   766,   771,   773,   774,
     777,   780,   783,   785,   787,   790,   794,   799,   804,   810,
     814,   819,   822,   824,   826,   828,   830,   832,   834,   836,
     838,   839,   844,   845,   850,   851,   855,   859,   863,   866,
     870,   874,   876,   881,   885,   887,   892,   896,   899,   901,
     904,   905,   910,   917,   924,   925,   926,   934,   935,   936,
     944,   950,   955,   956,   957,   967,   968,   969,   977,   978,
     979,   980,   990,   991,   992,   999,  1000,  1007,  1008,  1009,
    1019,  1021,  1023,  1025,  1027,  1029,  1031,  1033,  1036,  1038,
    1040,  1042,  1048,  1050,  1053,  1055,  1057,  1059,  1063,  1065,
    1069,  1071,  1076,  1083,  1087,  1093,  1096,  1101,  1103,  1107,
    1114,  1123,  1128,  1135,  1140,  1143,  1150,  1153,  1158,  1165,
    1168,  1173,  1176,  1181,  1183,  1185,  1187,  1191,  1193,  1198,
    1200,  1205,  1207,  1211,  1213,  1215,  1220,  1222,  1226,  1230,
    1231,  1237,  1240,  1245,  1251,  1257,  1260,  1265,  1270,  1274,
    1278,  1282,  1285,  1287,  1292,  1293,  1299,  1300,  1306,  1312,
    1314,  1316,  1323,  1325,  1327,  1329,  1331,  1334,  1336,  1339,
    1341,  1343,  1345,  1347,  1349,  1351,  1353,  1356,  1360,  1362,
    1365,  1367,  1368,  1373,  1375,  1378,  1381,  1385,  1388,  1392,
    1394,  1395,  1397,  1399,  1402,  1404,  1407,  1409,  1412,  1414,
    1415,  1420,  1423,  1427,  1429,  1434,  1437,  1439,  1441,  1443,
    1445,  1447,  1450,  1453,  1457,  1459,  1461,  1464,  1467,  1469,
    1471,  1473,  1475,  1477,  1479,  1481,  1483,  1485,  1487,  1489,
    1491,  1493,  1495,  1497,  1499,  1500,  1505,  1508,  1512,  1515,
    1522,  1531,  1536,  1543,  1548,  1555,  1558,  1563,  1570,  1573,
    1578,  1581,  1586,  1588,  1589,  1591,  1593,  1595,  1597,  1599,
    1601,  1603,  1607,  1609,  1613,  1617,  1621,  1623,  1627,  1629,
    1633,  1635,  1637,  1640,  1642,  1644,  1646,  1649,  1652,  1654,
    1656,  1657,  1662,  1664,  1667,  1669,  1673,  1677,  1680,  1682,
    1684,  1686,  1688,  1690,  1692,  1694,  1696,  1698,  1700,  1702,
    1704,  1705,  1707,  1708,  1710,  1713,  1716,  1717,  1719,  1721,
    1723,  1725,  1726,  1730,  1732,  1735
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     155,     0,    -1,    -1,   156,   157,    -1,   158,   304,    -1,
     313,    -1,   159,    -1,   158,   312,   159,    -1,     1,   159,
      -1,   164,    -1,    -1,    46,   160,   143,   157,   144,    -1,
     162,   250,   228,   253,    -1,   163,   304,    -1,   313,    -1,
     164,    -1,   163,   312,   164,    -1,     1,   164,    -1,    -1,
      45,   185,   165,   185,    -1,     6,   186,    -1,   164,    40,
     168,    -1,   164,    41,   168,    -1,   164,    42,   168,    -1,
     164,    43,   168,    -1,   164,    44,   164,    -1,    47,   143,
     162,   144,    -1,   166,    -1,   174,   116,   169,    -1,   275,
      90,   169,    -1,   224,   145,   195,   307,    90,   169,    -1,
     224,   146,    51,    90,   169,    -1,   224,   146,    55,    90,
     169,    -1,   224,    88,    55,    90,   169,    -1,   224,    88,
      51,    90,   169,    -1,   277,    90,   169,    -1,   181,   116,
     202,    -1,   174,   116,   191,    -1,   174,   116,   202,    -1,
     167,    -1,   181,   116,   169,    -1,   181,   116,   166,    -1,
     169,    -1,   167,    37,   167,    -1,   167,    38,   167,    -1,
      39,   305,   167,    -1,   130,   169,    -1,   190,    -1,   167,
      -1,   173,    -1,   170,    -1,   243,    -1,   243,   303,   301,
     197,    -1,    -1,    97,   172,   234,   162,   144,    -1,   300,
     197,    -1,   300,   197,   171,    -1,   224,   146,   301,   197,
      -1,   224,   146,   301,   197,   171,    -1,   224,    88,   301,
     197,    -1,   224,    88,   301,   197,   171,    -1,    32,   197,
      -1,    31,   197,    -1,    30,   196,    -1,    21,   196,    -1,
      22,   196,    -1,   176,    -1,    92,   175,   306,    -1,   176,
      -1,    92,   175,   306,    -1,   178,    -1,   178,   177,    -1,
     178,    98,   180,    -1,   178,    98,   180,   147,   179,    -1,
     178,    98,    -1,   178,    98,   147,   179,    -1,    98,   180,
      -1,    98,   180,   147,   179,    -1,    98,    -1,    98,   147,
     179,    -1,   180,    -1,    92,   175,   306,    -1,   177,   147,
      -1,   178,   177,   147,    -1,   177,    -1,   178,   177,    -1,
     274,    -1,   224,   145,   195,   307,    -1,   224,   146,    51,
      -1,   224,    88,    51,    -1,   224,   146,    55,    -1,   224,
      88,    55,    -1,    89,    55,    -1,   277,    -1,   274,    -1,
     224,   145,   195,   307,    -1,   224,   146,    51,    -1,   224,
      88,    51,    -1,   224,   146,    55,    -1,   224,    88,    55,
      -1,    89,    55,    -1,   277,    -1,    51,    -1,    55,    -1,
      89,   182,    -1,   182,    -1,   224,    88,   182,    -1,    51,
      -1,    55,    -1,    52,    -1,   188,    -1,   189,    -1,   184,
      -1,   270,    -1,   185,    -1,    -1,   186,   147,   187,   185,
      -1,   121,    -1,   122,    -1,   123,    -1,    72,    -1,    73,
      -1,    74,    -1,    80,    -1,    81,    -1,   119,    -1,    76,
      -1,   120,    -1,    77,    -1,    75,    -1,    86,    -1,    87,
      -1,   124,    -1,   125,    -1,   126,    -1,    98,    -1,   127,
      -1,   128,    -1,    71,    -1,   130,    -1,   131,    -1,    69,
      -1,    70,    -1,    84,    -1,    85,    -1,   148,    -1,    48,
      -1,    49,    -1,    50,    -1,    46,    -1,    47,    -1,    45,
      -1,    37,    -1,     7,    -1,    21,    -1,    16,    -1,     3,
      -1,     5,    -1,    26,    -1,    15,    -1,    14,    -1,    10,
      -1,     9,    -1,    36,    -1,    20,    -1,    25,    -1,     4,
      -1,    22,    -1,    34,    -1,    39,    -1,    38,    -1,    23,
      -1,     8,    -1,    24,    -1,    30,    -1,    33,    -1,    32,
      -1,    13,    -1,    35,    -1,     6,    -1,    17,    -1,    31,
      -1,    11,    -1,    12,    -1,    18,    -1,    19,    -1,   181,
     116,   190,    -1,   181,   116,   190,    44,   190,    -1,   275,
      90,   190,    -1,   275,    90,   190,    44,   190,    -1,   224,
     145,   195,   307,    90,   190,    -1,   224,   146,    51,    90,
     190,    -1,   224,   146,    55,    90,   190,    -1,   224,    88,
      51,    90,   190,    -1,   224,    88,    55,    90,   190,    -1,
      89,    55,    90,   190,    -1,   277,    90,   190,    -1,   190,
      82,   190,    -1,   190,    83,   190,    -1,   190,   124,   190,
      -1,   190,   125,   190,    -1,   190,   126,   190,    -1,   190,
     127,   190,    -1,   190,   128,   190,    -1,   190,    71,   190,
      -1,   129,    58,    71,   190,    -1,   129,    59,    71,   190,
      -1,    69,   190,    -1,    70,   190,    -1,   190,   121,   190,
      -1,   190,   122,   190,    -1,   190,   123,   190,    -1,   190,
      72,   190,    -1,   190,   119,   190,    -1,   190,    76,   190,
      -1,   190,   120,   190,    -1,   190,    77,   190,    -1,   190,
      73,   190,    -1,   190,    74,   190,    -1,   190,    75,   190,
      -1,   190,    80,   190,    -1,   190,    81,   190,    -1,   130,
     190,    -1,   131,   190,    -1,   190,    86,   190,    -1,   190,
      87,   190,    -1,   190,    78,   190,    -1,   190,    79,   190,
      -1,   190,   117,   190,   305,   118,   190,    -1,   203,    -1,
     190,    -1,   313,    -1,   201,   308,    -1,   201,   147,   298,
     308,    -1,   298,   308,    -1,   149,   195,   306,    -1,   313,
      -1,   193,    -1,   313,    -1,   196,    -1,   201,   147,    -1,
     201,   147,   298,   147,    -1,   298,   147,    -1,   173,    -1,
     201,   200,    -1,   298,   200,    -1,   201,   147,   298,   200,
      -1,   199,    -1,    -1,   198,   196,    -1,    99,   191,    -1,
     147,   199,    -1,   313,    -1,   191,    -1,    98,   191,    -1,
     201,   147,   191,    -1,   201,   147,    98,   191,    -1,   201,
     147,   263,   191,    -1,   201,   147,   263,    98,   191,    -1,
     201,   147,   191,    -1,   201,   147,    98,   191,    -1,    98,
     191,    -1,   254,    -1,   255,    -1,   259,    -1,   260,    -1,
     261,    -1,   276,    -1,   277,    -1,    52,    -1,    -1,     7,
     204,   161,    10,    -1,    -1,    93,   167,   205,   306,    -1,
      -1,    93,   206,   306,    -1,    92,   162,   150,    -1,   224,
      88,    55,    -1,    89,    55,    -1,    95,   192,   151,    -1,
      96,   297,   144,    -1,    30,    -1,    31,   149,   196,   306,
      -1,    31,   149,   306,    -1,    31,    -1,    39,   149,   167,
     306,    -1,    39,   149,   306,    -1,   300,   245,    -1,   244,
      -1,   244,   245,    -1,    -1,   100,   207,   239,   240,    -1,
      11,   168,   225,   162,   227,    10,    -1,    12,   168,   225,
     162,   228,    10,    -1,    -1,    -1,    18,   208,   168,   226,
     209,   162,    10,    -1,    -1,    -1,    19,   210,   168,   226,
     211,   162,    10,    -1,    16,   168,   304,   248,    10,    -1,
      16,   304,   248,    10,    -1,    -1,    -1,    20,   229,    25,
     212,   168,   226,   213,   162,    10,    -1,    -1,    -1,     3,
     214,   183,   278,   215,   161,    10,    -1,    -1,    -1,    -1,
       3,   216,    86,   167,   217,   309,   218,   161,    10,    -1,
      -1,    -1,     4,   219,   183,   220,   161,    10,    -1,    -1,
       5,   184,   221,   280,   161,    10,    -1,    -1,    -1,     5,
     295,   303,   222,   184,   223,   280,   161,    10,    -1,    21,
      -1,    22,    -1,    23,    -1,    24,    -1,   203,    -1,   309,
      -1,    13,    -1,   309,    13,    -1,   309,    -1,    27,    -1,
     228,    -1,    14,   168,   225,   162,   227,    -1,   313,    -1,
      15,   162,    -1,   181,    -1,   174,    -1,   283,    -1,    92,
     232,   306,    -1,   230,    -1,   231,   147,   230,    -1,   231,
      -1,   231,   147,    98,   283,    -1,   231,   147,    98,   283,
     147,   231,    -1,   231,   147,    98,    -1,   231,   147,    98,
     147,   231,    -1,    98,   283,    -1,    98,   283,   147,   231,
      -1,    98,    -1,    98,   147,   231,    -1,   285,   147,   288,
     147,   291,   294,    -1,   285,   147,   288,   147,   291,   147,
     285,   294,    -1,   285,   147,   288,   294,    -1,   285,   147,
     288,   147,   285,   294,    -1,   285,   147,   291,   294,    -1,
     285,   147,    -1,   285,   147,   291,   147,   285,   294,    -1,
     285,   294,    -1,   288,   147,   291,   294,    -1,   288,   147,
     291,   147,   285,   294,    -1,   288,   294,    -1,   288,   147,
     285,   294,    -1,   291,   294,    -1,   291,   147,   285,   294,
      -1,   293,    -1,   313,    -1,   235,    -1,   121,   236,   121,
      -1,    79,    -1,   121,   233,   236,   121,    -1,   305,    -1,
     305,   152,   237,   305,    -1,   238,    -1,   237,   147,   238,
      -1,    51,    -1,   282,    -1,   149,   281,   236,   150,    -1,
     281,    -1,   108,   162,   144,    -1,    29,   162,    10,    -1,
      -1,    28,   242,   234,   162,    10,    -1,   173,   241,    -1,
     243,   303,   301,   194,    -1,   243,   303,   301,   194,   245,
      -1,   243,   303,   301,   197,   241,    -1,   300,   193,    -1,
     224,   146,   301,   194,    -1,   224,    88,   301,   193,    -1,
     224,    88,   302,    -1,   224,   146,   193,    -1,   224,    88,
     193,    -1,    32,   193,    -1,    32,    -1,   224,   145,   195,
     307,    -1,    -1,   143,   246,   234,   162,   144,    -1,    -1,
      26,   247,   234,   162,    10,    -1,    17,   201,   225,   162,
     249,    -1,   228,    -1,   248,    -1,     8,   251,   252,   225,
     162,   250,    -1,   313,    -1,   191,    -1,   202,    -1,   313,
      -1,    91,   181,    -1,   313,    -1,     9,   162,    -1,   313,
      -1,   273,    -1,   269,    -1,   268,    -1,   272,    -1,    60,
      -1,    63,    -1,   105,    63,    -1,   105,   256,    63,    -1,
     257,    -1,   256,   257,    -1,    65,    -1,    -1,    64,   258,
     162,   144,    -1,   111,    -1,   112,   263,    -1,   106,    61,
      -1,   106,   256,    61,    -1,   102,    62,    -1,   102,   256,
      62,    -1,   109,    -1,    -1,   263,    -1,   264,    -1,   263,
     264,    -1,   110,    -1,   265,   110,    -1,   266,    -1,   265,
     266,    -1,   114,    -1,    -1,   113,   267,   162,   144,    -1,
     103,    63,    -1,   103,   256,    63,    -1,   270,    -1,   101,
     105,   257,    63,    -1,   101,   271,    -1,   184,    -1,    54,
      -1,    53,    -1,    56,    -1,    63,    -1,   105,    63,    -1,
     104,    63,    -1,   104,   256,    63,    -1,    58,    -1,    59,
      -1,   129,    58,    -1,   129,    59,    -1,    51,    -1,    54,
      -1,    53,    -1,    56,    -1,    55,    -1,   274,    -1,   274,
      -1,    34,    -1,    33,    -1,    35,    -1,    36,    -1,    49,
      -1,    48,    -1,    66,    -1,    67,    -1,   309,    -1,    -1,
     120,   279,   168,   309,    -1,     1,   309,    -1,   149,   281,
     306,    -1,   281,   309,    -1,   285,   147,   289,   147,   291,
     294,    -1,   285,   147,   289,   147,   291,   147,   285,   294,
      -1,   285,   147,   289,   294,    -1,   285,   147,   289,   147,
     285,   294,    -1,   285,   147,   291,   294,    -1,   285,   147,
     291,   147,   285,   294,    -1,   285,   294,    -1,   289,   147,
     291,   294,    -1,   289,   147,   291,   147,   285,   294,    -1,
     289,   294,    -1,   289,   147,   285,   294,    -1,   291,   294,
      -1,   291,   147,   285,   294,    -1,   293,    -1,    -1,    55,
      -1,    54,    -1,    53,    -1,    56,    -1,   282,    -1,    51,
      -1,   283,    -1,    92,   232,   306,    -1,   284,    -1,   285,
     147,   284,    -1,    51,   116,   191,    -1,    51,   116,   224,
      -1,   287,    -1,   288,   147,   287,    -1,   286,    -1,   289,
     147,   286,    -1,   126,    -1,    98,    -1,   290,    51,    -1,
     290,    -1,   123,    -1,    99,    -1,   292,    51,    -1,   147,
     293,    -1,   313,    -1,   276,    -1,    -1,   149,   296,   167,
     306,    -1,   313,    -1,   298,   308,    -1,   299,    -1,   298,
     147,   299,    -1,   191,    91,   191,    -1,    57,   191,    -1,
      51,    -1,    55,    -1,    52,    -1,    51,    -1,    55,    -1,
      52,    -1,   188,    -1,    51,    -1,    52,    -1,   188,    -1,
     146,    -1,    88,    -1,    -1,   312,    -1,    -1,   310,    -1,
     305,   150,    -1,   305,   151,    -1,    -1,   310,    -1,   147,
      -1,   152,    -1,   310,    -1,    -1,   153,   311,   262,    -1,
     309,    -1,   312,   152,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1149,  1149,  1149,  1159,  1165,  1169,  1173,  1177,  1183,
    1185,  1184,  1196,  1222,  1228,  1232,  1236,  1240,  1246,  1246,
    1250,  1254,  1258,  1262,  1266,  1270,  1274,  1279,  1280,  1284,
    1288,  1292,  1296,  1300,  1305,  1309,  1314,  1318,  1322,  1326,
    1329,  1333,  1340,  1341,  1345,  1349,  1353,  1357,  1360,  1367,
    1368,  1371,  1372,  1376,  1375,  1388,  1392,  1397,  1401,  1406,
    1410,  1415,  1419,  1423,  1427,  1431,  1437,  1441,  1447,  1448,
    1454,  1458,  1462,  1466,  1470,  1474,  1478,  1482,  1486,  1490,
    1496,  1497,  1503,  1507,  1513,  1517,  1523,  1527,  1531,  1535,
    1539,  1543,  1549,  1555,  1562,  1566,  1570,  1574,  1578,  1582,
    1588,  1594,  1601,  1605,  1608,  1612,  1616,  1622,  1623,  1624,
    1625,  1630,  1637,  1638,  1641,  1645,  1645,  1651,  1652,  1653,
    1654,  1655,  1656,  1657,  1658,  1659,  1660,  1661,  1662,  1663,
    1664,  1665,  1666,  1667,  1668,  1669,  1670,  1671,  1672,  1673,
    1674,  1675,  1676,  1677,  1678,  1679,  1682,  1682,  1682,  1683,
    1683,  1684,  1684,  1684,  1685,  1685,  1685,  1685,  1686,  1686,
    1686,  1687,  1687,  1687,  1688,  1688,  1688,  1688,  1689,  1689,
    1689,  1689,  1690,  1690,  1690,  1690,  1691,  1691,  1691,  1691,
    1692,  1692,  1692,  1692,  1693,  1693,  1696,  1700,  1704,  1708,
    1712,  1716,  1720,  1724,  1728,  1733,  1738,  1743,  1747,  1751,
    1755,  1759,  1763,  1767,  1771,  1775,  1779,  1783,  1787,  1791,
    1795,  1799,  1803,  1807,  1811,  1815,  1819,  1823,  1827,  1831,
    1835,  1839,  1843,  1847,  1851,  1855,  1859,  1863,  1867,  1871,
    1877,  1884,  1885,  1889,  1893,  1899,  1905,  1906,  1909,  1910,
    1911,  1915,  1919,  1925,  1929,  1933,  1937,  1941,  1947,  1947,
    1958,  1964,  1968,  1974,  1978,  1982,  1986,  1990,  1994,  2000,
    2004,  2008,  2014,  2015,  2016,  2017,  2018,  2019,  2020,  2021,
    2026,  2025,  2036,  2036,  2040,  2040,  2044,  2048,  2052,  2056,
    2060,  2064,  2068,  2072,  2076,  2080,  2084,  2088,  2092,  2093,
    2099,  2098,  2111,  2118,  2125,  2125,  2125,  2131,  2131,  2131,
    2137,  2143,  2148,  2150,  2147,  2157,  2161,  2156,  2174,  2178,
    2183,  2173,  2197,  2201,  2196,  2214,  2213,  2226,  2227,  2226,
    2240,  2244,  2248,  2252,  2258,  2265,  2266,  2267,  2270,  2271,
    2274,  2275,  2283,  2284,  2290,  2294,  2297,  2301,  2307,  2311,
    2317,  2321,  2325,  2329,  2333,  2337,  2341,  2345,  2349,  2355,
    2359,  2363,  2367,  2371,  2375,  2379,  2383,  2387,  2391,  2395,
    2399,  2403,  2407,  2411,  2417,  2418,  2425,  2430,  2435,  2442,
    2446,  2452,  2453,  2456,  2461,  2464,  2468,  2474,  2478,  2485,
    2484,  2497,  2507,  2511,  2516,  2523,  2527,  2531,  2535,  2539,
    2543,  2547,  2551,  2555,  2562,  2561,  2574,  2573,  2587,  2595,
    2604,  2607,  2614,  2617,  2621,  2622,  2625,  2629,  2632,  2636,
    2639,  2640,  2641,  2642,  2645,  2646,  2647,  2651,  2657,  2658,
    2664,  2669,  2668,  2679,  2683,  2689,  2693,  2699,  2703,  2709,
    2712,  2713,  2716,  2717,  2720,  2726,  2732,  2733,  2736,  2743,
    2742,  2756,  2760,  2767,  2771,  2778,  2785,  2786,  2787,  2788,
    2789,  2793,  2799,  2803,  2809,  2810,  2811,  2815,  2821,  2825,
    2829,  2833,  2837,  2843,  2849,  2853,  2857,  2861,  2865,  2869,
    2876,  2885,  2886,  2889,  2894,  2893,  2902,  2909,  2915,  2921,
    2925,  2929,  2933,  2937,  2941,  2945,  2949,  2953,  2957,  2961,
    2965,  2969,  2973,  2978,  2984,  2989,  2994,  2999,  3006,  3010,
    3017,  3021,  3027,  3031,  3037,  3044,  3051,  3055,  3061,  3065,
    3071,  3072,  3075,  3080,  3087,  3088,  3091,  3098,  3102,  3109,
    3114,  3114,  3139,  3140,  3146,  3150,  3156,  3160,  3166,  3167,
    3168,  3171,  3172,  3173,  3174,  3177,  3178,  3179,  3182,  3183,
    3186,  3187,  3190,  3191,  3194,  3197,  3200,  3201,  3202,  3205,
    3206,  3210,  3209,  3216,  3217,  3221
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "keyword_class", "keyword_module",
  "keyword_def", "keyword_undef", "keyword_begin", "keyword_rescue",
  "keyword_ensure", "keyword_end", "keyword_if", "keyword_unless",
  "keyword_then", "keyword_elsif", "keyword_else", "keyword_case",
  "keyword_when", "keyword_while", "keyword_until", "keyword_for",
  "keyword_break", "keyword_next", "keyword_redo", "keyword_retry",
  "keyword_in", "keyword_do", "keyword_do_cond", "keyword_do_block",
  "keyword_do_LAMBDA", "keyword_return", "keyword_yield", "keyword_super",
  "keyword_self", "keyword_nil", "keyword_true", "keyword_false",
  "keyword_and", "keyword_or", "keyword_not", "modifier_if",
  "modifier_unless", "modifier_while", "modifier_until", "modifier_rescue",
  "keyword_alias", "keyword_BEGIN", "keyword_END", "keyword__LINE__",
  "keyword__FILE__", "keyword__ENCODING__", "tIDENTIFIER", "tFID", "tGVAR",
  "tIVAR", "tCONSTANT", "tCVAR", "tLABEL", "tINTEGER", "tFLOAT", "tCHAR",
  "tXSTRING", "tREGEXP", "tSTRING", "tSTRING_PART", "tSTRING_MID",
  "tNTH_REF", "tBACK_REF", "tREGEXP_END", "tUPLUS", "tUMINUS", "tPOW",
  "tCMP", "tEQ", "tEQQ", "tNEQ", "tGEQ", "tLEQ", "tANDOP", "tOROP",
  "tMATCH", "tNMATCH", "tDOT2", "tDOT3", "tAREF", "tASET", "tLSHFT",
  "tRSHFT", "tCOLON2", "tCOLON3", "tOP_ASGN", "tASSOC", "tLPAREN",
  "tLPAREN_ARG", "tRPAREN", "tLBRACK", "tLBRACE", "tLBRACE_ARG", "tSTAR",
  "tAMPER", "tLAMBDA", "tSYMBEG", "tREGEXP_BEG", "tWORDS_BEG",
  "tSYMBOLS_BEG", "tSTRING_BEG", "tXSTRING_BEG", "tSTRING_DVAR", "tLAMBEG",
  "tHEREDOC_BEG", "tHEREDOC_END", "tLITERAL_DELIM", "tHD_LITERAL_DELIM",
  "tHD_STRING_PART", "tHD_STRING_MID", "tLOWEST", "'='", "'?'", "':'",
  "'>'", "'<'", "'|'", "'^'", "'&'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "tUMINUS_NUM", "'!'", "'~'", "idNULL", "idRespond_to", "idIFUNC",
  "idCFUNC", "id_core_set_method_alias", "id_core_set_variable_alias",
  "id_core_undef_method", "id_core_define_method",
  "id_core_define_singleton_method", "id_core_set_postexe", "tLAST_TOKEN",
  "'{'", "'}'", "'['", "'.'", "','", "'`'", "'('", "')'", "']'", "';'",
  "'\\n'", "$accept", "program", "$@1", "top_compstmt", "top_stmts",
  "top_stmt", "@2", "bodystmt", "compstmt", "stmts", "stmt", "$@3",
  "command_asgn", "expr", "expr_value", "command_call", "block_command",
  "cmd_brace_block", "$@4", "command", "mlhs", "mlhs_inner", "mlhs_basic",
  "mlhs_item", "mlhs_list", "mlhs_post", "mlhs_node", "lhs", "cname",
  "cpath", "fname", "fsym", "undef_list", "$@5", "op", "reswords", "arg",
  "arg_value", "aref_args", "paren_args", "opt_paren_args",
  "opt_call_args", "call_args", "command_args", "@6", "block_arg",
  "opt_block_arg", "args", "mrhs", "primary", "$@7", "$@8", "$@9", "@10",
  "$@11", "$@12", "$@13", "$@14", "$@15", "$@16", "@17", "@18", "@19",
  "@20", "@21", "@22", "@23", "@24", "$@25", "@26", "primary_value",
  "then", "do", "if_tail", "opt_else", "for_var", "f_marg", "f_marg_list",
  "f_margs", "block_param", "opt_block_param", "block_param_def",
  "opt_bv_decl", "bv_decls", "bvar", "f_larglist", "lambda_body",
  "do_block", "$@27", "block_call", "method_call", "brace_block", "@28",
  "@29", "case_body", "cases", "opt_rescue", "exc_list", "exc_var",
  "opt_ensure", "literal", "string", "string_rep", "string_interp", "@30",
  "xstring", "regexp", "heredoc", "opt_heredoc_bodies", "heredoc_bodies",
  "heredoc_body", "heredoc_string_rep", "heredoc_string_interp", "@31",
  "words", "symbol", "basic_symbol", "sym", "symbols", "numeric",
  "variable", "var_lhs", "var_ref", "backref", "superclass", "$@32",
  "f_arglist", "f_args", "f_bad_arg", "f_norm_arg", "f_arg_item", "f_arg",
  "f_opt", "f_block_opt", "f_block_optarg", "f_optarg", "restarg_mark",
  "f_rest_arg", "blkarg_mark", "f_block_arg", "opt_f_block_arg",
  "singleton", "$@33", "assoc_list", "assocs", "assoc", "operation",
  "operation2", "operation3", "dot_or_colon", "opt_terms", "opt_nl",
  "rparen", "rbracket", "trailer", "term", "nl", "$@34", "terms", "none", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,    61,    63,    58,    62,
      60,   124,    94,    38,    43,    45,    42,    47,    37,   371,
      33,   126,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   123,   125,    91,    46,    44,    96,    40,
      41,    93,    59,    10
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   154,   156,   155,   157,   158,   158,   158,   158,   159,
     160,   159,   161,   162,   163,   163,   163,   163,   165,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     166,   166,   167,   167,   167,   167,   167,   167,   168,   169,
     169,   170,   170,   172,   171,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   174,   174,   175,   175,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     177,   177,   178,   178,   179,   179,   180,   180,   180,   180,
     180,   180,   180,   180,   181,   181,   181,   181,   181,   181,
     181,   181,   182,   182,   183,   183,   183,   184,   184,   184,
     184,   184,   185,   185,   186,   187,   186,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     191,   192,   192,   192,   192,   193,   194,   194,   195,   195,
     195,   195,   195,   196,   196,   196,   196,   196,   198,   197,
     199,   200,   200,   201,   201,   201,   201,   201,   201,   202,
     202,   202,   203,   203,   203,   203,   203,   203,   203,   203,
     204,   203,   205,   203,   206,   203,   203,   203,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     207,   203,   203,   203,   208,   209,   203,   210,   211,   203,
     203,   203,   212,   213,   203,   214,   215,   203,   216,   217,
     218,   203,   219,   220,   203,   221,   203,   222,   223,   203,
     203,   203,   203,   203,   224,   225,   225,   225,   226,   226,
     227,   227,   228,   228,   229,   229,   230,   230,   231,   231,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   234,   234,   235,   235,   235,   236,
     236,   237,   237,   238,   238,   239,   239,   240,   240,   242,
     241,   243,   243,   243,   243,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   246,   245,   247,   245,   248,   249,
     249,   250,   250,   251,   251,   251,   252,   252,   253,   253,
     254,   254,   254,   254,   255,   255,   255,   255,   256,   256,
     257,   258,   257,   257,   257,   259,   259,   260,   260,   261,
     262,   262,   263,   263,   264,   264,   265,   265,   266,   267,
     266,   268,   268,   269,   269,   270,   271,   271,   271,   271,
     271,   271,   272,   272,   273,   273,   273,   273,   274,   274,
     274,   274,   274,   275,   276,   276,   276,   276,   276,   276,
     276,   277,   277,   278,   279,   278,   278,   280,   280,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   282,   282,   282,   282,   283,   283,
     284,   284,   285,   285,   286,   287,   288,   288,   289,   289,
     290,   290,   291,   291,   292,   292,   293,   294,   294,   295,
     296,   295,   297,   297,   298,   298,   299,   299,   300,   300,
     300,   301,   301,   301,   301,   302,   302,   302,   303,   303,
     304,   304,   305,   305,   306,   307,   308,   308,   308,   309,
     309,   311,   310,   312,   312,   313
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     1,     3,     2,     1,
       0,     5,     4,     2,     1,     1,     3,     2,     0,     4,
       2,     3,     3,     3,     3,     3,     4,     1,     3,     3,
       6,     5,     5,     5,     5,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     2,     1,     1,     1,
       1,     1,     4,     0,     5,     2,     3,     4,     5,     4,
       5,     2,     2,     2,     2,     2,     1,     3,     1,     3,
       1,     2,     3,     5,     2,     4,     2,     4,     1,     3,
       1,     3,     2,     3,     1,     2,     1,     4,     3,     3,
       3,     3,     2,     1,     1,     4,     3,     3,     3,     3,
       2,     1,     1,     1,     2,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     5,     3,     5,
       6,     5,     5,     5,     5,     4,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     4,     4,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     3,     3,     3,     3,     6,     1,
       1,     1,     2,     4,     2,     3,     1,     1,     1,     1,
       2,     4,     2,     1,     2,     2,     4,     1,     0,     2,
       2,     2,     1,     1,     2,     3,     4,     4,     5,     3,
       4,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     4,     0,     4,     0,     3,     3,     3,     2,     3,
       3,     1,     4,     3,     1,     4,     3,     2,     1,     2,
       0,     4,     6,     6,     0,     0,     7,     0,     0,     7,
       5,     4,     0,     0,     9,     0,     0,     7,     0,     0,
       0,     9,     0,     0,     6,     0,     6,     0,     0,     9,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     5,     1,     2,     1,     1,     1,     3,     1,     3,
       1,     4,     6,     3,     5,     2,     4,     1,     3,     6,
       8,     4,     6,     4,     2,     6,     2,     4,     6,     2,
       4,     2,     4,     1,     1,     1,     3,     1,     4,     1,
       4,     1,     3,     1,     1,     4,     1,     3,     3,     0,
       5,     2,     4,     5,     5,     2,     4,     4,     3,     3,
       3,     2,     1,     4,     0,     5,     0,     5,     5,     1,
       1,     6,     1,     1,     1,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     2,     3,     1,     2,
       1,     0,     4,     1,     2,     2,     3,     2,     3,     1,
       0,     1,     1,     2,     1,     2,     1,     2,     1,     0,
       4,     2,     3,     1,     4,     2,     1,     1,     1,     1,
       1,     2,     2,     3,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     4,     2,     3,     2,     6,
       8,     4,     6,     4,     6,     2,     4,     6,     2,     4,
       2,     4,     1,     0,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     3,     3,     3,     1,     3,     1,     3,
       1,     1,     2,     1,     1,     1,     2,     2,     1,     1,
       0,     4,     1,     2,     1,     3,     3,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     0,     1,     2,     2,     0,     1,     1,     1,
       1,     0,     3,     1,     2,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     1,     0,   305,   312,     0,     0,   270,
       0,     0,   540,   294,   297,     0,   320,   321,   322,   323,
     281,   248,   248,   466,   465,   467,   468,   542,     0,    10,
       0,   470,   469,   458,   530,   460,   459,   462,   461,   454,
     455,   414,   415,   471,   472,     0,     0,     0,     0,   274,
     555,   555,    78,   290,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     3,   540,     6,     9,    27,    39,
      42,    50,    49,     0,    66,     0,    70,    80,     0,    47,
     229,     0,    51,   288,   262,   263,   264,   265,   266,   412,
     411,   443,   413,   410,   464,     0,   267,   268,   248,     5,
       8,     0,     0,     0,   156,   166,   157,   179,   153,   172,
     162,   161,   182,   183,   177,   160,   159,   155,   180,   184,
     185,   164,   154,   167,   171,   173,   165,   158,   174,   181,
     176,   175,   168,   178,   163,   152,   170,   169,   151,   149,
     150,   146,   147,   148,   107,   109,   108,   141,   142,   138,
     120,   121,   122,   129,   126,   128,   123,   124,   143,   144,
     130,   131,   135,   125,   127,   117,   118,   119,   132,   133,
     134,   136,   137,   139,   140,   145,   520,   315,   110,   111,
     464,   519,     0,   175,   168,   178,   163,   146,   147,   107,
     108,     0,   112,   114,    20,   113,     0,     0,     0,    48,
       0,     0,     0,   464,     0,   268,     0,   549,   551,   540,
       0,   553,   550,   541,     0,     0,   320,   321,   281,   284,
     392,     0,     0,     0,   335,   334,   324,     0,     0,   464,
     268,     0,     0,     0,     0,     0,   243,   230,   253,    64,
     247,   555,   555,   524,    65,    63,   542,    62,     0,   555,
     391,    61,   542,     0,   543,    18,     0,     0,   207,     0,
     208,   278,     0,     0,     0,   540,    15,   542,    68,    14,
     272,   542,     0,   546,   546,   231,     0,     0,   546,   522,
       0,     0,    76,     0,    86,    93,   493,   448,   447,   449,
     450,     0,   446,   445,   427,   421,   420,   423,     0,     0,
     418,   441,     0,   452,     0,   416,     0,   425,     0,   456,
     457,    46,   222,   223,     4,   541,     0,     0,     0,     0,
       0,     0,     0,   379,   381,     0,    82,     0,    74,    71,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   555,     0,
     539,   538,     0,   396,   394,   289,     0,     0,   385,    55,
     287,   102,   103,     0,   105,     0,     0,   268,     0,   313,
       0,   493,   317,     0,   115,     0,   555,   278,   326,     0,
     325,     0,     0,   555,     0,     0,     0,     0,   430,     0,
       0,     0,   554,     0,     0,   278,   456,   457,     0,   555,
       0,   302,   527,   254,   250,     0,     0,   244,   252,     0,
     245,   542,     0,   283,   249,   542,   239,   555,   555,   238,
     542,   286,    45,     0,     0,     0,     0,     0,     0,    17,
     542,   276,    13,   541,    67,   542,   275,   279,   548,   232,
     547,   548,   234,   280,   523,    92,    84,     0,    79,     0,
       0,   555,     0,   499,   496,   495,   494,   497,     0,   511,
     515,   514,   510,   493,     0,   376,   498,   500,   502,   555,
     508,   555,   513,   555,     0,   492,   451,     0,     0,   434,
     439,   438,   424,   432,     0,   436,   428,   419,   442,   453,
     417,   426,     0,     0,     7,    21,    22,    23,    24,    25,
      43,    44,   555,     0,    28,    37,     0,    38,   542,     0,
      72,    83,    41,    40,     0,   186,   253,    36,   204,   212,
     217,   218,   219,   214,   216,   226,   227,   220,   221,   197,
     198,   224,   225,   542,   213,   215,   209,   210,   211,   199,
     200,   201,   202,   203,   531,   536,   532,   537,   390,   248,
     388,   542,   531,   533,   532,   534,   389,   248,   531,   532,
     248,   555,   555,    29,   188,    35,   196,    53,    56,   102,
     103,   104,     0,   474,   306,   473,     0,   555,     0,   309,
       0,   542,   493,     0,     0,     0,     0,   271,   555,   555,
     402,   555,   327,   186,   535,   532,   542,   531,   532,   555,
     552,   431,     0,     0,   301,   329,   295,   328,   298,   535,
     277,     0,   542,   531,   532,   555,     0,   526,     0,   255,
     251,     0,   555,   525,   282,   544,   235,   240,   242,   285,
      19,     0,    26,   195,    69,    16,   273,   546,    85,    77,
      89,    91,   542,   531,   532,     0,   499,     0,   347,   338,
     340,   542,   336,   542,     0,     0,   291,     0,   485,   518,
       0,   488,   512,     0,   490,   516,   444,     0,     0,   433,
     435,   437,   205,   206,   367,   542,     0,   365,   364,   261,
       0,    81,    75,     0,     0,     0,     0,     0,     0,   387,
      59,     0,   393,     0,     0,   237,   386,    57,   236,   382,
      52,     0,     0,     0,   555,   476,     0,     0,   102,   103,
     106,   542,     0,     0,   521,   542,     0,   478,   318,   116,
     403,   404,   555,   405,     0,   555,   332,     0,     0,   330,
       0,     0,   393,     0,     0,     0,   300,     0,     0,     0,
       0,   393,     0,   256,     0,   257,   246,   555,    11,   233,
      87,   504,   542,     0,   345,     0,   501,     0,   369,     0,
       0,   503,   555,   555,   517,   555,   509,   555,   555,   422,
       0,   499,   542,     0,   555,   506,   555,   555,   363,     0,
       0,   259,    73,   187,     0,    34,   193,    33,   194,    60,
     545,     0,    31,   191,    32,   192,    58,   383,   384,     0,
       0,   189,     0,     0,     0,   393,   310,   314,   477,   316,
     493,     0,     0,   407,   333,     0,    12,   409,     0,   292,
       0,   293,   255,   555,     0,     0,   303,   258,   241,   337,
     348,     0,   343,   339,   375,     0,   378,   377,     0,   481,
       0,   483,     0,   489,     0,   486,   491,   440,     0,     0,
     366,   354,   356,     0,   359,     0,   361,   380,   260,   228,
      30,   190,   397,   395,     0,   475,   307,     0,     0,     0,
     406,     0,    94,   101,     0,   408,     0,   399,   400,   398,
     296,   299,     0,     0,   346,     0,   341,   373,   542,   371,
     374,   555,   555,   555,   555,     0,   505,   368,   555,   555,
     555,   507,   555,   555,    54,     0,     0,   100,     0,   555,
       0,   555,   555,     0,   344,     0,     0,   370,   482,     0,
     479,   484,   487,   278,     0,     0,   351,     0,   353,   360,
       0,   357,   362,   311,   319,   535,    99,   542,   531,   532,
     401,   331,   304,   342,   372,   555,   535,   277,   555,   555,
     555,   555,   393,   480,   352,     0,   349,   355,   358,   555,
     350
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    64,    65,    66,   256,   385,   386,   265,
     266,   433,    68,    69,   200,    70,    71,   578,   714,    72,
      73,   267,    74,    75,    76,   458,    77,   201,   374,   375,
     192,   193,   194,   596,   178,   179,    79,   238,   272,   558,
     706,   425,   426,   247,   248,   240,   417,   427,   517,    80,
     196,   445,   271,   286,   214,   749,   215,   750,   626,   892,
     101,   717,   102,   722,   877,   103,   590,   381,   595,   820,
     259,   389,   616,   738,   739,   228,   659,   660,   661,   782,
     686,   687,   767,   898,   899,   474,   666,   324,   512,    82,
      83,   370,   572,   571,   401,   889,   599,   732,   822,   826,
      84,    85,   299,   300,   488,    86,    87,    88,   610,   631,
     493,   494,   495,   678,    89,    90,    91,   293,    92,    93,
     203,   204,    96,   205,   584,   716,   593,   594,   476,   477,
     478,   479,   480,   785,   786,   481,   482,   483,   484,   774,
     668,   182,   380,   277,   428,   243,   231,   621,   560,   362,
     210,   422,   423,   702,   449,   390,   254,   398,   213,   269
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -764
static const yytype_int16 yypact[] =
{
    -764,   114,  2475,  -764,  7319,    41,  -764,  5605,  6688,  -764,
    8827,  8827,  5066,  -764,  -764,  9175,  8015,  8015,  -764,  -764,
    8015,  3174,  2730,  -764,  -764,  -764,  -764,   282,  6688,  -764,
       9,  -764,  -764,  5739,  2878,  -764,  -764,  5873,  -764,  -764,
    -764,  -764,  -764,  -764,  -764,  8943,  8943,    86,  4362,  8827,
    8131,  8363,  6965,  -764,  6396,   780,   492,   914,   948,   630,
    -764,   101,  9059,  8943,  -764,   376,  -764,   957,  -764,   540,
    -764,  -764,   156,   122,  -764,    94,  9287,  -764,   146,  2857,
      37,   193,    25,    57,  -764,  -764,  -764,  -764,  -764,  -764,
    -764,  -764,  -764,  -764,    49,   217,  -764,   419,    50,  -764,
    -764,  9399,   230,  9399,  -764,  -764,  -764,  -764,  -764,  -764,
    -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,
    -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,
    -764,    30,    43,    60,   104,  -764,  -764,  -764,  -764,  -764,
    -764,   201,   272,  -764,   292,  -764,   308,  -764,  -764,  -764,
    -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,
    -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,
    -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,
    -764,  -764,    25,  -764,  -764,  -764,  -764,  -764,  -764,  -764,
    -764,  6542,  -764,  -764,   178,  -764,  3511,   279,  4515,   540,
      68,   211,   206,    19,   267,    40,    68,  -764,  -764,   376,
     320,  -764,  -764,   207,  8827,  8827,  -764,  -764,  -764,   215,
     218,   227,   333,   611,  -764,  -764,  -764,   247,   373,   113,
     125,    50,  8943,  8943,  8943,  8943,  -764,  2857,   312,  -764,
    -764,   261,   269,  -764,  -764,  -764,  4950,  -764,  8015,  8015,
    -764,  -764,  5204,  8827,  -764,  -764,   285,  4668,  -764,   297,
     362,   477,  7551,  4362,   286,   376,   957,   296,   329,  -764,
     540,   296,   311,   341,   350,  -764,   312,   323,   350,  -764,
     421,  9511,   331,   324,   327,   339,   842,  -764,  -764,  -764,
    -764,   958,  -764,  -764,  -764,  -764,  -764,  -764,   501,   898,
    -764,  -764,   993,  -764,  1026,  -764,  1077,  -764,   638,   403,
     427,  -764,  -764,  -764,  -764,  5320,  8827,  8827,  8827,  8827,
    7551,  8827,  8827,  -764,  -764,  8479,  -764,  4362,  7084,   343,
    8479,  8943,  8943,  8943,  8943,  8943,  8943,  8943,  8943,  8943,
    8943,  8943,  8943,  8943,  8943,  8943,  8943,  8943,  8943,  8943,
    8943,  8943,  8943,  8943,  8943,  8943,  8943,  9796,  8015,  9877,
    -764,  -764, 10849,  -764,  -764,  -764,  9059,  9059,  -764,   383,
    -764,   346,   378,   409,  -764,   116,   356,  -764,  8827,  -764,
    8827,   894,  -764,   444,  -764,   503,   507,    42,  -764,  3655,
     498,  8943,  9958,  8015, 10039,  8943,  8943,  3933,   501,   320,
    8595,   508,  -764,   130,   130,   138,  -764,  -764, 10120,  8015,
   10201,  -764,  -764,  -764,  -764,  8943,  7203,  -764,  -764,  8247,
    -764,   296,   372,  -764,  -764,   296,  -764,   391,   395,  -764,
      55,  -764,  -764,  6688,  4071,   401,  9958, 10039,  8943,   957,
     296,  -764,  -764,  5455,   415,   296,  -764,  -764,  7435,  -764,
    -764,  8363,  -764,  -764,  -764,   360,    94,  9511,  -764,  9511,
   10282,  8015, 10363,   448,  -764,  -764,  -764,  -764,   749,  -764,
    -764,  -764,  -764,   757,    66,  -764,  -764,  -764,  -764,   432,
    -764,   442,   543,   455,   567,  -764,  -764,   549,  4668,  -764,
    -764,  -764,   501,  -764,   570,  -764,  -764,  -764,  -764,  -764,
    -764,  -764,  8943,  8943,  -764,  -764,  -764,  -764,  -764,  -764,
    -764,  -764,    32,  8943,  -764,   475,   482,  -764,   296,  9511,
     494,  -764,  -764,  -764,   534,  1972,  -764,  -764,   362,  9733,
    9733,  9733,  9733,  1045,  1045,  9815,  6707,  9733,  9733,  3005,
    3005,   845,   845,  2561,  1045,  1045,  1021,  1021,   738,   433,
     433,   362,   362,   362,  3266,  6128,  3358,  6249,  -764,   218,
    -764,   296,   490,  -764,   527,  -764,  -764,  3026,  -764,  -764,
    1996,    32,    32,  -764,  2267,  -764,  2857,  -764,  -764,  -764,
     360,  -764,   376,  -764,  -764,  -764, 10444,  8015, 10525,   540,
    3511,    55,   757,  3511,   376,  6834,  6688,  -764,  8711,   647,
    -764,   673,  -764,  2709,  6007,  2582,   296,    61,   208,   647,
    -764,   501,   654,    71,  -764,  -764,  -764,  -764,  -764,    95,
      99,   218,   296,   141,   143,   218,  8827,  -764,  8943,   312,
    -764,  7667,   269,  -764,  -764,  -764,  -764,  7203,  8247,  -764,
    -764,   529,  -764,  2857,   -11,   957,  -764,   350,   343,  -764,
     398,    45,   296,    48,   179,  8943,  -764,   749,   495,  -764,
     532,   296,  -764,   296,  4812,  4668,  -764,   757,  -764,  -764,
     757,  -764,  -764,   670,  -764,  -764,  -764,   541,  4668,  -764,
    -764,  -764,   362,   362,  -764,   735,  4812,  -764,  -764,   542,
    7783,  -764,  -764,  9511,  9059,  8943,   574,  9059,  9059,  -764,
     383,   547,   561,  9059,  9059,  -764,  -764,   383,  -764,    57,
     156,  4812,  4668,  8943,    32,  -764,  8827,  3511,   398,    45,
    -764,   296,   376,   683,  -764,   296,   690,  -764,  -764,  -764,
     475,  -764,   610,  -764,  4224,   697,  -764,  8827,   700,  -764,
    8943,  8943,   273,  8943,  8943,   703,  -764,  7899,  3794,  4812,
    4812,   145,   130,  -764,  8943,  -764,  -764,   572,  -764,  -764,
     387,  -764,   296,  1044,   573,  1101,  -764,   564,   576,   712,
     585,  -764,   587,   589,  -764,   590,  -764,   592,   590,  -764,
     600,   629,   296,   625,   605,  -764,   607,   609,  -764,   750,
    8943,   612,  -764,  2857,  8943,  -764,  2857,  -764,  2857,  -764,
    -764,  9059,  -764,  2857,  -764,  2857,  -764,  -764,  -764,   751,
     619,  2857,  4668,   376,   754,  -764,  -764,  -764,  -764,  -764,
     894,  9623,    68,  -764,  -764,  4812,  -764,  -764,    68,  -764,
    8943,  -764,  -764,   164,   755,   764,  -764,  -764,  8247,  -764,
     620,  1044,   593,  -764,  -764,   766,  -764,  -764,   757,  -764,
     670,  -764,   670,  -764,   670,  -764,  -764,  -764,  9735,   656,
    -764,   882,  -764,   882,  -764,   670,  -764,  -764,   633,  2857,
    -764,  2857,  -764,  -764,   639,  -764,  -764,  3511,  3511,   732,
    -764,   380,   327,   339,  3511,  -764,  3655,  -764,  -764,  -764,
    -764,  -764,  4812,  1044,   620,  1044,   645,  -764,   367,  -764,
    -764,   590,   649,   590,   590,   739,   394,  -764,   651,   660,
     590,  -764,   668,   590,  -764,   791,   806,   360, 10606,  8015,
   10687,   507,   673,   813,   620,  1044,   766,  -764,  -764,   670,
    -764,  -764,  -764,  -764, 10768,   882,  -764,   670,  -764,  -764,
     670,  -764,  -764,  -764,  -764,   103,    45,   296,    77,   127,
    -764,  -764,  -764,   620,  -764,   590,   679,   680,   590,   688,
     590,   590,   134,  -764,  -764,   670,  -764,  -764,  -764,   590,
    -764
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -764,  -764,  -764,   396,  -764,    31,  -764,  -456,   318,  -764,
      53,  -764,  -312,     1,    -5,   -22,  -764,  -578,  -764,   -15,
     821,  -165,   -18,   -62,  -256,  -418,     0,  1585,  -313,   734,
      -3,   -13,  -764,  -764,  -282,  -764,  1031,  1477,  -764,    46,
     276,  -286,   129,     4,  -764,  -373,  -226,    11,  -298,    -7,
    -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,
    -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,  -764,
     338,  -196,  -387,   -72,  -562,  -764,  -686,  -698,   195,  -764,
    -486,  -764,  -596,  -764,   -73,  -764,  -764,   144,  -764,  -764,
    -764,   -74,  -764,  -764,  -378,  -764,   -61,  -764,  -764,  -764,
    -764,  -764,   814,   934,  -764,  -764,  -764,  -764,  -764,  -211,
    -426,  -764,   365,  -764,  -764,  -764,    14,  -764,  -764,  -764,
    1245,  1493,   871,  1543,  -764,  -764,    59,  -230,  -757,  -432,
    -585,   -97,  -636,  -763,    23,   219,  -764,  -564,  -764,  -262,
     563,  -764,  -764,  -764,     3,  -380,   865,  -295,  -764,   705,
       5,   -24,  -208,  -548,  -245,    26,    96,  -764,    13,    -2
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -556
static const yytype_int16 yytable[] =
{
      99,   236,   236,   253,   177,   236,   206,   209,   226,   365,
     397,   199,   199,   199,   329,   255,   420,   618,   522,   242,
     242,   612,   195,   242,   485,   457,   251,   241,   241,   452,
     268,   241,   527,   454,   776,   100,   662,   735,   211,   633,
     311,   649,   195,   630,   431,   226,   630,   745,   275,   279,
     270,   292,   282,   274,   278,    67,   475,    67,   742,   444,
     581,   273,   559,   446,   567,   840,   679,   570,   250,   226,
     314,   633,   561,   -88,   751,   557,   363,   565,   315,   843,
     565,   388,   771,   363,   388,   711,   712,   492,   900,   783,
     -96,   211,   321,   322,   226,   664,   226,   559,   440,   567,
     911,   692,   369,   773,   760,   -67,   777,   606,   212,  -463,
     557,   684,   565,   360,     3,   625,   -97,   582,  -466,   485,
     -97,   787,   799,   622,   -99,  -324,   557,  -308,   565,   806,
     396,  -465,   438,  -277,   723,   -94,   -81,   726,   -94,  -463,
     -98,   261,   625,   894,   368,   239,   244,   -95,  -467,   245,
    -101,   743,   257,   685,   557,   565,  -101,   615,  -100,   309,
     310,   212,   518,  -100,   -88,   -94,   -96,   625,   -98,   900,
     -95,   361,   911,   815,   665,   652,  -466,   -96,   557,   734,
     565,   400,  -324,  -324,   323,   679,   859,   611,   292,  -465,
    -277,  -277,  -468,   364,  -532,   -88,   -86,   924,   -88,   249,
     364,   -88,   420,   457,   -90,   679,  -467,   843,   208,   403,
     404,   485,   776,   634,   399,   199,   199,   636,   747,   456,
     207,   208,   639,   207,   208,   662,   764,   953,   812,   -96,
     -96,   236,   644,   236,   236,   211,   583,   646,   325,   418,
     418,   326,   -89,   663,  -531,   268,   -91,   429,  -532,   242,
    -468,   242,  -531,   430,   432,   -97,   -97,   241,   633,   241,
     -86,   814,   330,   457,   630,   630,   250,   771,   207,   208,
     442,   887,   -93,   720,   226,   792,   771,   368,   443,   -98,
     -98,   357,   207,   208,   902,   -92,   -95,   -95,   -88,  -470,
     -90,   211,   -87,   625,   392,   -90,   212,   909,   744,   912,
     731,   721,   212,   514,   557,   212,   565,   366,   523,   268,
     691,   505,   506,   507,   508,   439,   378,   199,   199,   199,
     199,   226,   510,   511,   -98,   384,   -90,   391,   520,   -90,
     485,   662,   -90,   662,   387,   408,   516,   400,   358,   359,
      81,   516,    81,   236,   573,   575,   504,  -470,   202,   202,
     202,   393,   394,   227,   202,   202,   429,   395,   202,   402,
    -469,   212,   725,   830,   246,   836,   264,   249,    67,   450,
     450,   959,  -528,   509,   450,   421,   252,   424,   236,   589,
    -458,   591,   522,   724,   600,   436,    81,   202,   405,   -95,
     283,   429,   409,   410,   236,   648,  -462,   456,   411,   962,
     202,   585,   759,   415,  -529,   566,   756,   429,   416,   662,
     896,   613,   460,  -393,   283,  -464,   419,   748,  -469,   632,
     640,   915,   916,   788,  -535,   418,   418,  -268,   434,   617,
     617,   252,    99,   331,  -458,   208,   441,   457,  -458,   376,
     566,   376,   393,   437,   586,   -66,   236,   195,  -278,   208,
     226,   647,   226,   766,  -462,   888,   566,   456,   633,   429,
     579,   662,   447,   662,   580,   630,  -462,   453,   918,   461,
     462,   212,  -464,  -464,   502,  -393,   455,   669,   459,   669,
     577,   669,   934,   566,  -268,  -268,  -535,    67,   448,  -528,
     521,  -458,  -458,   662,   208,  -528,   645,   451,   503,   212,
     212,   587,   588,   208,   331,  -278,  -278,   486,   566,   367,
     688,   602,   226,   597,   926,   598,   264,   818,   614,   696,
     208,  -529,   635,  -462,  -462,   919,   920,  -529,   207,   208,
    -393,   756,  -393,  -393,    81,  -101,    81,   701,   637,   587,
     588,  -535,   638,  -535,  -535,   642,   656,  -531,   464,   465,
     466,   467,   202,   202,   839,   301,   295,   296,   485,   354,
     355,   356,   -81,   700,   655,   708,   -93,   438,   708,   688,
     688,   707,   236,   775,   710,   435,   778,   321,   322,   667,
     703,   264,   701,   729,   202,   429,   202,   202,   784,   670,
     202,   202,   728,  -100,   672,    81,   733,   736,   701,   736,
      81,    81,   673,   297,   298,   699,   -96,   736,   715,   516,
     195,   489,   676,   705,   490,   491,   705,   704,   675,   283,
     727,   752,  -253,   708,   -92,   625,   884,   199,   701,   690,
     418,   456,   886,   947,   566,   807,   557,   -88,   565,   768,
     757,   693,   763,   -98,   656,   264,   464,   465,   466,   467,
     694,   801,   557,    81,   202,   202,   202,   202,    81,   202,
     202,   768,   734,   202,   746,    81,   283,   699,   202,   406,
     407,   705,   523,   758,   -90,   795,   797,   -95,   212,   765,
     680,   802,   804,   490,   491,   779,   226,   737,   734,  -254,
     212,   307,   794,   817,   295,   296,   202,   701,   800,   501,
     819,   821,   295,   296,   202,   202,   825,   601,   -87,   212,
     829,   813,   688,   831,   844,   609,   202,   199,   202,   838,
     841,   656,   846,   464,   465,   466,   467,    81,   845,   847,
     823,   202,   828,   827,   848,    81,   850,   852,   199,   854,
     895,   297,   298,   450,   857,   858,   860,   202,   816,   297,
     298,   901,   861,   903,   863,   418,   865,   904,   768,  -255,
     867,   872,   468,   873,   876,   890,   910,   893,   913,   470,
     669,   669,    81,   669,   891,   669,   669,   907,   617,   870,
    -256,    81,   669,   914,   669,   669,   781,   917,   464,   465,
     466,   467,   925,   471,   933,   283,   929,   283,   935,   202,
     656,   943,   464,   465,   466,   467,   677,   937,   463,   331,
     464,   465,   466,   467,   226,   940,   944,   897,   212,   464,
     465,   466,   467,   952,   344,   345,    81,   468,  -531,  -532,
     641,   736,   955,   469,   470,   965,   224,   379,   958,   875,
     960,   657,   294,   961,   295,   296,   709,   658,   212,   468,
     951,   226,   762,   954,   808,   469,   470,   283,   471,   681,
     950,   472,   352,   353,   354,   355,   356,    98,   969,    98,
     302,   304,   306,   308,   927,    98,    98,    98,   181,   878,
     471,    98,    98,   472,   908,    98,   772,   382,   208,     0,
       0,   297,   298,   463,     0,   464,   465,   466,   467,   669,
     669,   669,   669,     0,   236,     0,   669,   669,   669,   212,
     669,   669,     0,    98,    98,     0,   331,   429,   212,   600,
     736,     0,     0,   701,   212,   202,     0,    98,    81,     0,
       0,    81,     0,   781,   468,   464,   465,   466,   467,     0,
     469,   470,     0,     0,     0,   463,     0,   464,   465,   466,
     467,     0,     0,   669,     0,     0,   669,   669,   669,   669,
     496,     0,   295,   296,   202,   471,   566,   669,   472,   352,
     353,   354,   355,   356,   468,     0,     0,   303,   295,   296,
     469,   470,   769,   770,     0,     0,   468,     0,     0,     0,
       0,   473,   469,   470,     0,     0,   780,   316,   317,   318,
     319,   320,    81,    81,   789,   471,     0,     0,   472,   297,
     298,   305,   295,   296,     0,     0,    81,   471,     0,     0,
     472,   486,   295,   296,    81,   297,   298,     0,     0,   809,
     810,   283,   202,     0,     0,   202,   202,     0,     0,     0,
       0,   202,   202,   592,   671,     0,   674,   237,   237,    81,
      81,   237,   824,     0,   202,    81,   498,   295,   296,   297,
     298,    98,     0,    98,     0,     0,   833,   834,   835,   297,
     298,     0,    81,     0,     0,   202,   258,   260,     0,    98,
      98,   237,   237,     0,     0,     0,    81,    81,    81,   499,
     295,   296,   331,   312,   313,   656,     0,   464,   465,   466,
     467,     0,     0,     0,   297,   298,     0,   344,   345,     0,
       0,    98,     0,    98,    98,     0,   331,    98,    98,     0,
       0,     0,    98,     0,     0,     0,     0,    98,    98,     0,
     874,   344,   345,     0,     0,     0,   657,   297,   298,   202,
     500,   295,   296,   885,   351,   352,   353,   354,   355,   356,
      81,     0,   656,     0,   464,   465,   466,   467,     0,   881,
       0,     0,     0,    81,     0,     0,   349,   350,   351,   352,
     353,   354,   355,   356,     0,     0,     0,     0,     0,     0,
      98,    98,    98,    98,    98,    98,    98,    98,   297,   298,
      98,     0,    98,   657,     0,    98,   906,     0,     0,   842,
       0,     0,   921,     0,   922,     0,     0,     0,     0,     0,
     923,     0,     0,     0,     0,    81,    81,     0,     0,     0,
       0,     0,    81,    98,    81,   487,     0,     0,     0,     0,
      81,    98,    98,   497,     0,     0,   497,     0,   497,     0,
     497,     0,   497,    98,     0,    98,     0,    94,     0,    94,
       0,     0,   180,     0,    98,     0,     0,   202,    98,     0,
     229,     0,    98,   237,   237,   237,   312,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,   237,     0,   237,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,   284,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   284,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,   849,   851,     0,   853,     0,
     855,   856,     0,     0,     0,     0,   180,   862,   180,   864,
     866,     0,     0,    98,     0,     0,   237,     0,     0,     0,
       0,   525,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,   539,   540,   541,   542,   543,   544,   545,
     546,   547,   548,   549,   550,   551,   552,   553,     0,   237,
       0,     0,     0,     0,     0,     0,     0,   574,   576,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   603,     0,   237,     0,   574,   576,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
     237,    94,     0,    94,     0,     0,   237,   237,     0,     0,
     237,     0,    98,     0,     0,    98,     0,     0,    98,     0,
       0,     0,     0,     0,   928,   930,   931,   932,     0,   643,
       0,   936,   938,   939,     0,   941,   942,     0,     0,   237,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,    98,   237,     0,     0,    95,     0,    95,     0,     0,
       0,     0,    94,     0,     0,     0,     0,    94,    94,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   963,     0,
       0,   964,   966,   967,   968,     0,   284,     0,   276,    98,
      98,     0,   970,   682,   683,     0,     0,     0,     0,     0,
       0,    95,     0,    98,   237,    97,     0,    97,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,   230,    98,
      94,     0,    98,    98,     0,    94,     0,     0,    98,    98,
       0,     0,    94,   284,     0,     0,    98,    98,     0,     0,
       0,    98,    98,     0,     0,     0,     0,    78,     0,    78,
       0,    97,     0,     0,     0,   285,     0,     0,     0,    98,
     225,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,    98,    98,     0,     0,   237,   285,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,     0,     0,    78,    94,     0,     0,     0,     0,     0,
       0,     0,    94,     0,   377,     0,   377,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,     0,   237,     0,     0,     0,    98,     0,   237,   237,
       0,     0,     0,     0,     0,     0,     0,    98,     0,    94,
       0,     0,     0,     0,     0,     0,   237,     0,    94,    95,
      98,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   284,     0,   284,     0,     0,     0,     0,   412,
     413,   414,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   603,   793,     0,   796,   798,
       0,     0,     0,    94,   803,   805,     0,     0,     0,    97,
       0,    97,    98,    98,   811,     0,     0,     0,     0,    98,
      95,    98,     0,     0,     0,    95,    95,    98,     0,     0,
       0,     0,     0,     0,   284,     0,     0,     0,     0,     0,
       0,   796,   798,     0,   803,   805,     0,     0,   237,     0,
       0,    78,     0,    78,    98,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,   515,     0,     0,    97,    97,   526,    95,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
      95,   237,     0,     0,   285,   869,     0,     0,     0,     0,
       0,     0,   871,     0,     0,    94,     0,     0,    94,     0,
       0,     0,    78,     0,     0,     0,     0,    78,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,   871,     0,    97,     0,     0,     0,     0,     0,   237,
      97,   285,     0,     0,     0,     0,     0,   526,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
      95,     0,   627,   629,     0,     0,   276,     0,     0,     0,
      78,     0,     0,     0,     0,    78,     0,     0,     0,    94,
      94,     0,    78,     0,     0,   524,     0,     0,     0,     0,
       0,     0,     0,    94,     0,   629,     0,    95,   276,     0,
       0,    94,    97,     0,     0,     0,    95,     0,   284,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,    94,    94,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,     0,    97,     0,    94,
       0,    95,    78,     0,     0,     0,    97,     0,     0,     0,
     689,     0,     0,    94,    94,    94,  -555,     0,     0,     0,
     285,     0,   285,     0,  -555,  -555,  -555,     0,     0,  -555,
    -555,  -555,     0,  -555,     0,     0,   695,     0,     0,    78,
       0,     0,  -555,  -555,     0,     0,     0,     0,    78,     0,
       0,    97,     0,  -555,  -555,     0,  -555,  -555,  -555,  -555,
    -555,     0,     0,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,     0,    94,   344,   345,
       0,     0,   285,     0,     0,     0,   882,     0,     0,     0,
      94,     0,     0,    78,     0,   730,     0,     0,     0,     0,
       0,     0,     0,    95,  -555,     0,    95,     0,     0,   346,
       0,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,     0,     0,   180,     0,   753,     0,     0,   755,     0,
       0,     0,     0,     0,   629,   276,     0,     0,     0,  -230,
       0,     0,    94,    94,     0,     0,     0,     0,     0,    94,
       0,    94,   761,    97,     0,     0,    97,    94,     0,  -555,
    -555,     0,  -555,     0,     0,   249,  -555,     0,  -555,  -555,
       0,     0,     0,     0,     0,     0,     0,    95,    95,     0,
       0,     0,     0,     0,     0,     0,     0,   791,     0,     0,
       0,    95,     0,     0,     0,    78,     0,     0,    78,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    95,     0,    97,    97,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     0,     0,   832,     0,     0,    95,     0,    97,
       0,   837,     0,     0,     0,     0,   285,     0,     0,     0,
       0,    95,    95,    95,     0,     0,     0,     0,     0,    78,
      78,     0,     0,     0,    97,    97,     0,     0,     0,     0,
      97,     0,     0,    78,     0,     0,     0,   868,     0,     0,
       0,    78,     0,     0,     0,     0,     0,    97,     0,   524,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,    97,    97,     0,     0,    78,    78,     0,     0,
       0,     0,    78,     0,     0,    95,     0,     0,     0,     0,
       0,   713,     0,     0,     0,   276,     0,     0,    95,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    78,    78,    78,     0,     0,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,     0,     0,   344,   345,    97,     0,     0,     0,     0,
       0,     0,     0,     0,   883,     0,     0,     0,    97,     0,
      95,    95,     0,     0,     0,     0,     0,    95,     0,    95,
       0,     0,     0,     0,   346,    95,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,     0,    78,     0,     0,
       0,   377,     0,     0,     0,     0,   880,     0,     0,     0,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,    97,     0,     0,     0,     0,     0,    97,     0,    97,
       0,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    78,    78,     0,     0,     0,     0,     0,    78,
       0,    78,     0,     0,     0,  -555,     4,    78,     5,     6,
       7,     8,     9,     0,     0,     0,    10,    11,     0,     0,
       0,    12,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    20,    21,    22,    23,    24,
      25,    26,     0,     0,    27,     0,     0,     0,     0,     0,
      28,    29,    30,    31,    32,     0,    33,    34,    35,    36,
      37,    38,     0,    39,    40,    41,     0,     0,    42,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,     0,     0,    48,    49,     0,
      50,    51,     0,    52,     0,    53,    54,    55,    56,    57,
      58,    59,  -277,     0,    60,     0,     0,     0,     0,     0,
    -277,  -277,  -277,     0,     0,  -277,  -277,  -277,     0,  -277,
       0,     0,     0,     0,    61,    62,    63,     0,     0,  -277,
    -277,  -277,     0,     0,     0,     0,     0,     0,     0,  -277,
    -277,     0,  -277,  -277,  -277,  -277,  -277,  -555,  -555,     0,
       0,     0,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,     0,     0,   344,   345,     0,
       0,     0,     0,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,     0,     0,  -277,  -277,
    -277,     0,   741,  -277,     0,     0,     0,     0,   346,  -277,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
    -277,     0,     0,     0,     0,     0,     0,     0,   -99,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,     0,     0,     0,   208,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -277,  -277,  -277,  -277,
    -392,     0,  -277,  -277,  -277,  -277,     0,     0,  -392,  -392,
    -392,     0,     0,  -392,  -392,  -392,     0,  -392,     0,     0,
       0,     0,     0,   695,     0,     0,  -392,  -392,  -392,     0,
       0,     0,     0,     0,     0,     0,     0,  -392,  -392,     0,
    -392,  -392,  -392,  -392,  -392,     0,     0,     0,     0,     0,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,     0,     0,   344,   345,     0,     0,     0,
       0,  -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,
    -392,  -392,  -392,  -392,     0,     0,  -392,  -392,  -392,     0,
       0,  -392,     0,     0,     0,     0,   346,  -392,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -392,     0,  -392,
    -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -392,  -392,  -392,  -392,  -392,  -269,   249,
    -392,  -392,  -392,  -392,     0,     0,  -269,  -269,  -269,     0,
       0,  -269,  -269,  -269,     0,  -269,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -269,  -269,  -269,     0,     0,
       0,     0,     0,     0,     0,  -269,  -269,     0,  -269,  -269,
    -269,  -269,  -269,     0,     0,     0,     0,     0,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,     0,     0,   344,   345,     0,     0,     0,     0,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,     0,     0,  -269,  -269,  -269,     0,     0,  -269,
       0,     0,     0,     0,   346,  -269,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,  -269,     0,     0,     0,
       0,     0,     0,     0,     0,  -269,  -269,  -269,  -269,  -269,
    -269,  -269,  -269,  -269,  -269,  -269,  -269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -269,  -269,  -269,  -269,  -555,     0,  -269,  -269,
    -269,  -269,     0,     0,  -555,  -555,  -555,     0,     0,  -555,
    -555,  -555,     0,  -555,     0,     0,     0,     0,     0,     0,
       0,     0,  -555,  -555,  -555,     0,     0,     0,     0,     0,
       0,     0,     0,  -555,  -555,     0,  -555,  -555,  -555,  -555,
    -555,     0,     0,     0,     0,     0,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,  -556,  -556,     0,
       0,   344,   345,     0,     0,     0,     0,  -555,  -555,  -555,
    -555,  -555,  -555,  -555,  -555,  -555,  -555,  -555,  -555,  -555,
       0,     0,  -555,  -555,  -555,     0,     0,  -555,     0,     0,
       0,     0,     0,  -555,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -555,     0,  -555,  -555,  -555,  -555,  -555,
    -555,  -555,  -555,  -555,  -555,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -555,
    -555,  -555,  -555,  -555,  -284,   249,  -555,  -555,  -555,  -555,
       0,     0,  -284,  -284,  -284,     0,     0,  -284,  -284,  -284,
       0,  -284,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -284,  -284,     0,     0,     0,     0,     0,     0,     0,
       0,  -284,  -284,     0,  -284,  -284,  -284,  -284,  -284,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,     0,     0,
    -284,  -284,  -284,     0,     0,  -284,  -535,     0,     0,     0,
       0,  -284,     0,     0,  -535,  -535,  -535,     0,     0,     0,
    -535,  -535,     0,  -535,     0,     0,     0,     0,     0,     0,
       0,  -284,  -535,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,  -284,  -284,  -535,  -535,     0,  -535,  -535,  -535,  -535,
    -535,     0,     0,     0,     0,     0,     0,     0,  -284,  -284,
    -284,  -284,     0,   246,  -284,  -284,  -284,  -284,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -535,  -535,  -535,
    -535,  -535,  -535,  -535,  -535,  -535,  -535,  -535,  -535,  -535,
       0,     0,  -535,  -535,  -535,     0,   697,     0,  -277,     0,
       0,     0,     0,     0,     0,     0,  -277,  -277,  -277,     0,
       0,     0,  -277,  -277,     0,  -277,     0,     0,     0,     0,
       0,     0,   -97,  -535,     0,  -535,  -535,  -535,  -535,  -535,
    -535,  -535,  -535,  -535,  -535,  -277,  -277,     0,  -277,  -277,
    -277,  -277,  -277,     0,     0,     0,     0,     0,     0,  -535,
    -535,  -535,  -535,   -89,     0,     0,  -535,     0,  -535,  -535,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,  -277,
    -277,  -277,     0,     0,  -277,  -277,  -277,     0,   698,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -99,  -277,     0,  -277,  -277,  -277,
    -277,  -277,  -277,  -277,  -277,  -277,  -277,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -277,  -277,  -277,   -91,     0,     0,  -277,     0,
    -277,  -277,   262,     0,     5,     6,     7,     8,     9,  -555,
    -555,  -555,    10,    11,     0,     0,  -555,    12,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,    20,    21,    22,    23,    24,    25,    26,     0,     0,
      27,     0,     0,     0,     0,     0,    28,     0,    30,    31,
      32,     0,    33,    34,    35,    36,    37,    38,     0,    39,
      40,    41,     0,     0,    42,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,     0,     0,    48,    49,     0,    50,    51,     0,    52,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,     0,     5,     6,
       7,     8,     9,  -555,  -555,  -555,    10,    11,     0,  -555,
    -555,    12,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    20,    21,    22,    23,    24,
      25,    26,     0,     0,    27,     0,     0,     0,     0,     0,
      28,     0,    30,    31,    32,     0,    33,    34,    35,    36,
      37,    38,     0,    39,    40,    41,     0,     0,    42,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,     0,     0,    48,    49,     0,
      50,    51,     0,    52,     0,    53,    54,    55,    56,    57,
      58,    59,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,     0,     0,     0,
       0,     0,     0,     0,     0,   262,     0,     5,     6,     7,
       8,     9,     0,     0,  -555,    10,    11,  -555,  -555,  -555,
      12,  -555,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,    20,    21,    22,    23,    24,    25,
      26,     0,     0,    27,     0,     0,     0,     0,     0,    28,
       0,    30,    31,    32,     0,    33,    34,    35,    36,    37,
      38,     0,    39,    40,    41,     0,     0,    42,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    47,     0,     0,    48,    49,     0,    50,
      51,     0,    52,     0,    53,    54,    55,    56,    57,    58,
      59,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,     0,     0,     0,     0,
       0,     0,     0,     0,   262,     0,     5,     6,     7,     8,
       9,     0,     0,  -555,    10,    11,  -555,  -555,  -555,    12,
       0,    13,    14,    15,    16,    17,    18,    19,     0,     0,
       0,     0,     0,    20,    21,    22,    23,    24,    25,    26,
       0,     0,    27,     0,     0,     0,     0,     0,    28,     0,
      30,    31,    32,     0,    33,    34,    35,    36,    37,    38,
       0,    39,    40,    41,     0,     0,    42,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,     0,     0,    48,    49,     0,    50,    51,
       0,    52,     0,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,    63,     0,     0,     0,     0,     0,
       0,     0,     4,     0,     5,     6,     7,     8,     9,     0,
       0,     0,    10,    11,     0,  -555,  -555,    12,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,    20,    21,    22,    23,    24,    25,    26,     0,     0,
      27,     0,     0,     0,     0,     0,    28,    29,    30,    31,
      32,     0,    33,    34,    35,    36,    37,    38,     0,    39,
      40,    41,     0,     0,    42,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,     0,     0,    48,    49,     0,    50,    51,     0,    52,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -555,     0,     0,     0,     0,
       0,     0,     0,  -555,  -555,   262,     0,     5,     6,     7,
       8,     9,     0,  -555,  -555,    10,    11,     0,     0,     0,
      12,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,    20,    21,    22,    23,    24,    25,
      26,     0,     0,    27,     0,     0,     0,     0,     0,    28,
       0,    30,    31,    32,     0,    33,    34,    35,    36,    37,
      38,     0,    39,    40,    41,     0,     0,    42,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    47,     0,     0,    48,    49,     0,    50,
      51,     0,    52,     0,    53,    54,    55,    56,    57,    58,
      59,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,     0,     0,     0,     0,
       0,     0,     0,   262,     0,     5,     6,     7,     8,     9,
       0,     0,     0,    10,    11,     0,  -555,  -555,    12,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,    20,    21,    22,    23,    24,    25,    26,     0,
       0,    27,     0,     0,     0,     0,     0,    28,     0,    30,
      31,    32,     0,    33,    34,    35,    36,    37,    38,     0,
      39,    40,    41,     0,     0,    42,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    47,     0,     0,   263,    49,     0,    50,    51,     0,
      52,     0,    53,    54,    55,    56,    57,    58,    59,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -555,     0,  -555,  -555,   262,     0,     5,     6,
       7,     8,     9,     0,     0,     0,    10,    11,     0,     0,
       0,    12,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    20,    21,    22,    23,    24,
      25,    26,     0,     0,    27,     0,     0,     0,     0,     0,
      28,     0,    30,    31,    32,     0,    33,    34,    35,    36,
      37,    38,     0,    39,    40,    41,     0,     0,    42,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,     0,     0,    48,    49,     0,
      50,    51,     0,    52,     0,    53,    54,    55,    56,    57,
      58,    59,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -555,     0,  -555,  -555,   262,
       0,     5,     6,     7,     8,     9,     0,     0,     0,    10,
      11,     0,     0,     0,    12,     0,    13,    14,    15,    16,
      17,    18,    19,     0,     0,     0,     0,     0,    20,    21,
      22,    23,    24,    25,    26,     0,     0,    27,     0,     0,
       0,     0,     0,    28,     0,    30,    31,    32,     0,    33,
      34,    35,    36,    37,    38,     0,    39,    40,    41,     0,
       0,    42,     0,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,     0,     0,
      48,    49,     0,    50,    51,     0,    52,     0,    53,    54,
      55,    56,    57,    58,    59,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    62,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -555,   262,     0,     5,     6,     7,     8,     9,
    -555,  -555,  -555,    10,    11,     0,     0,     0,    12,     0,
      13,    14,    15,    16,    17,    18,    19,     0,     0,     0,
       0,     0,    20,    21,    22,    23,    24,    25,    26,     0,
       0,    27,     0,     0,     0,     0,     0,    28,     0,    30,
      31,    32,     0,    33,    34,    35,    36,    37,    38,     0,
      39,    40,    41,     0,     0,    42,     0,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    47,     0,     0,    48,    49,     0,    50,    51,     0,
      52,     0,    53,    54,    55,    56,    57,    58,    59,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     5,     6,     7,     0,     9,     0,     0,
       0,    10,    11,     0,  -555,  -555,    12,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
      20,    21,    22,    23,    24,    25,    26,     0,     0,   221,
       0,     0,     0,     0,     0,     0,     0,     0,    31,    32,
       0,    33,    34,    35,    36,    37,    38,   232,    39,    40,
      41,     0,     0,    42,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   197,
       0,     0,   198,    49,     0,    50,    51,     0,   233,   234,
      53,    54,    55,    56,    57,    58,    59,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       6,     7,     0,     9,     0,     0,     0,    10,    11,    61,
     235,    63,    12,     0,    13,    14,    15,    16,    17,    18,
      19,     0,     0,     0,     0,     0,    20,    21,    22,    23,
      24,    25,    26,   208,     0,    27,     0,     0,     0,     0,
       0,     0,     0,     0,    31,    32,     0,    33,    34,    35,
      36,    37,    38,     0,    39,    40,    41,     0,     0,    42,
       0,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   197,     0,     0,   198,    49,
       0,    50,    51,     0,     0,     0,    53,    54,    55,    56,
      57,    58,    59,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     6,     7,
       0,     9,     0,     0,     0,    10,    11,     0,   207,   208,
      12,     0,    13,    14,    15,    16,    17,    18,    19,     0,
       0,     0,     0,     0,    20,    21,    22,    23,    24,    25,
      26,     0,     0,    27,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,     0,    33,    34,    35,    36,    37,
      38,     0,    39,    40,    41,     0,     0,    42,     0,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   197,     0,     0,   198,    49,     0,    50,
      51,     0,     0,     0,    53,    54,    55,    56,    57,    58,
      59,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     5,     6,     7,     8,     9,     0,     0,
       0,    10,    11,    61,    62,    63,    12,     0,    13,    14,
      15,    16,    17,    18,    19,     0,     0,     0,     0,     0,
      20,    21,    22,    23,    24,    25,    26,   208,     0,    27,
       0,     0,     0,     0,     0,    28,    29,    30,    31,    32,
       0,    33,    34,    35,    36,    37,    38,     0,    39,    40,
      41,     0,     0,    42,     0,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    47,
       0,     0,    48,    49,     0,    50,    51,     0,    52,     0,
      53,    54,    55,    56,    57,    58,    59,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      62,    63,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,     0,    10,    11,     0,     0,
       0,    12,   402,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    20,    21,    22,    23,    24,
      25,    26,     0,     0,    27,     0,     0,     0,     0,     0,
      28,     0,    30,    31,    32,     0,    33,    34,    35,    36,
      37,    38,     0,    39,    40,    41,     0,     0,    42,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,     0,     0,    48,    49,     0,
      50,    51,     0,    52,     0,    53,    54,    55,    56,    57,
      58,    59,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,     0,     0,     0,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,     0,     0,     0,     0,     0,
     138,   139,   140,   141,   142,   143,   144,   145,    35,    36,
     146,    38,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   147,   148,   149,   150,   151,   152,
     153,   154,   155,     0,     0,   156,   157,     0,     0,   158,
     159,   160,   161,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   162,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,     0,   173,   174,     0,     0,     0,
       0,     0,  -528,  -528,  -528,     0,  -528,     0,     0,     0,
    -528,  -528,     0,   175,   176,  -528,     0,  -528,  -528,  -528,
    -528,  -528,  -528,  -528,     0,  -528,     0,     0,     0,  -528,
    -528,  -528,  -528,  -528,  -528,  -528,     0,     0,  -528,     0,
       0,     0,     0,     0,     0,     0,     0,  -528,  -528,     0,
    -528,  -528,  -528,  -528,  -528,  -528,  -528,  -528,  -528,  -528,
       0,     0,  -528,     0,     0,  -528,  -528,     0,  -528,  -528,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -528,     0,
       0,  -528,  -528,     0,  -528,  -528,     0,  -528,  -528,  -528,
    -528,  -528,  -528,  -528,  -528,  -528,     0,     0,  -528,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -528,  -528,
    -528,     0,     0,     0,     0,     0,  -529,  -529,  -529,     0,
    -529,     0,  -528,     0,  -529,  -529,     0,     0,  -528,  -529,
       0,  -529,  -529,  -529,  -529,  -529,  -529,  -529,     0,  -529,
       0,     0,     0,  -529,  -529,  -529,  -529,  -529,  -529,  -529,
       0,     0,  -529,     0,     0,     0,     0,     0,     0,     0,
       0,  -529,  -529,     0,  -529,  -529,  -529,  -529,  -529,  -529,
    -529,  -529,  -529,  -529,     0,     0,  -529,     0,     0,  -529,
    -529,     0,  -529,  -529,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -529,     0,     0,  -529,  -529,     0,  -529,  -529,
       0,  -529,  -529,  -529,  -529,  -529,  -529,  -529,  -529,  -529,
       0,     0,  -529,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -529,  -529,  -529,     0,     0,     0,     0,     0,
    -531,  -531,  -531,     0,  -531,     0,  -529,     0,  -531,  -531,
       0,     0,  -529,  -531,     0,  -531,  -531,  -531,  -531,  -531,
    -531,  -531,     0,     0,     0,     0,     0,  -531,  -531,  -531,
    -531,  -531,  -531,  -531,     0,     0,  -531,     0,     0,     0,
       0,     0,     0,     0,     0,  -531,  -531,     0,  -531,  -531,
    -531,  -531,  -531,  -531,  -531,  -531,  -531,  -531,     0,     0,
    -531,     0,     0,  -531,  -531,     0,  -531,  -531,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -531,   740,     0,  -531,
    -531,     0,  -531,  -531,     0,  -531,  -531,  -531,  -531,  -531,
    -531,  -531,  -531,  -531,     0,     0,  -531,     0,     0,     0,
       0,     0,     0,   -97,     0,     0,     0,     0,     0,     0,
       0,  -533,  -533,  -533,     0,  -533,  -531,  -531,  -531,  -533,
    -533,     0,     0,     0,  -533,     0,  -533,  -533,  -533,  -533,
    -533,  -533,  -533,     0,     0,     0,  -531,     0,  -533,  -533,
    -533,  -533,  -533,  -533,  -533,     0,     0,  -533,     0,     0,
       0,     0,     0,     0,     0,     0,  -533,  -533,     0,  -533,
    -533,  -533,  -533,  -533,  -533,  -533,  -533,  -533,  -533,     0,
       0,  -533,     0,     0,  -533,  -533,     0,  -533,  -533,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -533,     0,     0,
    -533,  -533,     0,  -533,  -533,     0,  -533,  -533,  -533,  -533,
    -533,  -533,  -533,  -533,  -533,     0,     0,  -533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -534,  -534,  -534,     0,  -534,  -533,  -533,  -533,
    -534,  -534,     0,     0,     0,  -534,     0,  -534,  -534,  -534,
    -534,  -534,  -534,  -534,     0,     0,     0,  -533,     0,  -534,
    -534,  -534,  -534,  -534,  -534,  -534,     0,     0,  -534,     0,
       0,     0,     0,     0,     0,     0,     0,  -534,  -534,     0,
    -534,  -534,  -534,  -534,  -534,  -534,  -534,  -534,  -534,  -534,
       0,     0,  -534,     0,     0,  -534,  -534,     0,  -534,  -534,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -534,     0,
       0,  -534,  -534,     0,  -534,  -534,     0,  -534,  -534,  -534,
    -534,  -534,  -534,  -534,  -534,  -534,     0,     0,  -534,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -534,  -534,
    -534,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -534,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,     0,     0,     0,   128,   129,   130,   183,
     184,   185,   186,   135,   136,   137,     0,     0,     0,     0,
       0,   138,   139,   140,   187,   188,   143,   189,   145,   287,
     288,   190,   289,     0,     0,     0,     0,     0,     0,   290,
       0,     0,     0,     0,     0,   147,   148,   149,   150,   151,
     152,   153,   154,   155,     0,     0,   156,   157,     0,     0,
     158,   159,   160,   161,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   162,     0,     0,     0,     0,     0,
       0,   291,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,     0,   173,   174,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   175,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,     0,
       0,     0,   128,   129,   130,   183,   184,   185,   186,   135,
     136,   137,     0,     0,     0,     0,     0,   138,   139,   140,
     187,   188,   143,   189,   145,   287,   288,   190,   289,     0,
       0,     0,     0,     0,     0,   290,     0,     0,     0,     0,
       0,   147,   148,   149,   150,   151,   152,   153,   154,   155,
       0,     0,   156,   157,     0,     0,   158,   159,   160,   161,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     162,     0,     0,     0,     0,     0,     0,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,     0,   173,   174,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     175,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,     0,     0,     0,   128,   129,
     130,   183,   184,   185,   186,   135,   136,   137,     0,     0,
       0,     0,     0,   138,   139,   140,   187,   188,   143,   189,
     145,     0,     0,   190,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   147,   148,   149,
     150,   151,   152,   153,   154,   155,     0,     0,   156,   157,
       0,     0,   158,   159,   160,   161,     0,     0,   331,   332,
     333,   334,   335,   336,   337,   338,   162,   340,   341,   191,
       0,     0,     0,   344,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,     0,   173,   174,
       0,     0,     0,     0,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   175,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,     0,     0,     0,   128,   129,   130,   183,   184,   185,
     186,   135,   136,   137,     0,     0,     0,     0,     0,   138,
     139,   140,   187,   188,   143,   189,   145,     0,     0,   190,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     0,     0,   156,   157,     0,     0,   158,   159,
     160,   161,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   162,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,     0,   173,   174,     0,     0,     5,     6,
       7,     0,     9,     0,     0,     0,    10,    11,     0,     0,
       0,    12,   175,    13,    14,    15,   216,   217,    18,    19,
       0,     0,     0,     0,     0,   218,   219,   220,    23,    24,
      25,    26,     0,     0,   221,     0,     0,     0,     0,     0,
       0,     0,     0,    31,    32,     0,    33,    34,    35,    36,
      37,    38,     0,    39,    40,    41,     0,     0,    42,     0,
       0,    43,    44,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   280,     0,     0,   198,    49,     0,
      50,    51,     0,     0,     0,    53,    54,    55,    56,    57,
      58,    59,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     6,     7,
       0,     9,     0,     0,   223,    10,    11,     0,     0,     0,
      12,     0,    13,    14,    15,   216,   217,    18,    19,     0,
       0,     0,   281,     0,   218,   219,   220,    23,    24,    25,
      26,     0,     0,   221,     0,     0,     0,     0,     0,     0,
       0,     0,    31,    32,     0,    33,    34,    35,    36,    37,
      38,     0,    39,    40,    41,     0,     0,    42,     0,     0,
      43,    44,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   280,     0,     0,   198,    49,     0,    50,
      51,     0,     0,     0,    53,    54,    55,    56,    57,    58,
      59,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     0,
       9,     0,     0,   223,    10,    11,     0,     0,     0,    12,
       0,    13,    14,    15,   216,   217,    18,    19,     0,     0,
       0,   519,     0,   218,   219,   220,    23,    24,    25,    26,
       0,     0,   221,     0,     0,     0,     0,     0,     0,     0,
       0,    31,    32,     0,    33,    34,    35,    36,    37,    38,
     232,    39,    40,    41,     0,     0,    42,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,     0,     0,   198,    49,     0,    50,    51,
       0,   628,   234,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,   489,     0,     0,   490,   491,     0,     0,
       0,     0,     5,     6,     7,     8,     9,     0,     0,     0,
      10,    11,    61,   235,    63,    12,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,    20,
      21,    22,    23,    24,    25,    26,     0,     0,    27,     0,
       0,     0,     0,     0,    28,    29,    30,    31,    32,     0,
      33,    34,    35,    36,    37,    38,     0,    39,    40,    41,
       0,     0,    42,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,     0,
       0,    48,    49,     0,    50,    51,     0,    52,     0,    53,
      54,    55,    56,    57,    58,    59,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     0,     9,     0,     0,     0,    10,    11,    61,    62,
      63,    12,     0,    13,    14,    15,   216,   217,    18,    19,
       0,     0,     0,     0,     0,   218,   219,   220,    23,    24,
      25,    26,     0,     0,   221,     0,     0,     0,     0,     0,
       0,     0,     0,    31,    32,     0,    33,    34,    35,    36,
      37,    38,   232,    39,    40,    41,     0,     0,    42,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   197,     0,     0,   198,    49,     0,
      50,    51,     0,   628,     0,    53,    54,    55,    56,    57,
      58,    59,     0,     0,    60,   489,     0,     0,   490,   491,
       0,     0,     0,     0,     5,     6,     7,     8,     9,     0,
       0,     0,    10,    11,    61,   235,    63,    12,     0,    13,
      14,    15,    16,    17,    18,    19,     0,     0,     0,     0,
       0,    20,    21,    22,    23,    24,    25,    26,     0,     0,
      27,     0,     0,     0,     0,     0,    28,     0,    30,    31,
      32,     0,    33,    34,    35,    36,    37,    38,     0,    39,
      40,    41,     0,     0,    42,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,     0,     0,    48,    49,     0,    50,    51,     0,    52,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     0,     9,     0,     0,     0,    10,    11,
      61,    62,    63,    12,     0,    13,    14,    15,   216,   217,
      18,    19,     0,     0,     0,     0,     0,   218,   219,   220,
      23,    24,    25,    26,     0,     0,   221,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,     0,    33,    34,
      35,    36,    37,    38,     0,    39,    40,    41,     0,     0,
      42,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   197,     0,     0,   198,
      49,     0,    50,    51,     0,   754,     0,    53,    54,    55,
      56,    57,    58,    59,     0,     0,    60,   489,     0,     0,
     490,   491,     0,     0,     0,     0,     5,     6,     7,     0,
       9,     0,     0,     0,    10,    11,    61,   235,    63,    12,
       0,    13,    14,    15,   216,   217,    18,    19,     0,     0,
       0,     0,     0,   218,   219,   220,    23,    24,    25,    26,
       0,     0,   221,     0,     0,     0,     0,     0,     0,     0,
       0,    31,    32,     0,    33,    34,    35,    36,    37,    38,
       0,    39,    40,    41,     0,     0,    42,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,     0,     0,   198,    49,     0,    50,    51,
       0,   790,     0,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,   489,     0,     0,   490,   491,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,    61,   235,    63,    12,     0,    13,    14,    15,
     216,   217,    18,    19,     0,     0,     0,     0,     0,   218,
     219,   220,    23,    24,    25,    26,     0,     0,   221,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,     0,
      33,    34,    35,    36,    37,    38,     0,    39,    40,    41,
       0,     0,    42,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,     0,
       0,   198,    49,     0,    50,    51,     0,   628,     0,    53,
      54,    55,    56,    57,    58,    59,     0,     0,    60,   489,
       0,     0,   490,   491,     0,     0,     0,     0,     5,     6,
       7,     0,     9,     0,     0,     0,    10,    11,    61,   235,
      63,    12,     0,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,    20,    21,    22,    23,    24,
      25,    26,     0,     0,   221,     0,     0,     0,     0,     0,
       0,     0,     0,    31,    32,     0,    33,    34,    35,    36,
      37,    38,   232,    39,    40,    41,     0,     0,    42,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   197,     0,     0,   198,    49,     0,
      50,    51,     0,   233,   234,    53,    54,    55,    56,    57,
      58,    59,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     0,     9,     0,
       0,     0,    10,    11,    61,   235,    63,    12,     0,    13,
      14,    15,   216,   217,    18,    19,     0,     0,     0,     0,
       0,   218,   219,   220,    23,    24,    25,    26,     0,     0,
     221,     0,     0,     0,     0,     0,     0,     0,     0,    31,
      32,     0,    33,    34,    35,    36,    37,    38,   232,    39,
      40,    41,     0,     0,    42,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     197,     0,     0,   198,    49,     0,    50,    51,     0,   233,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     0,     9,     0,     0,     0,    10,    11,
      61,   235,    63,    12,     0,    13,    14,    15,   216,   217,
      18,    19,     0,     0,     0,     0,     0,   218,   219,   220,
      23,    24,    25,    26,     0,     0,   221,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,     0,    33,    34,
      35,    36,    37,    38,   232,    39,    40,    41,     0,     0,
      42,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   197,     0,     0,   198,
      49,     0,    50,    51,     0,     0,   234,    53,    54,    55,
      56,    57,    58,    59,     0,     0,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     0,
       9,     0,     0,     0,    10,    11,    61,   235,    63,    12,
       0,    13,    14,    15,   216,   217,    18,    19,     0,     0,
       0,     0,     0,   218,   219,   220,    23,    24,    25,    26,
       0,     0,   221,     0,     0,     0,     0,     0,     0,     0,
       0,    31,    32,     0,    33,    34,    35,    36,    37,    38,
     232,    39,    40,    41,     0,     0,    42,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,     0,     0,   198,    49,     0,    50,    51,
       0,     0,     0,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,    61,   235,    63,    12,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,    20,
      21,    22,    23,    24,    25,    26,     0,     0,   221,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,     0,
      33,    34,    35,    36,    37,    38,     0,    39,    40,    41,
       0,     0,    42,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,     0,
       0,   198,    49,     0,    50,    51,     0,   513,     0,    53,
      54,    55,    56,    57,    58,    59,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     0,     9,     0,     0,     0,    10,    11,    61,   235,
      63,    12,     0,    13,    14,    15,   216,   217,    18,    19,
       0,     0,     0,     0,     0,   218,   219,   220,    23,    24,
      25,    26,     0,     0,   221,     0,     0,     0,     0,     0,
       0,     0,     0,    31,    32,     0,    33,    34,    35,    36,
      37,    38,     0,    39,    40,    41,     0,     0,    42,     0,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   197,     0,     0,   198,    49,     0,
      50,    51,     0,   233,     0,    53,    54,    55,    56,    57,
      58,    59,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     0,     9,     0,
       0,     0,    10,    11,    61,   235,    63,    12,     0,    13,
      14,    15,   216,   217,    18,    19,     0,     0,     0,     0,
       0,   218,   219,   220,    23,    24,    25,    26,     0,     0,
     221,     0,     0,     0,     0,     0,     0,     0,     0,    31,
      32,     0,    33,    34,    35,    36,    37,    38,     0,    39,
      40,    41,     0,     0,    42,     0,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     197,     0,     0,   198,    49,     0,    50,    51,     0,   513,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     0,     9,     0,     0,     0,    10,    11,
      61,   235,    63,    12,     0,    13,    14,    15,    16,    17,
      18,    19,     0,     0,     0,     0,     0,    20,    21,    22,
      23,    24,    25,    26,     0,     0,    27,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,     0,    33,    34,
      35,    36,    37,    38,     0,    39,    40,    41,     0,     0,
      42,     0,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   197,     0,     0,   198,
      49,     0,    50,    51,     0,     0,     0,    53,    54,    55,
      56,    57,    58,    59,     0,     0,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     0,
       9,     0,     0,     0,    10,    11,    61,    62,    63,    12,
       0,    13,    14,    15,   216,   217,    18,    19,     0,     0,
       0,     0,     0,   218,   219,   220,    23,    24,    25,    26,
       0,     0,   221,     0,     0,     0,     0,     0,     0,     0,
       0,    31,    32,     0,    33,    34,    35,    36,    37,    38,
       0,    39,    40,    41,     0,     0,    42,     0,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,     0,     0,   198,    49,     0,    50,    51,
       0,     0,     0,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,    61,   235,    63,    12,     0,    13,    14,    15,
      16,    17,    18,    19,     0,     0,     0,     0,     0,    20,
      21,    22,    23,    24,    25,    26,     0,     0,   221,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,     0,
      33,    34,    35,    36,    37,    38,     0,    39,    40,    41,
       0,     0,    42,     0,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,     0,
       0,   198,    49,     0,    50,    51,     0,     0,     0,    53,
      54,    55,    56,    57,    58,    59,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     0,     9,     0,     0,     0,    10,    11,    61,   235,
      63,    12,     0,    13,    14,    15,   216,   217,    18,    19,
       0,     0,     0,     0,     0,   218,   219,   220,    23,    24,
      25,    26,     0,     0,   221,     0,     0,     0,     0,     0,
       0,     0,     0,    31,    32,     0,    33,    34,    35,    36,
      37,    38,     0,    39,    40,    41,     0,     0,    42,     0,
       0,    43,    44,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   222,     0,     0,    48,    49,     0,
      50,    51,     0,    52,     0,    53,    54,    55,    56,    57,
      58,    59,     0,     0,    60,     0,     0,     0,     0,     0,
       5,     6,     7,     0,     9,     0,     0,     0,    10,    11,
       0,     0,     0,    12,   223,    13,    14,    15,   216,   217,
      18,    19,     0,     0,     0,     0,     0,   218,   219,   220,
      23,    24,    25,    26,     0,     0,   221,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,     0,    33,    34,
      35,    36,    37,    38,     0,    39,    40,    41,     0,     0,
      42,     0,     0,    43,    44,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   280,     0,     0,   327,
      49,     0,    50,    51,     0,   328,     0,    53,    54,    55,
      56,    57,    58,    59,     0,     0,    60,     0,     0,     0,
       0,     0,     5,     6,     7,     0,     9,     0,     0,     0,
      10,    11,     0,     0,     0,    12,   223,    13,    14,    15,
     216,   217,    18,    19,     0,     0,     0,     0,     0,   218,
     219,   220,    23,    24,    25,    26,     0,     0,   221,     0,
       0,     0,     0,     0,     0,     0,     0,    31,    32,     0,
     371,    34,    35,    36,   372,    38,     0,    39,    40,    41,
       0,     0,    42,     0,     0,    43,    44,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   373,     0,
       0,   198,    49,     0,    50,    51,     0,     0,     0,    53,
      54,    55,    56,    57,    58,    59,     0,     0,    60,     0,
       0,     0,     0,     0,     5,     6,     7,     0,     9,     0,
       0,     0,    10,    11,     0,     0,     0,    12,   223,    13,
      14,    15,   216,   217,    18,    19,     0,     0,     0,     0,
       0,   218,   219,   220,    23,    24,    25,    26,     0,     0,
     221,     0,     0,     0,     0,     0,     0,     0,     0,    31,
      32,     0,    33,    34,    35,    36,    37,    38,     0,    39,
      40,    41,     0,     0,    42,     0,     0,    43,    44,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     280,     0,     0,   327,    49,     0,    50,    51,     0,     0,
       0,    53,    54,    55,    56,    57,    58,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     5,     6,     7,     0,
       9,     0,     0,     0,    10,    11,     0,     0,     0,    12,
     223,    13,    14,    15,   216,   217,    18,    19,     0,     0,
       0,     0,     0,   218,   219,   220,    23,    24,    25,    26,
       0,     0,   221,     0,     0,     0,     0,     0,     0,     0,
       0,    31,    32,     0,    33,    34,    35,    36,    37,    38,
       0,    39,    40,    41,     0,     0,    42,     0,     0,    43,
      44,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   879,     0,     0,   198,    49,     0,    50,    51,
       0,     0,     0,    53,    54,    55,    56,    57,    58,    59,
       0,     0,    60,     0,     0,     0,     0,     0,     5,     6,
       7,     0,     9,     0,     0,     0,    10,    11,     0,     0,
       0,    12,   223,    13,    14,    15,   216,   217,    18,    19,
       0,     0,     0,     0,     0,   218,   219,   220,    23,    24,
      25,    26,     0,     0,   221,     0,     0,     0,     0,     0,
       0,     0,     0,    31,    32,     0,    33,    34,    35,    36,
      37,    38,     0,    39,    40,    41,     0,     0,    42,     0,
       0,    43,    44,     0,   331,  -556,  -556,  -556,  -556,   336,
     337,     0,     0,  -556,  -556,     0,     0,     0,     0,   344,
     345,     0,     0,     0,   905,     0,     0,   198,    49,     0,
      50,    51,     0,     0,     0,    53,    54,    55,    56,    57,
      58,    59,     0,     0,    60,     0,     0,   554,   555,     0,
       0,   556,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,     0,     0,   223,   147,   148,   149,   150,   151,
     152,   153,   154,   155,     0,     0,   156,   157,     0,     0,
     158,   159,   160,   161,     0,     0,   331,   332,   333,   334,
     335,   336,   337,     0,   162,   340,   341,     0,     0,     0,
       0,   344,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,     0,   173,   174,   562,   563,
       0,     0,   564,     0,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   175,   249,   147,   148,   149,   150,
     151,   152,   153,   154,   155,     0,     0,   156,   157,     0,
       0,   158,   159,   160,   161,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   162,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,     0,   173,   174,   604,
     555,     0,     0,   605,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,   249,   147,   148,   149,
     150,   151,   152,   153,   154,   155,     0,     0,   156,   157,
       0,     0,   158,   159,   160,   161,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   162,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,     0,   173,   174,
     607,   563,     0,     0,   608,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   175,   249,   147,   148,
     149,   150,   151,   152,   153,   154,   155,     0,     0,   156,
     157,     0,     0,   158,   159,   160,   161,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   162,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,     0,   173,
     174,   619,   555,     0,     0,   620,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,   249,   147,
     148,   149,   150,   151,   152,   153,   154,   155,     0,     0,
     156,   157,     0,     0,   158,   159,   160,   161,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   162,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,     0,
     173,   174,   623,   563,     0,     0,   624,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   175,   249,
     147,   148,   149,   150,   151,   152,   153,   154,   155,     0,
       0,   156,   157,     0,     0,   158,   159,   160,   161,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   162,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
       0,   173,   174,   650,   555,     0,     0,   651,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   175,
     249,   147,   148,   149,   150,   151,   152,   153,   154,   155,
       0,     0,   156,   157,     0,     0,   158,   159,   160,   161,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     162,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,     0,   173,   174,   653,   563,     0,     0,   654,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     175,   249,   147,   148,   149,   150,   151,   152,   153,   154,
     155,     0,     0,   156,   157,     0,     0,   158,   159,   160,
     161,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   162,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,     0,   173,   174,   718,   555,     0,     0,   719,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   175,   249,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     0,     0,   156,   157,     0,     0,   158,   159,
     160,   161,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   162,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,     0,   173,   174,   568,   563,     0,     0,
     569,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   175,   249,   147,   148,   149,   150,   151,   152,
     153,   154,   155,     0,     0,   156,   157,     0,     0,   158,
     159,   160,   161,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   162,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,     0,   173,   174,   945,   555,     0,
       0,   946,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   175,   249,   147,   148,   149,   150,   151,
     152,   153,   154,   155,     0,     0,   156,   157,     0,     0,
     158,   159,   160,   161,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   162,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,     0,   173,   174,   948,   563,
       0,     0,   949,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   175,   249,   147,   148,   149,   150,
     151,   152,   153,   154,   155,     0,     0,   156,   157,     0,
       0,   158,   159,   160,   161,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   162,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,     0,   173,   174,   956,
     555,     0,     0,   957,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,   249,   147,   148,   149,
     150,   151,   152,   153,   154,   155,     0,     0,   156,   157,
       0,     0,   158,   159,   160,   161,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   162,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,     0,   173,   174,
     568,   563,     0,     0,   569,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   175,   249,   147,   148,
     149,   150,   151,   152,   153,   154,   155,     0,     0,   156,
     157,     0,     0,   158,   159,   160,   161,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   162,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,     0,   173,
     174,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175
};

static const yytype_int16 yycheck[] =
{
       2,    16,    17,    27,     7,    20,    11,    12,    15,    83,
     206,    10,    11,    12,    76,    28,   242,   404,   330,    16,
      17,   399,     8,    20,   286,   281,    22,    16,    17,   274,
      48,    20,   330,   278,   670,     4,   468,   599,    12,   419,
      62,   459,    28,   416,   252,    52,   419,   609,    50,    51,
      49,    54,    52,    50,    51,     2,   286,     4,   606,   267,
     373,    50,   357,   271,   359,   763,   492,   362,    22,    76,
      65,   451,   358,    25,   622,   357,    26,   359,    65,   765,
     362,    13,   667,    26,    13,   571,   572,   298,   845,   685,
      13,    65,    37,    38,   101,    29,   103,   392,   263,   394,
     863,   519,    98,   667,   652,   116,   670,   393,    12,    90,
     392,    79,   394,    88,     0,   410,    13,     1,    88,   381,
      25,   685,   700,   409,    25,    88,   408,    86,   410,   707,
      90,    88,    90,    88,   590,   116,   147,   593,    25,    90,
      13,    55,   437,   841,    98,    16,    17,    13,    88,    20,
      25,    90,   143,   121,   436,   437,   116,    27,   116,    58,
      59,    65,   327,    25,   116,   116,    25,   462,    25,   926,
      25,   146,   935,   721,   108,   461,   146,   116,   460,    15,
     462,    17,   145,   146,    28,   611,   782,   398,   191,   146,
     145,   146,    88,   143,   149,   147,   147,   895,   150,   149,
     143,   153,   428,   459,    25,   631,   146,   893,   153,   214,
     215,   473,   848,   421,   209,   214,   215,   425,   147,   281,
     152,   153,   430,   152,   153,   657,   658,   925,   714,   152,
     153,   246,   440,   248,   249,   209,   120,   445,   116,   241,
     242,   147,   147,   473,   149,   263,   147,   249,   149,   246,
     146,   248,   149,   252,   253,   152,   153,   246,   638,   248,
     147,   717,   116,   519,   637,   638,   220,   852,   152,   153,
     265,   833,   147,   586,   281,   693,   861,   231,   265,   152,
     153,    88,   152,   153,   848,   147,   152,   153,   147,    88,
     147,   265,   147,   588,    88,   116,   200,   861,    90,   863,
     598,   587,   206,   325,   586,   209,   588,    90,   330,   327,
     518,   316,   317,   318,   319,   262,    86,   316,   317,   318,
     319,   328,   321,   322,   116,   147,   147,   116,   328,   150,
     592,   763,   153,   765,    55,    88,   325,    17,   145,   146,
       2,   330,     4,   358,   366,   367,   315,   146,    10,    11,
      12,   145,   146,    15,    16,    17,   358,    90,    20,   152,
      88,   265,   592,    90,   149,   752,    48,   149,   315,   273,
     274,   935,    26,   320,   278,   246,   149,   248,   393,   378,
      88,   380,   694,   591,   386,    88,    48,    49,    55,   116,
      52,   393,   145,   146,   409,   457,    88,   459,    25,   947,
      62,   375,   647,    91,    26,   359,   632,   409,   147,   841,
     842,   400,    88,    26,    76,    88,   147,   613,   146,   416,
     433,   877,   878,   685,    26,   427,   428,    88,   143,   403,
     404,   149,   434,    71,    88,   153,   150,   693,   146,   101,
     394,   103,   145,   146,    88,   116,   461,   433,    88,   153,
     457,   448,   459,   661,   146,   833,   410,   519,   838,   461,
      51,   893,   151,   895,    55,   838,    88,   144,    88,   145,
     146,   375,   145,   146,    71,    88,    55,   479,   147,   481,
      97,   483,    88,   437,   145,   146,    88,   434,   147,   143,
     147,   145,   146,   925,   153,   149,   443,   147,    71,   403,
     404,   145,   146,   153,    71,   145,   146,    63,   462,    90,
     512,    13,   519,    10,   147,     8,   198,   725,    10,   543,
     153,   143,   150,   145,   146,   145,   146,   149,   152,   153,
     143,   757,   145,   146,   196,   116,   198,   561,   147,   145,
     146,   143,   147,   145,   146,   144,    51,   149,    53,    54,
      55,    56,   214,   215,   762,    63,    64,    65,   820,   126,
     127,   128,   147,   559,   116,   567,   147,    90,   570,   571,
     572,   567,   587,   670,   570,   257,   673,    37,    38,   147,
      90,   263,   606,   596,   246,   587,   248,   249,   685,   147,
     252,   253,   595,   116,    51,   257,   598,   599,   622,   601,
     262,   263,   147,   111,   112,   559,   116,   609,   582,   598,
     596,   110,    63,   567,   113,   114,   570,    90,    51,   281,
     594,   626,   147,   625,   147,   920,   822,   626,   652,   147,
     632,   693,   828,   919,   588,   709,   918,   147,   920,   663,
     637,   147,   147,   116,    51,   327,    53,    54,    55,    56,
     116,    90,   934,   315,   316,   317,   318,   319,   320,   321,
     322,   685,    15,   325,    10,   327,   328,   621,   330,    58,
      59,   625,   694,   144,   147,   697,   698,   116,   582,   147,
     110,   703,   704,   113,   114,   144,   693,    14,    15,   147,
     594,    61,   118,    10,    64,    65,   358,   721,   151,    61,
      10,    91,    64,    65,   366,   367,     9,   389,   147,   613,
      10,   716,   714,    10,   150,   397,   378,   716,   380,   147,
     147,    51,    10,    53,    54,    55,    56,   389,   152,   144,
     732,   393,   737,   735,   147,   397,   147,   147,   737,   147,
     147,   111,   112,   647,   144,   116,   121,   409,   722,   111,
     112,   848,   147,   850,   147,   757,   147,   854,   782,   147,
      10,    10,    92,   144,    10,    10,   863,   147,   865,    99,
     772,   773,   434,   775,    10,   777,   778,   121,   752,   801,
     147,   443,   784,   144,   786,   787,    51,    55,    53,    54,
      55,    56,   147,   123,    55,   457,   147,   459,   147,   461,
      51,    10,    53,    54,    55,    56,   488,   147,    51,    71,
      53,    54,    55,    56,   821,   147,    10,    51,   722,    53,
      54,    55,    56,    10,    86,    87,   488,    92,   149,   149,
     434,   833,   929,    98,    99,   147,    15,   103,   935,   813,
     937,    92,    62,   940,    64,    65,   570,    98,   752,    92,
     922,   858,   657,   926,   710,    98,    99,   519,   123,   494,
     921,   126,   124,   125,   126,   127,   128,     2,   965,     4,
      56,    57,    58,    59,   898,    10,    11,    12,     7,   820,
     123,    16,    17,   126,   861,    20,   667,   182,   153,    -1,
      -1,   111,   112,    51,    -1,    53,    54,    55,    56,   901,
     902,   903,   904,    -1,   919,    -1,   908,   909,   910,   813,
     912,   913,    -1,    48,    49,    -1,    71,   919,   822,   921,
     922,    -1,    -1,   947,   828,   587,    -1,    62,   590,    -1,
      -1,   593,    -1,    51,    92,    53,    54,    55,    56,    -1,
      98,    99,    -1,    -1,    -1,    51,    -1,    53,    54,    55,
      56,    -1,    -1,   955,    -1,    -1,   958,   959,   960,   961,
      62,    -1,    64,    65,   626,   123,   920,   969,   126,   124,
     125,   126,   127,   128,    92,    -1,    -1,    63,    64,    65,
      98,    99,   664,   665,    -1,    -1,    92,    -1,    -1,    -1,
      -1,   149,    98,    99,    -1,    -1,   678,    40,    41,    42,
      43,    44,   664,   665,   686,   123,    -1,    -1,   126,   111,
     112,    63,    64,    65,    -1,    -1,   678,   123,    -1,    -1,
     126,    63,    64,    65,   686,   111,   112,    -1,    -1,   711,
     712,   693,   694,    -1,    -1,   697,   698,    -1,    -1,    -1,
      -1,   703,   704,   149,   481,    -1,   483,    16,    17,   711,
     712,    20,   734,    -1,   716,   717,    63,    64,    65,   111,
     112,   196,    -1,   198,    -1,    -1,   748,   749,   750,   111,
     112,    -1,   734,    -1,    -1,   737,    45,    46,    -1,   214,
     215,    50,    51,    -1,    -1,    -1,   748,   749,   750,    63,
      64,    65,    71,    62,    63,    51,    -1,    53,    54,    55,
      56,    -1,    -1,    -1,   111,   112,    -1,    86,    87,    -1,
      -1,   246,    -1,   248,   249,    -1,    71,   252,   253,    -1,
      -1,    -1,   257,    -1,    -1,    -1,    -1,   262,   263,    -1,
     812,    86,    87,    -1,    -1,    -1,    92,   111,   112,   801,
      63,    64,    65,   825,   123,   124,   125,   126,   127,   128,
     812,    -1,    51,    -1,    53,    54,    55,    56,    -1,   821,
      -1,    -1,    -1,   825,    -1,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
     315,   316,   317,   318,   319,   320,   321,   322,   111,   112,
     325,    -1,   327,    92,    -1,   330,   858,    -1,    -1,    98,
      -1,    -1,   884,    -1,   886,    -1,    -1,    -1,    -1,    -1,
     892,    -1,    -1,    -1,    -1,   877,   878,    -1,    -1,    -1,
      -1,    -1,   884,   358,   886,   291,    -1,    -1,    -1,    -1,
     892,   366,   367,   299,    -1,    -1,   302,    -1,   304,    -1,
     306,    -1,   308,   378,    -1,   380,    -1,     2,    -1,     4,
      -1,    -1,     7,    -1,   389,    -1,    -1,   919,   393,    -1,
      15,    -1,   397,   232,   233,   234,   235,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   409,    -1,    -1,   246,    -1,   248,
     249,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,   434,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   443,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,   461,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   772,   773,    -1,   775,    -1,
     777,   778,    -1,    -1,    -1,    -1,   101,   784,   103,   786,
     787,    -1,    -1,   488,    -1,    -1,   325,    -1,    -1,    -1,
      -1,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,    -1,   358,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   366,   367,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   391,    -1,   393,    -1,   395,   396,    -1,    -1,
      -1,   400,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     409,   196,    -1,   198,    -1,    -1,   415,   416,    -1,    -1,
     419,    -1,   587,    -1,    -1,   590,    -1,    -1,   593,    -1,
      -1,    -1,    -1,    -1,   901,   902,   903,   904,    -1,   438,
      -1,   908,   909,   910,    -1,   912,   913,    -1,    -1,   448,
      -1,    -1,   451,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   626,   461,    -1,    -1,     2,    -1,     4,    -1,    -1,
      -1,    -1,   257,    -1,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   955,    -1,
      -1,   958,   959,   960,   961,    -1,   281,    -1,    51,   664,
     665,    -1,   969,   502,   503,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    -1,   678,   513,     2,    -1,     4,    -1,    -1,
      -1,   686,    -1,    -1,    -1,    -1,    -1,    -1,    15,   694,
     315,    -1,   697,   698,    -1,   320,    -1,    -1,   703,   704,
      -1,    -1,   327,   328,    -1,    -1,   711,   712,    -1,    -1,
      -1,   716,   717,    -1,    -1,    -1,    -1,     2,    -1,     4,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,   734,
      15,    -1,   737,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   748,   749,   750,    -1,    -1,   587,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   598,
      -1,    -1,    -1,    48,   389,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   397,    -1,   101,    -1,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   628,
      -1,    -1,   631,    -1,    -1,    -1,   801,    -1,   637,   638,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   812,    -1,   434,
      -1,    -1,    -1,    -1,    -1,    -1,   655,    -1,   443,   196,
     825,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   457,    -1,   459,    -1,    -1,    -1,    -1,   232,
     233,   234,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   690,    -1,    -1,    -1,   694,   695,    -1,   697,   698,
      -1,    -1,    -1,   488,   703,   704,    -1,    -1,    -1,   196,
      -1,   198,   877,   878,   713,    -1,    -1,    -1,    -1,   884,
     257,   886,    -1,    -1,    -1,   262,   263,   892,    -1,    -1,
      -1,    -1,    -1,    -1,   519,    -1,    -1,    -1,    -1,    -1,
      -1,   740,   741,    -1,   743,   744,    -1,    -1,   747,    -1,
      -1,   196,    -1,   198,   919,   754,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     257,    -1,   325,    -1,    -1,   262,   263,   330,   315,    -1,
      -1,    -1,    -1,   320,    -1,    -1,    -1,    -1,    -1,    -1,
     327,   790,    -1,    -1,   281,   794,    -1,    -1,    -1,    -1,
      -1,    -1,   801,    -1,    -1,   590,    -1,    -1,   593,    -1,
      -1,    -1,   257,    -1,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,    -1,
      -1,   830,    -1,   320,    -1,    -1,    -1,    -1,    -1,   838,
     327,   328,    -1,    -1,    -1,    -1,    -1,   400,    -1,    -1,
      -1,    -1,   389,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     397,    -1,   415,   416,    -1,    -1,   419,    -1,    -1,    -1,
     315,    -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,   664,
     665,    -1,   327,    -1,    -1,   330,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   678,    -1,   448,    -1,   434,   451,    -1,
      -1,   686,   389,    -1,    -1,    -1,   443,    -1,   693,    -1,
     397,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     919,    -1,    -1,    -1,    -1,    -1,   711,   712,    -1,    -1,
      -1,    -1,   717,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   389,    -1,    -1,   434,    -1,   734,
      -1,   488,   397,    -1,    -1,    -1,   443,    -1,    -1,    -1,
     513,    -1,    -1,   748,   749,   750,     0,    -1,    -1,    -1,
     457,    -1,   459,    -1,     8,     9,    10,    -1,    -1,    13,
      14,    15,    -1,    17,    -1,    -1,    44,    -1,    -1,   434,
      -1,    -1,    26,    27,    -1,    -1,    -1,    -1,   443,    -1,
      -1,   488,    -1,    37,    38,    -1,    40,    41,    42,    43,
      44,    -1,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,   812,    86,    87,
      -1,    -1,   519,    -1,    -1,    -1,   821,    -1,    -1,    -1,
     825,    -1,    -1,   488,    -1,   598,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   590,    88,    -1,   593,    -1,    -1,   117,
      -1,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,   858,    -1,   628,    -1,    -1,   631,    -1,
      -1,    -1,    -1,    -1,   637,   638,    -1,    -1,    -1,   147,
      -1,    -1,   877,   878,    -1,    -1,    -1,    -1,    -1,   884,
      -1,   886,   655,   590,    -1,    -1,   593,   892,    -1,   143,
     144,    -1,   146,    -1,    -1,   149,   150,    -1,   152,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   664,   665,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   690,    -1,    -1,
      -1,   678,    -1,    -1,    -1,   590,    -1,    -1,   593,   686,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   711,   712,    -1,   664,   665,    -1,
     717,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   678,    -1,    -1,   747,    -1,    -1,   734,    -1,   686,
      -1,   754,    -1,    -1,    -1,    -1,   693,    -1,    -1,    -1,
      -1,   748,   749,   750,    -1,    -1,    -1,    -1,    -1,   664,
     665,    -1,    -1,    -1,   711,   712,    -1,    -1,    -1,    -1,
     717,    -1,    -1,   678,    -1,    -1,    -1,   790,    -1,    -1,
      -1,   686,    -1,    -1,    -1,    -1,    -1,   734,    -1,   694,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   748,   749,   750,    -1,    -1,   711,   712,    -1,    -1,
      -1,    -1,   717,    -1,    -1,   812,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,   838,    -1,    -1,   825,   734,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   748,   749,   750,    -1,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    86,    87,   812,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   821,    -1,    -1,    -1,   825,    -1,
     877,   878,    -1,    -1,    -1,    -1,    -1,   884,    -1,   886,
      -1,    -1,    -1,    -1,   117,   892,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,   812,    -1,    -1,
      -1,   858,    -1,    -1,    -1,    -1,   821,    -1,    -1,    -1,
     825,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     877,   878,    -1,    -1,    -1,    -1,    -1,   884,    -1,   886,
      -1,    -1,    -1,    -1,    -1,   892,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   877,   878,    -1,    -1,    -1,    -1,    -1,   884,
      -1,   886,    -1,    -1,    -1,     0,     1,   892,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,
      -1,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    46,    47,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    -1,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,
      95,    96,    -1,    98,    -1,   100,   101,   102,   103,   104,
     105,   106,     0,    -1,   109,    -1,    -1,    -1,    -1,    -1,
       8,     9,    10,    -1,    -1,    13,    14,    15,    -1,    17,
      -1,    -1,    -1,    -1,   129,   130,   131,    -1,    -1,    27,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    41,    42,    43,    44,   152,   153,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    -1,    -1,    86,    87,
      88,    -1,    90,    91,    -1,    -1,    -1,    -1,   117,    97,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,   145,   146,   147,
       0,    -1,   150,   151,   152,   153,    -1,    -1,     8,     9,
      10,    -1,    -1,    13,    14,    15,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    26,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    -1,    -1,    86,    87,    88,    -1,
      -1,    91,    -1,    -1,    -1,    -1,   117,    97,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,   144,   145,   146,   147,     0,   149,
     150,   151,   152,   153,    -1,    -1,     8,     9,    10,    -1,
      -1,    13,    14,    15,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,    41,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    86,    87,    88,    -1,    -1,    91,
      -1,    -1,    -1,    -1,   117,    97,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,     0,    -1,   150,   151,
     152,   153,    -1,    -1,     8,     9,    10,    -1,    -1,    13,
      14,    15,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    37,    38,    -1,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    -1,    86,    87,    88,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    97,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,   145,   146,   147,     0,   149,   150,   151,   152,   153,
      -1,    -1,     8,     9,    10,    -1,    -1,    13,    14,    15,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    41,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    88,    -1,    -1,    91,     0,    -1,    -1,    -1,
      -1,    97,    -1,    -1,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    26,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    37,    38,    -1,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,   145,
     146,   147,    -1,   149,   150,   151,   152,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    -1,    86,    87,    88,    -1,    90,    -1,     0,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,    -1,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    37,    38,    -1,    40,    41,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,   143,
     144,   145,   146,   147,    -1,    -1,   150,    -1,   152,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    -1,    -1,    86,    87,    88,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,   145,   146,   147,    -1,    -1,   150,    -1,
     152,   153,     1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    -1,    -1,    15,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    47,    48,
      49,    -1,    51,    52,    53,    54,    55,    56,    -1,    58,
      59,    60,    -1,    -1,    63,    -1,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    92,    93,    -1,    95,    96,    -1,    98,
      -1,   100,   101,   102,   103,   104,   105,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,
       5,     6,     7,   152,   153,    10,    11,    12,    -1,    14,
      15,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    47,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    -1,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,
      95,    96,    -1,    98,    -1,   100,   101,   102,   103,   104,
     105,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    10,    11,    12,   152,   153,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    -1,    58,    59,    60,    -1,    -1,    63,    -1,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,    95,
      96,    -1,    98,    -1,   100,   101,   102,   103,   104,   105,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    10,    11,    12,   152,   153,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      47,    48,    49,    -1,    51,    52,    53,    54,    55,    56,
      -1,    58,    59,    60,    -1,    -1,    63,    -1,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    92,    93,    -1,    95,    96,
      -1,    98,    -1,   100,   101,   102,   103,   104,   105,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    -1,   152,   153,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,    48,
      49,    -1,    51,    52,    53,    54,    55,    56,    -1,    58,
      59,    60,    -1,    -1,    63,    -1,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    92,    93,    -1,    95,    96,    -1,    98,
      -1,   100,   101,   102,   103,   104,   105,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,   153,     1,    -1,     3,     4,     5,
       6,     7,    -1,     9,    10,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    47,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    -1,    58,    59,    60,    -1,    -1,    63,    -1,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,    95,
      96,    -1,    98,    -1,   100,   101,   102,   103,   104,   105,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    -1,   152,   153,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    47,
      48,    49,    -1,    51,    52,    53,    54,    55,    56,    -1,
      58,    59,    60,    -1,    -1,    63,    -1,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    92,    93,    -1,    95,    96,    -1,
      98,    -1,   100,   101,   102,   103,   104,   105,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,   152,   153,     1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,
      -1,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    47,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    -1,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,
      95,    96,    -1,    98,    -1,   100,   101,   102,   103,   104,
     105,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   150,    -1,   152,   153,     1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    47,    48,    49,    -1,    51,
      52,    53,    54,    55,    56,    -1,    58,    59,    60,    -1,
      -1,    63,    -1,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      92,    93,    -1,    95,    96,    -1,    98,    -1,   100,   101,
     102,   103,   104,   105,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,     1,    -1,     3,     4,     5,     6,     7,
     152,   153,    10,    11,    12,    -1,    -1,    -1,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    47,
      48,    49,    -1,    51,    52,    53,    54,    55,    56,    -1,
      58,    59,    60,    -1,    -1,    63,    -1,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    92,    93,    -1,    95,    96,    -1,
      98,    -1,   100,   101,   102,   103,   104,   105,   106,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    11,    12,    -1,   152,   153,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      -1,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    -1,    63,    -1,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    92,    93,    -1,    95,    96,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,   129,
     130,   131,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,   153,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    -1,    51,    52,    53,
      54,    55,    56,    -1,    58,    59,    60,    -1,    -1,    63,
      -1,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,
      -1,    95,    96,    -1,    -1,    -1,   100,   101,   102,   103,
     104,   105,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,    -1,    -1,    -1,    11,    12,    -1,   152,   153,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    -1,    58,    59,    60,    -1,    -1,    63,    -1,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,    95,
      96,    -1,    -1,    -1,   100,   101,   102,   103,   104,   105,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,   129,   130,   131,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,   153,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,
      -1,    51,    52,    53,    54,    55,    56,    -1,    58,    59,
      60,    -1,    -1,    63,    -1,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    92,    93,    -1,    95,    96,    -1,    98,    -1,
     100,   101,   102,   103,   104,   105,   106,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
     130,   131,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,
      -1,    16,   152,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    47,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    -1,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,
      95,    96,    -1,    98,    -1,   100,   101,   102,   103,   104,
     105,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    -1,    80,    81,    -1,    -1,    84,
      85,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,    -1,   148,   149,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    -1,    63,    -1,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    92,    93,    -1,    95,    96,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,   143,    -1,    11,    12,    -1,    -1,   149,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    26,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    -1,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    92,    93,    -1,    95,    96,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,    -1,   143,    -1,    11,    12,
      -1,    -1,   149,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      63,    -1,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    92,
      93,    -1,    95,    96,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,     7,   129,   130,   131,    11,
      12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    -1,   149,    -1,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    -1,
      -1,    63,    -1,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      92,    93,    -1,    95,    96,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,   129,   130,   131,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,   149,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    -1,    63,    -1,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    92,    93,    -1,    95,    96,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    -1,    80,    81,    -1,    -1,
      84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    -1,    80,    81,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
      -1,    -1,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    -1,    80,    81,
      -1,    -1,    84,    85,    86,    87,    -1,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    98,    80,    81,   101,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   148,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    -1,    80,    81,    -1,    -1,    84,    85,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,   130,   131,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,
      -1,    16,   148,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    -1,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,
      95,    96,    -1,    -1,    -1,   100,   101,   102,   103,   104,
     105,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,    -1,    -1,   129,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,   147,    -1,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    -1,    51,    52,    53,    54,    55,
      56,    -1,    58,    59,    60,    -1,    -1,    63,    -1,    -1,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,    95,
      96,    -1,    -1,    -1,   100,   101,   102,   103,   104,   105,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,   129,    11,    12,    -1,    -1,    -1,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,   147,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    -1,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    92,    93,    -1,    95,    96,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,    -1,    -1,   113,   114,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,   129,   130,   131,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    -1,    58,    59,    60,
      -1,    -1,    63,    -1,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    92,    93,    -1,    95,    96,    -1,    98,    -1,   100,
     101,   102,   103,   104,   105,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,   129,   130,
     131,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,
      95,    96,    -1,    98,    -1,   100,   101,   102,   103,   104,
     105,   106,    -1,    -1,   109,   110,    -1,    -1,   113,   114,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,   129,   130,   131,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    47,    48,
      49,    -1,    51,    52,    53,    54,    55,    56,    -1,    58,
      59,    60,    -1,    -1,    63,    -1,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    92,    93,    -1,    95,    96,    -1,    98,
      -1,   100,   101,   102,   103,   104,   105,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,
     129,   130,   131,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    54,    55,    56,    -1,    58,    59,    60,    -1,    -1,
      63,    -1,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,
      93,    -1,    95,    96,    -1,    98,    -1,   100,   101,   102,
     103,   104,   105,   106,    -1,    -1,   109,   110,    -1,    -1,
     113,   114,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   129,   130,   131,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    -1,    51,    52,    53,    54,    55,    56,
      -1,    58,    59,    60,    -1,    -1,    63,    -1,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    92,    93,    -1,    95,    96,
      -1,    98,    -1,   100,   101,   102,   103,   104,   105,   106,
      -1,    -1,   109,   110,    -1,    -1,   113,   114,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,   129,   130,   131,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    -1,    58,    59,    60,
      -1,    -1,    63,    -1,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    92,    93,    -1,    95,    96,    -1,    98,    -1,   100,
     101,   102,   103,   104,   105,   106,    -1,    -1,   109,   110,
      -1,    -1,   113,   114,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,   129,   130,
     131,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,
      95,    96,    -1,    98,    99,   100,   101,   102,   103,   104,
     105,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,   129,   130,   131,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    -1,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    -1,    -1,    63,    -1,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    92,    93,    -1,    95,    96,    -1,    98,
      -1,   100,   101,   102,   103,   104,   105,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,
     129,   130,   131,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      63,    -1,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,
      93,    -1,    95,    96,    -1,    -1,    99,   100,   101,   102,
     103,   104,   105,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   129,   130,   131,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    -1,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    92,    93,    -1,    95,    96,
      -1,    -1,    -1,   100,   101,   102,   103,   104,   105,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,   129,   130,   131,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    -1,    58,    59,    60,
      -1,    -1,    63,    -1,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    92,    93,    -1,    95,    96,    -1,    98,    -1,   100,
     101,   102,   103,   104,   105,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,   129,   130,
     131,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    -1,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,
      95,    96,    -1,    98,    -1,   100,   101,   102,   103,   104,
     105,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,   129,   130,   131,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    -1,    51,    52,    53,    54,    55,    56,    -1,    58,
      59,    60,    -1,    -1,    63,    -1,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    92,    93,    -1,    95,    96,    -1,    98,
      -1,   100,   101,   102,   103,   104,   105,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,
     129,   130,   131,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    54,    55,    56,    -1,    58,    59,    60,    -1,    -1,
      63,    -1,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,
      93,    -1,    95,    96,    -1,    -1,    -1,   100,   101,   102,
     103,   104,   105,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   129,   130,   131,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    -1,    51,    52,    53,    54,    55,    56,
      -1,    58,    59,    60,    -1,    -1,    63,    -1,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    92,    93,    -1,    95,    96,
      -1,    -1,    -1,   100,   101,   102,   103,   104,   105,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,   129,   130,   131,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    -1,    58,    59,    60,
      -1,    -1,    63,    -1,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    92,    93,    -1,    95,    96,    -1,    -1,    -1,   100,
     101,   102,   103,   104,   105,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,   129,   130,
     131,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    -1,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,
      95,    96,    -1,    98,    -1,   100,   101,   102,   103,   104,
     105,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,
      -1,    -1,    -1,    16,   129,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    54,    55,    56,    -1,    58,    59,    60,    -1,    -1,
      63,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    92,
      93,    -1,    95,    96,    -1,    98,    -1,   100,   101,   102,
     103,   104,   105,   106,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,    -1,    -1,    -1,    16,   129,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,
      51,    52,    53,    54,    55,    56,    -1,    58,    59,    60,
      -1,    -1,    63,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    92,    93,    -1,    95,    96,    -1,    -1,    -1,   100,
     101,   102,   103,   104,   105,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,    -1,    -1,    -1,    16,   129,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    -1,    51,    52,    53,    54,    55,    56,    -1,    58,
      59,    60,    -1,    -1,    63,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    92,    93,    -1,    95,    96,    -1,    -1,
      -1,   100,   101,   102,   103,   104,   105,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,    -1,    -1,    -1,    16,
     129,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    -1,    51,    52,    53,    54,    55,    56,
      -1,    58,    59,    60,    -1,    -1,    63,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    92,    93,    -1,    95,    96,
      -1,    -1,    -1,   100,   101,   102,   103,   104,   105,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,
      -1,    16,   129,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    -1,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    66,    67,    -1,    71,    72,    73,    74,    75,    76,
      77,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    89,    -1,    -1,    92,    93,    -1,
      95,    96,    -1,    -1,    -1,   100,   101,   102,   103,   104,
     105,   106,    -1,    -1,   109,    -1,    -1,    51,    52,    -1,
      -1,    55,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,    -1,   129,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    -1,    80,    81,    -1,    -1,
      84,    85,    86,    87,    -1,    -1,    71,    72,    73,    74,
      75,    76,    77,    -1,    98,    80,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,    51,    52,
      -1,    -1,    55,    -1,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   148,   149,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    -1,    -1,    80,    81,    -1,
      -1,    84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,    51,
      52,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    -1,    80,    81,
      -1,    -1,    84,    85,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
      51,    52,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    -1,    80,
      81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,    51,    52,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    -1,    -1,
      80,    81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,    51,    52,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    -1,
      -1,    80,    81,    -1,    -1,    84,    85,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,    51,    52,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   148,
     149,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      -1,    -1,    80,    81,    -1,    -1,    84,    85,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,    51,    52,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     148,   149,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    -1,    -1,    80,    81,    -1,    -1,    84,    85,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,    51,    52,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,   149,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    -1,    -1,    80,    81,    -1,    -1,    84,    85,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,   130,   131,    51,    52,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   148,   149,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    -1,    80,    81,    -1,    -1,    84,
      85,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,    51,    52,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   148,   149,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    -1,    -1,    80,    81,    -1,    -1,
      84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,    51,    52,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   148,   149,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    -1,    -1,    80,    81,    -1,
      -1,    84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,    51,
      52,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,   149,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    -1,    -1,    80,    81,
      -1,    -1,    84,    85,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
      51,    52,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   148,   149,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    -1,    -1,    80,
      81,    -1,    -1,    84,    85,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   148
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   155,   156,     0,     1,     3,     4,     5,     6,     7,
      11,    12,    16,    18,    19,    20,    21,    22,    23,    24,
      30,    31,    32,    33,    34,    35,    36,    39,    45,    46,
      47,    48,    49,    51,    52,    53,    54,    55,    56,    58,
      59,    60,    63,    66,    67,    69,    70,    89,    92,    93,
      95,    96,    98,   100,   101,   102,   103,   104,   105,   106,
     109,   129,   130,   131,   157,   158,   159,   164,   166,   167,
     169,   170,   173,   174,   176,   177,   178,   180,   181,   190,
     203,   224,   243,   244,   254,   255,   259,   260,   261,   268,
     269,   270,   272,   273,   274,   275,   276,   277,   300,   313,
     159,   214,   216,   219,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    45,    46,
      47,    48,    49,    50,    51,    52,    55,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    80,    81,    84,    85,
      86,    87,    98,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   130,   131,   148,   149,   184,   188,   189,
     274,   276,   295,    33,    34,    35,    36,    48,    49,    51,
      55,   101,   184,   185,   186,   270,   204,    89,    92,   167,
     168,   181,   224,   274,   275,   277,   168,   152,   153,   168,
     304,   309,   310,   312,   208,   210,    21,    22,    30,    31,
      32,    39,    89,   129,   174,   181,   203,   224,   229,   274,
     277,   300,    57,    98,    99,   130,   173,   190,   191,   196,
     199,   201,   298,   299,   196,   196,   149,   197,   198,   149,
     193,   197,   149,   305,   310,   185,   160,   143,   190,   224,
     190,    55,     1,    92,   162,   163,   164,   175,   176,   313,
     167,   206,   192,   201,   298,   313,   191,   297,   298,   313,
      89,   147,   180,   224,   274,   277,   207,    53,    54,    56,
      63,   105,   184,   271,    62,    64,    65,   111,   112,   256,
     257,    63,   256,    63,   256,    63,   256,    61,   256,    58,
      59,   169,   190,   190,   304,   312,    40,    41,    42,    43,
      44,    37,    38,    28,   241,   116,   147,    92,    98,   177,
     116,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    86,    87,   117,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    88,   145,   146,
      88,   146,   303,    26,   143,   245,    90,    90,   193,   197,
     245,    51,    55,    89,   182,   183,   224,   277,    86,   183,
     296,   221,   303,   105,   147,   161,   162,    55,    13,   225,
     309,   116,    88,   145,   146,    90,    90,   225,   311,   304,
      17,   248,   152,   168,   168,    55,    58,    59,    88,   145,
     146,    25,   191,   191,   191,    91,   147,   200,   313,   147,
     200,   196,   305,   306,   196,   195,   196,   201,   298,   313,
     167,   306,   167,   165,   143,   162,    88,   146,    90,   164,
     175,   150,   304,   312,   306,   205,   306,   151,   147,   308,
     310,   147,   308,   144,   308,    55,   177,   178,   179,   147,
      88,   145,   146,    51,    53,    54,    55,    56,    92,    98,
      99,   123,   126,   149,   239,   281,   282,   283,   284,   285,
     286,   289,   290,   291,   292,   293,    63,   257,   258,   110,
     113,   114,   263,   264,   265,   266,    62,   257,    63,    63,
      63,    61,    71,    71,   159,   168,   168,   168,   168,   164,
     167,   167,   242,    98,   169,   191,   201,   202,   175,   147,
     180,   147,   166,   169,   181,   190,   191,   202,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,    51,    52,    55,   188,   193,   301,
     302,   195,    51,    52,    55,   188,   193,   301,    51,    55,
     301,   247,   246,   169,   190,   169,   190,    97,   171,    51,
      55,   182,     1,   120,   278,   309,    88,   145,   146,   167,
     220,   167,   149,   280,   281,   222,   187,    10,     8,   250,
     313,   162,    13,   190,    51,    55,   195,    51,    55,   162,
     262,   263,   248,   201,    10,    27,   226,   309,   226,    51,
      55,   301,   195,    51,    55,   301,   212,   191,    98,   191,
     199,   263,   298,   299,   306,   150,   306,   147,   147,   306,
     185,   157,   144,   190,   306,   164,   306,   298,   177,   179,
      51,    55,   195,    51,    55,   116,    51,    92,    98,   230,
     231,   232,   283,   281,    29,   108,   240,   147,   294,   313,
     147,   294,    51,   147,   294,    51,    63,   162,   267,   264,
     110,   266,   190,   190,    79,   121,   234,   235,   313,   191,
     147,   306,   179,   147,   116,    44,   305,    90,    90,   193,
     197,   305,   307,    90,    90,   193,   194,   197,   313,   194,
     197,   234,   234,    44,   172,   309,   279,   215,    51,    55,
     182,   195,   217,   161,   306,   281,   161,   309,   184,   185,
     191,   202,   251,   313,    15,   228,   313,    14,   227,   228,
      90,    90,   307,    90,    90,   228,    10,   147,   225,   209,
     211,   307,   168,   191,    98,   191,   200,   298,   144,   308,
     307,   191,   232,   147,   283,   147,   306,   236,   305,   162,
     162,   284,   289,   291,   293,   285,   286,   291,   285,   144,
     162,    51,   233,   236,   285,   287,   288,   291,   293,   162,
      98,   191,   179,   190,   118,   169,   190,   169,   190,   171,
     151,    90,   169,   190,   169,   190,   171,   245,   241,   162,
     162,   190,   234,   168,   161,   307,   309,    10,   306,    10,
     223,    91,   252,   313,   162,     9,   253,   313,   168,    10,
      90,    10,   191,   162,   162,   162,   226,   191,   147,   306,
     231,   147,    98,   230,   150,   152,    10,   144,   147,   294,
     147,   294,   147,   294,   147,   294,   294,   144,   116,   236,
     121,   147,   294,   147,   294,   147,   294,    10,   191,   190,
     169,   190,    10,   144,   162,   309,    10,   218,   280,    89,
     181,   224,   274,   277,   225,   162,   225,   228,   248,   249,
      10,    10,   213,   147,   231,   147,   283,    51,   237,   238,
     282,   285,   291,   285,   285,    89,   224,   121,   288,   291,
     285,   287,   291,   285,   144,   161,   161,    55,    88,   145,
     146,   162,   162,   162,   231,   147,   147,   305,   294,   147,
     294,   294,   294,    55,    88,   147,   294,   147,   294,   294,
     147,   294,   294,    10,    10,    51,    55,   195,    51,    55,
     250,   227,    10,   231,   238,   285,    51,    55,   285,   291,
     285,   285,   307,   294,   294,   147,   294,   294,   294,   285,
     294
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (p, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, p)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, p); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, parser_state *p)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, p)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    parser_state *p;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (p);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, parser_state *p)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, p)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    parser_state *p;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, p);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, parser_state *p)
#else
static void
yy_reduce_print (yyvsp, yyrule, p)
    YYSTYPE *yyvsp;
    int yyrule;
    parser_state *p;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , p);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, p); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, parser_state *p)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, p)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    parser_state *p;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (p);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (parser_state *p);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (parser_state *p)
#else
int
yyparse (p)
    parser_state *p;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 1149 "src\\parse.y"
    {
                      p->lstate = EXPR_BEG;
                      if (!p->locals) p->locals = cons(0,0);
                    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 1154 "src\\parse.y"
    {
                      p->tree = new_scope(p, (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 1160 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (2)].nd);
                    ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 1166 "src\\parse.y"
    {
                      (yyval.nd) = new_begin(p, 0);
                    ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 1170 "src\\parse.y"
    {
                      (yyval.nd) = new_begin(p, (yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 1174 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (3)].nd), newline_node((yyvsp[(3) - (3)].nd)));
                    ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 1178 "src\\parse.y"
    {
                      (yyval.nd) = new_begin(p, 0);
                    ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 1185 "src\\parse.y"
    {
                      (yyval.nd) = local_switch(p);
                    ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 1189 "src\\parse.y"
    {
                      yyerror(p, "BEGIN not supported");
                      local_resume(p, (yyvsp[(2) - (5)].nd));
                      (yyval.nd) = 0;
                    ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 1200 "src\\parse.y"
    {
                      if ((yyvsp[(2) - (4)].nd)) {
                        (yyval.nd) = new_rescue(p, (yyvsp[(1) - (4)].nd), (yyvsp[(2) - (4)].nd), (yyvsp[(3) - (4)].nd));
                      }
                      else if ((yyvsp[(3) - (4)].nd)) {
                        yywarn(p, "else without rescue is useless");
                        (yyval.nd) = push((yyvsp[(1) - (4)].nd), (yyvsp[(3) - (4)].nd));
                      }
                      else {
                        (yyval.nd) = (yyvsp[(1) - (4)].nd);
                      }
                      if ((yyvsp[(4) - (4)].nd)) {
                        if ((yyval.nd)) {
                          (yyval.nd) = new_ensure(p, (yyval.nd), (yyvsp[(4) - (4)].nd));
                        }
                        else {
                          (yyval.nd) = push((yyvsp[(4) - (4)].nd), new_nil(p));
                        }
                      }
                    ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 1223 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (2)].nd);
                    ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 1229 "src\\parse.y"
    {
                      (yyval.nd) = new_begin(p, 0);
                    ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 1233 "src\\parse.y"
    {
                      (yyval.nd) = new_begin(p, (yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 1237 "src\\parse.y"
    {
                        (yyval.nd) = push((yyvsp[(1) - (3)].nd), newline_node((yyvsp[(3) - (3)].nd)));
                    ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 1241 "src\\parse.y"
    {
                      (yyval.nd) = new_begin(p, (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 1246 "src\\parse.y"
    {p->lstate = EXPR_FNAME;;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 1247 "src\\parse.y"
    {
                      (yyval.nd) = new_alias(p, (yyvsp[(2) - (4)].id), (yyvsp[(4) - (4)].id));
                    ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 1251 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (2)].nd);
                    ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 1255 "src\\parse.y"
    {
                        (yyval.nd) = new_if(p, cond((yyvsp[(3) - (3)].nd)), (yyvsp[(1) - (3)].nd), 0);
                    ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 1259 "src\\parse.y"
    {
                      (yyval.nd) = new_unless(p, cond((yyvsp[(3) - (3)].nd)), (yyvsp[(1) - (3)].nd), 0);
                    ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 1263 "src\\parse.y"
    {
                      (yyval.nd) = new_while(p, cond((yyvsp[(3) - (3)].nd)), (yyvsp[(1) - (3)].nd));
                    ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 1267 "src\\parse.y"
    {
                      (yyval.nd) = new_until(p, cond((yyvsp[(3) - (3)].nd)), (yyvsp[(1) - (3)].nd));
                    ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 1271 "src\\parse.y"
    {
                      (yyval.nd) = new_rescue(p, (yyvsp[(1) - (3)].nd), list1(list3(0, 0, (yyvsp[(3) - (3)].nd))), 0);
                    ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 1275 "src\\parse.y"
    {
                      yyerror(p, "END not suported");
                      (yyval.nd) = new_postexe(p, (yyvsp[(3) - (4)].nd));
                    ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 1281 "src\\parse.y"
    {
                      (yyval.nd) = new_masgn(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 1285 "src\\parse.y"
    {
                      (yyval.nd) = new_op_asgn(p, (yyvsp[(1) - (3)].nd), (yyvsp[(2) - (3)].id), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 1289 "src\\parse.y"
    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[(1) - (6)].nd), intern("[]",2), (yyvsp[(3) - (6)].nd)), (yyvsp[(5) - (6)].id), (yyvsp[(6) - (6)].nd));
                    ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 1293 "src\\parse.y"
    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[(1) - (5)].nd), (yyvsp[(3) - (5)].id), 0), (yyvsp[(4) - (5)].id), (yyvsp[(5) - (5)].nd));
                    ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 1297 "src\\parse.y"
    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[(1) - (5)].nd), (yyvsp[(3) - (5)].id), 0), (yyvsp[(4) - (5)].id), (yyvsp[(5) - (5)].nd));
                    ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 1301 "src\\parse.y"
    {
                      yyerror(p, "constant re-assignment");
                      (yyval.nd) = 0;
                    ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 1306 "src\\parse.y"
    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[(1) - (5)].nd), (yyvsp[(3) - (5)].id), 0), (yyvsp[(4) - (5)].id), (yyvsp[(5) - (5)].nd));
                    ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 1310 "src\\parse.y"
    {
                      backref_error(p, (yyvsp[(1) - (3)].nd));
                      (yyval.nd) = new_begin(p, 0);
                    ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 1315 "src\\parse.y"
    {
                      (yyval.nd) = new_asgn(p, (yyvsp[(1) - (3)].nd), new_array(p, (yyvsp[(3) - (3)].nd)));
                    ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 1319 "src\\parse.y"
    {
                      (yyval.nd) = new_masgn(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 1323 "src\\parse.y"
    {
                      (yyval.nd) = new_masgn(p, (yyvsp[(1) - (3)].nd), new_array(p, (yyvsp[(3) - (3)].nd)));
                    ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 1330 "src\\parse.y"
    {
                      (yyval.nd) = new_asgn(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 1334 "src\\parse.y"
    {
                      (yyval.nd) = new_asgn(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 1342 "src\\parse.y"
    {
                      (yyval.nd) = new_and(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 1346 "src\\parse.y"
    {
                      (yyval.nd) = new_or(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 1350 "src\\parse.y"
    {
                      (yyval.nd) = call_uni_op(p, cond((yyvsp[(3) - (3)].nd)), "!");
                    ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 1354 "src\\parse.y"
    {
                      (yyval.nd) = call_uni_op(p, cond((yyvsp[(2) - (2)].nd)), "!");
                    ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 1361 "src\\parse.y"
    {
                      if (!(yyvsp[(1) - (1)].nd)) (yyval.nd) = new_nil(p);
                      else (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 1376 "src\\parse.y"
    {
                      local_nest(p);
                    ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 1382 "src\\parse.y"
    {
                      (yyval.nd) = new_block(p, (yyvsp[(3) - (5)].nd), (yyvsp[(4) - (5)].nd));
                      local_unnest(p);
                    ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 1389 "src\\parse.y"
    {
                      (yyval.nd) = new_fcall(p, (yyvsp[(1) - (2)].id), (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 1393 "src\\parse.y"
    {
                      args_with_block(p, (yyvsp[(2) - (3)].nd), (yyvsp[(3) - (3)].nd));
                      (yyval.nd) = new_fcall(p, (yyvsp[(1) - (3)].id), (yyvsp[(2) - (3)].nd));
                    ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 1398 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (4)].nd), (yyvsp[(3) - (4)].id), (yyvsp[(4) - (4)].nd));
                    ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 1402 "src\\parse.y"
    {
                      args_with_block(p, (yyvsp[(4) - (5)].nd), (yyvsp[(5) - (5)].nd));
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (5)].nd), (yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].nd));
                   ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 1407 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (4)].nd), (yyvsp[(3) - (4)].id), (yyvsp[(4) - (4)].nd));
                    ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 1411 "src\\parse.y"
    {
                      args_with_block(p, (yyvsp[(4) - (5)].nd), (yyvsp[(5) - (5)].nd));
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (5)].nd), (yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].nd));
                    ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 1416 "src\\parse.y"
    {
                      (yyval.nd) = new_super(p, (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 1420 "src\\parse.y"
    {
                      (yyval.nd) = new_yield(p, (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 1424 "src\\parse.y"
    {
                      (yyval.nd) = new_return(p, ret_args(p, (yyvsp[(2) - (2)].nd)));
                    ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 1428 "src\\parse.y"
    {
                      (yyval.nd) = new_break(p, ret_args(p, (yyvsp[(2) - (2)].nd)));
                    ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 1432 "src\\parse.y"
    {
                      (yyval.nd) = new_next(p, ret_args(p, (yyvsp[(2) - (2)].nd)));
                    ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 1438 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 1442 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (3)].nd);
                    ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 1449 "src\\parse.y"
    {
                      (yyval.nd) = list1((yyvsp[(2) - (3)].nd));
                    ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 1455 "src\\parse.y"
    {
                      (yyval.nd) = list1((yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 1459 "src\\parse.y"
    {
                      (yyval.nd) = list1(push((yyvsp[(1) - (2)].nd),(yyvsp[(2) - (2)].nd)));
                    ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 1463 "src\\parse.y"
    {
                      (yyval.nd) = list2((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 1467 "src\\parse.y"
    {
                      (yyval.nd) = list3((yyvsp[(1) - (5)].nd), (yyvsp[(3) - (5)].nd), (yyvsp[(5) - (5)].nd));
                    ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 1471 "src\\parse.y"
    {
                      (yyval.nd) = list2((yyvsp[(1) - (2)].nd), new_nil(p));
                    ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 1475 "src\\parse.y"
    {
                      (yyval.nd) = list3((yyvsp[(1) - (4)].nd), new_nil(p), (yyvsp[(4) - (4)].nd));
                    ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 1479 "src\\parse.y"
    {
                      (yyval.nd) = list2(0, (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 1483 "src\\parse.y"
    {
                      (yyval.nd) = list3(0, (yyvsp[(2) - (4)].nd), (yyvsp[(4) - (4)].nd));
                    ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 1487 "src\\parse.y"
    {
                      (yyval.nd) = list2(0, new_nil(p));
                    ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 1491 "src\\parse.y"
    {
                      (yyval.nd) = list3(0, new_nil(p), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 1498 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (3)].nd);
                    ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 1504 "src\\parse.y"
    {
                      (yyval.nd) = list1((yyvsp[(1) - (2)].nd));
                    ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 1508 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (3)].nd), (yyvsp[(2) - (3)].nd));
                    ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 1514 "src\\parse.y"
    {
                      (yyval.nd) = list1((yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 1518 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (2)].nd), (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 1524 "src\\parse.y"
    {
                      assignable(p, (yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 1528 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (4)].nd), intern("[]",2), (yyvsp[(3) - (4)].nd));
                    ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 1532 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].id), 0);
                    ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 1536 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].id), 0);
                    ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 1540 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].id), 0);
                    ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 1544 "src\\parse.y"
    {
                      if (p->in_def || p->in_single)
                        yyerror(p, "dynamic constant assignment");
                      (yyval.nd) = new_colon2(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].id));
                    ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 1550 "src\\parse.y"
    {
                      if (p->in_def || p->in_single)
                        yyerror(p, "dynamic constant assignment");
                      (yyval.nd) = new_colon3(p, (yyvsp[(2) - (2)].id));
                    ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 1556 "src\\parse.y"
    {
                      backref_error(p, (yyvsp[(1) - (1)].nd));
                      (yyval.nd) = 0;
                    ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 1563 "src\\parse.y"
    {
                      assignable(p, (yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1567 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (4)].nd), intern("[]",2), (yyvsp[(3) - (4)].nd));
                    ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 1571 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].id), 0);
                    ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 1575 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].id), 0);
                    ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 1579 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].id), 0);
                    ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 1583 "src\\parse.y"
    {
                      if (p->in_def || p->in_single)
                        yyerror(p, "dynamic constant assignment");
                      (yyval.nd) = new_colon2(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].id));
                    ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 1589 "src\\parse.y"
    {
                      if (p->in_def || p->in_single)
                        yyerror(p, "dynamic constant assignment");
                      (yyval.nd) = new_colon3(p, (yyvsp[(2) - (2)].id));
                    ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 1595 "src\\parse.y"
    {
                      backref_error(p, (yyvsp[(1) - (1)].nd));
                      (yyval.nd) = 0;
                    ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 1602 "src\\parse.y"
    {
                      yyerror(p, "class/module name must be CONSTANT");
                    ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1609 "src\\parse.y"
    {
                      (yyval.nd) = cons((node*)1, nsym((yyvsp[(2) - (2)].id)));
                    ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1613 "src\\parse.y"
    {
                      (yyval.nd) = cons((node*)0, nsym((yyvsp[(1) - (1)].id)));
                    ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 1617 "src\\parse.y"
    {
                      (yyval.nd) = cons((yyvsp[(1) - (3)].nd), nsym((yyvsp[(3) - (3)].id)));
                    ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 1626 "src\\parse.y"
    {
                      p->lstate = EXPR_ENDFN;
                      (yyval.id) = (yyvsp[(1) - (1)].id);
                    ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 1631 "src\\parse.y"
    {
                      p->lstate = EXPR_ENDFN;
                      (yyval.id) = (yyvsp[(1) - (1)].id);
                    ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 1642 "src\\parse.y"
    {
                      (yyval.nd) = new_undef(p, (yyvsp[(1) - (1)].id));
                    ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 1645 "src\\parse.y"
    {p->lstate = EXPR_FNAME;;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 1646 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (4)].nd), nsym((yyvsp[(4) - (4)].id)));
                    ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 1651 "src\\parse.y"
    { (yyval.id) = intern_c('|');   ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 1652 "src\\parse.y"
    { (yyval.id) = intern_c('^');   ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 1653 "src\\parse.y"
    { (yyval.id) = intern_c('&');   ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 1654 "src\\parse.y"
    { (yyval.id) = intern("<=>",3); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1655 "src\\parse.y"
    { (yyval.id) = intern("==",2);  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1656 "src\\parse.y"
    { (yyval.id) = intern("===",3); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 1657 "src\\parse.y"
    { (yyval.id) = intern("=~",2);  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 1658 "src\\parse.y"
    { (yyval.id) = intern("!~",2);  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 1659 "src\\parse.y"
    { (yyval.id) = intern_c('>');   ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 1660 "src\\parse.y"
    { (yyval.id) = intern(">=",2);  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 1661 "src\\parse.y"
    { (yyval.id) = intern_c('<');   ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 1662 "src\\parse.y"
    { (yyval.id) = intern("<=",2);  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1663 "src\\parse.y"
    { (yyval.id) = intern("!=",2);  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 1664 "src\\parse.y"
    { (yyval.id) = intern("<<",2);  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 1665 "src\\parse.y"
    { (yyval.id) = intern(">>",2);  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 1666 "src\\parse.y"
    { (yyval.id) = intern_c('+');   ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 1667 "src\\parse.y"
    { (yyval.id) = intern_c('-');   ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1668 "src\\parse.y"
    { (yyval.id) = intern_c('*');   ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 1669 "src\\parse.y"
    { (yyval.id) = intern_c('*');   ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 1670 "src\\parse.y"
    { (yyval.id) = intern_c('/');   ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1671 "src\\parse.y"
    { (yyval.id) = intern_c('%');   ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1672 "src\\parse.y"
    { (yyval.id) = intern("**",2);  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 1673 "src\\parse.y"
    { (yyval.id) = intern_c('!');   ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 1674 "src\\parse.y"
    { (yyval.id) = intern_c('~');   ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1675 "src\\parse.y"
    { (yyval.id) = intern("+@",2);  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1676 "src\\parse.y"
    { (yyval.id) = intern("-@",2);  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1677 "src\\parse.y"
    { (yyval.id) = intern("[]",2);  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1678 "src\\parse.y"
    { (yyval.id) = intern("[]=",3); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 1679 "src\\parse.y"
    { (yyval.id) = intern_c('`');   ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1697 "src\\parse.y"
    {
                      (yyval.nd) = new_asgn(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1701 "src\\parse.y"
    {
                      (yyval.nd) = new_asgn(p, (yyvsp[(1) - (5)].nd), new_rescue(p, (yyvsp[(3) - (5)].nd), list1(list3(0, 0, (yyvsp[(5) - (5)].nd))), 0));
                    ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1705 "src\\parse.y"
    {
                      (yyval.nd) = new_op_asgn(p, (yyvsp[(1) - (3)].nd), (yyvsp[(2) - (3)].id), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1709 "src\\parse.y"
    {
                      (yyval.nd) = new_op_asgn(p, (yyvsp[(1) - (5)].nd), (yyvsp[(2) - (5)].id), new_rescue(p, (yyvsp[(3) - (5)].nd), list1(list3(0, 0, (yyvsp[(5) - (5)].nd))), 0));
                    ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1713 "src\\parse.y"
    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[(1) - (6)].nd), intern("[]",2), (yyvsp[(3) - (6)].nd)), (yyvsp[(5) - (6)].id), (yyvsp[(6) - (6)].nd));
                    ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1717 "src\\parse.y"
    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[(1) - (5)].nd), (yyvsp[(3) - (5)].id), 0), (yyvsp[(4) - (5)].id), (yyvsp[(5) - (5)].nd));
                    ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1721 "src\\parse.y"
    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[(1) - (5)].nd), (yyvsp[(3) - (5)].id), 0), (yyvsp[(4) - (5)].id), (yyvsp[(5) - (5)].nd));
                    ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1725 "src\\parse.y"
    {
                      (yyval.nd) = new_op_asgn(p, new_call(p, (yyvsp[(1) - (5)].nd), (yyvsp[(3) - (5)].id), 0), (yyvsp[(4) - (5)].id), (yyvsp[(5) - (5)].nd));
                    ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1729 "src\\parse.y"
    {
                      yyerror(p, "constant re-assignment");
                      (yyval.nd) = new_begin(p, 0);
                    ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1734 "src\\parse.y"
    {
                      yyerror(p, "constant re-assignment");
                      (yyval.nd) = new_begin(p, 0);
                    ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1739 "src\\parse.y"
    {
                      backref_error(p, (yyvsp[(1) - (3)].nd));
                      (yyval.nd) = new_begin(p, 0);
                    ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1744 "src\\parse.y"
    {
                      (yyval.nd) = new_dot2(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1748 "src\\parse.y"
    {
                      (yyval.nd) = new_dot3(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1752 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "+", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1756 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "-", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1760 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "*", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1764 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "/", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1768 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "%", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1772 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "**", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1776 "src\\parse.y"
    {
                      (yyval.nd) = call_uni_op(p, call_bin_op(p, (yyvsp[(2) - (4)].nd), "**", (yyvsp[(4) - (4)].nd)), "-@");
                    ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1780 "src\\parse.y"
    {
                      (yyval.nd) = call_uni_op(p, call_bin_op(p, (yyvsp[(2) - (4)].nd), "**", (yyvsp[(4) - (4)].nd)), "-@");
                    ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1784 "src\\parse.y"
    {
                      (yyval.nd) = call_uni_op(p, (yyvsp[(2) - (2)].nd), "+@");
                    ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1788 "src\\parse.y"
    {
                      (yyval.nd) = call_uni_op(p, (yyvsp[(2) - (2)].nd), "-@");
                    ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1792 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "|", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1796 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "^", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1800 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "&", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1804 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "<=>", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1808 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), ">", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1812 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), ">=", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1816 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "<", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1820 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "<=", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1824 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "==", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1828 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "===", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1832 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "!=", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1836 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "=~", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1840 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "!~", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1844 "src\\parse.y"
    {
                      (yyval.nd) = call_uni_op(p, cond((yyvsp[(2) - (2)].nd)), "!");
                    ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1848 "src\\parse.y"
    {
                      (yyval.nd) = call_uni_op(p, cond((yyvsp[(2) - (2)].nd)), "~");
                    ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1852 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), "<<", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1856 "src\\parse.y"
    {
                      (yyval.nd) = call_bin_op(p, (yyvsp[(1) - (3)].nd), ">>", (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1860 "src\\parse.y"
    {
                      (yyval.nd) = new_and(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1864 "src\\parse.y"
    {
                      (yyval.nd) = new_or(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1868 "src\\parse.y"
    {
                      (yyval.nd) = new_if(p, cond((yyvsp[(1) - (6)].nd)), (yyvsp[(3) - (6)].nd), (yyvsp[(6) - (6)].nd));
                    ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1872 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1878 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                      if (!(yyval.nd)) (yyval.nd) = new_nil(p);
                    ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1886 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (2)].nd);
                    ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1890 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (4)].nd), new_hash(p, (yyvsp[(3) - (4)].nd)));
                    ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1894 "src\\parse.y"
    {
                      (yyval.nd) = cons(new_hash(p, (yyvsp[(1) - (2)].nd)), 0);
                    ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1900 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (3)].nd);
                    ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1912 "src\\parse.y"
    {
                      (yyval.nd) = cons((yyvsp[(1) - (2)].nd),0);
                    ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1916 "src\\parse.y"
    {
                      (yyval.nd) = cons(push((yyvsp[(1) - (4)].nd), new_hash(p, (yyvsp[(3) - (4)].nd))), 0);
                    ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1920 "src\\parse.y"
    {
                      (yyval.nd) = cons(list1(new_hash(p, (yyvsp[(1) - (2)].nd))), 0);
                    ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1926 "src\\parse.y"
    {
                      (yyval.nd) = cons(list1((yyvsp[(1) - (1)].nd)), 0);
                    ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1930 "src\\parse.y"
    {
                      (yyval.nd) = cons((yyvsp[(1) - (2)].nd), (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1934 "src\\parse.y"
    {
                      (yyval.nd) = cons(list1(new_hash(p, (yyvsp[(1) - (2)].nd))), (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1938 "src\\parse.y"
    {
                      (yyval.nd) = cons(push((yyvsp[(1) - (4)].nd), new_hash(p, (yyvsp[(3) - (4)].nd))), (yyvsp[(4) - (4)].nd));
                    ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1942 "src\\parse.y"
    {
                      (yyval.nd) = cons(0, (yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1947 "src\\parse.y"
    {
                      (yyval.stack) = p->cmdarg_stack;
                      CMDARG_PUSH(1);
                    ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1952 "src\\parse.y"
    {
                      p->cmdarg_stack = (yyvsp[(1) - (2)].stack);
                      (yyval.nd) = (yyvsp[(2) - (2)].nd);
                    ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1959 "src\\parse.y"
    {
                      (yyval.nd) = new_block_arg(p, (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1965 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (2)].nd);
                    ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1969 "src\\parse.y"
    {
                      (yyval.nd) = 0;
                    ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1975 "src\\parse.y"
    {
                      (yyval.nd) = cons((yyvsp[(1) - (1)].nd), 0);
                    ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1979 "src\\parse.y"
    {
                      (yyval.nd) = cons(new_splat(p, (yyvsp[(2) - (2)].nd)), 0);
                    ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1983 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1987 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (4)].nd), new_splat(p, (yyvsp[(4) - (4)].nd)));
                    ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1991 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (4)].nd), (yyvsp[(4) - (4)].nd));
                    ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1995 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (5)].nd), new_splat(p, (yyvsp[(5) - (5)].nd)));
                    ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 2001 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 2005 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (4)].nd), new_splat(p, (yyvsp[(4) - (4)].nd)));
                    ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 2009 "src\\parse.y"
    {
                      (yyval.nd) = list1(new_splat(p, (yyvsp[(2) - (2)].nd)));
                    ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 2022 "src\\parse.y"
    {
                      (yyval.nd) = new_fcall(p, (yyvsp[(1) - (1)].id), 0);
                    ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 2026 "src\\parse.y"
    {
                      (yyvsp[(1) - (1)].stack) = p->cmdarg_stack;
                      p->cmdarg_stack = 0;
                    ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 2032 "src\\parse.y"
    {
                      p->cmdarg_stack = (yyvsp[(1) - (4)].stack);
                      (yyval.nd) = (yyvsp[(3) - (4)].nd);
                    ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 2036 "src\\parse.y"
    {p->lstate = EXPR_ENDARG;;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 2037 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (4)].nd);
                    ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 2040 "src\\parse.y"
    {p->lstate = EXPR_ENDARG;;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 2041 "src\\parse.y"
    {
                      (yyval.nd) = 0;
                    ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 2045 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (3)].nd);
                    ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 2049 "src\\parse.y"
    {
                      (yyval.nd) = new_colon2(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].id));
                    ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 2053 "src\\parse.y"
    {
                      (yyval.nd) = new_colon3(p, (yyvsp[(2) - (2)].id));
                    ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 2057 "src\\parse.y"
    {
                      (yyval.nd) = new_array(p, (yyvsp[(2) - (3)].nd));
                    ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 2061 "src\\parse.y"
    {
                      (yyval.nd) = new_hash(p, (yyvsp[(2) - (3)].nd));
                    ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 2065 "src\\parse.y"
    {
                      (yyval.nd) = new_return(p, 0);
                    ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 2069 "src\\parse.y"
    {
                      (yyval.nd) = new_yield(p, (yyvsp[(3) - (4)].nd));
                    ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 2073 "src\\parse.y"
    {
                      (yyval.nd) = new_yield(p, 0);
                    ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 2077 "src\\parse.y"
    {
                      (yyval.nd) = new_yield(p, 0);
                    ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 2081 "src\\parse.y"
    {
                      (yyval.nd) = call_uni_op(p, cond((yyvsp[(3) - (4)].nd)), "!");
                    ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 2085 "src\\parse.y"
    {
                      (yyval.nd) = call_uni_op(p, new_nil(p), "!");
                    ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2089 "src\\parse.y"
    {
                      (yyval.nd) = new_fcall(p, (yyvsp[(1) - (2)].id), cons(0, (yyvsp[(2) - (2)].nd)));
                    ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 2094 "src\\parse.y"
    {
                      call_with_block(p, (yyvsp[(1) - (2)].nd), (yyvsp[(2) - (2)].nd));
                      (yyval.nd) = (yyvsp[(1) - (2)].nd);
                    ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2099 "src\\parse.y"
    {
                      local_nest(p);
                      (yyval.num) = p->lpar_beg;
                      p->lpar_beg = ++p->paren_nest;
                    ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 2106 "src\\parse.y"
    {
                      p->lpar_beg = (yyvsp[(2) - (4)].num);
                      (yyval.nd) = new_lambda(p, (yyvsp[(3) - (4)].nd), (yyvsp[(4) - (4)].nd));
                      local_unnest(p);
                    ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2115 "src\\parse.y"
    {
                      (yyval.nd) = new_if(p, cond((yyvsp[(2) - (6)].nd)), (yyvsp[(4) - (6)].nd), (yyvsp[(5) - (6)].nd));
                    ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2122 "src\\parse.y"
    {
                      (yyval.nd) = new_unless(p, cond((yyvsp[(2) - (6)].nd)), (yyvsp[(4) - (6)].nd), (yyvsp[(5) - (6)].nd));
                    ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2125 "src\\parse.y"
    {COND_PUSH(1);;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2125 "src\\parse.y"
    {COND_POP();;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2128 "src\\parse.y"
    {
                      (yyval.nd) = new_while(p, cond((yyvsp[(3) - (7)].nd)), (yyvsp[(6) - (7)].nd));
                    ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2131 "src\\parse.y"
    {COND_PUSH(1);;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2131 "src\\parse.y"
    {COND_POP();;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2134 "src\\parse.y"
    {
                      (yyval.nd) = new_until(p, cond((yyvsp[(3) - (7)].nd)), (yyvsp[(6) - (7)].nd));
                    ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2140 "src\\parse.y"
    {
                      (yyval.nd) = new_case(p, (yyvsp[(2) - (5)].nd), (yyvsp[(4) - (5)].nd));
                    ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2144 "src\\parse.y"
    {
                      (yyval.nd) = new_case(p, 0, (yyvsp[(3) - (4)].nd));
                    ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2148 "src\\parse.y"
    {COND_PUSH(1);;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2150 "src\\parse.y"
    {COND_POP();;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2153 "src\\parse.y"
    {
                      (yyval.nd) = new_for(p, (yyvsp[(2) - (9)].nd), (yyvsp[(5) - (9)].nd), (yyvsp[(8) - (9)].nd));
                    ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2157 "src\\parse.y"
    {
                      (yyval.num) = p->lineno;
                    ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2161 "src\\parse.y"
    {
                      if (p->in_def || p->in_single)
                        yyerror(p, "class definition in method body");
                      (yyval.nd) = local_switch(p);
                    ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2168 "src\\parse.y"
    {
                      (yyval.nd) = new_class(p, (yyvsp[(3) - (7)].nd), (yyvsp[(4) - (7)].nd), (yyvsp[(6) - (7)].nd));
                      SET_LINENO((yyval.nd), (yyvsp[(2) - (7)].num));
                      local_resume(p, (yyvsp[(5) - (7)].nd));
                    ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2174 "src\\parse.y"
    {
                      (yyval.num) = p->lineno;
                    ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2178 "src\\parse.y"
    {
                      (yyval.num) = p->in_def;
                      p->in_def = 0;
                    ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2183 "src\\parse.y"
    {
                      (yyval.nd) = cons(local_switch(p), (node*)(intptr_t)p->in_single);
                      p->in_single = 0;
                    ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2189 "src\\parse.y"
    {
                      (yyval.nd) = new_sclass(p, (yyvsp[(4) - (9)].nd), (yyvsp[(8) - (9)].nd));
                      SET_LINENO((yyval.nd), (yyvsp[(2) - (9)].num));
                      local_resume(p, (yyvsp[(7) - (9)].nd)->car);
                      p->in_def = (yyvsp[(5) - (9)].num);
                      p->in_single = (int)(intptr_t)(yyvsp[(7) - (9)].nd)->cdr;
                    ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2197 "src\\parse.y"
    {
                      (yyval.num) = p->lineno;
                    ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2201 "src\\parse.y"
    {
                      if (p->in_def || p->in_single)
                        yyerror(p, "module definition in method body");
                      (yyval.nd) = local_switch(p);
                    ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2208 "src\\parse.y"
    {
                      (yyval.nd) = new_module(p, (yyvsp[(3) - (6)].nd), (yyvsp[(5) - (6)].nd));
                      SET_LINENO((yyval.nd), (yyvsp[(2) - (6)].num));
                      local_resume(p, (yyvsp[(4) - (6)].nd));
                    ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2214 "src\\parse.y"
    {
                      p->in_def++;
                      (yyval.nd) = local_switch(p);
                    ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2221 "src\\parse.y"
    {
                      (yyval.nd) = new_def(p, (yyvsp[(2) - (6)].id), (yyvsp[(4) - (6)].nd), (yyvsp[(5) - (6)].nd));
                      local_resume(p, (yyvsp[(3) - (6)].nd));
                      p->in_def--;
                    ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2226 "src\\parse.y"
    {p->lstate = EXPR_FNAME;;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2227 "src\\parse.y"
    {
                      p->in_single++;
                      p->lstate = EXPR_ENDFN; /* force for args */
                      (yyval.nd) = local_switch(p);
                    ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2235 "src\\parse.y"
    {
                      (yyval.nd) = new_sdef(p, (yyvsp[(2) - (9)].nd), (yyvsp[(5) - (9)].id), (yyvsp[(7) - (9)].nd), (yyvsp[(8) - (9)].nd));
                      local_resume(p, (yyvsp[(6) - (9)].nd));
                      p->in_single--;
                    ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2241 "src\\parse.y"
    {
                      (yyval.nd) = new_break(p, 0);
                    ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2245 "src\\parse.y"
    {
                      (yyval.nd) = new_next(p, 0);
                    ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2249 "src\\parse.y"
    {
                      (yyval.nd) = new_redo(p);
                    ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2253 "src\\parse.y"
    {
                      (yyval.nd) = new_retry(p);
                    ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2259 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                      if (!(yyval.nd)) (yyval.nd) = new_nil(p);
                    ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2278 "src\\parse.y"
    {
                      (yyval.nd) = new_if(p, cond((yyvsp[(2) - (5)].nd)), (yyvsp[(4) - (5)].nd), (yyvsp[(5) - (5)].nd));
                    ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2285 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (2)].nd);
                    ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2291 "src\\parse.y"
    {
                      (yyval.nd) = list1(list1((yyvsp[(1) - (1)].nd)));
                    ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2298 "src\\parse.y"
    {
                      (yyval.nd) = new_arg(p, (yyvsp[(1) - (1)].id));
                    ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2302 "src\\parse.y"
    {
                      (yyval.nd) = new_masgn(p, (yyvsp[(2) - (3)].nd), 0);
                    ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2308 "src\\parse.y"
    {
                      (yyval.nd) = list1((yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2312 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2318 "src\\parse.y"
    {
                      (yyval.nd) = list3((yyvsp[(1) - (1)].nd),0,0);
                    ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2322 "src\\parse.y"
    {
                      (yyval.nd) = list3((yyvsp[(1) - (4)].nd), new_arg(p, (yyvsp[(4) - (4)].id)), 0);
                    ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2326 "src\\parse.y"
    {
                      (yyval.nd) = list3((yyvsp[(1) - (6)].nd), new_arg(p, (yyvsp[(4) - (6)].id)), (yyvsp[(6) - (6)].nd));
                    ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2330 "src\\parse.y"
    {
                      (yyval.nd) = list3((yyvsp[(1) - (3)].nd), (node*)-1, 0);
                    ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2334 "src\\parse.y"
    {
                      (yyval.nd) = list3((yyvsp[(1) - (5)].nd), (node*)-1, (yyvsp[(5) - (5)].nd));
                    ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2338 "src\\parse.y"
    {
                      (yyval.nd) = list3(0, new_arg(p, (yyvsp[(2) - (2)].id)), 0);
                    ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2342 "src\\parse.y"
    {
                      (yyval.nd) = list3(0, new_arg(p, (yyvsp[(2) - (4)].id)), (yyvsp[(4) - (4)].nd));
                    ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2346 "src\\parse.y"
    {
                      (yyval.nd) = list3(0, (node*)-1, 0);
                    ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2350 "src\\parse.y"
    {
                      (yyval.nd) = list3(0, (node*)-1, (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2356 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (6)].nd), (yyvsp[(3) - (6)].nd), (yyvsp[(5) - (6)].id), 0, (yyvsp[(6) - (6)].id));
                    ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2360 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (8)].nd), (yyvsp[(3) - (8)].nd), (yyvsp[(5) - (8)].id), (yyvsp[(7) - (8)].nd), (yyvsp[(8) - (8)].id));
                    ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2364 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (4)].nd), (yyvsp[(3) - (4)].nd), 0, 0, (yyvsp[(4) - (4)].id));
                    ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2368 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (6)].nd), (yyvsp[(3) - (6)].nd), 0, (yyvsp[(5) - (6)].nd), (yyvsp[(6) - (6)].id));
                    ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2372 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (4)].nd), 0, (yyvsp[(3) - (4)].id), 0, (yyvsp[(4) - (4)].id));
                    ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2376 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (2)].nd), 0, 1, 0, 0);
                    ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2380 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (6)].nd), 0, (yyvsp[(3) - (6)].id), (yyvsp[(5) - (6)].nd), (yyvsp[(6) - (6)].id));
                    ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2384 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (2)].nd), 0, 0, 0, (yyvsp[(2) - (2)].id));
                    ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2388 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[(1) - (4)].nd), (yyvsp[(3) - (4)].id), 0, (yyvsp[(4) - (4)].id));
                    ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2392 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[(1) - (6)].nd), (yyvsp[(3) - (6)].id), (yyvsp[(5) - (6)].nd), (yyvsp[(6) - (6)].id));
                    ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2396 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[(1) - (2)].nd), 0, 0, (yyvsp[(2) - (2)].id));
                    ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2400 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[(1) - (4)].nd), 0, (yyvsp[(3) - (4)].nd), (yyvsp[(4) - (4)].id));
                    ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2404 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, 0, (yyvsp[(1) - (2)].id), 0, (yyvsp[(2) - (2)].id));
                    ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2408 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, 0, (yyvsp[(1) - (4)].id), (yyvsp[(3) - (4)].nd), (yyvsp[(4) - (4)].id));
                    ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2412 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, 0, 0, 0, (yyvsp[(1) - (1)].id));
                    ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2419 "src\\parse.y"
    {
                      p->cmd_start = TRUE;
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2426 "src\\parse.y"
    {
                      local_add_f(p, 0);
                      (yyval.nd) = 0;
                    ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2431 "src\\parse.y"
    {
                      local_add_f(p, 0);
                      (yyval.nd) = 0;
                    ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2436 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (4)].nd);
                    ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2443 "src\\parse.y"
    {
                      (yyval.nd) = 0;
                    ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2447 "src\\parse.y"
    {
                      (yyval.nd) = 0;
                    ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2457 "src\\parse.y"
    {
                      local_add_f(p, (yyvsp[(1) - (1)].id));
                      new_bv(p, (yyvsp[(1) - (1)].id));
                    ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2465 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (4)].nd);
                    ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2469 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2475 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (3)].nd);
                    ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2479 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (3)].nd);
                    ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2485 "src\\parse.y"
    {
                      local_nest(p);
                    ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2491 "src\\parse.y"
    {
                      (yyval.nd) = new_block(p,(yyvsp[(3) - (5)].nd),(yyvsp[(4) - (5)].nd));
                      local_unnest(p);
                    ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2498 "src\\parse.y"
    {
                      if ((yyvsp[(1) - (2)].nd)->car == (node*)NODE_YIELD) {
                        yyerror(p, "block given to yield");
                      }
                      else {
                        call_with_block(p, (yyvsp[(1) - (2)].nd), (yyvsp[(2) - (2)].nd));
                      }
                      (yyval.nd) = (yyvsp[(1) - (2)].nd);
                    ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2508 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (4)].nd), (yyvsp[(3) - (4)].id), (yyvsp[(4) - (4)].nd));
                    ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2512 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (5)].nd), (yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].nd));
                      call_with_block(p, (yyval.nd), (yyvsp[(5) - (5)].nd));
                    ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2517 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (5)].nd), (yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].nd));
                      call_with_block(p, (yyval.nd), (yyvsp[(5) - (5)].nd));
                    ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2524 "src\\parse.y"
    {
                      (yyval.nd) = new_fcall(p, (yyvsp[(1) - (2)].id), (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2528 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (4)].nd), (yyvsp[(3) - (4)].id), (yyvsp[(4) - (4)].nd));
                    ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2532 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (4)].nd), (yyvsp[(3) - (4)].id), (yyvsp[(4) - (4)].nd));
                    ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2536 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].id), 0);
                    ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2540 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (3)].nd), intern("call",4), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2544 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (3)].nd), intern("call",4), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2548 "src\\parse.y"
    {
                      (yyval.nd) = new_super(p, (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2552 "src\\parse.y"
    {
                      (yyval.nd) = new_zsuper(p);
                    ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2556 "src\\parse.y"
    {
                      (yyval.nd) = new_call(p, (yyvsp[(1) - (4)].nd), intern("[]",2), (yyvsp[(3) - (4)].nd));
                    ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2562 "src\\parse.y"
    {
                      local_nest(p);
                      (yyval.num) = p->lineno;
                    ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2568 "src\\parse.y"
    {
                      (yyval.nd) = new_block(p,(yyvsp[(3) - (5)].nd),(yyvsp[(4) - (5)].nd));
                      SET_LINENO((yyval.nd), (yyvsp[(2) - (5)].num));
                      local_unnest(p);
                    ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2574 "src\\parse.y"
    {
                      local_nest(p);
                      (yyval.num) = p->lineno;
                    ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2580 "src\\parse.y"
    {
                      (yyval.nd) = new_block(p,(yyvsp[(3) - (5)].nd),(yyvsp[(4) - (5)].nd));
                      SET_LINENO((yyval.nd), (yyvsp[(2) - (5)].num));
                      local_unnest(p);
                    ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2590 "src\\parse.y"
    {
                      (yyval.nd) = cons(cons((yyvsp[(2) - (5)].nd), (yyvsp[(4) - (5)].nd)), (yyvsp[(5) - (5)].nd));
                    ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2596 "src\\parse.y"
    {
                      if ((yyvsp[(1) - (1)].nd)) {
                        (yyval.nd) = cons(cons(0, (yyvsp[(1) - (1)].nd)), 0);
                      }
                      else {
                        (yyval.nd) = 0;
                      }
                    ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2610 "src\\parse.y"
    {
                      (yyval.nd) = list1(list3((yyvsp[(2) - (6)].nd), (yyvsp[(3) - (6)].nd), (yyvsp[(5) - (6)].nd)));
                      if ((yyvsp[(6) - (6)].nd)) (yyval.nd) = append((yyval.nd), (yyvsp[(6) - (6)].nd));
                    ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2618 "src\\parse.y"
    {
                        (yyval.nd) = list1((yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2626 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (2)].nd);
                    ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2633 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (2)].nd);
                    ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2648 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (2)].nd);
                    ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2652 "src\\parse.y"
    {
                      (yyval.nd) = new_dstr(p, push((yyvsp[(2) - (3)].nd), (yyvsp[(3) - (3)].nd)));
                    ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2659 "src\\parse.y"
    {
                      (yyval.nd) = append((yyvsp[(1) - (2)].nd), (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2665 "src\\parse.y"
    {
                      (yyval.nd) = list1((yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2669 "src\\parse.y"
    {
                      (yyval.nd) = p->lex_strterm;
                      p->lex_strterm = NULL;
                    ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2675 "src\\parse.y"
    {
                      p->lex_strterm = (yyvsp[(2) - (4)].nd);
                      (yyval.nd) = list2((yyvsp[(1) - (4)].nd), (yyvsp[(3) - (4)].nd));
                    ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2680 "src\\parse.y"
    {
                      (yyval.nd) = list1(new_literal_delim(p));
                    ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2684 "src\\parse.y"
    {
                      (yyval.nd) = list1(new_literal_delim(p));
                    ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2690 "src\\parse.y"
    {
                        (yyval.nd) = (yyvsp[(2) - (2)].nd);
                    ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2694 "src\\parse.y"
    {
                      (yyval.nd) = new_dxstr(p, push((yyvsp[(2) - (3)].nd), (yyvsp[(3) - (3)].nd)));
                    ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2700 "src\\parse.y"
    {
                        (yyval.nd) = (yyvsp[(2) - (2)].nd);
                    ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2704 "src\\parse.y"
    {
                      (yyval.nd) = new_dregx(p, (yyvsp[(2) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2721 "src\\parse.y"
    {
                      parser_heredoc_info * inf = parsing_heredoc_inf(p);
                      inf->doc = push(inf->doc, new_str(p, "", 0));
                      heredoc_end(p);
                    ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2727 "src\\parse.y"
    {
                      heredoc_end(p);
                    ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2737 "src\\parse.y"
    {
                      parser_heredoc_info * inf = parsing_heredoc_inf(p);
                      inf->doc = push(inf->doc, (yyvsp[(1) - (1)].nd));
                      heredoc_treat_nextline(p);
                    ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2743 "src\\parse.y"
    {
                      (yyval.nd) = p->lex_strterm;
                      p->lex_strterm = NULL;
                    ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2749 "src\\parse.y"
    {
                      parser_heredoc_info * inf = parsing_heredoc_inf(p);
                      p->lex_strterm = (yyvsp[(2) - (4)].nd);
                      inf->doc = push(push(inf->doc, (yyvsp[(1) - (4)].nd)), (yyvsp[(3) - (4)].nd));
                    ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2757 "src\\parse.y"
    {
                      (yyval.nd) = new_words(p, list1((yyvsp[(2) - (2)].nd)));
                    ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2761 "src\\parse.y"
    {
                      (yyval.nd) = new_words(p, push((yyvsp[(2) - (3)].nd), (yyvsp[(3) - (3)].nd)));
                    ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2768 "src\\parse.y"
    {
                      (yyval.nd) = new_sym(p, (yyvsp[(1) - (1)].id));
                    ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2772 "src\\parse.y"
    {
                      p->lstate = EXPR_END;
                      (yyval.nd) = new_dsym(p, push((yyvsp[(3) - (4)].nd), (yyvsp[(4) - (4)].nd)));
                    ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2779 "src\\parse.y"
    {
                      p->lstate = EXPR_END;
                      (yyval.id) = (yyvsp[(2) - (2)].id);
                    ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2790 "src\\parse.y"
    {
                      (yyval.id) = new_strsym(p, (yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2794 "src\\parse.y"
    {
                      (yyval.id) = new_strsym(p, (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2800 "src\\parse.y"
    {
                      (yyval.nd) = new_symbols(p, list1((yyvsp[(2) - (2)].nd)));
                    ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2804 "src\\parse.y"
    {
                      (yyval.nd) = new_symbols(p, push((yyvsp[(2) - (3)].nd), (yyvsp[(3) - (3)].nd)));
                    ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2812 "src\\parse.y"
    {
                      (yyval.nd) = negate_lit(p, (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2816 "src\\parse.y"
    {
                      (yyval.nd) = negate_lit(p, (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2822 "src\\parse.y"
    {
                      (yyval.nd) = new_lvar(p, (yyvsp[(1) - (1)].id));
                    ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2826 "src\\parse.y"
    {
                      (yyval.nd) = new_ivar(p, (yyvsp[(1) - (1)].id));
                    ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2830 "src\\parse.y"
    {
                      (yyval.nd) = new_gvar(p, (yyvsp[(1) - (1)].id));
                    ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2834 "src\\parse.y"
    {
                      (yyval.nd) = new_cvar(p, (yyvsp[(1) - (1)].id));
                    ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2838 "src\\parse.y"
    {
                      (yyval.nd) = new_const(p, (yyvsp[(1) - (1)].id));
                    ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2844 "src\\parse.y"
    {
                      assignable(p, (yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2850 "src\\parse.y"
    {
                      (yyval.nd) = var_reference(p, (yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2854 "src\\parse.y"
    {
                      (yyval.nd) = new_nil(p);
                    ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2858 "src\\parse.y"
    {
                      (yyval.nd) = new_self(p);
                       ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2862 "src\\parse.y"
    {
                      (yyval.nd) = new_true(p);
                       ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2866 "src\\parse.y"
    {
                      (yyval.nd) = new_false(p);
                       ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2870 "src\\parse.y"
    {
                      if (!p->filename) {
                        p->filename = "(null)";
                      }
                      (yyval.nd) = new_str(p, p->filename, strlen(p->filename));
                    ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2877 "src\\parse.y"
    {
                      char buf[16];

                      snprintf(buf, sizeof(buf), "%d", p->lineno);
                      (yyval.nd) = new_int(p, buf, 10);
                    ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 2890 "src\\parse.y"
    {
                      (yyval.nd) = 0;
                    ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 2894 "src\\parse.y"
    {
                      p->lstate = EXPR_BEG;
                      p->cmd_start = TRUE;
                    ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 2899 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(3) - (4)].nd);
                    ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 2903 "src\\parse.y"
    {
                      yyerrok;
                      (yyval.nd) = 0;
                    ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 2910 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(2) - (3)].nd);
                      p->lstate = EXPR_BEG;
                      p->cmd_start = TRUE;
                    ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 2916 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (2)].nd);
                    ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 2922 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (6)].nd), (yyvsp[(3) - (6)].nd), (yyvsp[(5) - (6)].id), 0, (yyvsp[(6) - (6)].id));
                    ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 2926 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (8)].nd), (yyvsp[(3) - (8)].nd), (yyvsp[(5) - (8)].id), (yyvsp[(7) - (8)].nd), (yyvsp[(8) - (8)].id));
                    ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 2930 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (4)].nd), (yyvsp[(3) - (4)].nd), 0, 0, (yyvsp[(4) - (4)].id));
                    ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 2934 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (6)].nd), (yyvsp[(3) - (6)].nd), 0, (yyvsp[(5) - (6)].nd), (yyvsp[(6) - (6)].id));
                    ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 2938 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (4)].nd), 0, (yyvsp[(3) - (4)].id), 0, (yyvsp[(4) - (4)].id));
                    ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 2942 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (6)].nd), 0, (yyvsp[(3) - (6)].id), (yyvsp[(5) - (6)].nd), (yyvsp[(6) - (6)].id));
                    ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 2946 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, (yyvsp[(1) - (2)].nd), 0, 0, 0, (yyvsp[(2) - (2)].id));
                    ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 2950 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[(1) - (4)].nd), (yyvsp[(3) - (4)].id), 0, (yyvsp[(4) - (4)].id));
                    ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 2954 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[(1) - (6)].nd), (yyvsp[(3) - (6)].id), (yyvsp[(5) - (6)].nd), (yyvsp[(6) - (6)].id));
                    ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 2958 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[(1) - (2)].nd), 0, 0, (yyvsp[(2) - (2)].id));
                    ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 2962 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, (yyvsp[(1) - (4)].nd), 0, (yyvsp[(3) - (4)].nd), (yyvsp[(4) - (4)].id));
                    ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 2966 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, 0, (yyvsp[(1) - (2)].id), 0, (yyvsp[(2) - (2)].id));
                    ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 2970 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, 0, (yyvsp[(1) - (4)].id), (yyvsp[(3) - (4)].nd), (yyvsp[(4) - (4)].id));
                    ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 2974 "src\\parse.y"
    {
                      (yyval.nd) = new_args(p, 0, 0, 0, 0, (yyvsp[(1) - (1)].id));
                    ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 2978 "src\\parse.y"
    {
                      local_add_f(p, 0);
                      (yyval.nd) = new_args(p, 0, 0, 0, 0, 0);
                    ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 2985 "src\\parse.y"
    {
                      yyerror(p, "formal argument cannot be a constant");
                      (yyval.nd) = 0;
                    ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 2990 "src\\parse.y"
    {
                      yyerror(p, "formal argument cannot be an instance variable");
                      (yyval.nd) = 0;
                    ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 2995 "src\\parse.y"
    {
                      yyerror(p, "formal argument cannot be a global variable");
                      (yyval.nd) = 0;
                    ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3000 "src\\parse.y"
    {
                      yyerror(p, "formal argument cannot be a class variable");
                      (yyval.nd) = 0;
                    ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3007 "src\\parse.y"
    {
                      (yyval.id) = 0;
                    ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3011 "src\\parse.y"
    {
                      local_add_f(p, (yyvsp[(1) - (1)].id));
                      (yyval.id) = (yyvsp[(1) - (1)].id);
                    ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3018 "src\\parse.y"
    {
                      (yyval.nd) = new_arg(p, (yyvsp[(1) - (1)].id));
                    ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3022 "src\\parse.y"
    {
                      (yyval.nd) = new_masgn(p, (yyvsp[(2) - (3)].nd), 0);
                    ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3028 "src\\parse.y"
    {
                      (yyval.nd) = list1((yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3032 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3038 "src\\parse.y"
    {
                      local_add_f(p, (yyvsp[(1) - (3)].id));
                      (yyval.nd) = cons(nsym((yyvsp[(1) - (3)].id)), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3045 "src\\parse.y"
    {
                      local_add_f(p, (yyvsp[(1) - (3)].id));
                      (yyval.nd) = cons(nsym((yyvsp[(1) - (3)].id)), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3052 "src\\parse.y"
    {
                      (yyval.nd) = list1((yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3056 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3062 "src\\parse.y"
    {
                      (yyval.nd) = list1((yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3066 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3076 "src\\parse.y"
    {
                      local_add_f(p, (yyvsp[(2) - (2)].id));
                      (yyval.id) = (yyvsp[(2) - (2)].id);
                    ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3081 "src\\parse.y"
    {
                      local_add_f(p, 0);
                      (yyval.id) = -1;
                    ;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3092 "src\\parse.y"
    {
                      local_add_f(p, (yyvsp[(2) - (2)].id));
                      (yyval.id) = (yyvsp[(2) - (2)].id);
                    ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3099 "src\\parse.y"
    {
                      (yyval.id) = (yyvsp[(2) - (2)].id);
                    ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3103 "src\\parse.y"
    {
                      local_add_f(p, 0);
                      (yyval.id) = 0;
                    ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3110 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                      if (!(yyval.nd)) (yyval.nd) = new_nil(p);
                    ;}
    break;

  case 520:

/* Line 1455 of yacc.c  */
#line 3114 "src\\parse.y"
    {p->lstate = EXPR_BEG;;}
    break;

  case 521:

/* Line 1455 of yacc.c  */
#line 3115 "src\\parse.y"
    {
                      if ((yyvsp[(3) - (4)].nd) == 0) {
                        yyerror(p, "can't define singleton method for ().");
                      }
                      else {
                        switch ((enum node_type)(int)(intptr_t)(yyvsp[(3) - (4)].nd)->car) {
                        case NODE_STR:
                        case NODE_DSTR:
                        case NODE_XSTR:
                        case NODE_DXSTR:
                        case NODE_DREGX:
                        case NODE_MATCH:
                        case NODE_FLOAT:
                        case NODE_ARRAY:
                        case NODE_HEREDOC:
                          yyerror(p, "can't define singleton method for literals");
                        default:
                          break;
                        }
                      }
                      (yyval.nd) = (yyvsp[(3) - (4)].nd);
                    ;}
    break;

  case 523:

/* Line 1455 of yacc.c  */
#line 3141 "src\\parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (2)].nd);
                    ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3147 "src\\parse.y"
    {
                      (yyval.nd) = list1((yyvsp[(1) - (1)].nd));
                    ;}
    break;

  case 525:

/* Line 1455 of yacc.c  */
#line 3151 "src\\parse.y"
    {
                      (yyval.nd) = push((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 526:

/* Line 1455 of yacc.c  */
#line 3157 "src\\parse.y"
    {
                      (yyval.nd) = cons((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    ;}
    break;

  case 527:

/* Line 1455 of yacc.c  */
#line 3161 "src\\parse.y"
    {
                      (yyval.nd) = cons(new_sym(p, (yyvsp[(1) - (2)].id)), (yyvsp[(2) - (2)].nd));
                    ;}
    break;

  case 549:

/* Line 1455 of yacc.c  */
#line 3205 "src\\parse.y"
    {yyerrok;;}
    break;

  case 551:

/* Line 1455 of yacc.c  */
#line 3210 "src\\parse.y"
    {
                      p->lineno++;
                      p->column = 0;
                    ;}
    break;

  case 554:

/* Line 1455 of yacc.c  */
#line 3217 "src\\parse.y"
    {yyerrok;;}
    break;

  case 555:

/* Line 1455 of yacc.c  */
#line 3221 "src\\parse.y"
    {
                      (yyval.nd) = 0;
                    ;}
    break;



/* Line 1455 of yacc.c  */
#line 9218 "E:\\workspace\\cocos2d-x-2.2.1\\projects\\RMXP_runner\\mrubysrc\\mruby\\build\\host\\src\\y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (p, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (p, yymsg);
	  }
	else
	  {
	    yyerror (p, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, p);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, p);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (p, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, p);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, p);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 3225 "src\\parse.y"

#define yylval  (*((YYSTYPE*)(p->ylval)))

static void
yyerror(parser_state *p, const char *s)
{
  char* c;
  int n;

  if (! p->capture_errors) {
#ifdef ENABLE_STDIO
    if (p->filename) {
      fprintf(stderr, "%s:%d:%d: %s\n", p->filename, p->lineno, p->column, s);
    }
    else {
      fprintf(stderr, "line %d:%d: %s\n", p->lineno, p->column, s);
    }
#endif
  }
  else if (p->nerr < sizeof(p->error_buffer) / sizeof(p->error_buffer[0])) {
    n = strlen(s);
    c = (char *)parser_palloc(p, n + 1);
    memcpy(c, s, n + 1);
    p->error_buffer[p->nerr].message = c;
    p->error_buffer[p->nerr].lineno = p->lineno;
    p->error_buffer[p->nerr].column = p->column;
  }
  p->nerr++;
}

static void
yyerror_i(parser_state *p, const char *fmt, int i)
{
  char buf[256];

  snprintf(buf, sizeof(buf), fmt, i);
  yyerror(p, buf);
}

static void
yywarn(parser_state *p, const char *s)
{
  char* c;
  int n;

  if (! p->capture_errors) {
#ifdef ENABLE_STDIO
    if (p->filename) {
      fprintf(stderr, "%s:%d:%d: %s\n", p->filename, p->lineno, p->column, s);
    }
    else {
      fprintf(stderr, "line %d:%d: %s\n", p->lineno, p->column, s);
    }
#endif
  }
  else if (p->nwarn < sizeof(p->warn_buffer) / sizeof(p->warn_buffer[0])) {
    n = strlen(s);
    c = (char *)parser_palloc(p, n + 1);
    memcpy(c, s, n + 1);
    p->warn_buffer[p->nwarn].message = c;
    p->warn_buffer[p->nwarn].lineno = p->lineno;
    p->warn_buffer[p->nwarn].column = p->column;
  }
  p->nwarn++;
}

static void
yywarning(parser_state *p, const char *s)
{
  yywarn(p, s);
}

static void
yywarning_s(parser_state *p, const char *fmt, const char *s)
{
  char buf[256];

  snprintf(buf, sizeof(buf), fmt, s);
  yywarning(p, buf);
}

static void
backref_error(parser_state *p, node *n)
{
  int c;

  c = (int)(intptr_t)n->car;

  if (c == NODE_NTH_REF) {
    yyerror_i(p, "can't set variable $%d", (int)(intptr_t)n->cdr);
  } else if (c == NODE_BACK_REF) {
    yyerror_i(p, "can't set variable $%c", (int)(intptr_t)n->cdr);
  } else {
    mrb_bug(p->mrb, "Internal error in backref_error() : n=>car == %d", c);
  }
}

static mrb_bool peeks(parser_state *p, const char *s);
static mrb_bool skips(parser_state *p, const char *s);

static inline int
nextc(parser_state *p)
{
  int c;

  if (p->pb) {
    node *tmp;

    c = (int)(intptr_t)p->pb->car;
    tmp = p->pb;
    p->pb = p->pb->cdr;
    cons_free(tmp);
  }
  else {
#ifdef ENABLE_STDIO
    if (p->f) {
      if (feof(p->f)) goto eof;
      c = fgetc(p->f);
      if (c == EOF) goto eof;
    }
    else
#endif
      if (!p->s || p->s >= p->send) {
        goto eof;
      }
      else {
        c = (unsigned char)*p->s++;
      }
  }
  p->column++;
  return c;

  eof:
  if (!p->cxt) return -1;
  else {
    if (p->cxt->partial_hook(p) < 0)
      return -1;
    return -2;
  }
}

static void
pushback(parser_state *p, int c)
{
  if (c < 0) return;
  p->column--;
  p->pb = cons((node*)(intptr_t)c, p->pb);
}

static void
skip(parser_state *p, char term)
{
  int c;

  for (;;) {
    c = nextc(p);
    if (c < 0) break;
    if (c == term) break;
  }
}

static mrb_bool
peek_n(parser_state *p, int c, int n)
{
  node *list = 0;
  int c0;

  do {
    c0 = nextc(p);
    if (c0 < 0) return FALSE;
    list = push(list, (node*)(intptr_t)c0);
  } while(n--);
  if (p->pb) {
    p->pb = append(p->pb, (node*)list);
  }
  else {
    p->pb = list;
  }
  if (c0 == c) return TRUE;
  return FALSE;
}
#define peek(p,c) peek_n((p), (c), 0)

static mrb_bool
peeks(parser_state *p, const char *s)
{
  int len = strlen(s);

#ifdef ENABLE_STDIO
  if (p->f) {
    int n = 0;
    while (*s) {
      if (!peek_n(p, *s++, n++)) return FALSE;
    }
    return TRUE;
  }
  else
#endif
    if (p->s && p->s + len >= p->send) {
      if (memcmp(p->s, s, len) == 0) return TRUE;
    }
  return FALSE;
}

static mrb_bool
skips(parser_state *p, const char *s)
{
  int c;

  for (;;) {
    // skip until first char
    for (;;) {
      c = nextc(p);
      if (c < 0) return c;
      if (c == *s) break;
    }
    s++;
    if (peeks(p, s)) {
      int len = strlen(s);

      while (len--) {
        nextc(p);
      }
      return TRUE;
    }
    else{
      s--;
    }
  }
  return FALSE;
}


static int
newtok(parser_state *p)
{
  p->bidx = 0;
  return p->column - 1;
}

static void
tokadd(parser_state *p, int c)
{
  if (p->bidx < MRB_PARSER_BUF_SIZE) {
    p->buf[p->bidx++] = c;
  }
}

static int
toklast(parser_state *p)
{
  return p->buf[p->bidx-1];
}

static void
tokfix(parser_state *p)
{
  if (p->bidx >= MRB_PARSER_BUF_SIZE) {
    yyerror(p, "string too long (truncated)");
  }
  p->buf[p->bidx] = '\0';
}

static const char*
tok(parser_state *p)
{
  return p->buf;
}

static int
toklen(parser_state *p)
{
  return p->bidx;
}

#define IS_ARG() (p->lstate == EXPR_ARG || p->lstate == EXPR_CMDARG)
#define IS_END() (p->lstate == EXPR_END || p->lstate == EXPR_ENDARG || p->lstate == EXPR_ENDFN)
#define IS_BEG() (p->lstate == EXPR_BEG || p->lstate == EXPR_MID || p->lstate == EXPR_VALUE || p->lstate == EXPR_CLASS)
#define IS_SPCARG(c) (IS_ARG() && space_seen && !ISSPACE(c))
#define IS_LABEL_POSSIBLE() ((p->lstate == EXPR_BEG && !cmd_state) || IS_ARG())
#define IS_LABEL_SUFFIX(n) (peek_n(p, ':',(n)) && !peek_n(p, ':', (n)+1))

static int
scan_oct(const int *start, int len, int *retlen)
{
  const int *s = start;
  int retval = 0;

  /* mrb_assert(len <= 3) */
  while (len-- && *s >= '0' && *s <= '7') {
    retval <<= 3;
    retval |= *s++ - '0';
  }
  *retlen = s - start;

  return retval;
}

static int
scan_hex(const int *start, int len, int *retlen)
{
  static const char hexdigit[] = "0123456789abcdef0123456789ABCDEF";
  register const int *s = start;
  register int retval = 0;
  char *tmp;

  /* mrb_assert(len <= 2) */
  while (len-- && *s && (tmp = (char*)strchr(hexdigit, *s))) {
    retval <<= 4;
    retval |= (tmp - hexdigit) & 15;
    s++;
  }
  *retlen = s - start;

  return retval;
}

static int
read_escape(parser_state *p)
{
  int c;

  switch (c = nextc(p)) {
  case '\\':/* Backslash */
    return c;

  case 'n':/* newline */
    return '\n';

  case 't':/* horizontal tab */
    return '\t';

  case 'r':/* carriage-return */
    return '\r';

  case 'f':/* form-feed */
    return '\f';

  case 'v':/* vertical tab */
    return '\13';

  case 'a':/* alarm(bell) */
    return '\007';

  case 'e':/* escape */
    return 033;

  case '0': case '1': case '2': case '3': /* octal constant */
  case '4': case '5': case '6': case '7':
  {
    int buf[3];
    int i;

    buf[0] = c;
    for (i=1; i<3; i++) {
      buf[i] = nextc(p);
      if (buf[i] < 0) goto eof;
      if (buf[i] < '0' || '7' < buf[i]) {
        pushback(p, buf[i]);
        break;
      }
    }
    c = scan_oct(buf, i, &i);
  }
  return c;

  case 'x':     /* hex constant */
  {
    int buf[2];
    int i;

    for (i=0; i<2; i++) {
      buf[i] = nextc(p);
      if (buf[i] < 0) goto eof;
      if (!ISXDIGIT(buf[i])) {
        pushback(p, buf[i]);
        break;
      }
    }
    c = scan_hex(buf, i, &i);
    if (i == 0) {
      yyerror(p, "Invalid escape character syntax");
      return 0;
    }
  }
  return c;

  case 'b':/* backspace */
    return '\010';

  case 's':/* space */
    return ' ';

  case 'M':
    if ((c = nextc(p)) != '-') {
      yyerror(p, "Invalid escape character syntax");
      pushback(p, c);
      return '\0';
    }
    if ((c = nextc(p)) == '\\') {
      return read_escape(p) | 0x80;
    }
    else if (c < 0) goto eof;
    else {
      return ((c & 0xff) | 0x80);
    }

  case 'C':
    if ((c = nextc(p)) != '-') {
      yyerror(p, "Invalid escape character syntax");
      pushback(p, c);
      return '\0';
    }
  case 'c':
    if ((c = nextc(p))== '\\') {
      c = read_escape(p);
    }
    else if (c == '?')
      return 0177;
    else if (c < 0) goto eof;
    return c & 0x9f;

    eof:
  case -1:
  case -2:
    yyerror(p, "Invalid escape character syntax");
    return '\0';

  default:
    return c;
  }
}


static int
parse_string(parser_state *p)
{
  int c;
  string_type type = (string_type)(intptr_t)p->lex_strterm->car;
  int nest_level = (intptr_t)p->lex_strterm->cdr->car;
  int beg = (intptr_t)p->lex_strterm->cdr->cdr->car;
  int end = (intptr_t)p->lex_strterm->cdr->cdr->cdr;
  parser_heredoc_info *hinf = (type & STR_FUNC_HEREDOC) ? parsing_heredoc_inf(p) : NULL;

  newtok(p);
  while ((c = nextc(p)) != end || nest_level != 0) {
    if (hinf && (c == '\n' || c < 0)) {
      int line_head;
      tokadd(p, '\n');
      tokfix(p);
      p->lineno++;
      p->column = 0;
      line_head = hinf->line_head;
      hinf->line_head = TRUE;
      if (line_head) {
        /* check whether end of heredoc */
        const char *s = tok(p);
        int len = toklen(p);
        if (hinf->allow_indent) {
          while (ISSPACE(*s) && len > 0) {
            ++s;
            --len;
          }
        }
        if ((len-1 == hinf->term_len) && (strncmp(s, hinf->term, len-1) == 0)) {
          return tHEREDOC_END;
        }
      }
      if (c < 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "can't find heredoc delimiter \"%s\" anywhere before EOF", hinf->term);
        yyerror(p, buf);
        return 0;
      }
      yylval.nd = new_str(p, tok(p), toklen(p));
      return tHD_STRING_MID;
    }
    if (c < 0) {
      yyerror(p, "unterminated string meets end of file");
      return 0;
    }
    else if (c == beg) {
      nest_level++;
      p->lex_strterm->cdr->car = (node*)(intptr_t)nest_level;
    }
    else if (c == end) {
      nest_level--;
      p->lex_strterm->cdr->car = (node*)(intptr_t)nest_level;
    }
    else if (c == '\\') {
      c = nextc(p);
      if (type & STR_FUNC_EXPAND) {
        if (c == end || c == beg) {
          tokadd(p, c);
        }
        else if ((c == '\n') && (type & STR_FUNC_ARRAY)) {
          p->lineno++;
          p->column = 0;
          tokadd(p, '\n');
        }
        else {
          if (type & STR_FUNC_REGEXP) {
            tokadd(p, '\\');
            if (c >= 0)
              tokadd(p, c);
          } else {
            pushback(p, c);
            tokadd(p, read_escape(p));
          }
          if (hinf)
            hinf->line_head = FALSE;
        }
      } else {
        if (c != beg && c != end) {
          switch (c) {
          case '\n':
            p->lineno++;
            p->column = 0;
            break;

          case '\\':
            break;

          default:
            if (! ISSPACE(c))
              tokadd(p, '\\');
          }
        }
        tokadd(p, c);
      }
      continue;
    }
    else if ((c == '#') && (type & STR_FUNC_EXPAND)) {
      c = nextc(p);
      if (c == '{') {
        tokfix(p);
        p->lstate = EXPR_BEG;
        p->cmd_start = TRUE;
        yylval.nd = new_str(p, tok(p), toklen(p));
        if (hinf) {
          hinf->line_head = FALSE;
          return tHD_STRING_PART;
        }
        return tSTRING_PART;
      }
      tokadd(p, '#');
      pushback(p, c);
      continue;
    }
    if ((type & STR_FUNC_ARRAY) && ISSPACE(c)) {
      if (toklen(p) == 0) {
        do {
          if (c == '\n') {
            p->lineno++;
            p->column = 0;
            heredoc_treat_nextline(p);
            if (p->parsing_heredoc != NULL) {
              return tHD_LITERAL_DELIM;
            }
          }
        } while (ISSPACE(c = nextc(p)));
        pushback(p, c);
        return tLITERAL_DELIM;
      } else {
        pushback(p, c);
        tokfix(p);
        yylval.nd = new_str(p, tok(p), toklen(p));
        return tSTRING_MID;
      }
    }

    tokadd(p, c);

  }

  tokfix(p);
  p->lstate = EXPR_END;
  end_strterm(p);

  if (type & STR_FUNC_XQUOTE) {
    yylval.nd = new_xstr(p, tok(p), toklen(p));
    return tXSTRING;
  }

  if (type & STR_FUNC_REGEXP) {
    int f = 0;
    int c;
    char *s = strndup(tok(p), toklen(p));
    char flags[3];
    char *flag = flags;
    char *dup;

    newtok(p);
    while (c = nextc(p), c >= 0 && ISALPHA(c)) {
      switch (c) {
      case 'i': f |= 1; break;
      case 'x': f |= 2; break;
      case 'm': f |= 4; break;
      default: tokadd(p, c); break;
      }
    }
    pushback(p, c);
    if (toklen(p)) {
      char msg[128];
      tokfix(p);
      snprintf(msg, sizeof(msg), "unknown regexp option%s - %s",
          toklen(p) > 1 ? "s" : "", tok(p));
      yyerror(p, msg);
    }
    if (f != 0) {
      if (f & 1) *flag++ = 'i';
      if (f & 2) *flag++ = 'x';
      if (f & 4) *flag++ = 'm';
      dup = strndup(flags, (size_t)(flag - flags));
    }
    else {
      dup = NULL;
    }
    yylval.nd = new_regx(p, s, dup);

    return tREGEXP;
  }

  yylval.nd = new_str(p, tok(p), toklen(p));
  return tSTRING;
}


static int
heredoc_identifier(parser_state *p)
{
  int c;
  int type = str_heredoc;
  mrb_bool indent = FALSE;
  mrb_bool quote = FALSE;
  node *newnode;
  parser_heredoc_info *info;

  c = nextc(p);
  if (ISSPACE(c) || c == '=') {
    pushback(p, c);
    return 0;
  }
  if (c == '-') {
    indent = TRUE;
    c = nextc(p);
  }
  if (c == '\'' || c == '"') {
    int term = c;
    if (c == '\'')
      quote = TRUE;
    newtok(p);
    while ((c = nextc(p)) >= 0 && c != term) {
      if (c == '\n') {
        c = -1;
        break;
      }
      tokadd(p, c);
    }
    if (c < 0) {
      yyerror(p, "unterminated here document identifier");
      return 0;
    }
  } else {
    if (c < 0) {
      return 0;                 /* missing here document identifier */
    }
    if (! identchar(c)) {
      pushback(p, c);
      if (indent) pushback(p, '-');
      return 0;
    }
    newtok(p);
    do {
      tokadd(p, c);
    } while ((c = nextc(p)) >= 0 && identchar(c));
    pushback(p, c);
  }
  tokfix(p);
  newnode = new_heredoc(p);
  info = (parser_heredoc_info*)newnode->cdr;
  info->term = strndup(tok(p), toklen(p));
  info->term_len = toklen(p);
  if (! quote)
    type |= STR_FUNC_EXPAND;
  info->type = (string_type)type;
  info->allow_indent = indent;
  info->line_head = TRUE;
  info->doc = NULL;
  p->heredocs_from_nextline = push(p->heredocs_from_nextline, newnode);
  p->lstate = EXPR_END;

  yylval.nd = newnode;
  return tHEREDOC_BEG;
}

static int
arg_ambiguous(parser_state *p)
{
  yywarning(p, "ambiguous first argument; put parentheses or even spaces");
  return 1;
}

#include "lex.def"

static int
parser_yylex(parser_state *p)
{
  register int c;
  int space_seen = 0;
  int cmd_state;
  enum mrb_lex_state_enum last_state;
  int token_column;

  if (p->lex_strterm) {
    if (is_strterm_type(p, STR_FUNC_HEREDOC)) {
      if (p->parsing_heredoc != NULL)
        return parse_string(p);
    }
    else
      return parse_string(p);
  }
  cmd_state = p->cmd_start;
  p->cmd_start = FALSE;
  retry:
  last_state = p->lstate;
  switch (c = nextc(p)) {
  case '\0':    /* NUL */
  case '\004':  /* ^D */
  case '\032':  /* ^Z */
    return 0;
  case -1:      /* end of script. */
    if (p->heredocs_from_nextline)
      goto maybe_heredoc;
    return 0;

  /* white spaces */
  case ' ': case '\t': case '\f': case '\r':
  case '\13':   /* '\v' */
    space_seen = 1;
    goto retry;

  case '#':     /* it's a comment */
    skip(p, '\n');
    /* fall through */
  case -2:      /* end of partial script. */
  case '\n':
    maybe_heredoc:
    heredoc_treat_nextline(p);
  switch (p->lstate) {
  case EXPR_BEG:
  case EXPR_FNAME:
  case EXPR_DOT:
  case EXPR_CLASS:
  case EXPR_VALUE:
    p->lineno++;
    p->column = 0;
    if (p->parsing_heredoc != NULL) {
      return parse_string(p);
    }
    goto retry;
  default:
    break;
  }
  if (p->parsing_heredoc != NULL) {
    return '\n';
  }
  while ((c = nextc(p))) {
    switch (c) {
    case ' ': case '\t': case '\f': case '\r':
    case '\13': /* '\v' */
      space_seen = 1;
      break;
    case '.':
      if ((c = nextc(p)) != '.') {
        pushback(p, c);
        pushback(p, '.');
        goto retry;
      }
    case -1:                  /* EOF */
    case -2:                  /* end of partial script */
      goto normal_newline;
    default:
      pushback(p, c);
      goto normal_newline;
    }
  }
  normal_newline:
  p->cmd_start = TRUE;
  p->lstate = EXPR_BEG;
  return '\n';

  case '*':
    if ((c = nextc(p)) == '*') {
      if ((c = nextc(p)) == '=') {
        yylval.id = intern("**",2);
        p->lstate = EXPR_BEG;
        return tOP_ASGN;
      }
      pushback(p, c);
      c = tPOW;
    }
    else {
      if (c == '=') {
        yylval.id = intern_c('*');
        p->lstate = EXPR_BEG;
        return tOP_ASGN;
      }
      pushback(p, c);
      if (IS_SPCARG(c)) {
        yywarning(p, "`*' interpreted as argument prefix");
        c = tSTAR;
      }
      else if (IS_BEG()) {
        c = tSTAR;
      }
      else {
        c = '*';
      }
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    } else {
      p->lstate = EXPR_BEG;
    }
    return c;

  case '!':
    c = nextc(p);
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
      if (c == '@') {
        return '!';
      }
    }
    else {
      p->lstate = EXPR_BEG;
    }
    if (c == '=') {
      return tNEQ;
    }
    if (c == '~') {
      return tNMATCH;
    }
    pushback(p, c);
    return '!';

  case '=':
    if (p->column == 1) {
      if (peeks(p, "begin\n")) {
        skips(p, "\n=end\n");
        goto retry;
      }
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    } else {
      p->lstate = EXPR_BEG;
    }
    if ((c = nextc(p)) == '=') {
      if ((c = nextc(p)) == '=') {
        return tEQQ;
      }
      pushback(p, c);
      return tEQ;
    }
    if (c == '~') {
      return tMATCH;
    }
    else if (c == '>') {
      return tASSOC;
    }
    pushback(p, c);
    return '=';

  case '<':
    last_state = p->lstate;
    c = nextc(p);
    if (c == '<' &&
        p->lstate != EXPR_DOT &&
        p->lstate != EXPR_CLASS &&
        !IS_END() &&
        (!IS_ARG() || space_seen)) {
      int token = heredoc_identifier(p);
      if (token)
        return token;
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    } else {
      p->lstate = EXPR_BEG;
      if (p->lstate == EXPR_CLASS) {
        p->cmd_start = TRUE;
      }
    }
    if (c == '=') {
      if ((c = nextc(p)) == '>') {
        return tCMP;
      }
      pushback(p, c);
      return tLEQ;
    }
    if (c == '<') {
      if ((c = nextc(p)) == '=') {
        yylval.id = intern("<<",2);
        p->lstate = EXPR_BEG;
        return tOP_ASGN;
      }
      pushback(p, c);
      return tLSHFT;
    }
    pushback(p, c);
    return '<';

  case '>':
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    } else {
      p->lstate = EXPR_BEG;
    }
    if ((c = nextc(p)) == '=') {
      return tGEQ;
    }
    if (c == '>') {
      if ((c = nextc(p)) == '=') {
        yylval.id = intern(">>",2);
        p->lstate = EXPR_BEG;
        return tOP_ASGN;
      }
      pushback(p, c);
      return tRSHFT;
    }
    pushback(p, c);
    return '>';

  case '"':
    p->lex_strterm = new_strterm(p, str_dquote, '"', 0);
    return tSTRING_BEG;

  case '\'':
    p->lex_strterm = new_strterm(p, str_squote, '\'', 0);
    return parse_string(p);

  case '`':
    if (p->lstate == EXPR_FNAME) {
      p->lstate = EXPR_ENDFN;
      return '`';
    }
    if (p->lstate == EXPR_DOT) {
      if (cmd_state)
        p->lstate = EXPR_CMDARG;
      else
        p->lstate = EXPR_ARG;
      return '`';
    }
    p->lex_strterm = new_strterm(p, str_xquote, '`', 0);
    return tXSTRING_BEG;

  case '?':
    if (IS_END()) {
      p->lstate = EXPR_VALUE;
      return '?';
    }
    c = nextc(p);
    if (c < 0) {
      yyerror(p, "incomplete character syntax");
      return 0;
    }
    if (isspace(c)) {
      if (!IS_ARG()) {
        int c2;
        switch (c) {
        case ' ':
          c2 = 's';
          break;
        case '\n':
          c2 = 'n';
          break;
        case '\t':
          c2 = 't';
          break;
        case '\v':
          c2 = 'v';
          break;
        case '\r':
          c2 = 'r';
          break;
        case '\f':
          c2 = 'f';
          break;
        default:
          c2 = 0;
          break;
        }
        if (c2) {
          char buf[256];
          snprintf(buf, sizeof(buf), "invalid character syntax; use ?\\%c", c2);
          yyerror(p, buf);
        }
      }
      ternary:
      pushback(p, c);
      p->lstate = EXPR_VALUE;
      return '?';
    }
    token_column = newtok(p);
    // need support UTF-8 if configured
    if ((isalnum(c) || c == '_')) {
      int c2 = nextc(p);
      pushback(p, c2);
      if ((isalnum(c2) || c2 == '_')) {
        goto ternary;
      }
    }
    if (c == '\\') {
      c = nextc(p);
      if (c == 'u') {
#if 0
        tokadd_utf8(p);
#endif
      }
      else {
        pushback(p, c);
        c = read_escape(p);
        tokadd(p, c);
      }
    }
    else {
      tokadd(p, c);
    }
    tokfix(p);
    yylval.nd = new_str(p, tok(p), toklen(p));
    p->lstate = EXPR_END;
    return tCHAR;

  case '&':
    if ((c = nextc(p)) == '&') {
      p->lstate = EXPR_BEG;
      if ((c = nextc(p)) == '=') {
        yylval.id = intern("&&",2);
        p->lstate = EXPR_BEG;
        return tOP_ASGN;
      }
      pushback(p, c);
      return tANDOP;
    }
    else if (c == '=') {
      yylval.id = intern_c('&');
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    pushback(p, c);
    if (IS_SPCARG(c)) {
      yywarning(p, "`&' interpreted as argument prefix");
      c = tAMPER;
    }
    else if (IS_BEG()) {
      c = tAMPER;
    }
    else {
      c = '&';
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    } else {
      p->lstate = EXPR_BEG;
    }
    return c;

  case '|':
    if ((c = nextc(p)) == '|') {
      p->lstate = EXPR_BEG;
      if ((c = nextc(p)) == '=') {
        yylval.id = intern("||",2);
        p->lstate = EXPR_BEG;
        return tOP_ASGN;
      }
      pushback(p, c);
      return tOROP;
    }
    if (c == '=') {
      yylval.id = intern_c('|');
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    }
    else {
      p->lstate = EXPR_BEG;
    }
    pushback(p, c);
    return '|';

  case '+':
    c = nextc(p);
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
      if (c == '@') {
        return tUPLUS;
      }
      pushback(p, c);
      return '+';
    }
    if (c == '=') {
      yylval.id = intern_c('+');
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    if (IS_BEG() || (IS_SPCARG(c) && arg_ambiguous(p))) {
      p->lstate = EXPR_BEG;
      pushback(p, c);
      if (c >= 0 && ISDIGIT(c)) {
        c = '+';
        goto start_num;
      }
      return tUPLUS;
    }
    p->lstate = EXPR_BEG;
    pushback(p, c);
    return '+';

  case '-':
    c = nextc(p);
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
      if (c == '@') {
        return tUMINUS;
      }
      pushback(p, c);
      return '-';
    }
    if (c == '=') {
      yylval.id = intern_c('-');
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    if (c == '>') {
      p->lstate = EXPR_ENDFN;
      return tLAMBDA;
    }
    if (IS_BEG() || (IS_SPCARG(c) && arg_ambiguous(p))) {
      p->lstate = EXPR_BEG;
      pushback(p, c);
      if (c >= 0 && ISDIGIT(c)) {
        return tUMINUS_NUM;
      }
      return tUMINUS;
    }
    p->lstate = EXPR_BEG;
    pushback(p, c);
    return '-';

  case '.':
    p->lstate = EXPR_BEG;
    if ((c = nextc(p)) == '.') {
      if ((c = nextc(p)) == '.') {
        return tDOT3;
      }
      pushback(p, c);
      return tDOT2;
    }
    pushback(p, c);
    if (c >= 0 && ISDIGIT(c)) {
      yyerror(p, "no .<digit> floating literal anymore; put 0 before dot");
    }
    p->lstate = EXPR_DOT;
    return '.';

    start_num:
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
  {
    int is_float, seen_point, seen_e, nondigit;

    is_float = seen_point = seen_e = nondigit = 0;
    p->lstate = EXPR_END;
    token_column = newtok(p);
    if (c == '-' || c == '+') {
      tokadd(p, c);
      c = nextc(p);
    }
    if (c == '0') {
#define no_digits() do {yyerror(p,"numeric literal without digits"); return 0;} while (0)
      int start = toklen(p);
      c = nextc(p);
      if (c == 'x' || c == 'X') {
        /* hexadecimal */
        c = nextc(p);
        if (c >= 0 && ISXDIGIT(c)) {
          do {
            if (c == '_') {
              if (nondigit) break;
              nondigit = c;
              continue;
            }
            if (!ISXDIGIT(c)) break;
            nondigit = 0;
            tokadd(p, tolower(c));
          } while ((c = nextc(p)) >= 0);
        }
        pushback(p, c);
        tokfix(p);
        if (toklen(p) == start) {
          no_digits();
        }
        else if (nondigit) goto trailing_uc;
        yylval.nd = new_int(p, tok(p), 16);
        return tINTEGER;
      }
      if (c == 'b' || c == 'B') {
        /* binary */
        c = nextc(p);
        if (c == '0' || c == '1') {
          do {
            if (c == '_') {
              if (nondigit) break;
              nondigit = c;
              continue;
            }
            if (c != '0' && c != '1') break;
            nondigit = 0;
            tokadd(p, c);
          } while ((c = nextc(p)) >= 0);
        }
        pushback(p, c);
        tokfix(p);
        if (toklen(p) == start) {
          no_digits();
        }
        else if (nondigit) goto trailing_uc;
        yylval.nd = new_int(p, tok(p), 2);
        return tINTEGER;
      }
      if (c == 'd' || c == 'D') {
        /* decimal */
        c = nextc(p);
        if (c >= 0 && ISDIGIT(c)) {
          do {
            if (c == '_') {
              if (nondigit) break;
              nondigit = c;
              continue;
            }
            if (!ISDIGIT(c)) break;
            nondigit = 0;
            tokadd(p, c);
          } while ((c = nextc(p)) >= 0);
        }
        pushback(p, c);
        tokfix(p);
        if (toklen(p) == start) {
          no_digits();
        }
        else if (nondigit) goto trailing_uc;
        yylval.nd = new_int(p, tok(p), 10);
        return tINTEGER;
      }
      if (c == '_') {
        /* 0_0 */
        goto octal_number;
      }
      if (c == 'o' || c == 'O') {
        /* prefixed octal */
        c = nextc(p);
        if (c < 0 || c == '_' || !ISDIGIT(c)) {
          no_digits();
        }
      }
      if (c >= '0' && c <= '7') {
        /* octal */
        octal_number:
        do {
          if (c == '_') {
            if (nondigit) break;
            nondigit = c;
            continue;
          }
          if (c < '0' || c > '9') break;
          if (c > '7') goto invalid_octal;
          nondigit = 0;
          tokadd(p, c);
        } while ((c = nextc(p)) >= 0);

        if (toklen(p) > start) {
          pushback(p, c);
          tokfix(p);
          if (nondigit) goto trailing_uc;
          yylval.nd = new_int(p, tok(p), 8);
          return tINTEGER;
        }
        if (nondigit) {
          pushback(p, c);
          goto trailing_uc;
        }
      }
      if (c > '7' && c <= '9') {
        invalid_octal:
        yyerror(p, "Invalid octal digit");
      }
      else if (c == '.' || c == 'e' || c == 'E') {
        tokadd(p, '0');
      }
      else {
        pushback(p, c);
        yylval.nd = new_int(p, "0", 10);
        return tINTEGER;
      }
    }

    for (;;) {
      switch (c) {
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        nondigit = 0;
        tokadd(p, c);
        break;

      case '.':
        if (nondigit) goto trailing_uc;
        if (seen_point || seen_e) {
          goto decode_num;
        }
        else {
          int c0 = nextc(p);
          if (c0 < 0 || !ISDIGIT(c0)) {
            pushback(p, c0);
            goto decode_num;
          }
          c = c0;
        }
        tokadd(p, '.');
        tokadd(p, c);
        is_float++;
        seen_point++;
        nondigit = 0;
        break;

      case 'e':
      case 'E':
        if (nondigit) {
          pushback(p, c);
          c = nondigit;
          goto decode_num;
        }
        if (seen_e) {
          goto decode_num;
        }
        tokadd(p, c);
        seen_e++;
        is_float++;
        nondigit = c;
        c = nextc(p);
        if (c != '-' && c != '+') continue;
        tokadd(p, c);
        nondigit = c;
        break;

      case '_':       /* `_' in number just ignored */
        if (nondigit) goto decode_num;
        nondigit = c;
        break;

      default:
        goto decode_num;
      }
      c = nextc(p);
    }

    decode_num:
    pushback(p, c);
    if (nondigit) {
      trailing_uc:
      yyerror_i(p, "trailing `%c' in number", nondigit);
    }
    tokfix(p);
    if (is_float) {
      double d;
      char *endp;

      errno = 0;
      d = strtod(tok(p), &endp);
      if (d == 0 && endp == tok(p)) {
        yywarning_s(p, "corrupted float value %s", tok(p));
      }
      else if (errno == ERANGE) {
        yywarning_s(p, "float %s out of range", tok(p));
        errno = 0;
      }
      yylval.nd = new_float(p, tok(p));
      return tFLOAT;
    }
    yylval.nd = new_int(p, tok(p), 10);
    return tINTEGER;
  }

  case ')':
  case ']':
    p->paren_nest--;
  case '}':
    COND_LEXPOP();
    CMDARG_LEXPOP();
    if (c == ')')
      p->lstate = EXPR_ENDFN;
    else
      p->lstate = EXPR_ENDARG;
    return c;

  case ':':
    c = nextc(p);
    if (c == ':') {
      if (IS_BEG() || p->lstate == EXPR_CLASS || IS_SPCARG(-1)) {
        p->lstate = EXPR_BEG;
        return tCOLON3;
      }
      p->lstate = EXPR_DOT;
      return tCOLON2;
    }
    if (IS_END() || ISSPACE(c)) {
      pushback(p, c);
      p->lstate = EXPR_BEG;
      return ':';
    }
    pushback(p, c);
    p->lstate = EXPR_FNAME;
    return tSYMBEG;

  case '/':
    if (IS_BEG()) {
      p->lex_strterm = new_strterm(p, str_regexp, '/', 0);
      return tREGEXP_BEG;
    }
    if ((c = nextc(p)) == '=') {
      yylval.id = intern_c('/');
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    pushback(p, c);
    if (IS_SPCARG(c)) {
      p->lex_strterm = new_strterm(p, str_regexp, '/', 0);
      return tREGEXP_BEG;
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    } else {
      p->lstate = EXPR_BEG;
    }
    return '/';

  case '^':
    if ((c = nextc(p)) == '=') {
      yylval.id = intern_c('^');
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    } else {
      p->lstate = EXPR_BEG;
    }
    pushback(p, c);
    return '^';

  case ';':
    p->lstate = EXPR_BEG;
    return ';';

  case ',':
    p->lstate = EXPR_BEG;
    return ',';

  case '~':
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      if ((c = nextc(p)) != '@') {
        pushback(p, c);
      }
      p->lstate = EXPR_ARG;
    }
    else {
      p->lstate = EXPR_BEG;
    }
    return '~';

  case '(':
    if (IS_BEG()) {
      c = tLPAREN;
    }
    else if (IS_SPCARG(-1)) {
      c = tLPAREN_ARG;
    }
    p->paren_nest++;
    COND_PUSH(0);
    CMDARG_PUSH(0);
    p->lstate = EXPR_BEG;
    return c;

  case '[':
    p->paren_nest++;
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
      if ((c = nextc(p)) == ']') {
        if ((c = nextc(p)) == '=') {
          return tASET;
        }
        pushback(p, c);
        return tAREF;
      }
      pushback(p, c);
      return '[';
    }
    else if (IS_BEG()) {
      c = tLBRACK;
    }
    else if (IS_ARG() && space_seen) {
      c = tLBRACK;
    }
    p->lstate = EXPR_BEG;
    COND_PUSH(0);
    CMDARG_PUSH(0);
    return c;

  case '{':
    if (p->lpar_beg && p->lpar_beg == p->paren_nest) {
      p->lstate = EXPR_BEG;
      p->lpar_beg = 0;
      p->paren_nest--;
      COND_PUSH(0);
      CMDARG_PUSH(0);
      return tLAMBEG;
    }
    if (IS_ARG() || p->lstate == EXPR_END || p->lstate == EXPR_ENDFN)
      c = '{';          /* block (primary) */
    else if (p->lstate == EXPR_ENDARG)
      c = tLBRACE_ARG;  /* block (expr) */
    else
      c = tLBRACE;      /* hash */
    COND_PUSH(0);
    CMDARG_PUSH(0);
    p->lstate = EXPR_BEG;
    return c;

  case '\\':
    c = nextc(p);
    if (c == '\n') {
      p->lineno++;
      p->column = 0;
      space_seen = 1;
      goto retry; /* skip \\n */
    }
    pushback(p, c);
    return '\\';

  case '%':
    if (IS_BEG()) {
      int term;
      int paren;

      c = nextc(p);
      quotation:
      if (c < 0 || !ISALNUM(c)) {
        term = c;
        c = 'Q';
      }
      else {
        term = nextc(p);
        if (isalnum(term)) {
          yyerror(p, "unknown type of %string");
          return 0;
        }
      }
      if (c < 0 || term < 0) {
        yyerror(p, "unterminated quoted string meets end of file");
        return 0;
      }
      paren = term;
      if (term == '(') term = ')';
      else if (term == '[') term = ']';
      else if (term == '{') term = '}';
      else if (term == '<') term = '>';
      else paren = 0;

      switch (c) {
      case 'Q':
        p->lex_strterm = new_strterm(p, str_dquote, term, paren);
        return tSTRING_BEG;

      case 'q':
        p->lex_strterm = new_strterm(p, str_squote, term, paren);
        return parse_string(p);

      case 'W':
        p->lex_strterm = new_strterm(p, str_dword, term, paren);
        return tWORDS_BEG;

      case 'w':
        p->lex_strterm = new_strterm(p, str_sword, term, paren);
        return tWORDS_BEG;

      case 'x':
        p->lex_strterm = new_strterm(p, str_xquote, term, paren);
        return tXSTRING_BEG;

      case 'r':
        p->lex_strterm = new_strterm(p, str_regexp, term, paren);
        return tREGEXP_BEG;

      case 's':
        p->lex_strterm = new_strterm(p, str_ssym, term, paren);
        return tSYMBEG;

      case 'I':
        p->lex_strterm = new_strterm(p, str_dsymbols, term, paren);
        return tSYMBOLS_BEG;

      case 'i':
        p->lex_strterm = new_strterm(p, str_ssymbols, term, paren);
        return tSYMBOLS_BEG;

      default:
        yyerror(p, "unknown type of %string");
        return 0;
      }
    }
    if ((c = nextc(p)) == '=') {
      yylval.id = intern_c('%');
      p->lstate = EXPR_BEG;
      return tOP_ASGN;
    }
    if (IS_SPCARG(c)) {
      goto quotation;
    }
    if (p->lstate == EXPR_FNAME || p->lstate == EXPR_DOT) {
      p->lstate = EXPR_ARG;
    } else {
      p->lstate = EXPR_BEG;
    }
    pushback(p, c);
    return '%';

  case '$':
    p->lstate = EXPR_END;
    token_column = newtok(p);
    c = nextc(p);
    if (c < 0) {
      yyerror(p, "incomplete global variable syntax");
      return 0;
    }
    switch (c) {
    case '_':     /* $_: last read line string */
      c = nextc(p);
      if (c >= 0 && identchar(c)) { /* if there is more after _ it is a variable */
        tokadd(p, '$');
        tokadd(p, c);
        break;
      }
      pushback(p, c);
      c = '_';
      /* fall through */
    case '~':     /* $~: match-data */
    case '*':     /* $*: argv */
    case '$':     /* $$: pid */
    case '?':     /* $?: last status */
    case '!':     /* $!: error string */
    case '@':     /* $@: error position */
    case '/':     /* $/: input record separator */
    case '\\':    /* $\: output record separator */
    case ';':     /* $;: field separator */
    case ',':     /* $,: output field separator */
    case '.':     /* $.: last read line number */
    case '=':     /* $=: ignorecase */
    case ':':     /* $:: load path */
    case '<':     /* $<: reading filename */
    case '>':     /* $>: default output handle */
    case '\"':    /* $": already loaded files */
      tokadd(p, '$');
      tokadd(p, c);
      tokfix(p);
      yylval.id = intern_cstr(tok(p));
      return tGVAR;

    case '-':
      tokadd(p, '$');
      tokadd(p, c);
      c = nextc(p);
      pushback(p, c);
      gvar:
      tokfix(p);
      yylval.id = intern_cstr(tok(p));
      return tGVAR;

    case '&':     /* $&: last match */
    case '`':     /* $`: string before last match */
    case '\'':    /* $': string after last match */
    case '+':     /* $+: string matches last pattern */
      if (last_state == EXPR_FNAME) {
        tokadd(p, '$');
        tokadd(p, c);
        goto gvar;
      }
      yylval.nd = new_back_ref(p, c);
      return tBACK_REF;

    case '1': case '2': case '3':
    case '4': case '5': case '6':
    case '7': case '8': case '9':
      do {
        tokadd(p, c);
        c = nextc(p);
      } while (c >= 0 && isdigit(c));
      pushback(p, c);
      if (last_state == EXPR_FNAME) goto gvar;
      tokfix(p);
      yylval.nd = new_nth_ref(p, atoi(tok(p)));
      return tNTH_REF;

    default:
      if (!identchar(c)) {
        pushback(p,  c);
        return '$';
      }
    case '0':
      tokadd(p, '$');
    }
    break;

    case '@':
      c = nextc(p);
      token_column = newtok(p);
      tokadd(p, '@');
      if (c == '@') {
        tokadd(p, '@');
        c = nextc(p);
      }
      if (c < 0) {
        if (p->bidx == 1) {
          yyerror(p, "incomplete instance variable syntax");
        }
        else {
          yyerror(p, "incomplete class variable syntax");
        }
        return 0;
      }
      else if (isdigit(c)) {
        if (p->bidx == 1) {
          yyerror_i(p, "`@%c' is not allowed as an instance variable name", c);
        }
        else {
          yyerror_i(p, "`@@%c' is not allowed as a class variable name", c);
        }
        return 0;
      }
      if (!identchar(c)) {
        pushback(p, c);
        return '@';
      }
      break;

    case '_':
      token_column = newtok(p);
      break;

    default:
      if (!identchar(c)) {
        yyerror_i(p,  "Invalid char `\\x%02X' in expression", c);
        goto retry;
      }

      token_column = newtok(p);
      break;
  }

  do {
    tokadd(p, c);
    c = nextc(p);
    if (c < 0) break;
  } while (identchar(c));
  if (token_column == 0 && toklen(p) == 7 && (c < 0 || c == '\n') &&
      strncmp(tok(p), "__END__", toklen(p)) == 0)
    return -1;

  switch (tok(p)[0]) {
  case '@': case '$':
    pushback(p, c);
    break;
  default:
    if ((c == '!' || c == '?') && !peek(p, '=')) {
      tokadd(p, c);
    }
    else {
      pushback(p, c);
    }
  }
  tokfix(p);
  {
    int result = 0;

    last_state = p->lstate;
    switch (tok(p)[0]) {
    case '$':
      p->lstate = EXPR_END;
      result = tGVAR;
      break;
    case '@':
      p->lstate = EXPR_END;
      if (tok(p)[1] == '@')
        result = tCVAR;
      else
        result = tIVAR;
      break;

    default:
      if (toklast(p) == '!' || toklast(p) == '?') {
        result = tFID;
      }
      else {
        if (p->lstate == EXPR_FNAME) {
          if ((c = nextc(p)) == '=' && !peek(p, '~') && !peek(p, '>') &&
              (!peek(p, '=') || (peek_n(p, '>', 1)))) {
            result = tIDENTIFIER;
            tokadd(p, c);
            tokfix(p);
          }
          else {
            pushback(p, c);
          }
        }
        if (result == 0 && isupper((int)(unsigned char)tok(p)[0])) {
          result = tCONSTANT;
        }
        else {
          result = tIDENTIFIER;
        }
      }

      if (IS_LABEL_POSSIBLE()) {
        if (IS_LABEL_SUFFIX(0)) {
          p->lstate = EXPR_BEG;
          nextc(p);
          tokfix(p);
          yylval.id = intern_cstr(tok(p));
          return tLABEL;
        }
      }
      if (p->lstate != EXPR_DOT) {
        const struct kwtable *kw;

        /* See if it is a reserved word.  */
        kw = mrb_reserved_word(tok(p), toklen(p));
        if (kw) {
          enum mrb_lex_state_enum state = p->lstate;
          p->lstate = kw->state;
          if (state == EXPR_FNAME) {
            yylval.id = intern_cstr(kw->name);
            return kw->id[0];
          }
          if (p->lstate == EXPR_BEG) {
            p->cmd_start = TRUE;
          }
          if (kw->id[0] == keyword_do) {
            if (p->lpar_beg && p->lpar_beg == p->paren_nest) {
              p->lpar_beg = 0;
              p->paren_nest--;
              return keyword_do_LAMBDA;
            }
            if (COND_P()) return keyword_do_cond;
            if (CMDARG_P() && state != EXPR_CMDARG)
              return keyword_do_block;
            if (state == EXPR_ENDARG || state == EXPR_BEG)
              return keyword_do_block;
            return keyword_do;
          }
          if (state == EXPR_BEG || state == EXPR_VALUE)
            return kw->id[0];
          else {
            if (kw->id[0] != kw->id[1])
              p->lstate = EXPR_BEG;
            return kw->id[1];
          }
        }
      }

      if (IS_BEG() || p->lstate == EXPR_DOT || IS_ARG()) {
        if (cmd_state) {
          p->lstate = EXPR_CMDARG;
        }
        else {
          p->lstate = EXPR_ARG;
        }
      }
      else if (p->lstate == EXPR_FNAME) {
        p->lstate = EXPR_ENDFN;
      }
      else {
        p->lstate = EXPR_END;
      }
    }
    {
      mrb_sym ident = intern_cstr(tok(p));

      yylval.id = ident;
#if 0
      if (last_state != EXPR_DOT && islower(tok(p)[0]) && lvar_defined(ident)) {
        p->lstate = EXPR_END;
      }
#endif
    }
    return result;
  }
}

static int
yylex(void *lval, parser_state *p)
{
  int t;

  p->ylval = lval;
  t = parser_yylex(p);

  return t;
}

static void
parser_init_cxt(parser_state *p, mrbc_context *cxt)
{
  if (!cxt) return;
  if (cxt->lineno) p->lineno = cxt->lineno;
  if (cxt->filename) mrb_parser_set_filename(p, cxt->filename);
  if (cxt->syms) {
    int i;

    p->locals = cons(0,0);
    for (i=0; i<cxt->slen; i++) {
      local_add_f(p, cxt->syms[i]);
    }
  }
  p->capture_errors = cxt->capture_errors;
  if (cxt->partial_hook) {
    p->cxt = cxt;
  }
}

static void
parser_update_cxt(parser_state *p, mrbc_context *cxt)
{
  node *n, *n0;
  int i = 0;

  if (!cxt) return;
  if ((int)(intptr_t)p->tree->car != NODE_SCOPE) return;
  n0 = n = p->tree->cdr->car;
  while (n) {
    i++;
    n = n->cdr;
  }
  cxt->syms = (mrb_sym *)mrb_realloc(p->mrb, cxt->syms, i*sizeof(mrb_sym));
  cxt->slen = i;
  for (i=0, n=n0; n; i++,n=n->cdr) {
    cxt->syms[i] = sym(n->car);
  }
}

void mrb_codedump_all(mrb_state*, struct RProc*);
void mrb_parser_dump(mrb_state *mrb, node *tree, int offset);

void
mrb_parser_parse(parser_state *p, mrbc_context *c)
{
  if (setjmp(p->jmp) != 0) {
    yyerror(p, "memory allocation error");
    p->nerr++;
    p->tree = 0;
    return;
  }

  p->cmd_start = TRUE;
  p->in_def = p->in_single = 0;
  p->nerr = p->nwarn = 0;
  p->lex_strterm = NULL;

  parser_init_cxt(p, c);
  yyparse(p);
  if (!p->tree) {
    p->tree = new_nil(p);
  }
  parser_update_cxt(p, c);
  if (c && c->dump_result) {
    mrb_parser_dump(p->mrb, p->tree, 0);
  }
}

parser_state*
mrb_parser_new(mrb_state *mrb)
{
  mrb_pool *pool;
  parser_state *p;
  static const parser_state parser_state_zero = { 0 };

  pool = mrb_pool_open(mrb);
  if (!pool) return 0;
  p = (parser_state *)mrb_pool_alloc(pool, sizeof(parser_state));
  if (!p) return 0;

  *p = parser_state_zero;
  p->mrb = mrb;
  p->pool = pool;

  p->s = p->send = NULL;
#ifdef ENABLE_STDIO
  p->f = NULL;
#endif

  p->cmd_start = TRUE;
  p->in_def = p->in_single = 0;

  p->capture_errors = FALSE;
  p->lineno = 1;
  p->column = 0;
#if defined(PARSER_TEST) || defined(PARSER_DEBUG)
  yydebug = 1;
#endif

  p->lex_strterm = NULL;
  p->all_heredocs = p->parsing_heredoc = NULL;
  p->lex_strterm_before_heredoc = NULL;

  p->current_filename_index = -1;
  p->filename_table = NULL;
  p->filename_table_length = 0;

  return p;
}

void
mrb_parser_free(parser_state *p) {
  mrb_pool_close(p->pool);
}

mrbc_context*
mrbc_context_new(mrb_state *mrb)
{
  mrbc_context *c;

  c = (mrbc_context *)mrb_calloc(mrb, 1, sizeof(mrbc_context));
  return c;
}

void
mrbc_context_free(mrb_state *mrb, mrbc_context *cxt)
{
  mrb_free(mrb, cxt->syms);
  mrb_free(mrb, cxt);
}

const char*
mrbc_filename(mrb_state *mrb, mrbc_context *c, const char *s)
{
  if (s) {
    int len = strlen(s);
    char *p = (char *)mrb_alloca(mrb, len + 1);

    memcpy(p, s, len + 1);
    c->filename = p;
  }
  return c->filename;
}

void
mrbc_partial_hook(mrb_state *mrb, mrbc_context *c, int (*func)(struct mrb_parser_state*), void *data)
{
  c->partial_hook = func;
  c->partial_data = data;
}

void
mrb_parser_set_filename(struct mrb_parser_state *p, const char *f)
{
  mrb_sym sym;
  size_t len;
  size_t i;
  mrb_sym* new_table;

  sym = mrb_intern_cstr(p->mrb, f);
  p->filename = mrb_sym2name_len(p->mrb, sym, &len);
  p->lineno = (p->filename_table_length > 0)? 0 : 1;

  for(i = 0; i < p->filename_table_length; ++i) {
    if(p->filename_table[i] == sym) {
      p->current_filename_index = i;
      return;
    }
  }

  p->current_filename_index = p->filename_table_length++;

  new_table = parser_palloc(p, sizeof(mrb_sym) * p->filename_table_length);
  if (p->filename_table) {
    memcpy(new_table, p->filename_table, sizeof(mrb_sym) * p->filename_table_length);
  }
  p->filename_table = new_table;
  p->filename_table[p->filename_table_length - 1] = sym;
}

char const* mrb_parser_get_filename(struct mrb_parser_state* p, uint16_t idx) {
  if (idx >= p->filename_table_length) { return NULL; }
  else {
    size_t len;
    return mrb_sym2name_len(p->mrb, p->filename_table[idx], &len);
  }
}

#ifdef ENABLE_STDIO
parser_state*
mrb_parse_file(mrb_state *mrb, FILE *f, mrbc_context *c)
{
  parser_state *p;

  p = mrb_parser_new(mrb);
  if (!p) return 0;
  p->s = p->send = NULL;
  p->f = f;

  mrb_parser_parse(p, c);
  return p;
}
#endif

parser_state*
mrb_parse_nstring(mrb_state *mrb, const char *s, int len, mrbc_context *c)
{
  parser_state *p;

  p = mrb_parser_new(mrb);
  if (!p) return 0;
  p->s = s;
  p->send = s + len;

  mrb_parser_parse(p, c);
  return p;
}

parser_state*
mrb_parse_string(mrb_state *mrb, const char *s, mrbc_context *c)
{
  return mrb_parse_nstring(mrb, s, strlen(s), c);
}

static mrb_value
load_exec(mrb_state *mrb, parser_state *p, mrbc_context *c)
{
  struct RClass *target = mrb->object_class;
  struct RProc *proc;
  mrb_value v;

  if (!p) {
    return mrb_undef_value();
  }
  if (!p->tree || p->nerr) {
    if (p->capture_errors) {
      char buf[256];
      int n;

      n = snprintf(buf, sizeof(buf), "line %d: %s\n",
          p->error_buffer[0].lineno, p->error_buffer[0].message);
      mrb->exc = mrb_obj_ptr(mrb_exc_new(mrb, E_SYNTAX_ERROR, buf, n));
      mrb_parser_free(p);
      return mrb_undef_value();
    }
    else {
      static const char msg[] = "syntax error";
      mrb->exc = mrb_obj_ptr(mrb_exc_new(mrb, E_SYNTAX_ERROR, msg, sizeof(msg) - 1));
      mrb_parser_free(p);
      return mrb_undef_value();
    }
  }
  proc = mrb_generate_code(mrb, p);
  mrb_parser_free(p);
  if (proc == NULL) {
    static const char msg[] = "codegen error";
    mrb->exc = mrb_obj_ptr(mrb_exc_new(mrb, E_SCRIPT_ERROR, msg, sizeof(msg) - 1));
    return mrb_undef_value();
  }
  if (c) {
    if (c->dump_result) mrb_codedump_all(mrb, proc);
    if (c->no_exec) return mrb_obj_value(proc);
    if (c->target_class) {
      target = c->target_class;
    }
  }
  proc->target_class = target;
  if (mrb->c->ci) {
    mrb->c->ci->target_class = target;
  }
  v = mrb_context_run(mrb, proc, mrb_top_self(mrb), 0);
  if (mrb->exc) return mrb_nil_value();
  return v;
}

#ifdef ENABLE_STDIO
mrb_value
mrb_load_file_cxt(mrb_state *mrb, FILE *f, mrbc_context *c)
{
  return load_exec(mrb, mrb_parse_file(mrb, f, c), c);
}

mrb_value
mrb_load_file(mrb_state *mrb, FILE *f)
{
  return mrb_load_file_cxt(mrb, f, NULL);
}
#endif

mrb_value
mrb_load_nstring_cxt(mrb_state *mrb, const char *s, int len, mrbc_context *c)
{
  return load_exec(mrb, mrb_parse_nstring(mrb, s, len, c), c);
}

mrb_value
mrb_load_nstring(mrb_state *mrb, const char *s, int len)
{
  return mrb_load_nstring_cxt(mrb, s, len, NULL);
}

mrb_value
mrb_load_string_cxt(mrb_state *mrb, const char *s, mrbc_context *c)
{
  return mrb_load_nstring_cxt(mrb, s, strlen(s), c);
}

mrb_value
mrb_load_string(mrb_state *mrb, const char *s)
{
  return mrb_load_string_cxt(mrb, s, NULL);
}

#ifdef ENABLE_STDIO

static void
dump_prefix(int offset)
{
  while (offset--) {
    putc(' ', stdout);
    putc(' ', stdout);
  }
}

static void
dump_recur(mrb_state *mrb, node *tree, int offset)
{
  while (tree) {
    mrb_parser_dump(mrb, tree->car, offset);
    tree = tree->cdr;
  }
}

#endif

void
mrb_parser_dump(mrb_state *mrb, node *tree, int offset)
{
#ifdef ENABLE_STDIO
  int n;

  if (!tree) return;
  again:
  dump_prefix(offset);
  n = (int)(intptr_t)tree->car;
  tree = tree->cdr;
  switch (n) {
  case NODE_BEGIN:
    printf("NODE_BEGIN:\n");
    dump_recur(mrb, tree, offset+1);
    break;

  case NODE_RESCUE:
    printf("NODE_RESCUE:\n");
    if (tree->car) {
      dump_prefix(offset+1);
      printf("body:\n");
      mrb_parser_dump(mrb, tree->car, offset+2);
    }
    tree = tree->cdr;
    if (tree->car) {
      node *n2 = tree->car;

      dump_prefix(offset+1);
      printf("rescue:\n");
      while (n2) {
        node *n3 = n2->car;
        if (n3->car) {
          dump_prefix(offset+2);
          printf("handle classes:\n");
          dump_recur(mrb, n3->car, offset+3);
        }
        if (n3->cdr->car) {
          dump_prefix(offset+2);
          printf("exc_var:\n");
          mrb_parser_dump(mrb, n3->cdr->car, offset+3);
        }
        if (n3->cdr->cdr->car) {
          dump_prefix(offset+2);
          printf("rescue body:\n");
          mrb_parser_dump(mrb, n3->cdr->cdr->car, offset+3);
        }
        n2 = n2->cdr;
      }
    }
    tree = tree->cdr;
    if (tree->car) {
      dump_prefix(offset+1);
      printf("else:\n");
      mrb_parser_dump(mrb, tree->car, offset+2);
    }
    break;

  case NODE_ENSURE:
    printf("NODE_ENSURE:\n");
    dump_prefix(offset+1);
    printf("body:\n");
    mrb_parser_dump(mrb, tree->car, offset+2);
    dump_prefix(offset+1);
    printf("ensure:\n");
    mrb_parser_dump(mrb, tree->cdr->cdr, offset+2);
    break;

  case NODE_LAMBDA:
    printf("NODE_BLOCK:\n");
    goto block;

  case NODE_BLOCK:
    block:
    printf("NODE_BLOCK:\n");
  tree = tree->cdr;
  if (tree->car) {
    node *n = tree->car;

    if (n->car) {
      dump_prefix(offset+1);
      printf("mandatory args:\n");
      dump_recur(mrb, n->car, offset+2);
    }
    n = n->cdr;
    if (n->car) {
      dump_prefix(offset+1);
      printf("optional args:\n");
      {
        node *n2 = n->car;

        while (n2) {
          dump_prefix(offset+2);
          printf("%s=", mrb_sym2name(mrb, sym(n2->car->car)));
          mrb_parser_dump(mrb, n2->car->cdr, 0);
          n2 = n2->cdr;
        }
      }
    }
    n = n->cdr;
    if (n->car) {
      dump_prefix(offset+1);
      printf("rest=*%s\n", mrb_sym2name(mrb, sym(n->car)));
    }
    n = n->cdr;
    if (n->car) {
      dump_prefix(offset+1);
      printf("post mandatory args:\n");
      dump_recur(mrb, n->car, offset+2);
    }
    n = n->cdr;
    if (n) {
      dump_prefix(offset+1);
      printf("blk=&%s\n", mrb_sym2name(mrb, sym(n)));
    }
  }
  dump_prefix(offset+1);
  printf("body:\n");
  mrb_parser_dump(mrb, tree->cdr->car, offset+2);
  break;

  case NODE_IF:
    printf("NODE_IF:\n");
    dump_prefix(offset+1);
    printf("cond:\n");
    mrb_parser_dump(mrb, tree->car, offset+2);
    dump_prefix(offset+1);
    printf("then:\n");
    mrb_parser_dump(mrb, tree->cdr->car, offset+2);
    if (tree->cdr->cdr->car) {
      dump_prefix(offset+1);
      printf("else:\n");
      mrb_parser_dump(mrb, tree->cdr->cdr->car, offset+2);
    }
    break;

  case NODE_AND:
    printf("NODE_AND:\n");
    mrb_parser_dump(mrb, tree->car, offset+1);
    mrb_parser_dump(mrb, tree->cdr, offset+1);
    break;

  case NODE_OR:
    printf("NODE_OR:\n");
    mrb_parser_dump(mrb, tree->car, offset+1);
    mrb_parser_dump(mrb, tree->cdr, offset+1);
    break;

  case NODE_CASE:
    printf("NODE_CASE:\n");
    if (tree->car) {
      mrb_parser_dump(mrb, tree->car, offset+1);
    }
    tree = tree->cdr;
    while (tree) {
      dump_prefix(offset+1);
      printf("case:\n");
      dump_recur(mrb, tree->car->car, offset+2);
      dump_prefix(offset+1);
      printf("body:\n");
      mrb_parser_dump(mrb, tree->car->cdr, offset+2);
      tree = tree->cdr;
    }
    break;

  case NODE_WHILE:
    printf("NODE_WHILE:\n");
    dump_prefix(offset+1);
    printf("cond:\n");
    mrb_parser_dump(mrb, tree->car, offset+2);
    dump_prefix(offset+1);
    printf("body:\n");
    mrb_parser_dump(mrb, tree->cdr, offset+2);
    break;

  case NODE_UNTIL:
    printf("NODE_UNTIL:\n");
    dump_prefix(offset+1);
    printf("cond:\n");
    mrb_parser_dump(mrb, tree->car, offset+2);
    dump_prefix(offset+1);
    printf("body:\n");
    mrb_parser_dump(mrb, tree->cdr, offset+2);
    break;

  case NODE_FOR:
    printf("NODE_FOR:\n");
    dump_prefix(offset+1);
    printf("var:\n");
    {
      node *n2 = tree->car;

      if (n2->car) {
        dump_prefix(offset+2);
        printf("pre:\n");
        dump_recur(mrb, n2->car, offset+3);
      }
      n2 = n2->cdr;
      if (n2) {
        if (n2->car) {
          dump_prefix(offset+2);
          printf("rest:\n");
          mrb_parser_dump(mrb, n2->car, offset+3);
        }
        n2 = n2->cdr;
        if (n2) {
          if (n2->car) {
            dump_prefix(offset+2);
            printf("post:\n");
            dump_recur(mrb, n2->car, offset+3);
          }
        }
      }
    }
    tree = tree->cdr;
    dump_prefix(offset+1);
    printf("in:\n");
    mrb_parser_dump(mrb, tree->car, offset+2);
    tree = tree->cdr;
    dump_prefix(offset+1);
    printf("do:\n");
    mrb_parser_dump(mrb, tree->car, offset+2);
    break;

  case NODE_SCOPE:
    printf("NODE_SCOPE:\n");
    {
      node *n2 = tree->car;

      if (n2  && (n2->car || n2->cdr)) {
        dump_prefix(offset+1);
        printf("local variables:\n");
        dump_prefix(offset+2);
        while (n2) {
          if (n2->car) {
            if (n2 != tree->car) printf(", ");
            printf("%s", mrb_sym2name(mrb, sym(n2->car)));
          }
          n2 = n2->cdr;
        }
        printf("\n");
      }
    }
    tree = tree->cdr;
    offset++;
    goto again;

  case NODE_FCALL:
  case NODE_CALL:
    printf("NODE_CALL:\n");
    mrb_parser_dump(mrb, tree->car, offset+1);
    dump_prefix(offset+1);
    printf("method='%s' (%d)\n",
        mrb_sym2name(mrb, sym(tree->cdr->car)),
        (int)(intptr_t)tree->cdr->car);
    tree = tree->cdr->cdr->car;
    if (tree) {
      dump_prefix(offset+1);
      printf("args:\n");
      dump_recur(mrb, tree->car, offset+2);
      if (tree->cdr) {
        dump_prefix(offset+1);
        printf("block:\n");
        mrb_parser_dump(mrb, tree->cdr, offset+2);
      }
    }
    break;

  case NODE_DOT2:
    printf("NODE_DOT2:\n");
    mrb_parser_dump(mrb, tree->car, offset+1);
    mrb_parser_dump(mrb, tree->cdr, offset+1);
    break;

  case NODE_DOT3:
    printf("NODE_DOT3:\n");
    mrb_parser_dump(mrb, tree->car, offset+1);
    mrb_parser_dump(mrb, tree->cdr, offset+1);
    break;

  case NODE_COLON2:
    printf("NODE_COLON2:\n");
    mrb_parser_dump(mrb, tree->car, offset+1);
    dump_prefix(offset+1);
    printf("::%s\n", mrb_sym2name(mrb, sym(tree->cdr)));
    break;

  case NODE_COLON3:
    printf("NODE_COLON3:\n");
    dump_prefix(offset+1);
    printf("::%s\n", mrb_sym2name(mrb, sym(tree)));
    break;

  case NODE_ARRAY:
    printf("NODE_ARRAY:\n");
    dump_recur(mrb, tree, offset+1);
    break;

  case NODE_HASH:
    printf("NODE_HASH:\n");
    while (tree) {
      dump_prefix(offset+1);
      printf("key:\n");
      mrb_parser_dump(mrb, tree->car->car, offset+2);
      dump_prefix(offset+1);
      printf("value:\n");
      mrb_parser_dump(mrb, tree->car->cdr, offset+2);
      tree = tree->cdr;
    }
    break;

  case NODE_SPLAT:
    printf("NODE_SPLAT:\n");
    mrb_parser_dump(mrb, tree, offset+1);
    break;

  case NODE_ASGN:
    printf("NODE_ASGN:\n");
    dump_prefix(offset+1);
    printf("lhs:\n");
    mrb_parser_dump(mrb, tree->car, offset+2);
    dump_prefix(offset+1);
    printf("rhs:\n");
    mrb_parser_dump(mrb, tree->cdr, offset+2);
    break;

  case NODE_MASGN:
    printf("NODE_MASGN:\n");
    dump_prefix(offset+1);
    printf("mlhs:\n");
    {
      node *n2 = tree->car;

      if (n2->car) {
        dump_prefix(offset+2);
        printf("pre:\n");
        dump_recur(mrb, n2->car, offset+3);
      }
      n2 = n2->cdr;
      if (n2) {
        if (n2->car) {
          dump_prefix(offset+2);
          printf("rest:\n");
          if (n2->car == (node*)-1) {
            dump_prefix(offset+2);
            printf("(empty)\n");
          }
          else {
            mrb_parser_dump(mrb, n2->car, offset+3);
          }
        }
        n2 = n2->cdr;
        if (n2) {
          if (n2->car) {
            dump_prefix(offset+2);
            printf("post:\n");
            dump_recur(mrb, n2->car, offset+3);
          }
        }
      }
    }
    dump_prefix(offset+1);
    printf("rhs:\n");
    mrb_parser_dump(mrb, tree->cdr, offset+2);
    break;

  case NODE_OP_ASGN:
    printf("NODE_OP_ASGN:\n");
    dump_prefix(offset+1);
    printf("lhs:\n");
    mrb_parser_dump(mrb, tree->car, offset+2);
    tree = tree->cdr;
    dump_prefix(offset+1);
    printf("op='%s' (%d)\n", mrb_sym2name(mrb, sym(tree->car)), (int)(intptr_t)tree->car);
    tree = tree->cdr;
    mrb_parser_dump(mrb, tree->car, offset+1);
    break;

  case NODE_SUPER:
    printf("NODE_SUPER:\n");
    if (tree) {
      dump_prefix(offset+1);
      printf("args:\n");
      dump_recur(mrb, tree->car, offset+2);
      if (tree->cdr) {
        dump_prefix(offset+1);
        printf("block:\n");
        mrb_parser_dump(mrb, tree->cdr, offset+2);
      }
    }
    break;

  case NODE_ZSUPER:
    printf("NODE_ZSUPER\n");
    break;

  case NODE_RETURN:
    printf("NODE_RETURN:\n");
    mrb_parser_dump(mrb, tree, offset+1);
    break;

  case NODE_YIELD:
    printf("NODE_YIELD:\n");
    dump_recur(mrb, tree, offset+1);
    break;

  case NODE_BREAK:
    printf("NODE_BREAK:\n");
    mrb_parser_dump(mrb, tree, offset+1);
    break;

  case NODE_NEXT:
    printf("NODE_NEXT:\n");
    mrb_parser_dump(mrb, tree, offset+1);
    break;

  case NODE_REDO:
    printf("NODE_REDO\n");
    break;

  case NODE_RETRY:
    printf("NODE_RETRY\n");
    break;

  case NODE_LVAR:
    printf("NODE_LVAR %s\n", mrb_sym2name(mrb, sym(tree)));
    break;

  case NODE_GVAR:
    printf("NODE_GVAR %s\n", mrb_sym2name(mrb, sym(tree)));
    break;

  case NODE_IVAR:
    printf("NODE_IVAR %s\n", mrb_sym2name(mrb, sym(tree)));
    break;

  case NODE_CVAR:
    printf("NODE_CVAR %s\n", mrb_sym2name(mrb, sym(tree)));
    break;

  case NODE_CONST:
    printf("NODE_CONST %s\n", mrb_sym2name(mrb, sym(tree)));
    break;

  case NODE_MATCH:
    printf("NODE_MATCH:\n");
    dump_prefix(offset + 1);
    printf("lhs:\n");
    mrb_parser_dump(mrb, tree->car, offset + 2);
    dump_prefix(offset + 1);
    printf("rhs:\n");
    mrb_parser_dump(mrb, tree->cdr, offset + 2);
    break;

  case NODE_BACK_REF:
    printf("NODE_BACK_REF: $%c\n", (int)(intptr_t)tree);
    break;

  case NODE_NTH_REF:
    printf("NODE_NTH_REF: $%d\n", (int)(intptr_t)tree);
    break;

  case NODE_ARG:
    printf("NODE_ARG %s\n", mrb_sym2name(mrb, sym(tree)));
    break;

  case NODE_BLOCK_ARG:
    printf("NODE_BLOCK_ARG:\n");
    mrb_parser_dump(mrb, tree, offset+1);
    break;

  case NODE_INT:
    printf("NODE_INT %s base %d\n", (char*)tree->car, (int)(intptr_t)tree->cdr->car);
    break;

  case NODE_FLOAT:
    printf("NODE_FLOAT %s\n", (char*)tree);
    break;

  case NODE_NEGATE:
    printf("NODE_NEGATE\n");
    mrb_parser_dump(mrb, tree, offset+1);
    break;

  case NODE_STR:
    printf("NODE_STR \"%s\" len %d\n", (char*)tree->car, (int)(intptr_t)tree->cdr);
    break;

  case NODE_DSTR:
    printf("NODE_DSTR\n");
    dump_recur(mrb, tree, offset+1);
    break;

  case NODE_XSTR:
    printf("NODE_XSTR \"%s\" len %d\n", (char*)tree->car, (int)(intptr_t)tree->cdr);
    break;

  case NODE_DXSTR:
    printf("NODE_DXSTR\n");
    dump_recur(mrb, tree, offset+1);
    break;

  case NODE_REGX:
    printf("NODE_REGX /%s/%s\n", (char*)tree->car, (char*)tree->cdr);
    break;

  case NODE_DREGX:
    printf("NODE_DREGX\n");
    dump_recur(mrb, tree->car, offset+1);
    dump_prefix(offset);
    printf("tail: %s\n", (char*)tree->cdr->cdr->car);
    dump_prefix(offset);
    printf("opt: %s\n", (char*)tree->cdr->cdr->cdr);
    break;

  case NODE_SYM:
    printf("NODE_SYM :%s\n", mrb_sym2name(mrb, sym(tree)));
    break;

  case NODE_SELF:
    printf("NODE_SELF\n");
    break;

  case NODE_NIL:
    printf("NODE_NIL\n");
    break;

  case NODE_TRUE:
    printf("NODE_TRUE\n");
    break;

  case NODE_FALSE:
    printf("NODE_FALSE\n");
    break;

  case NODE_ALIAS:
    printf("NODE_ALIAS %s %s:\n",
        mrb_sym2name(mrb, sym(tree->car)),
        mrb_sym2name(mrb, sym(tree->cdr)));
    break;

  case NODE_UNDEF:
    printf("NODE_UNDEF");
    {
      node *t = tree;
      while (t) {
        printf(" %s", mrb_sym2name(mrb, sym(t->car)));
        t = t->cdr;
      }
    }
    printf(":\n");
    break;

  case NODE_CLASS:
    printf("NODE_CLASS:\n");
    if (tree->car->car == (node*)0) {
      dump_prefix(offset+1);
      printf(":%s\n", mrb_sym2name(mrb, sym(tree->car->cdr)));
    }
    else if (tree->car->car == (node*)1) {
      dump_prefix(offset+1);
      printf("::%s\n", mrb_sym2name(mrb, sym(tree->car->cdr)));
    }
    else {
      mrb_parser_dump(mrb, tree->car->car, offset+1);
      dump_prefix(offset+1);
      printf("::%s\n", mrb_sym2name(mrb, sym(tree->car->cdr)));
    }
    if (tree->cdr->car) {
      dump_prefix(offset+1);
      printf("super:\n");
      mrb_parser_dump(mrb, tree->cdr->car, offset+2);
    }
    dump_prefix(offset+1);
    printf("body:\n");
    mrb_parser_dump(mrb, tree->cdr->cdr->car->cdr, offset+2);
    break;

  case NODE_MODULE:
    printf("NODE_MODULE:\n");
    if (tree->car->car == (node*)0) {
      dump_prefix(offset+1);
      printf(":%s\n", mrb_sym2name(mrb, sym(tree->car->cdr)));
    }
    else if (tree->car->car == (node*)1) {
      dump_prefix(offset+1);
      printf("::%s\n", mrb_sym2name(mrb, sym(tree->car->cdr)));
    }
    else {
      mrb_parser_dump(mrb, tree->car->car, offset+1);
      dump_prefix(offset+1);
      printf("::%s\n", mrb_sym2name(mrb, sym(tree->car->cdr)));
    }
    dump_prefix(offset+1);
    printf("body:\n");
    mrb_parser_dump(mrb, tree->cdr->car->cdr, offset+2);
    break;

  case NODE_SCLASS:
    printf("NODE_SCLASS:\n");
    mrb_parser_dump(mrb, tree->car, offset+1);
    dump_prefix(offset+1);
    printf("body:\n");
    mrb_parser_dump(mrb, tree->cdr->car->cdr, offset+2);
    break;

  case NODE_DEF:
    printf("NODE_DEF:\n");
    dump_prefix(offset+1);
    printf("%s\n", mrb_sym2name(mrb, sym(tree->car)));
    tree = tree->cdr;
    {
      node *n2 = tree->car;

      if (n2 && (n2->car || n2->cdr)) {
        dump_prefix(offset+1);
        printf("local variables:\n");
        dump_prefix(offset+2);
        while (n2) {
          if (n2->car) {
            if (n2 != tree->car) printf(", ");
            printf("%s", mrb_sym2name(mrb, sym(n2->car)));
          }
          n2 = n2->cdr;
        }
        printf("\n");
      }
    }
    tree = tree->cdr;
    if (tree->car) {
      node *n = tree->car;

      if (n->car) {
        dump_prefix(offset+1);
        printf("mandatory args:\n");
        dump_recur(mrb, n->car, offset+2);
      }
      n = n->cdr;
      if (n->car) {
        dump_prefix(offset+1);
        printf("optional args:\n");
        {
          node *n2 = n->car;

          while (n2) {
            dump_prefix(offset+2);
            printf("%s=", mrb_sym2name(mrb, sym(n2->car->car)));
            mrb_parser_dump(mrb, n2->car->cdr, 0);
            n2 = n2->cdr;
          }
        }
      }
      n = n->cdr;
      if (n->car) {
        dump_prefix(offset+1);
        printf("rest=*%s\n", mrb_sym2name(mrb, sym(n->car)));
      }
      n = n->cdr;
      if (n->car) {
        dump_prefix(offset+1);
        printf("post mandatory args:\n");
        dump_recur(mrb, n->car, offset+2);
      }
      n = n->cdr;
      if (n) {
        dump_prefix(offset+1);
        printf("blk=&%s\n", mrb_sym2name(mrb, sym(n)));
      }
    }
    mrb_parser_dump(mrb, tree->cdr->car, offset+1);
    break;

  case NODE_SDEF:
    printf("NODE_SDEF:\n");
    mrb_parser_dump(mrb, tree->car, offset+1);
    tree = tree->cdr;
    dump_prefix(offset+1);
    printf(":%s\n", mrb_sym2name(mrb, sym(tree->car)));
    tree = tree->cdr->cdr;
    if (tree->car) {
      node *n = tree->car;

      if (n->car) {
        dump_prefix(offset+1);
        printf("mandatory args:\n");
        dump_recur(mrb, n->car, offset+2);
      }
      n = n->cdr;
      if (n->car) {
        dump_prefix(offset+1);
        printf("optional args:\n");
        {
          node *n2 = n->car;

          while (n2) {
            dump_prefix(offset+2);
            printf("%s=", mrb_sym2name(mrb, sym(n2->car->car)));
            mrb_parser_dump(mrb, n2->car->cdr, 0);
            n2 = n2->cdr;
          }
        }
      }
      n = n->cdr;
      if (n->car) {
        dump_prefix(offset+1);
        printf("rest=*%s\n", mrb_sym2name(mrb, sym(n->car)));
      }
      n = n->cdr;
      if (n->car) {
        dump_prefix(offset+1);
        printf("post mandatory args:\n");
        dump_recur(mrb, n->car, offset+2);
      }
      n = n->cdr;
      if (n) {
        dump_prefix(offset+1);
        printf("blk=&%s\n", mrb_sym2name(mrb, sym(n)));
      }
    }
    tree = tree->cdr;
    mrb_parser_dump(mrb, tree->car, offset+1);
    break;

  case NODE_POSTEXE:
    printf("NODE_POSTEXE:\n");
    mrb_parser_dump(mrb, tree, offset+1);
    break;

  case NODE_HEREDOC:
    printf("NODE_HEREDOC:\n");
    mrb_parser_dump(mrb, ((parser_heredoc_info*)tree)->doc, offset+1);
    break;

  default:
    printf("node type: %d (0x%x)\n", (int)n, (int)n);
    break;
  }
#endif
}

