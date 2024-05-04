/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "part1.y"

#include "../ast/ast.h"
#include "./smta.h"
#include<stdio.h>
extern int yylex();
extern int yylex_destroy();
extern int yywrap();
int yyerror(char *);
extern FILE * yyin;

astNode* root;

#line 84 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUM = 258,                     /* NUM  */
    NAME = 259,                    /* NAME  */
    PRINT = 260,                   /* PRINT  */
    READ = 261,                    /* READ  */
    RETURN = 262,                  /* RETURN  */
    IF = 263,                      /* IF  */
    ELSE = 264,                    /* ELSE  */
    WHILE = 265,                   /* WHILE  */
    INT = 266,                     /* INT  */
    VOID = 267,                    /* VOID  */
    EXTERN = 268,                  /* EXTERN  */
    LE = 269,                      /* LE  */
    GE = 270,                      /* GE  */
    EQ = 271,                      /* EQ  */
    NEQ = 272,                     /* NEQ  */
    LOWER_THAN_ELSE = 273          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define NUM 258
#define NAME 259
#define PRINT 260
#define READ 261
#define RETURN 262
#define IF 263
#define ELSE 264
#define WHILE 265
#define INT 266
#define VOID 267
#define EXTERN 268
#define LE 269
#define GE 270
#define EQ 271
#define NEQ 272
#define LOWER_THAN_ELSE 273

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 14 "part1.y"

	int ival;
	char * idname;
	astNode *nptr;
	vector<astNode *> *svec_ptr;

#line 180 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUM = 3,                        /* NUM  */
  YYSYMBOL_NAME = 4,                       /* NAME  */
  YYSYMBOL_PRINT = 5,                      /* PRINT  */
  YYSYMBOL_READ = 6,                       /* READ  */
  YYSYMBOL_RETURN = 7,                     /* RETURN  */
  YYSYMBOL_IF = 8,                         /* IF  */
  YYSYMBOL_ELSE = 9,                       /* ELSE  */
  YYSYMBOL_WHILE = 10,                     /* WHILE  */
  YYSYMBOL_INT = 11,                       /* INT  */
  YYSYMBOL_VOID = 12,                      /* VOID  */
  YYSYMBOL_EXTERN = 13,                    /* EXTERN  */
  YYSYMBOL_LE = 14,                        /* LE  */
  YYSYMBOL_GE = 15,                        /* GE  */
  YYSYMBOL_EQ = 16,                        /* EQ  */
  YYSYMBOL_NEQ = 17,                       /* NEQ  */
  YYSYMBOL_LOWER_THAN_ELSE = 18,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_19_ = 19,                       /* '('  */
  YYSYMBOL_20_ = 20,                       /* ')'  */
  YYSYMBOL_21_ = 21,                       /* ';'  */
  YYSYMBOL_22_ = 22,                       /* '{'  */
  YYSYMBOL_23_ = 23,                       /* '}'  */
  YYSYMBOL_24_ = 24,                       /* '<'  */
  YYSYMBOL_25_ = 25,                       /* '>'  */
  YYSYMBOL_26_ = 26,                       /* '='  */
  YYSYMBOL_27_ = 27,                       /* '+'  */
  YYSYMBOL_28_ = 28,                       /* '-'  */
  YYSYMBOL_29_ = 29,                       /* '*'  */
  YYSYMBOL_30_ = 30,                       /* '/'  */
  YYSYMBOL_YYACCEPT = 31,                  /* $accept  */
  YYSYMBOL_program = 32,                   /* program  */
  YYSYMBOL_print_line = 33,                /* print_line  */
  YYSYMBOL_read_line = 34,                 /* read_line  */
  YYSYMBOL_func = 35,                      /* func  */
  YYSYMBOL_code_block = 36,                /* code_block  */
  YYSYMBOL_stmts = 37,                     /* stmts  */
  YYSYMBOL_stmt = 38,                      /* stmt  */
  YYSYMBOL_condition_block = 39,           /* condition_block  */
  YYSYMBOL_if_condition = 40,              /* if_condition  */
  YYSYMBOL_else_condition = 41,            /* else_condition  */
  YYSYMBOL_while_block = 42,               /* while_block  */
  YYSYMBOL_bool_condition = 43,            /* bool_condition  */
  YYSYMBOL_assign_stmt = 44,               /* assign_stmt  */
  YYSYMBOL_expr = 45,                      /* expr  */
  YYSYMBOL_term = 46,                      /* term  */
  YYSYMBOL_var_decls = 47,                 /* var_decls  */
  YYSYMBOL_decl = 48,                      /* decl  */
  YYSYMBOL_call_func_stmt = 49,            /* call_func_stmt  */
  YYSYMBOL_return_stmt = 50                /* return_stmt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   132

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  46
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  117

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   273


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      19,    20,    29,    27,     2,    28,     2,    30,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    21,
      24,    26,    25,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    22,     2,    23,     2,     2,     2,     2,
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
      15,    16,    17,    18
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int8 yyrline[] =
{
       0,    39,    39,    41,    42,    46,    48,    50,    52,    57,
      66,    69,    70,    71,    72,    73,    74,    75,    76,    79,
      80,    81,    82,    85,    88,    89,    90,    91,    92,    93,
      96,    98,   100,   101,   102,   103,   104,   107,   108,   109,
     112,   113,   114,   118,   119,   122,   123
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUM", "NAME", "PRINT",
  "READ", "RETURN", "IF", "ELSE", "WHILE", "INT", "VOID", "EXTERN", "LE",
  "GE", "EQ", "NEQ", "LOWER_THAN_ELSE", "'('", "')'", "';'", "'{'", "'}'",
  "'<'", "'>'", "'='", "'+'", "'-'", "'*'", "'/'", "$accept", "program",
  "print_line", "read_line", "func", "code_block", "stmts", "stmt",
  "condition_block", "if_condition", "else_condition", "while_block",
  "bool_condition", "assign_stmt", "expr", "term", "var_decls", "decl",
  "call_func_stmt", "return_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-53)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -8,    13,    24,    21,    30,   -53,    48,    10,    42,    56,
      65,    67,   -53,    68,    72,    74,    76,    77,    78,    -1,
       0,    75,    79,    95,    81,    97,    81,   -53,   -53,    84,
      70,   -53,    85,   -53,    81,    82,    87,    88,     4,    90,
      91,   107,   -53,    50,   -53,   -53,    80,   -53,   -53,    70,
     -53,   -53,   -53,    81,   -53,    12,   108,    93,   -53,   -53,
      -2,    -2,    94,    14,    -2,    -2,    96,   -53,   -53,   105,
      60,   -53,   -53,    98,   -53,   100,   101,   103,   -53,   -53,
      -2,    -2,    -2,    -2,   104,    22,   106,   -53,    80,   -53,
     -53,   -53,   109,   -53,   110,   -53,   -53,   -53,   -53,   -53,
      -2,    -2,    -2,    -2,    -2,    -2,    80,   -53,   -53,   -53,
     -53,   -53,   -53,   -53,   -53,   -53,   -53
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     1,     0,     0,     0,     0,
       0,     0,     2,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     3,     4,     0,
       0,     7,     0,     8,     0,     0,     0,     0,     0,     0,
       0,     0,    18,     0,    12,    16,     0,    17,    13,     0,
      41,    14,    15,     0,     5,     0,     0,     0,    37,    38,
       0,     0,     0,    36,     0,     0,     0,    10,    11,    19,
       0,    40,     6,     0,    31,     0,     0,     0,    39,    46,
       0,     0,     0,     0,     0,     0,     0,    42,     0,    20,
       9,    30,     0,    44,     0,    32,    33,    34,    35,    21,
       0,     0,     0,     0,     0,     0,     0,    22,    43,    45,
      26,    27,    28,    29,    24,    25,    23
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -53,   -53,   -53,   -53,   -53,   -20,    69,   -43,   -53,   -53,
     -53,   -53,    51,   -53,    34,   -52,   -53,    83,    66,   -53
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,     7,    12,    42,    43,    44,    45,    46,
      89,    47,    84,    48,    62,    63,    49,    50,    51,    52
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      68,    58,    59,    69,    31,     1,    33,    58,    59,    78,
      23,    25,    85,    85,    54,    58,    59,    36,    37,    24,
      26,    10,    11,    60,     5,     4,    61,    68,    95,    96,
      97,    98,    61,    72,     6,     8,   100,   101,   102,   103,
      61,    80,    81,    82,    83,   107,   104,   105,   110,   111,
     112,   113,   114,   115,    35,    36,    37,    38,    39,     9,
      40,    13,    14,   116,    35,    36,    37,    38,    39,    15,
      40,    16,    30,    67,    35,    36,    37,    38,    39,    17,
      40,    41,    30,    90,    35,    36,    37,    38,    39,    73,
      40,    18,    30,    19,    77,    20,    27,    21,    22,    29,
      28,    32,    30,    30,    34,    53,    56,    57,    55,    64,
      65,    66,    75,    76,    88,    79,    86,    87,    70,    91,
      92,    74,    93,    94,    99,     0,   106,     0,     0,     0,
     108,   109,    71
};

static const yytype_int8 yycheck[] =
{
      43,     3,     4,    46,    24,    13,    26,     3,     4,    61,
      11,    11,    64,    65,    34,     3,     4,     5,     6,    20,
      20,    11,    12,    19,     0,    12,    28,    70,    80,    81,
      82,    83,    28,    53,    13,     5,    14,    15,    16,    17,
      28,    27,    28,    29,    30,    88,    24,    25,   100,   101,
     102,   103,   104,   105,     4,     5,     6,     7,     8,    11,
      10,    19,     6,   106,     4,     5,     6,     7,     8,     4,
      10,     4,    22,    23,     4,     5,     6,     7,     8,    11,
      10,    11,    22,    23,     4,     5,     6,     7,     8,    55,
      10,    19,    22,    19,    60,    19,    21,    20,    20,     4,
      21,     4,    22,    22,    20,    20,    19,    19,    26,    19,
      19,     4,     4,    20,     9,    21,    65,    21,    49,    21,
      20,    55,    21,    20,    20,    -1,    20,    -1,    -1,    -1,
      21,    21,    49
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    13,    32,    33,    12,     0,    13,    34,     5,    11,
      11,    12,    35,    19,     6,     4,     4,    11,    19,    19,
      19,    20,    20,    11,    20,    11,    20,    21,    21,     4,
      22,    36,     4,    36,    20,     4,     5,     6,     7,     8,
      10,    11,    36,    37,    38,    39,    40,    42,    44,    47,
      48,    49,    50,    20,    36,    26,    19,    19,     3,     4,
      19,    28,    45,    46,    19,    19,     4,    23,    38,    38,
      37,    48,    36,    45,    49,     4,    20,    45,    46,    21,
      27,    28,    29,    30,    43,    46,    43,    21,     9,    41,
      23,    21,    20,    21,    20,    46,    46,    46,    46,    20,
      14,    15,    16,    17,    24,    25,    20,    38,    21,    21,
      46,    46,    46,    46,    46,    46,    38
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    31,    32,    33,    34,    35,    35,    35,    35,    36,
      36,    37,    37,    38,    38,    38,    38,    38,    38,    39,
      39,    40,    41,    42,    43,    43,    43,    43,    43,    43,
      44,    44,    45,    45,    45,    45,    45,    46,    46,    46,
      47,    47,    48,    49,    49,    50,    50
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     7,     6,     7,     7,     5,     5,     4,
       3,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       3,     4,     2,     5,     3,     3,     3,     3,     3,     3,
       4,     3,     3,     3,     3,     3,     1,     1,     1,     2,
       2,     1,     3,     5,     4,     5,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
      if (yytable_value_is_error (yyn))
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: print_line read_line func  */
#line 39 "part1.y"
                                              {(yyval.nptr) = createProg((yyvsp[-2].nptr), (yyvsp[-1].nptr), (yyvsp[0].nptr));  printNode((yyval.nptr));
												root = (yyval.nptr);}
#line 1277 "y.tab.c"
    break;

  case 3: /* print_line: EXTERN VOID PRINT '(' INT ')' ';'  */
#line 41 "part1.y"
                                                    {(yyval.nptr) = createExtern("print");}
#line 1283 "y.tab.c"
    break;

  case 4: /* read_line: EXTERN INT READ '(' ')' ';'  */
#line 42 "part1.y"
                                                    {(yyval.nptr) = createExtern("read");}
#line 1289 "y.tab.c"
    break;

  case 5: /* func: INT NAME '(' INT NAME ')' code_block  */
#line 46 "part1.y"
                                                  {(yyval.nptr) = createFunc((yyvsp[-5].idname), createVar((yyvsp[-2].idname)), (yyvsp[0].nptr)); printNode((yyval.nptr));
													free((yyvsp[-5].idname)); free((yyvsp[-2].idname));}
#line 1296 "y.tab.c"
    break;

  case 6: /* func: VOID NAME '(' INT NAME ')' code_block  */
#line 48 "part1.y"
                                                  {(yyval.nptr) = createFunc((yyvsp[-5].idname), createVar((yyvsp[-2].idname)), (yyvsp[0].nptr)); printNode((yyval.nptr));
													free((yyvsp[-5].idname)); free((yyvsp[-2].idname));}
#line 1303 "y.tab.c"
    break;

  case 7: /* func: INT NAME '(' ')' code_block  */
#line 50 "part1.y"
                                                  {(yyval.nptr) = createFunc((yyvsp[-3].idname), NULL, (yyvsp[0].nptr)); printNode((yyval.nptr));
													free((yyvsp[-3].idname));}
#line 1310 "y.tab.c"
    break;

  case 8: /* func: VOID NAME '(' ')' code_block  */
#line 52 "part1.y"
                                                  {(yyval.nptr) = createFunc((yyvsp[-3].idname), NULL, (yyvsp[0].nptr)); printNode((yyval.nptr));
													free((yyvsp[-3].idname));}
#line 1317 "y.tab.c"
    break;

  case 9: /* code_block: '{' var_decls stmts '}'  */
#line 57 "part1.y"
                                        {
											vector<astNode*>* new_vec = new vector<astNode*>();
											new_vec->insert(new_vec->end(), (yyvsp[-2].svec_ptr)->begin(), (yyvsp[-2].svec_ptr)->end());
											new_vec->insert(new_vec->end(), (yyvsp[-1].svec_ptr)->begin(), (yyvsp[-1].svec_ptr)->end());
											(yyval.nptr) = createBlock(new_vec); 
											printNode((yyval.nptr));
											delete((yyvsp[-2].svec_ptr));
											delete((yyvsp[-1].svec_ptr));
										}
#line 1331 "y.tab.c"
    break;

  case 10: /* code_block: '{' stmts '}'  */
#line 66 "part1.y"
                                                                {(yyval.nptr) = createBlock((yyvsp[-1].svec_ptr)); printNode((yyval.nptr));}
#line 1337 "y.tab.c"
    break;

  case 11: /* stmts: stmts stmt  */
#line 69 "part1.y"
                                                                {(yyval.svec_ptr) = (yyvsp[-1].svec_ptr); (yyval.svec_ptr)->push_back((yyvsp[0].nptr));}
#line 1343 "y.tab.c"
    break;

  case 12: /* stmts: stmt  */
#line 70 "part1.y"
                                                                        {(yyval.svec_ptr) = new vector<astNode*>(); (yyval.svec_ptr)->push_back((yyvsp[0].nptr));}
#line 1349 "y.tab.c"
    break;

  case 13: /* stmt: assign_stmt  */
#line 71 "part1.y"
                                                                {(yyval.nptr) = (yyvsp[0].nptr); printNode((yyval.nptr));}
#line 1355 "y.tab.c"
    break;

  case 14: /* stmt: call_func_stmt  */
#line 72 "part1.y"
                                                                {(yyval.nptr) = (yyvsp[0].nptr); printNode((yyval.nptr));}
#line 1361 "y.tab.c"
    break;

  case 15: /* stmt: return_stmt  */
#line 73 "part1.y"
                                                                {(yyval.nptr) = (yyvsp[0].nptr); printNode((yyval.nptr));}
#line 1367 "y.tab.c"
    break;

  case 16: /* stmt: condition_block  */
#line 74 "part1.y"
                                                                {(yyval.nptr) = (yyvsp[0].nptr); printNode((yyval.nptr));}
#line 1373 "y.tab.c"
    break;

  case 17: /* stmt: while_block  */
#line 75 "part1.y"
                                                                {(yyval.nptr) = (yyvsp[0].nptr); printNode((yyval.nptr));}
#line 1379 "y.tab.c"
    break;

  case 18: /* stmt: code_block  */
#line 76 "part1.y"
                                                                {(yyval.nptr) = (yyvsp[0].nptr); printNode((yyval.nptr));}
#line 1385 "y.tab.c"
    break;

  case 19: /* condition_block: if_condition stmt  */
#line 79 "part1.y"
                                                                                        {(yyval.nptr) = createIf((yyvsp[-1].nptr), (yyvsp[0].nptr)); printNode((yyval.nptr));}
#line 1391 "y.tab.c"
    break;

  case 20: /* condition_block: if_condition stmt else_condition  */
#line 80 "part1.y"
                                                                                                        {(yyval.nptr) = createIf((yyvsp[-2].nptr), (yyvsp[-1].nptr), (yyvsp[0].nptr)); printNode((yyval.nptr));}
#line 1397 "y.tab.c"
    break;

  case 21: /* if_condition: IF '(' bool_condition ')'  */
#line 81 "part1.y"
                                                        {(yyval.nptr) = (yyvsp[-1].nptr); printNode((yyval.nptr));}
#line 1403 "y.tab.c"
    break;

  case 22: /* else_condition: ELSE stmt  */
#line 82 "part1.y"
                                                                        {(yyval.nptr) = (yyvsp[0].nptr); printNode((yyval.nptr));}
#line 1409 "y.tab.c"
    break;

  case 23: /* while_block: WHILE '(' bool_condition ')' stmt  */
#line 85 "part1.y"
                                                                        {(yyval.nptr) = createWhile((yyvsp[-2].nptr),(yyvsp[0].nptr));}
#line 1415 "y.tab.c"
    break;

  case 24: /* bool_condition: term '<' term  */
#line 88 "part1.y"
                                                        {(yyval.nptr) = createRExpr((yyvsp[-2].nptr), (yyvsp[0].nptr), lt); printNode((yyval.nptr));}
#line 1421 "y.tab.c"
    break;

  case 25: /* bool_condition: term '>' term  */
#line 89 "part1.y"
                                                                        {(yyval.nptr) = createRExpr((yyvsp[-2].nptr), (yyvsp[0].nptr), gt); printNode((yyval.nptr));}
#line 1427 "y.tab.c"
    break;

  case 26: /* bool_condition: term LE term  */
#line 90 "part1.y"
                                                                        {(yyval.nptr) = createRExpr((yyvsp[-2].nptr), (yyvsp[0].nptr), le); printNode((yyval.nptr));}
#line 1433 "y.tab.c"
    break;

  case 27: /* bool_condition: term GE term  */
#line 91 "part1.y"
                                                                        {(yyval.nptr) = createRExpr((yyvsp[-2].nptr), (yyvsp[0].nptr), ge); printNode((yyval.nptr));}
#line 1439 "y.tab.c"
    break;

  case 28: /* bool_condition: term EQ term  */
#line 92 "part1.y"
                                                                        {(yyval.nptr) = createRExpr((yyvsp[-2].nptr), (yyvsp[0].nptr), eq); printNode((yyval.nptr));}
#line 1445 "y.tab.c"
    break;

  case 29: /* bool_condition: term NEQ term  */
#line 93 "part1.y"
                                                                        {(yyval.nptr) = createRExpr((yyvsp[-2].nptr), (yyvsp[0].nptr), neq); printNode((yyval.nptr));}
#line 1451 "y.tab.c"
    break;

  case 30: /* assign_stmt: NAME '=' expr ';'  */
#line 96 "part1.y"
                                                                {(yyval.nptr) = createAsgn(createVar((yyvsp[-3].idname)), (yyvsp[-1].nptr)); printNode((yyval.nptr));
												free((yyvsp[-3].idname));}
#line 1458 "y.tab.c"
    break;

  case 31: /* assign_stmt: NAME '=' call_func_stmt  */
#line 98 "part1.y"
                                                                        {(yyval.nptr) = createAsgn(createVar((yyvsp[-2].idname)), (yyvsp[0].nptr)); printNode((yyval.nptr));
												free((yyvsp[-2].idname));}
#line 1465 "y.tab.c"
    break;

  case 32: /* expr: term '+' term  */
#line 100 "part1.y"
                                                                        {(yyval.nptr) = createBExpr((yyvsp[-2].nptr), (yyvsp[0].nptr), add); printNode((yyval.nptr));}
#line 1471 "y.tab.c"
    break;

  case 33: /* expr: term '-' term  */
#line 101 "part1.y"
                                                                                {(yyval.nptr) = createBExpr((yyvsp[-2].nptr), (yyvsp[0].nptr), sub); printNode((yyval.nptr));}
#line 1477 "y.tab.c"
    break;

  case 34: /* expr: term '*' term  */
#line 102 "part1.y"
                                                                                {(yyval.nptr) = createBExpr((yyvsp[-2].nptr), (yyvsp[0].nptr), mul); printNode((yyval.nptr));}
#line 1483 "y.tab.c"
    break;

  case 35: /* expr: term '/' term  */
#line 103 "part1.y"
                                                                                {(yyval.nptr) = createBExpr((yyvsp[-2].nptr), (yyvsp[0].nptr), divide); printNode((yyval.nptr));}
#line 1489 "y.tab.c"
    break;

  case 36: /* expr: term  */
#line 104 "part1.y"
                                                                                        {(yyval.nptr) = (yyvsp[0].nptr); printNode((yyval.nptr));}
#line 1495 "y.tab.c"
    break;

  case 37: /* term: NUM  */
#line 107 "part1.y"
                                                                                        {(yyval.nptr) = createCnst((yyvsp[0].ival)); printNode((yyval.nptr));}
#line 1501 "y.tab.c"
    break;

  case 38: /* term: NAME  */
#line 108 "part1.y"
                                                                                        {(yyval.nptr) = createVar((yyvsp[0].idname)); printNode((yyval.nptr)); free((yyvsp[0].idname));}
#line 1507 "y.tab.c"
    break;

  case 39: /* term: '-' term  */
#line 109 "part1.y"
                                                                                        {(yyval.nptr) = createUExpr((yyvsp[0].nptr), uminus); printNode((yyval.nptr));}
#line 1513 "y.tab.c"
    break;

  case 40: /* var_decls: var_decls decl  */
#line 112 "part1.y"
                                                        {(yyval.svec_ptr) = (yyvsp[-1].svec_ptr); (yyval.svec_ptr)->push_back((yyvsp[0].nptr));}
#line 1519 "y.tab.c"
    break;

  case 41: /* var_decls: decl  */
#line 113 "part1.y"
                                                                        {(yyval.svec_ptr) = new vector<astNode*>(); (yyval.svec_ptr)->push_back((yyvsp[0].nptr));}
#line 1525 "y.tab.c"
    break;

  case 42: /* decl: INT NAME ';'  */
#line 114 "part1.y"
                                                                {(yyval.nptr) = createDecl((yyvsp[-1].idname)); printNode((yyval.nptr));
										free((yyvsp[-1].idname));}
#line 1532 "y.tab.c"
    break;

  case 43: /* call_func_stmt: PRINT '(' NAME ')' ';'  */
#line 118 "part1.y"
                                        {(yyval.nptr) = createCall("print", createVar((yyvsp[-2].idname))); printNode((yyval.nptr)); free((yyvsp[-2].idname));}
#line 1538 "y.tab.c"
    break;

  case 44: /* call_func_stmt: READ '(' ')' ';'  */
#line 119 "part1.y"
                                                                {(yyval.nptr) = createCall("read"); printNode((yyval.nptr));}
#line 1544 "y.tab.c"
    break;

  case 45: /* return_stmt: RETURN '(' expr ')' ';'  */
#line 122 "part1.y"
                                                {(yyval.nptr) = createRet((yyvsp[-2].nptr)); printNode((yyval.nptr));}
#line 1550 "y.tab.c"
    break;

  case 46: /* return_stmt: RETURN expr ';'  */
#line 123 "part1.y"
                                                                        {(yyval.nptr) = createRet((yyvsp[-1].nptr)); printNode((yyval.nptr));}
#line 1556 "y.tab.c"
    break;


#line 1560 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
                      yytoken, &yylval);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 124 "part1.y"


int yyerror(char *s){
	exit(1);
}

astNode* ret_root(){
	return root;
}
