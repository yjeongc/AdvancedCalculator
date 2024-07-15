/*
 * Declarations for a calculator fb3-2
 */

/* interface to the lexer */
extern int yylineno; /*다른 파일에서 yylineno가 정의되어 있음. 파일이 개별 컴파일됨. 그래서 include 없이 extern을 쓸 수 있는 것*/
void yyerror(char *s, ...);

/* symbol table */
struct symbol {         /* a variable name */
  char *name;
  double value;
  struct ast *func;     /* ast는 소스코드의 구조를 트리 형태로 기록한 것. */
  struct symlist *syms; /* list of dummy args */
};

/* simple symtab of fixed size */
#define NHASH 9997
extern struct symbol symtab[NHASH];

struct symbol *lookup(char*);

/* list of symbols, for an argument list */
struct symlist {
  struct symbol *sym;
  struct symlist *next;
};

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

/* node types
 *  + - * / |
 *  0-7 comparison ops, bit coded 04 equal, 02 less, 01 greater
 *  M unary minus
 *  L expression or statement list
 *  I IF statement
 *  W WHILE statement
 *  N symbol ref
 *  = assignment
 *  S list of symbols
 *  F built in function call
 *  C user function call
 */ 

enum bifs {                     /* built-in functions */
  B_sqrt = 1,
  B_exp,
  B_log,
  B_print
};

/* nodes in the abstract syntax tree */
/* all have common initial nodetype */

struct ast {
  int nodetype;
  struct ast *l;
  struct ast *r;
};

struct fncall {                 /* built-in function */
  int nodetype;                 /* type F */
  struct ast *l;
  enum bifs functype;
};

struct ufncall {                /* user function */
  int nodetype;                 /* type C */
  struct ast *l;                /* list of arguments */
  struct symbol *s;
};

struct flow {
  int nodetype;                 /* type I or W */
  struct ast *cond;             /* condition */
  struct ast *tl;               /* then branch or do list */
  struct ast *el;               /* optional else branch */
};

struct numval {
  int nodetype;                 /* type K */
  double number;
};

struct symref {
  int nodetype;                 /* type N */
  struct symbol *s;
};

struct symasgn {
  int nodetype;                 /* type = */
  struct symbol *s;
  struct ast *v;                /* value */
};

/* build an AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newcall(struct symbol *s, struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);

/* define a function */
void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts);

/* evaluate an AST */
double eval(struct ast *);

/* delete and free an AST */
void treefree(struct ast *);

/* lexer and parser functions */
int yylex(void);
int yyparse(void);
