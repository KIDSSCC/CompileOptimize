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

/* "%code top" blocks.  */
#line 1 "src/parser.y"

    #include <iostream>
    #include <cstring>
    #include <assert.h>
    #include "parser.h"
    using namespace std;
    //来自main.cpp
    extern Ast ast;
    int yylex();
    int yyerror( char const * );

    std::string tempType;

    //为while循环中的break和continue做的准备。
    std::vector<int> new_WhileStmt_Pos;
    std::vector<loopctl*> loop_ctrl;

    //记录while句子个数，如果为0的时候遇见了break或continue要报错
    int whilenum = 0;
    bool if_FuncDefStmt_IsNull=0;

    ArrayType* arraytype;
    //数组索引
    int myindex;
    //存储进的等价一维int数组，重点计算size
    int* equalintarray;
    float* equalfloatarray;
    std::stack<Initlist*> initstack;
    Initlist* top;
    int bracenum= 0;
    
    int intParam_num_for_func=0;
    int floatParam_num_for_func=0;
    bool functionhasRet=false;

#line 104 "src/parser.cpp"




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

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_INTEGER = 4,                    /* INTEGER  */
  YYSYMBOL_FLOATNUMBER = 5,                /* FLOATNUMBER  */
  YYSYMBOL_IF = 6,                         /* IF  */
  YYSYMBOL_ELSE = 7,                       /* ELSE  */
  YYSYMBOL_WHILE = 8,                      /* WHILE  */
  YYSYMBOL_INT = 9,                        /* INT  */
  YYSYMBOL_VOID = 10,                      /* VOID  */
  YYSYMBOL_FLOAT = 11,                     /* FLOAT  */
  YYSYMBOL_LPAREN = 12,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 13,                    /* RPAREN  */
  YYSYMBOL_LBRACK = 14,                    /* LBRACK  */
  YYSYMBOL_RBRACK = 15,                    /* RBRACK  */
  YYSYMBOL_LBRACE = 16,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 17,                    /* RBRACE  */
  YYSYMBOL_SEMICOLON = 18,                 /* SEMICOLON  */
  YYSYMBOL_COMM = 19,                      /* COMM  */
  YYSYMBOL_ADD = 20,                       /* ADD  */
  YYSYMBOL_SUB = 21,                       /* SUB  */
  YYSYMBOL_MUL = 22,                       /* MUL  */
  YYSYMBOL_DIV = 23,                       /* DIV  */
  YYSYMBOL_MOD = 24,                       /* MOD  */
  YYSYMBOL_OR = 25,                        /* OR  */
  YYSYMBOL_AND = 26,                       /* AND  */
  YYSYMBOL_NOT = 27,                       /* NOT  */
  YYSYMBOL_LESS = 28,                      /* LESS  */
  YYSYMBOL_GREAT = 29,                     /* GREAT  */
  YYSYMBOL_EQ = 30,                        /* EQ  */
  YYSYMBOL_NE = 31,                        /* NE  */
  YYSYMBOL_LE = 32,                        /* LE  */
  YYSYMBOL_GE = 33,                        /* GE  */
  YYSYMBOL_ASSIGN = 34,                    /* ASSIGN  */
  YYSYMBOL_CONST = 35,                     /* CONST  */
  YYSYMBOL_RETURN = 36,                    /* RETURN  */
  YYSYMBOL_CONTINUE = 37,                  /* CONTINUE  */
  YYSYMBOL_BREAK = 38,                     /* BREAK  */
  YYSYMBOL_THEN = 39,                      /* THEN  */
  YYSYMBOL_YYACCEPT = 40,                  /* $accept  */
  YYSYMBOL_Program = 41,                   /* Program  */
  YYSYMBOL_Stmts = 42,                     /* Stmts  */
  YYSYMBOL_Stmt = 43,                      /* Stmt  */
  YYSYMBOL_LVal = 44,                      /* LVal  */
  YYSYMBOL_AssignStmt = 45,                /* AssignStmt  */
  YYSYMBOL_BlockStmt = 46,                 /* BlockStmt  */
  YYSYMBOL_47_1 = 47,                      /* $@1  */
  YYSYMBOL_BlankStmt = 48,                 /* BlankStmt  */
  YYSYMBOL_IfStmt = 49,                    /* IfStmt  */
  YYSYMBOL_ReturnStmt = 50,                /* ReturnStmt  */
  YYSYMBOL_WhileStmt = 51,                 /* WhileStmt  */
  YYSYMBOL_52_2 = 52,                      /* $@2  */
  YYSYMBOL_BrkStmt = 53,                   /* BrkStmt  */
  YYSYMBOL_CtnueStmt = 54,                 /* CtnueStmt  */
  YYSYMBOL_ExprStmt = 55,                  /* ExprStmt  */
  YYSYMBOL_Exp = 56,                       /* Exp  */
  YYSYMBOL_Cond = 57,                      /* Cond  */
  YYSYMBOL_PrimaryExp = 58,                /* PrimaryExp  */
  YYSYMBOL_UnaryExp = 59,                  /* UnaryExp  */
  YYSYMBOL_MulExp = 60,                    /* MulExp  */
  YYSYMBOL_AddExp = 61,                    /* AddExp  */
  YYSYMBOL_RelExp = 62,                    /* RelExp  */
  YYSYMBOL_EqExp = 63,                     /* EqExp  */
  YYSYMBOL_LAndExp = 64,                   /* LAndExp  */
  YYSYMBOL_LOrExp = 65,                    /* LOrExp  */
  YYSYMBOL_ConstExp = 66,                  /* ConstExp  */
  YYSYMBOL_Type = 67,                      /* Type  */
  YYSYMBOL_DeclStmt = 68,                  /* DeclStmt  */
  YYSYMBOL_ConstDecl = 69,                 /* ConstDecl  */
  YYSYMBOL_ConstDefList = 70,              /* ConstDefList  */
  YYSYMBOL_ConstDef = 71,                  /* ConstDef  */
  YYSYMBOL_72_3 = 72,                      /* @3  */
  YYSYMBOL_ConstInitVal = 73,              /* ConstInitVal  */
  YYSYMBOL_74_4 = 74,                      /* @4  */
  YYSYMBOL_ConstInitValList = 75,          /* ConstInitValList  */
  YYSYMBOL_ArrayIndex = 76,                /* ArrayIndex  */
  YYSYMBOL_VarDecl = 77,                   /* VarDecl  */
  YYSYMBOL_VarDefList = 78,                /* VarDefList  */
  YYSYMBOL_VarDef = 79,                    /* VarDef  */
  YYSYMBOL_80_5 = 80,                      /* @5  */
  YYSYMBOL_VarInitVal = 81,                /* VarInitVal  */
  YYSYMBOL_82_6 = 82,                      /* @6  */
  YYSYMBOL_VarInitValList = 83,            /* VarInitValList  */
  YYSYMBOL_FuncDef = 84,                   /* FuncDef  */
  YYSYMBOL_85_7 = 85,                      /* $@7  */
  YYSYMBOL_86_8 = 86,                      /* $@8  */
  YYSYMBOL_FuncParams = 87,                /* FuncParams  */
  YYSYMBOL_FuncParam = 88,                 /* FuncParam  */
  YYSYMBOL_FunArrayIndex = 89,             /* FunArrayIndex  */
  YYSYMBOL_FuncCall = 90,                  /* FuncCall  */
  YYSYMBOL_ParaList = 91                   /* ParaList  */
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
typedef yytype_uint8 yy_state_t;

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
#define YYFINAL  64
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   234

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  52
/* YYNRULES -- Number of rules.  */
#define YYNRULES  109
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  181

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   294


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
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    71,    71,    76,    77,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    95,   107,   123,   129,
     128,   138,   145,   150,   153,   158,   162,   169,   168,   189,
     201,   212,   217,   221,   224,   227,   230,   234,   238,   241,
     242,   245,   258,   264,   265,   270,   275,   283,   285,   292,
     300,   301,   305,   309,   313,   319,   320,   324,   330,   331,
     338,   339,   346,   349,   353,   357,   362,   363,   366,   369,
     373,   376,   396,   396,   441,   515,   578,   578,   629,   632,
     637,   640,   646,   649,   653,   656,   674,   694,   729,   729,
     779,   846,   913,   913,   974,   977,   983,   990,   983,  1037,
    1041,  1044,  1047,  1064,  1123,  1126,  1132,  1165,  1169,  1170
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
  "\"end of file\"", "error", "\"invalid token\"", "ID", "INTEGER",
  "FLOATNUMBER", "IF", "ELSE", "WHILE", "INT", "VOID", "FLOAT", "LPAREN",
  "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE", "SEMICOLON", "COMM",
  "ADD", "SUB", "MUL", "DIV", "MOD", "OR", "AND", "NOT", "LESS", "GREAT",
  "EQ", "NE", "LE", "GE", "ASSIGN", "CONST", "RETURN", "CONTINUE", "BREAK",
  "THEN", "$accept", "Program", "Stmts", "Stmt", "LVal", "AssignStmt",
  "BlockStmt", "$@1", "BlankStmt", "IfStmt", "ReturnStmt", "WhileStmt",
  "$@2", "BrkStmt", "CtnueStmt", "ExprStmt", "Exp", "Cond", "PrimaryExp",
  "UnaryExp", "MulExp", "AddExp", "RelExp", "EqExp", "LAndExp", "LOrExp",
  "ConstExp", "Type", "DeclStmt", "ConstDecl", "ConstDefList", "ConstDef",
  "@3", "ConstInitVal", "@4", "ConstInitValList", "ArrayIndex", "VarDecl",
  "VarDefList", "VarDef", "@5", "VarInitVal", "@6", "VarInitValList",
  "FuncDef", "$@7", "$@8", "FuncParams", "FuncParam", "FunArrayIndex",
  "FuncCall", "ParaList", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-131)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-97)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     196,    87,  -131,  -131,     6,  -131,  -131,  -131,  -131,    50,
      16,  -131,    50,    50,    50,    80,     8,    18,    43,    22,
     196,  -131,    58,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,    69,  -131,  -131,    88,    86,   125,  -131,    85,
     111,  -131,  -131,    50,    50,   116,    50,   119,  -131,  -131,
      76,    91,   107,    17,  -131,   196,  -131,  -131,  -131,   131,
    -131,   118,  -131,  -131,  -131,  -131,    50,  -131,    50,    50,
      50,    50,    50,     5,   120,  -131,  -131,  -131,  -131,    67,
      86,   126,    50,   129,   121,    50,    50,    50,    50,    50,
      50,    50,    50,  -131,    50,   160,    26,   124,  -131,  -131,
     127,  -131,  -131,  -131,    88,    88,    11,    35,   132,   144,
    -131,    50,  -131,   134,   196,   137,    86,    86,    86,    86,
      76,    76,    91,   107,  -131,    47,    44,   131,  -131,   136,
    -131,  -131,  -131,    80,    61,  -131,  -131,  -131,   147,   196,
     138,  -131,  -131,  -131,  -131,  -131,    11,    11,   148,    75,
    -131,   196,  -131,  -131,    47,    47,  -131,    99,  -131,   142,
    -131,    80,  -131,  -131,   100,  -131,  -131,    11,   145,   143,
     146,  -131,  -131,    47,  -131,  -131,    50,  -131,  -131,   158,
    -131
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    16,    36,    37,     0,    27,    63,    65,    64,     0,
      19,    22,     0,     0,     0,     0,     0,     0,     0,     0,
       2,     3,    35,     5,     6,    14,     7,     8,    11,    12,
      13,    15,     0,    39,    43,    47,    32,     0,     9,     0,
       0,    10,    38,   109,     0,    17,     0,     0,    35,    50,
      55,    58,    60,     0,    21,     0,    40,    41,    42,     0,
      26,     0,    30,    29,     1,     4,     0,    31,     0,     0,
       0,     0,     0,    85,    82,    84,    66,    67,   108,     0,
      62,     0,     0,     0,    33,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,     0,     0,    68,    70,    25,
       0,    44,    45,    46,    48,    49,     0,    87,     0,     0,
     106,     0,    80,     0,     0,     0,    51,    52,    53,    54,
      56,    57,    59,    61,    20,     0,     0,     0,    18,    92,
      90,    86,    88,   101,    85,    83,   107,    81,    23,     0,
      76,    74,    71,    72,    69,    91,     0,     0,     0,     0,
     100,     0,    28,    75,     0,     0,    94,     0,    89,   102,
      97,     0,    24,    78,     0,    73,    93,     0,     0,   103,
       0,    99,    77,     0,    95,   104,     0,    98,    79,     0,
     105
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -131,  -131,   104,   -16,     1,  -131,     9,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,    -9,    89,  -131,    -4,
      53,   -41,    36,    83,    90,   152,   -38,   -13,  -131,  -131,
    -131,    55,  -131,  -130,  -131,  -131,     4,  -131,  -131,    74,
    -131,   -74,  -131,  -131,  -131,  -131,  -131,  -131,    24,  -131,
    -131,  -131
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    19,    20,    21,    48,    23,    24,    55,    25,    26,
      27,    28,    47,    29,    30,    31,    32,    83,    33,    34,
      35,    36,    50,    51,    52,    84,   141,    37,    38,    39,
      97,    98,   155,   142,   154,   164,   107,    40,    74,    75,
     147,   131,   146,   157,    41,   108,   170,   149,   150,   169,
      42,    79
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      49,    22,    59,    80,    65,    45,    81,    61,    56,    57,
      58,     1,     2,     3,     1,     2,     3,   -96,    46,    44,
       9,    22,    64,     9,   163,   165,    60,   129,    12,    13,
      93,    12,    13,    54,    78,    14,    62,    49,    14,   106,
      44,    80,    94,   178,   113,   116,   117,   118,   119,    82,
       1,     2,     3,     1,     2,     3,    22,   100,    82,     9,
     125,    63,     9,   140,   101,   102,   103,    12,    13,   132,
      12,    13,   156,   158,    14,    44,    49,    14,   143,    65,
     110,    49,    49,    49,    80,    49,   111,    67,   160,     6,
       7,     8,    66,   174,   161,   106,    22,   130,   138,    43,
     126,    44,   136,    76,    86,    87,    71,    72,    88,    89,
      68,    69,    70,    80,    80,    22,   166,   172,   167,   173,
     148,    90,    91,   152,   104,   105,   120,   121,    73,    77,
      82,    85,    80,    92,    96,   162,    99,   130,   130,   109,
      22,   112,   114,   127,   133,   128,    94,   134,   148,   137,
     139,   159,    22,   145,   151,   153,   168,   176,   130,    95,
     175,    53,    10,     1,     2,     3,     4,   179,     5,     6,
       7,     8,     9,   180,   115,   122,    10,   124,    11,   177,
      12,    13,   144,   135,   123,   171,     0,    14,     0,     0,
       0,     0,     0,     0,     0,    15,    16,    17,    18,     1,
       2,     3,     4,     0,     5,     6,     7,     8,     9,     0,
       0,     0,    10,     0,    11,     0,    12,    13,     0,     0,
       0,     0,     0,    14,     0,     0,     0,     0,     0,     0,
       0,    15,    16,    17,    18
};

static const yytype_int16 yycheck[] =
{
       9,     0,    15,    44,    20,     1,    44,    16,    12,    13,
      14,     3,     4,     5,     3,     4,     5,    12,    12,    14,
      12,    20,     0,    12,   154,   155,    18,    16,    20,    21,
      13,    20,    21,    17,    43,    27,    18,    46,    27,    34,
      14,    82,    25,   173,    82,    86,    87,    88,    89,    14,
       3,     4,     5,     3,     4,     5,    55,    66,    14,    12,
      34,    18,    12,    16,    68,    69,    70,    20,    21,    34,
      20,    21,   146,   147,    27,    14,    85,    27,    34,    95,
      13,    90,    91,    92,   125,    94,    19,    18,    13,     9,
      10,    11,    34,   167,    19,    34,    95,   106,   114,    12,
      96,    14,   111,    18,    28,    29,    20,    21,    32,    33,
      22,    23,    24,   154,   155,   114,    17,    17,    19,    19,
     133,    30,    31,   139,    71,    72,    90,    91,     3,    18,
      14,    12,   173,    26,     3,   151,    18,   146,   147,    19,
     139,    15,    13,    19,    12,    18,    25,     3,   161,    15,
      13,     3,   151,    17,     7,    17,    14,    14,   167,    55,
      15,     9,    16,     3,     4,     5,     6,   176,     8,     9,
      10,    11,    12,    15,    85,    92,    16,    17,    18,   170,
      20,    21,   127,   109,    94,   161,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    36,    37,    38,     3,
       4,     5,     6,    -1,     8,     9,    10,    11,    12,    -1,
      -1,    -1,    16,    -1,    18,    -1,    20,    21,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    37,    38
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     8,     9,    10,    11,    12,
      16,    18,    20,    21,    27,    35,    36,    37,    38,    41,
      42,    43,    44,    45,    46,    48,    49,    50,    51,    53,
      54,    55,    56,    58,    59,    60,    61,    67,    68,    69,
      77,    84,    90,    12,    14,    76,    12,    52,    44,    56,
      62,    63,    64,    65,    17,    47,    59,    59,    59,    67,
      18,    56,    18,    18,     0,    43,    34,    18,    22,    23,
      24,    20,    21,     3,    78,    79,    18,    18,    56,    91,
      61,    66,    14,    57,    65,    12,    28,    29,    32,    33,
      30,    31,    26,    13,    25,    42,     3,    70,    71,    18,
      56,    59,    59,    59,    60,    60,    34,    76,    85,    19,
      13,    19,    15,    66,    13,    57,    61,    61,    61,    61,
      62,    62,    63,    64,    17,    34,    76,    19,    18,    16,
      56,    81,    34,    12,     3,    79,    56,    15,    43,    13,
      16,    66,    73,    34,    71,    17,    82,    80,    67,    87,
      88,     7,    43,    17,    74,    72,    81,    83,    81,     3,
      13,    19,    43,    73,    75,    73,    17,    19,    14,    89,
      86,    88,    17,    19,    81,    15,    14,    46,    73,    56,
      15
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    40,    41,    42,    42,    43,    43,    43,    43,    43,
      43,    43,    43,    43,    43,    43,    44,    44,    45,    47,
      46,    46,    48,    49,    49,    50,    50,    52,    51,    53,
      54,    55,    56,    57,    58,    58,    58,    58,    58,    59,
      59,    59,    59,    60,    60,    60,    60,    61,    61,    61,
      62,    62,    62,    62,    62,    63,    63,    63,    64,    64,
      65,    65,    66,    67,    67,    67,    68,    68,    69,    70,
      70,    71,    72,    71,    73,    73,    74,    73,    75,    75,
      76,    76,    77,    78,    78,    79,    79,    79,    80,    79,
      81,    81,    82,    81,    83,    83,    85,    86,    84,    87,
      87,    87,    88,    88,    89,    89,    90,    91,    91,    91
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     4,     0,
       4,     2,     1,     5,     7,     3,     2,     0,     6,     2,
       2,     2,     1,     1,     3,     1,     1,     1,     1,     1,
       2,     2,     2,     1,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       1,     3,     1,     1,     1,     1,     2,     2,     3,     3,
       1,     3,     0,     5,     1,     2,     0,     4,     1,     3,
       3,     4,     2,     3,     1,     1,     3,     2,     0,     5,
       1,     2,     0,     4,     1,     3,     0,     0,     8,     3,
       1,     0,     2,     3,     2,     4,     4,     3,     1,     0
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
  case 2: /* Program: Stmts  */
#line 71 "src/parser.y"
            {
        ast.setRoot((yyvsp[0].stmttype));
    }
#line 1329 "src/parser.cpp"
    break;

  case 3: /* Stmts: Stmt  */
#line 76 "src/parser.y"
           {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1335 "src/parser.cpp"
    break;

  case 4: /* Stmts: Stmts Stmt  */
#line 77 "src/parser.y"
                {
        (yyval.stmttype) = new SeqNode((yyvsp[-1].stmttype), (yyvsp[0].stmttype));
    }
#line 1343 "src/parser.cpp"
    break;

  case 5: /* Stmt: AssignStmt  */
#line 82 "src/parser.y"
                 { (yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1349 "src/parser.cpp"
    break;

  case 6: /* Stmt: BlockStmt  */
#line 83 "src/parser.y"
                {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1355 "src/parser.cpp"
    break;

  case 7: /* Stmt: IfStmt  */
#line 84 "src/parser.y"
             {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1361 "src/parser.cpp"
    break;

  case 8: /* Stmt: ReturnStmt  */
#line 85 "src/parser.y"
                 {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1367 "src/parser.cpp"
    break;

  case 9: /* Stmt: DeclStmt  */
#line 86 "src/parser.y"
               {(yyval.stmttype)=(yyvsp[0].stmttype); }
#line 1373 "src/parser.cpp"
    break;

  case 10: /* Stmt: FuncDef  */
#line 87 "src/parser.y"
              {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1379 "src/parser.cpp"
    break;

  case 11: /* Stmt: WhileStmt  */
#line 88 "src/parser.y"
                {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1385 "src/parser.cpp"
    break;

  case 12: /* Stmt: BrkStmt  */
#line 89 "src/parser.y"
              {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1391 "src/parser.cpp"
    break;

  case 13: /* Stmt: CtnueStmt  */
#line 90 "src/parser.y"
                {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1397 "src/parser.cpp"
    break;

  case 14: /* Stmt: BlankStmt  */
#line 91 "src/parser.y"
                {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1403 "src/parser.cpp"
    break;

  case 15: /* Stmt: ExprStmt  */
#line 92 "src/parser.y"
               {(yyval.stmttype)=(yyvsp[0].stmttype);}
#line 1409 "src/parser.cpp"
    break;

  case 16: /* LVal: ID  */
#line 95 "src/parser.y"
         {  
        SymbolEntry *se;
        se = identifiers->lookup((yyvsp[0].strtype));
        if(se == nullptr)
        {
            fprintf(stderr, "identifier %s is undeclared\n", (char*)(yyvsp[0].strtype));
            delete [](char*)(yyvsp[0].strtype);
            exit(EXIT_FAILURE); //报错退出
        }
        (yyval.exprtype) = new Id(se);
        delete [](yyvsp[0].strtype);
    }
#line 1426 "src/parser.cpp"
    break;

  case 17: /* LVal: ID ArrayIndex  */
#line 107 "src/parser.y"
                   {
        SymbolEntry* se;
        se = identifiers->lookup((yyvsp[-1].strtype));
        if(se == nullptr)
        {
            fprintf(stderr, "identifier %s is undeclared\n", (char*)(yyvsp[-1].strtype));
            delete [](char*)(yyvsp[-1].strtype);
            exit(EXIT_FAILURE); 
        }
        //找到了对应的符号表项
        (yyval.exprtype) = new Id(se, (yyvsp[0].exprtype));
        delete [](yyvsp[-1].strtype);
    }
#line 1444 "src/parser.cpp"
    break;

  case 18: /* AssignStmt: LVal ASSIGN Exp SEMICOLON  */
#line 123 "src/parser.y"
                              {
        (yyval.stmttype) = new AssignStmt((yyvsp[-3].exprtype), (yyvsp[-1].exprtype));
    }
#line 1452 "src/parser.cpp"
    break;

  case 19: /* $@1: %empty  */
#line 129 "src/parser.y"
        {identifiers = new SymbolTable(identifiers);}
#line 1458 "src/parser.cpp"
    break;

  case 20: /* BlockStmt: LBRACE $@1 Stmts RBRACE  */
#line 131 "src/parser.y"
        {
            //遇见左大括号时，创建新的作用域，其中的语句中涉及的变量作用域都会随之更新，直至遇见右大括号，恢复原来的作用域
            (yyval.stmttype) = new CompoundStmt((yyvsp[-1].stmttype));
            SymbolTable *top = identifiers;
            identifiers = identifiers->getPrev();
            delete top;
        }
#line 1470 "src/parser.cpp"
    break;

  case 21: /* BlockStmt: LBRACE RBRACE  */
#line 138 "src/parser.y"
                         {
            (yyval.stmttype)=new CompoundStmt();
            //
            if_FuncDefStmt_IsNull=1;
        }
#line 1480 "src/parser.cpp"
    break;

  case 22: /* BlankStmt: SEMICOLON  */
#line 145 "src/parser.y"
                {
        (yyval.stmttype) = new BlankStmt();
    }
#line 1488 "src/parser.cpp"
    break;

  case 23: /* IfStmt: IF LPAREN Cond RPAREN Stmt  */
#line 150 "src/parser.y"
                                            {
        (yyval.stmttype) = new IfStmt((yyvsp[-2].exprtype), (yyvsp[0].stmttype));
    }
#line 1496 "src/parser.cpp"
    break;

  case 24: /* IfStmt: IF LPAREN Cond RPAREN Stmt ELSE Stmt  */
#line 153 "src/parser.y"
                                           {
        (yyval.stmttype) = new IfElseStmt((yyvsp[-4].exprtype), (yyvsp[-2].stmttype), (yyvsp[0].stmttype));
    }
#line 1504 "src/parser.cpp"
    break;

  case 25: /* ReturnStmt: RETURN Exp SEMICOLON  */
#line 158 "src/parser.y"
                         {
        functionhasRet=true;
        (yyval.stmttype) = new ReturnStmt((yyvsp[-1].exprtype));
    }
#line 1513 "src/parser.cpp"
    break;

  case 26: /* ReturnStmt: RETURN SEMICOLON  */
#line 162 "src/parser.y"
                       {
        functionhasRet=true;
        (yyval.stmttype) = new ReturnStmt();
    }
#line 1522 "src/parser.cpp"
    break;

  case 27: /* $@2: %empty  */
#line 169 "src/parser.y"
    {
        //这个地方涉及到两个动态数组，一个是newWhileStmtPos，int数组，一个是loopctrl，loopctl的数组
        //核心还是loopctrl数组，newWhileStmtPos每一次代表了一个新的循环开始，从当前的loopctrl.size()之后的break和continue语句都是属于这个while的
        //采用动态数组的方式进行控制是因为考虑到有while循环嵌套的问题
        whilenum++;
        new_WhileStmt_Pos.push_back(loop_ctrl.size());
    }
#line 1534 "src/parser.cpp"
    break;

  case 28: /* WhileStmt: WHILE $@2 LPAREN Cond RPAREN Stmt  */
#line 176 "src/parser.y"
                             {
        (yyval.stmttype) = new WhileStmt((yyvsp[-2].exprtype), (yyvsp[0].stmttype));
        int lastloop=new_WhileStmt_Pos[new_WhileStmt_Pos.size()-1];
        new_WhileStmt_Pos.erase(new_WhileStmt_Pos.end()-1);
        for(int i=lastloop;i<(int)loop_ctrl.size();i++)
        {
            loop_ctrl[i]->setWhileStmt(dynamic_cast<WhileStmt*>((yyval.stmttype)));
        }
        loop_ctrl.erase(loop_ctrl.begin()+lastloop,loop_ctrl.end());
        whilenum--;
    }
#line 1550 "src/parser.cpp"
    break;

  case 29: /* BrkStmt: BREAK SEMICOLON  */
#line 189 "src/parser.y"
                      {
        (yyval.stmttype) = new BreakStmt();
        loop_ctrl.push_back(dynamic_cast<loopctl*>((yyval.stmttype)));
        if(whilenum==0)
        {
            fprintf(stderr, "BrkStmt not in WhileStmt！\n");
            exit(EXIT_FAILURE);
        }

    }
#line 1565 "src/parser.cpp"
    break;

  case 30: /* CtnueStmt: CONTINUE SEMICOLON  */
#line 201 "src/parser.y"
                         {
        (yyval.stmttype) = new ContinueStmt();
        loop_ctrl.push_back(dynamic_cast<loopctl*>((yyval.stmttype)));
        if(whilenum==0)
        {
            fprintf(stderr, "CtnueStmt not in WhileStmt！\n");
            exit(EXIT_FAILURE); 
        }
    }
#line 1579 "src/parser.cpp"
    break;

  case 31: /* ExprStmt: Exp SEMICOLON  */
#line 212 "src/parser.y"
                    {
        (yyval.stmttype) = new ExprStmt((yyvsp[-1].exprtype));
    }
#line 1587 "src/parser.cpp"
    break;

  case 32: /* Exp: AddExp  */
#line 217 "src/parser.y"
            {(yyval.exprtype) = (yyvsp[0].exprtype);}
#line 1593 "src/parser.cpp"
    break;

  case 33: /* Cond: LOrExp  */
#line 221 "src/parser.y"
           {(yyval.exprtype) = (yyvsp[0].exprtype);}
#line 1599 "src/parser.cpp"
    break;

  case 34: /* PrimaryExp: LPAREN LOrExp RPAREN  */
#line 224 "src/parser.y"
                          { 
        (yyval.exprtype) = (yyvsp[-1].exprtype);
    }
#line 1607 "src/parser.cpp"
    break;

  case 35: /* PrimaryExp: LVal  */
#line 227 "src/parser.y"
           {
        (yyval.exprtype) = (yyvsp[0].exprtype);
    }
#line 1615 "src/parser.cpp"
    break;

  case 36: /* PrimaryExp: INTEGER  */
#line 230 "src/parser.y"
              {
        SymbolEntry* se = new ConstantSymbolEntry(TypeSystem::intType, (yyvsp[0].itype));    
        (yyval.exprtype) = new Constant(se);
    }
#line 1624 "src/parser.cpp"
    break;

  case 37: /* PrimaryExp: FLOATNUMBER  */
#line 234 "src/parser.y"
                  {
        SymbolEntry *se = new ConstantSymbolEntry(TypeSystem::floatType, (yyvsp[0].ftype)); 
        (yyval.exprtype) = new Constant(se);
    }
#line 1633 "src/parser.cpp"
    break;

  case 38: /* PrimaryExp: FuncCall  */
#line 238 "src/parser.y"
               {(yyval.exprtype)=(yyvsp[0].exprtype);}
#line 1639 "src/parser.cpp"
    break;

  case 39: /* UnaryExp: PrimaryExp  */
#line 241 "src/parser.y"
                 {(yyval.exprtype) = (yyvsp[0].exprtype);}
#line 1645 "src/parser.cpp"
    break;

  case 40: /* UnaryExp: ADD UnaryExp  */
#line 242 "src/parser.y"
                   {
        (yyval.exprtype)=(yyvsp[0].exprtype);
    }
#line 1653 "src/parser.cpp"
    break;

  case 41: /* UnaryExp: SUB UnaryExp  */
#line 245 "src/parser.y"
                   {
        SymbolEntry* se=nullptr;
        if((yyvsp[0].exprtype)->getSymPtr()->getType()->isIndirectFloat())
        {
            se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());
        }
        else if(((yyvsp[0].exprtype)->getSymPtr()->getType()->isIndirectInt())||((yyvsp[0].exprtype)->getSymPtr()->getType()->isBool()))
        {
            se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        }

        (yyval.exprtype) = new UnaryExpr(se, UnaryExpr::SUB, (yyvsp[0].exprtype));
    }
#line 1671 "src/parser.cpp"
    break;

  case 42: /* UnaryExp: NOT UnaryExp  */
#line 258 "src/parser.y"
                   {
        SymbolEntry* se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new UnaryExpr(se, UnaryExpr::NOT, (yyvsp[0].exprtype));
    }
#line 1680 "src/parser.cpp"
    break;

  case 43: /* MulExp: UnaryExp  */
#line 264 "src/parser.y"
               {(yyval.exprtype) = (yyvsp[0].exprtype);}
#line 1686 "src/parser.cpp"
    break;

  case 44: /* MulExp: MulExp MUL UnaryExp  */
#line 265 "src/parser.y"
                          {
        SymbolEntry* se;
        se= new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::MUL, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1696 "src/parser.cpp"
    break;

  case 45: /* MulExp: MulExp DIV UnaryExp  */
#line 270 "src/parser.y"
                          {
        SymbolEntry* se;
        se= new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::DIV, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1706 "src/parser.cpp"
    break;

  case 46: /* MulExp: MulExp MOD UnaryExp  */
#line 275 "src/parser.y"
                          {
        SymbolEntry* se;
        se= new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::MOD, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1716 "src/parser.cpp"
    break;

  case 47: /* AddExp: MulExp  */
#line 283 "src/parser.y"
           {(yyval.exprtype) = (yyvsp[0].exprtype);}
#line 1722 "src/parser.cpp"
    break;

  case 48: /* AddExp: AddExp ADD MulExp  */
#line 286 "src/parser.y"
    {
        SymbolEntry* se;
        se= new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::ADD, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1732 "src/parser.cpp"
    break;

  case 49: /* AddExp: AddExp SUB MulExp  */
#line 293 "src/parser.y"
    {
        SymbolEntry* se;
        se= new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());     
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::SUB, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1742 "src/parser.cpp"
    break;

  case 50: /* RelExp: Exp  */
#line 300 "src/parser.y"
          {(yyval.exprtype) = (yyvsp[0].exprtype);}
#line 1748 "src/parser.cpp"
    break;

  case 51: /* RelExp: RelExp LESS AddExp  */
#line 301 "src/parser.y"
                         {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::LT, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1757 "src/parser.cpp"
    break;

  case 52: /* RelExp: RelExp GREAT AddExp  */
#line 305 "src/parser.y"
                          {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::GT, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1766 "src/parser.cpp"
    break;

  case 53: /* RelExp: RelExp LE AddExp  */
#line 309 "src/parser.y"
                       {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::LE, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1775 "src/parser.cpp"
    break;

  case 54: /* RelExp: RelExp GE AddExp  */
#line 313 "src/parser.y"
                       {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::GE, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1784 "src/parser.cpp"
    break;

  case 55: /* EqExp: RelExp  */
#line 319 "src/parser.y"
             {(yyval.exprtype) = (yyvsp[0].exprtype);}
#line 1790 "src/parser.cpp"
    break;

  case 56: /* EqExp: EqExp EQ RelExp  */
#line 320 "src/parser.y"
                      {
        SymbolEntry* se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::EQ, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1799 "src/parser.cpp"
    break;

  case 57: /* EqExp: EqExp NE RelExp  */
#line 324 "src/parser.y"
                      {
        SymbolEntry* se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::NE, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1808 "src/parser.cpp"
    break;

  case 58: /* LAndExp: EqExp  */
#line 330 "src/parser.y"
            {(yyval.exprtype) = (yyvsp[0].exprtype);}
#line 1814 "src/parser.cpp"
    break;

  case 59: /* LAndExp: LAndExp AND EqExp  */
#line 332 "src/parser.y"
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::AND, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1823 "src/parser.cpp"
    break;

  case 60: /* LOrExp: LAndExp  */
#line 338 "src/parser.y"
              {(yyval.exprtype) = (yyvsp[0].exprtype);}
#line 1829 "src/parser.cpp"
    break;

  case 61: /* LOrExp: LOrExp OR LAndExp  */
#line 340 "src/parser.y"
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        (yyval.exprtype) = new BinaryExpr(se, BinaryExpr::OR, (yyvsp[-2].exprtype), (yyvsp[0].exprtype));
    }
#line 1838 "src/parser.cpp"
    break;

  case 62: /* ConstExp: AddExp  */
#line 346 "src/parser.y"
             {(yyval.exprtype) = (yyvsp[0].exprtype);}
#line 1844 "src/parser.cpp"
    break;

  case 63: /* Type: INT  */
#line 349 "src/parser.y"
          {
        tempType="INT";
        (yyval.type) = TypeSystem::intType;
    }
#line 1853 "src/parser.cpp"
    break;

  case 64: /* Type: FLOAT  */
#line 353 "src/parser.y"
            {
        tempType="FLOAT";
        (yyval.type) = TypeSystem::floatType;
    }
#line 1862 "src/parser.cpp"
    break;

  case 65: /* Type: VOID  */
#line 357 "src/parser.y"
           {
        (yyval.type) = TypeSystem::voidType;
    }
#line 1870 "src/parser.cpp"
    break;

  case 66: /* DeclStmt: ConstDecl SEMICOLON  */
#line 362 "src/parser.y"
                          {(yyval.stmttype)=(yyvsp[-1].stmttype);}
#line 1876 "src/parser.cpp"
    break;

  case 67: /* DeclStmt: VarDecl SEMICOLON  */
#line 363 "src/parser.y"
                        {(yyval.stmttype)=(yyvsp[-1].stmttype);}
#line 1882 "src/parser.cpp"
    break;

  case 68: /* ConstDecl: CONST Type ConstDefList  */
#line 366 "src/parser.y"
                              {(yyval.stmttype) = (yyvsp[0].stmttype);}
#line 1888 "src/parser.cpp"
    break;

  case 69: /* ConstDefList: ConstDefList COMM ConstDef  */
#line 369 "src/parser.y"
                                 {
        (yyval.stmttype) = (yyvsp[-2].stmttype);
        (yyvsp[-2].stmttype)->setNext((yyvsp[0].stmttype));    //通过next指针将不同的节点串联起来，暂时只用于变量批量声明时，在同一级打印出各个声明语句
    }
#line 1897 "src/parser.cpp"
    break;

  case 70: /* ConstDefList: ConstDef  */
#line 373 "src/parser.y"
               {(yyval.stmttype) = (yyvsp[0].stmttype);}
#line 1903 "src/parser.cpp"
    break;

  case 71: /* ConstDef: ID ASSIGN ConstInitVal  */
#line 376 "src/parser.y"
                             {
        SymbolEntry *se;
        if(tempType=="INT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::intconstType, (yyvsp[-2].strtype), identifiers->getLevel());
            dynamic_cast<IdentifierSymbolEntry*>(se)->setValue((int)((yyvsp[0].exprtype)->getValue()));
        }
        if(tempType=="FLOAT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::floatconstType, (yyvsp[-2].strtype), identifiers->getLevel());
            dynamic_cast<IdentifierSymbolEntry*>(se)->setValue((yyvsp[0].exprtype)->getValue());
        }
        if(!identifiers->install1((yyvsp[-2].strtype), se))
        {
            fprintf(stderr, "ConstID %s is already defined\n", (char*)(yyvsp[-2].strtype));
            exit(EXIT_FAILURE);
        }
        (yyval.stmttype) = new DeclStmt(new Id(se),(yyvsp[0].exprtype));
        delete [](yyvsp[-2].strtype);
    }
#line 1928 "src/parser.cpp"
    break;

  case 72: /* @3: %empty  */
#line 396 "src/parser.y"
                           {
        //nums中存储了从最高维开始每一维的长度，const int a[5][2][7]=>nums:5,2,7
        std::vector<int> nums;
        ExprNode* eachdimension = (yyvsp[-1].exprtype);
        while(eachdimension)
        {
            nums.push_back(eachdimension->getValue());
            eachdimension = (ExprNode*)(eachdimension->getNext());
        }
        Type* type;
        Type* temp1;
        if(tempType=="INT")  type = TypeSystem::intconstType;
        if(tempType=="FLOAT") type = TypeSystem::floatconstType;
        //形成了一个ArrayType的链，互相之间以artype和parent保持联系
        for(auto it = nums.rbegin(); it != nums.rend(); it++) 
        {
            temp1 = new ArrayType(type, *it);
            if(type->isArray())
            {
                ((ArrayType*)type)->setParentType(temp1);
            }
            type = temp1;
        }
        //arraytype，当前的最顶层的ArrayType
        arraytype = (ArrayType*)type;

        myindex = 0;
        std::stack<Initlist*>().swap(initstack);
        SymbolEntry* se = new IdentifierSymbolEntry(arraytype, (yyvsp[-2].strtype), identifiers->getLevel());
        (yyval.se) = se;

        //这里的size是已经计算了整型或者浮点的位数了，const int a[5]=>equalintarray=5*32=160;
        int len_of_array=arraytype->getArraySize()/32;
        if(tempType=="INT")  equalintarray =new int[len_of_array];
        if(tempType=="FLOAT") equalfloatarray =new float[len_of_array];
    }
#line 1969 "src/parser.cpp"
    break;

  case 73: /* ConstDef: ID ArrayIndex ASSIGN @3 ConstInitVal  */
#line 432 "src/parser.y"
                {
        if(tempType=="INT")   ((IdentifierSymbolEntry*)(yyvsp[-1].se))->setIntArrayValue(equalintarray);
        if(tempType=="FLOAT")  ((IdentifierSymbolEntry*)(yyvsp[-1].se))->setFloatArrayValue(equalfloatarray);  
        identifiers->install((yyvsp[-4].strtype), (yyvsp[-1].se));
        (yyval.stmttype) = new DeclStmt(new Id((yyvsp[-1].se),(yyvsp[-3].exprtype)), (yyvsp[0].exprtype));
        delete [](yyvsp[-4].strtype);
    }
#line 1981 "src/parser.cpp"
    break;

  case 74: /* ConstInitVal: ConstExp  */
#line 441 "src/parser.y"
               {
        (yyval.exprtype) = (yyvsp[0].exprtype);
        //initstack为空，是声明一个常量，不为空，是声明一个常量数组
        if(!initstack.empty())
        {
            if(tempType=="INT")
            {
                equalintarray[myindex++] =(int)((yyvsp[0].exprtype)->getValue());
                Type* artp = initstack.top()->getSymPtr()->getType();
                if(artp == TypeSystem::intconstType)
                {
                    initstack.top()->AddExp((yyvsp[0].exprtype));
                }
                else
                {
                    while(artp)
                    {
                        if(((ArrayType*)artp)->getArrayType() != TypeSystem::intconstType)
                        {
                            //int a[2][4]={1,2,3,4,5,6,7,8}
                            artp = ((ArrayType*)artp)->getArrayType();
                            SymbolEntry* se = new ConstantSymbolEntry(artp);
                            Initlist* initlist = new Initlist(se);
                            initstack.top()->AddExp(initlist);
                            initstack.push(initlist);
                        }
                        else
                        {
                            initstack.top()->AddExp((yyvsp[0].exprtype));
                            while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                            {
                                artp = ((ArrayType*)artp)->getParentType();
                                initstack.pop();
                            }
                            break;
                        }
                    }
                }    
            } 
            if(tempType=="FLOAT")
            {
                equalfloatarray[myindex++] =(yyvsp[0].exprtype)->getValue();
                Type* artp = initstack.top()->getSymPtr()->getType();
                if(artp == TypeSystem::floatconstType)
                {
                    initstack.top()->AddExp((yyvsp[0].exprtype));
                }
                else
                {
                    while(artp)
                    {
                        if(((ArrayType*)artp)->getArrayType() != TypeSystem::floatconstType)
                        {
                            artp = ((ArrayType*)artp)->getArrayType();
                            SymbolEntry* se = new ConstantSymbolEntry(artp);
                            Initlist* initlist = new Initlist(se);
                            initstack.top()->AddExp(initlist);
                            initstack.push(initlist);
                        }
                        else
                        {
                            initstack.top()->AddExp((yyvsp[0].exprtype));
                            while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                            {
                                artp = ((ArrayType*)artp)->getParentType();
                                initstack.pop();
                            }
                            break;
                        }
                    }
                }
            } 
        }   
    }
#line 2060 "src/parser.cpp"
    break;

  case 75: /* ConstInitVal: LBRACE RBRACE  */
#line 515 "src/parser.y"
                    {
        if(tempType=="INT")
        {
            SymbolEntry* se;
            ExprNode* initlist;
            if(initstack.empty())
            {
                // 如果只用一个{}初始化数组，那么栈一定为空
                // 此时也没必要再加入栈了
                int num=arraytype->getArraySize()/32;
                memset(equalintarray, 0, num*4);
                myindex += num;
                se = new ConstantSymbolEntry(arraytype);
                initlist = new Initlist(se);
            }
            else
            {
                // 栈不空说明肯定不是只有{}
                // 此时需要确定{}到底占了几个元素
                int size = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArraySize()/32;
                memset(equalintarray + myindex, 0, size*4);
                myindex += size;
                Type* type = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType();
                se = new ConstantSymbolEntry(type);
                initlist = new Initlist(se);
                initstack.top()->AddExp(initlist);
                while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                {
                    initstack.pop();
                }
            }
            (yyval.exprtype) = initlist;
        }

        if(tempType=="FLOAT"){
            SymbolEntry* se;
            ExprNode* initlist;
            if(initstack.empty()){
                // 如果只用一个{}初始化数组，那么栈一定为空
                // 此时也没必要再加入栈了
                memset(equalfloatarray, 0, arraytype->getArraySize());
                myindex += arraytype->getArraySize()/32;
                se = new ConstantSymbolEntry(arraytype);
                initlist = new Initlist(se);
            }else{
                // 栈不空说明肯定不是只有{}
                // 此时需要确定{}到底占了几个元素
                int size = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArraySize();
                int len = size / ((FloatType *)TypeSystem::floatconstType)->getSize();
                memset(equalfloatarray + myindex, 0, size);
                myindex += len;
                Type* type = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType();
                se = new ConstantSymbolEntry(type);
                initlist = new Initlist(se);
                initstack.top()->AddExp(initlist);
                while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum){
                    initstack.pop();
                }
            }
            (yyval.exprtype) = initlist;
        }
        
    }
#line 2128 "src/parser.cpp"
    break;

  case 76: /* @4: %empty  */
#line 578 "src/parser.y"
             {
        //bracenum是用来确认当前是第几层大括号
        bracenum++;
        //第一层的时候，这个栈必定是空的
        if(!initstack.empty())
        {
            arraytype = (ArrayType*)(((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType());
        }
        //如果不经过上面的分支，即第一次进入这里，arraytype还是当前最顶层的数组类型
        SymbolEntry* se= new ConstantSymbolEntry(arraytype);
        Initlist* expr = new Initlist(se);
        if(!initstack.empty())
            initstack.top()->AddExp(expr);
        initstack.push(expr);
        (yyval.exprtype) = expr;
    }
#line 2149 "src/parser.cpp"
    break;

  case 77: /* ConstInitVal: LBRACE @4 ConstInitValList RBRACE  */
#line 594 "src/parser.y"
                              {
        bracenum--;
        while(initstack.top() != (yyvsp[-2].exprtype) && initstack.size() > (long unsigned int)(bracenum + 1))
        {
            initstack.pop();
        }
        if(initstack.top() == (yyvsp[-2].exprtype))
        {
            initstack.pop();
        }
        (yyval.exprtype) = (yyvsp[-2].exprtype);
        if(!initstack.empty())
        {
            while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
            {
                initstack.pop();
            }
        }
        
        if(tempType=="INT")
        {
            int size=((ArrayType*)((yyval.exprtype)->getSymPtr()->getType()))->getArraySize();
            while(myindex % (size/32) !=0 )
                equalintarray[myindex++] = 0;
        }
        if(tempType=="FLOAT")
        {
            while(myindex % (((ArrayType*)((yyval.exprtype)->getSymPtr()->getType()))->getArraySize()) !=0 )
                equalfloatarray[myindex++] = 0.0;
        }
        if(!initstack.empty())
            arraytype = (ArrayType*)(((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType());
    }
#line 2187 "src/parser.cpp"
    break;

  case 78: /* ConstInitValList: ConstInitVal  */
#line 629 "src/parser.y"
                   {
        (yyval.exprtype) = (yyvsp[0].exprtype);
    }
#line 2195 "src/parser.cpp"
    break;

  case 79: /* ConstInitValList: ConstInitValList COMM ConstInitVal  */
#line 632 "src/parser.y"
                                         {
        (yyval.exprtype) = (yyvsp[-2].exprtype);
    }
#line 2203 "src/parser.cpp"
    break;

  case 80: /* ArrayIndex: LBRACK ConstExp RBRACK  */
#line 637 "src/parser.y"
                             {
        (yyval.exprtype) = (yyvsp[-1].exprtype);
    }
#line 2211 "src/parser.cpp"
    break;

  case 81: /* ArrayIndex: ArrayIndex LBRACK ConstExp RBRACK  */
#line 640 "src/parser.y"
                                        {
        (yyval.exprtype) = (yyvsp[-3].exprtype);
        (yyvsp[-3].exprtype)->setNext((yyvsp[-1].exprtype));//把每个维度串起来
    }
#line 2220 "src/parser.cpp"
    break;

  case 82: /* VarDecl: Type VarDefList  */
#line 646 "src/parser.y"
                      {(yyval.stmttype) = (yyvsp[0].stmttype);}
#line 2226 "src/parser.cpp"
    break;

  case 83: /* VarDefList: VarDefList COMM VarDef  */
#line 649 "src/parser.y"
                             {
        (yyval.stmttype) = (yyvsp[-2].stmttype);
        (yyvsp[-2].stmttype)->setNext((yyvsp[0].stmttype));
    }
#line 2235 "src/parser.cpp"
    break;

  case 84: /* VarDefList: VarDef  */
#line 653 "src/parser.y"
             { (yyval.stmttype) = (yyvsp[0].stmttype);}
#line 2241 "src/parser.cpp"
    break;

  case 85: /* VarDef: ID  */
#line 656 "src/parser.y"
         {
        SymbolEntry *se;
        if(tempType=="INT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::intType, (yyvsp[0].strtype), identifiers->getLevel());
        }
         if(tempType=="FLOAT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::floatType, (yyvsp[0].strtype), identifiers->getLevel());
        }   
        if(!identifiers->install1((yyvsp[0].strtype), se))
        {
            fprintf(stderr, "VarID %s is already defined\n", (char*)(yyvsp[0].strtype));
            exit(EXIT_FAILURE);
        }
        (yyval.stmttype) = new DeclStmt(new Id(se));
        delete [](yyvsp[0].strtype);
    }
#line 2264 "src/parser.cpp"
    break;

  case 86: /* VarDef: ID ASSIGN VarInitVal  */
#line 674 "src/parser.y"
                           {
        SymbolEntry *se;
        if(tempType=="INT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::intType, (yyvsp[-2].strtype), identifiers->getLevel());
            dynamic_cast<IdentifierSymbolEntry*>(se)->setValue((int)((yyvsp[0].exprtype)->getValue()));
        }
        if(tempType=="FLOAT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::floatType, (yyvsp[-2].strtype), identifiers->getLevel());
            dynamic_cast<IdentifierSymbolEntry*>(se)->setValue((yyvsp[0].exprtype)->getValue());   
        }
        if(!identifiers->install1((yyvsp[-2].strtype), se))
        {
            fprintf(stderr, "VarID %s is already defined\n", (char*)(yyvsp[-2].strtype));
            exit(EXIT_FAILURE);
        }
        (yyval.stmttype) = new DeclStmt(new Id(se),(yyvsp[0].exprtype));
        delete [](yyvsp[-2].strtype);
    }
#line 2289 "src/parser.cpp"
    break;

  case 87: /* VarDef: ID ArrayIndex  */
#line 694 "src/parser.y"
                    {
        std::vector<int> nums;//存储各维大小
        ExprNode* eachdimension = (yyvsp[0].exprtype);
        while(eachdimension)
        {
            nums.push_back(eachdimension->getValue());
            eachdimension = (ExprNode*)(eachdimension->getNext());
        }
        Type *type;
        Type* temp1;
        if(tempType=="INT")
        {
            type = TypeSystem::intType;
        }  
        if(tempType=="FLOAT") 
        {
             type = TypeSystem::floatType;
        }
        for(auto it = nums.rbegin(); it != nums.rend(); it++) 
        {
            temp1 = new ArrayType(type, *it);
            if(type->isArray())
            {
                ((ArrayType*)type)->setParentType(temp1);
            }
            type = temp1;
        }
        arraytype = (ArrayType*)type;
        SymbolEntry* se = new IdentifierSymbolEntry(arraytype, (yyvsp[-1].strtype), identifiers->getLevel());
        ((IdentifierSymbolEntry*)se)->setquan0();
        identifiers->install((yyvsp[-1].strtype), se);
        (yyval.stmttype) = new DeclStmt(new Id(se,(yyvsp[0].exprtype)));
        delete [](yyvsp[-1].strtype);

    }
#line 2329 "src/parser.cpp"
    break;

  case 88: /* @5: %empty  */
#line 729 "src/parser.y"
                           {
        std::vector<int> nums;
        ExprNode* eachdimension = (yyvsp[-1].exprtype);
        while(eachdimension)
        {
            nums.push_back(eachdimension->getValue());
            eachdimension = (ExprNode*)(eachdimension->getNext());
        }
        Type* type;
        Type* temp1;
        //确定基础类型是int还是float
        if(tempType=="INT") type = TypeSystem::intType; 
        if(tempType=="FLOAT") type = TypeSystem::floatType;

        for(auto it = nums.rbegin(); it != nums.rend(); it++) 
        {
            temp1 = new ArrayType(type, *it);
            if(type->isArray())
            {
                ((ArrayType*)type)->setParentType(temp1);
            }
            type = temp1;
        }
        arraytype = (ArrayType*)type;
        myindex = 0;
        std::stack<Initlist*>().swap(initstack);
        SymbolEntry* se = new IdentifierSymbolEntry(arraytype, (yyvsp[-2].strtype), identifiers->getLevel());
        (yyval.se) = se;
        int len_of_array=arraytype->getArraySize()/32;
        if(tempType=="INT")  equalintarray =new int[len_of_array];
        if(tempType=="FLOAT") equalfloatarray =new float[len_of_array];
        
    }
#line 2367 "src/parser.cpp"
    break;

  case 89: /* VarDef: ID ArrayIndex ASSIGN @5 VarInitVal  */
#line 762 "src/parser.y"
                { 
        if(tempType=="INT")
        {
            ((IdentifierSymbolEntry*)(yyvsp[-1].se))->setIntArrayValue(equalintarray);
        }
        if(tempType=="FLOAT")
        {
            ((IdentifierSymbolEntry*)(yyvsp[-1].se))->setFloatArrayValue(equalfloatarray);  
        }  
        if(((Initlist*)(yyvsp[0].exprtype))->ifempty())
            ((IdentifierSymbolEntry*)(yyvsp[-1].se))->setquan0();
        identifiers->install((yyvsp[-4].strtype), (yyvsp[-1].se));
        (yyval.stmttype) = new DeclStmt(new Id((yyvsp[-1].se),(yyvsp[-3].exprtype)), (yyvsp[0].exprtype));
        delete [](yyvsp[-4].strtype);
    }
#line 2387 "src/parser.cpp"
    break;

  case 90: /* VarInitVal: Exp  */
#line 779 "src/parser.y"
          {  
        (yyval.exprtype) = (yyvsp[0].exprtype);
        //initstack栈不为空，说明当前是一个数组
        if(!initstack.empty())
        {
            if(tempType=="INT")
            {
                equalintarray[myindex++] =(int)((yyvsp[0].exprtype)->getValue());
                Type* artp = initstack.top()->getSymPtr()->getType();
                while(artp)
                {
                    if(((ArrayType*)artp)->getArrayType() != TypeSystem::intType)
                    {
                        artp = ((ArrayType*)artp)->getArrayType();
                        SymbolEntry* se = new ConstantSymbolEntry(artp);
                        Initlist* initlist = new Initlist(se);
                        initstack.top()->AddExp(initlist);
                        initstack.push(initlist);
                    }
                    else
                    {
                        initstack.top()->AddExp((yyvsp[0].exprtype));
                        while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                        {
                            artp = ((ArrayType*)artp)->getParentType();
                            initstack.pop();
                        }
                        break;
                    }
                }    
            } 

            if(tempType=="FLOAT"){
                equalfloatarray[myindex++] =(yyvsp[0].exprtype)->getValue();
                Type* artp = initstack.top()->getSymPtr()->getType();
                if(artp == TypeSystem::floatType)
                {
                    initstack.top()->AddExp((yyvsp[0].exprtype));
                }
                else
                {
                    while(artp)
                    {
                        if(((ArrayType*)artp)->getArrayType() != TypeSystem::floatType)
                        {
                            artp = ((ArrayType*)artp)->getArrayType();
                            SymbolEntry* se = new ConstantSymbolEntry(artp);
                            Initlist* initlist = new Initlist(se);
                            initstack.top()->AddExp(initlist);
                            initstack.push(initlist);
                        }
                        else
                        {
                            initstack.top()->AddExp((yyvsp[0].exprtype));
                            while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                            {
                                artp = ((ArrayType*)artp)->getParentType();
                                initstack.pop();
                            }
                            break;
                        }
                    }
                }
                    
            } 
        }       
    }
#line 2459 "src/parser.cpp"
    break;

  case 91: /* VarInitVal: LBRACE RBRACE  */
#line 846 "src/parser.y"
                    {
        if(tempType=="INT")
        { 
            SymbolEntry* se;
            ExprNode* initlist;
            if(initstack.empty())
            {
                // 如果只用一个{}初始化数组，那么栈一定为空
                // 此时也没必要再加入栈了
                int num=arraytype->getArraySize()/32;
                memset(equalintarray, 0, num*4);
                myindex += num;
                se = new ConstantSymbolEntry(arraytype);
                initlist = new Initlist(se);
            }
            else
            {
                // 栈不空说明肯定不是只有{}
                // 此时需要确定{}到底占了几个元素
                int size = ((ArrayType*)(((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType()))->getArraySize()/32;
                memset(equalintarray + myindex, 0, size*4);
                myindex += size;
                Type* type = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType();
                se = new ConstantSymbolEntry(type);
                initlist = new Initlist(se);
                initstack.top()->AddExp(initlist);
                while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                {
                    initstack.pop();
                }
            }
            (yyval.exprtype) = initlist;
        }

        if(tempType=="FLOAT"){
            SymbolEntry* se;
            ExprNode* initlist;
            if(initstack.empty())
            {
                // 如果只用一个{}初始化数组，那么栈一定为空
                // 此时也没必要再加入栈了
                memset(equalfloatarray, 0, arraytype->getArraySize());
                myindex += arraytype->getArraySize()/32 ;
                se = new ConstantSymbolEntry(arraytype);
                initlist = new Initlist(se);
            }
            else
            {
                // 栈不空说明肯定不是只有{}
                // 此时需要确定{}到底占了几个元素
                int size = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArraySize();
                int len = size/32 ;
                memset(equalfloatarray + myindex, 0, size);
                myindex += len;
                Type* type = ((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType();
                se = new ConstantSymbolEntry(type);
                initlist = new Initlist(se);
                initstack.top()->AddExp(initlist);
                while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
                {
                    initstack.pop();
                }
            }
            (yyval.exprtype) = initlist;
        }
        
    }
#line 2531 "src/parser.cpp"
    break;

  case 92: /* @6: %empty  */
#line 913 "src/parser.y"
             {
        bracenum++;
        SymbolEntry* se;
        if(!initstack.empty())
        {
            arraytype = (ArrayType*)(((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType());
        }
        se = new ConstantSymbolEntry(arraytype);
        Initlist* expr = new Initlist(se);
        if(!initstack.empty())
        {
            initstack.top()->AddExp(expr);
        }
        initstack.push(expr);
        (yyval.exprtype) = expr;
        
    }
#line 2553 "src/parser.cpp"
    break;

  case 93: /* VarInitVal: LBRACE @6 VarInitValList RBRACE  */
#line 930 "src/parser.y"
                            {
        bracenum--;
        while(initstack.top() != (yyvsp[-2].exprtype) && initstack.size() > (long unsigned int)(bracenum + 1))
        {
            initstack.pop();
        }
        if(initstack.top() == (yyvsp[-2].exprtype))
        {
            initstack.pop();
        }
        (yyval.exprtype) = (yyvsp[-2].exprtype);
        if(!initstack.empty())
        {
            while(initstack.top()->ifFull() && initstack.size() != (long unsigned int)bracenum)
            {
                initstack.pop();
            }
        }
        if(tempType=="INT")
        {
            int size=((ArrayType*)((yyval.exprtype)->getSymPtr()->getType()))->getArraySize();
            while(myindex % (size/32) !=0 )
            {
                equalintarray[myindex++] = 0;
            }
        }
        if(tempType=="FLOAT")
        {
            while(myindex % (((ArrayType*)((yyval.exprtype)->getSymPtr()->getType()))->getArraySize() !=0 ))
            {
                equalfloatarray[myindex++] = 0.0;
            }
        }
        
        if(!initstack.empty())
        {
            arraytype = (ArrayType*)(((ArrayType*)(initstack.top()->getSymPtr()->getType()))->getArrayType());
        }
        
    }
#line 2598 "src/parser.cpp"
    break;

  case 94: /* VarInitValList: VarInitVal  */
#line 974 "src/parser.y"
                 {
        (yyval.exprtype) = (yyvsp[0].exprtype);
    }
#line 2606 "src/parser.cpp"
    break;

  case 95: /* VarInitValList: VarInitValList COMM VarInitVal  */
#line 977 "src/parser.y"
                                     {
        (yyval.exprtype) = (yyvsp[-2].exprtype);
    }
#line 2614 "src/parser.cpp"
    break;

  case 96: /* $@7: %empty  */
#line 983 "src/parser.y"
             {
        identifiers = new SymbolTable(identifiers);
        intParam_num_for_func=0;
        floatParam_num_for_func=0;
        functionhasRet=false;
    }
#line 2625 "src/parser.cpp"
    break;

  case 97: /* $@8: %empty  */
#line 990 "src/parser.y"
    {
        FunctionType* functype=new FunctionType((yyvsp[-5].type),{}); 
        if((yyvsp[-1].stmttype))
        {
            std::vector<Type*> paramType;
            std::vector<SymbolEntry*> params;
            DeclStmt* tmp=dynamic_cast<DeclStmt*>((yyvsp[-1].stmttype));
            
            while(tmp)
            {
                params.push_back(dynamic_cast<IdentifierSymbolEntry*>(tmp->getID()->getSymPtr()));
                paramType.push_back(dynamic_cast<IdentifierSymbolEntry*>(tmp->getID()->getSymPtr())->getType());
                tmp=dynamic_cast<DeclStmt*>(tmp->getNext());
            }
            functype->setparams(params);
            functype->setparamsType(paramType);
            functype->setIntparamCount(intParam_num_for_func);
            functype->setFloatparamCount(floatParam_num_for_func);
        }   
        SymbolEntry *se = new IdentifierSymbolEntry(functype, (yyvsp[-4].strtype), identifiers->getPrev()->getLevel());
        if(!identifiers->getPrev()->install2((yyvsp[-4].strtype), se))
        {
            fprintf(stderr, "func %s is already defined\n", (char*)(yyvsp[-4].strtype));
            exit(EXIT_FAILURE);
        }    
    }
#line 2656 "src/parser.cpp"
    break;

  case 98: /* FuncDef: Type ID $@7 LPAREN FuncParams RPAREN $@8 BlockStmt  */
#line 1017 "src/parser.y"
    {
        SymbolEntry *se;
        se = identifiers->lookup((yyvsp[-6].strtype));
        assert(se != nullptr);
        if(((yyvsp[-7].type)==TypeSystem::voidType)&&(!functionhasRet))
        {
            auto tmpnode=new SeqNode((yyvsp[0].stmttype),new ReturnStmt());
            (yyvsp[0].stmttype)=tmpnode;
        }
        FunctionDef* newFunc=new FunctionDef(se,(yyvsp[-3].stmttype), (yyvsp[0].stmttype));
        (yyval.stmttype) = newFunc;
        //恢复原来的作用域，和函数本身关系不大
        SymbolTable *top = identifiers;
        identifiers = identifiers->getPrev();
        delete top;
        delete [](yyvsp[-6].strtype);
    }
#line 2678 "src/parser.cpp"
    break;

  case 99: /* FuncParams: FuncParams COMM FuncParam  */
#line 1037 "src/parser.y"
                                {
        (yyval.stmttype) = (yyvsp[-2].stmttype);
        (yyvsp[-2].stmttype)->setNext((yyvsp[0].stmttype));        //设置参数的next
    }
#line 2687 "src/parser.cpp"
    break;

  case 100: /* FuncParams: FuncParam  */
#line 1041 "src/parser.y"
                {
        (yyval.stmttype) = (yyvsp[0].stmttype);
    }
#line 2695 "src/parser.cpp"
    break;

  case 101: /* FuncParams: %empty  */
#line 1044 "src/parser.y"
             { (yyval.stmttype)=nullptr;}
#line 2701 "src/parser.cpp"
    break;

  case 102: /* FuncParam: Type ID  */
#line 1047 "src/parser.y"
              {
        SymbolEntry *se;
        if(tempType=="INT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::intType, (yyvsp[0].strtype), identifiers->getLevel(),intParam_num_for_func++);
        }
        if(tempType=="FLOAT")
        {
            se = new IdentifierSymbolEntry(TypeSystem::floatType, (yyvsp[0].strtype), identifiers->getLevel(),floatParam_num_for_func++);
        }
        //将这一符号表项存入当前作用域的符号表中
        identifiers->install((yyvsp[0].strtype), se);
        dynamic_cast<IdentifierSymbolEntry*>(se)->setLabel(SymbolTable::getLabel());
        dynamic_cast<IdentifierSymbolEntry*>(se)->setAddr(new Operand(se));
        (yyval.stmttype) = new DeclStmt(new Id(se));
        delete [](yyvsp[0].strtype);    
    }
#line 2723 "src/parser.cpp"
    break;

  case 103: /* FuncParam: Type ID FunArrayIndex  */
#line 1064 "src/parser.y"
                           {
        if(tempType=="FLOAT")  
        {
            SymbolEntry* se;
            ExprNode* temp = (yyvsp[0].exprtype);
            Type* arr = TypeSystem::floatType;
            Type* arr1;
            std::stack<ExprNode*> stk;
            while(temp)
            {
                stk.push(temp);
                temp = (ExprNode*)(temp->getNext());
            }
            while(!stk.empty())
            {
                arr1 = new ArrayType(arr, stk.top()->getValue());
                if(arr->isArray())
                    ((ArrayType*)arr)->setParentType(arr1);
                arr = arr1;
                stk.pop();
            }
            se = new IdentifierSymbolEntry(arr, (yyvsp[-1].strtype), identifiers->getLevel(),intParam_num_for_func++);
            identifiers->install((yyvsp[-1].strtype), se);
            ((IdentifierSymbolEntry*)se)->setLabel(SymbolTable::getLabel());
            ((IdentifierSymbolEntry*)se)->setAddr(new Operand(se));
            (yyval.stmttype) = new DeclStmt(new Id(se));
            delete [](yyvsp[-1].strtype);
        }
        else
        {
            SymbolEntry* se;
            ExprNode* temp = (yyvsp[0].exprtype);
            Type* arr = TypeSystem::intType;
            Type* arr1;
            std::stack<ExprNode*> stk;
            while(temp)
            {
                stk.push(temp);
                temp = (ExprNode*)(temp->getNext());
            }
            while(!stk.empty())
            {
                arr1 = new ArrayType(arr, stk.top()->getValue());
                if(arr->isArray())
                    ((ArrayType*)arr)->setParentType(arr1);
                arr = arr1;
                stk.pop();
            }
            se = new IdentifierSymbolEntry(arr, (yyvsp[-1].strtype), identifiers->getLevel(),intParam_num_for_func++);
            identifiers->install((yyvsp[-1].strtype), se);
            ((IdentifierSymbolEntry*)se)->setLabel(SymbolTable::getLabel());
            ((IdentifierSymbolEntry*)se)->setAddr(new Operand(se));
            (yyval.stmttype) = new DeclStmt(new Id(se));
            delete [](yyvsp[-1].strtype);
        }
        
    }
#line 2785 "src/parser.cpp"
    break;

  case 104: /* FunArrayIndex: LBRACK RBRACK  */
#line 1123 "src/parser.y"
                    {
        (yyval.exprtype) = new ExprNode(nullptr);
    }
#line 2793 "src/parser.cpp"
    break;

  case 105: /* FunArrayIndex: FunArrayIndex LBRACK Exp RBRACK  */
#line 1126 "src/parser.y"
                                       {
        (yyval.exprtype) = (yyvsp[-3].exprtype);
        (yyval.exprtype)->setNext((yyvsp[-1].exprtype));
    }
#line 2802 "src/parser.cpp"
    break;

  case 106: /* FuncCall: ID LPAREN ParaList RPAREN  */
#line 1132 "src/parser.y"
                                {
        //判断是不是时间相关函数
        string start_prefix=string((yyvsp[-3].strtype)).substr(0,15);
        string stop_prefix=string((yyvsp[-3].strtype)).substr(0,14);
        if(start_prefix=="_sysy_starttime")
        {
            SymbolEntry *se= identifiers->lookup(start_prefix);
            int lineno=stoi(string((yyvsp[-3].strtype)).substr(15,string((yyvsp[-3].strtype)).length()));
            SymbolEntry* param = new ConstantSymbolEntry(TypeSystem::intType, lineno);    
            (yyval.exprtype) = new FunctionCall(se,new Constant(param));
        }
        else if(stop_prefix=="_sysy_stoptime")
        {
            SymbolEntry *se= identifiers->lookup(stop_prefix);
            int lineno=stoi(string((yyvsp[-3].strtype)).substr(14,string((yyvsp[-3].strtype)).length()));
            SymbolEntry* param = new ConstantSymbolEntry(TypeSystem::intType, lineno);    
            (yyval.exprtype) = new FunctionCall(se,new Constant(param));
        }
        else
        {
            //要做的是，新建一个函数调用节点，需要包含的信息是函数名的符号表项，以及各个参数的符号表项
            SymbolEntry *se= identifiers->lookup((yyvsp[-3].strtype));
            //函数没有被声明过
            if(se == nullptr)
            {
                fprintf(stderr, "function %s is undeclared\n", (char*)(yyvsp[-3].strtype));
                exit(EXIT_FAILURE);
            }
            (yyval.exprtype)=new FunctionCall(se,(yyvsp[-1].exprtype));
        }
    }
#line 2838 "src/parser.cpp"
    break;

  case 107: /* ParaList: ParaList COMM Exp  */
#line 1165 "src/parser.y"
                        {
        (yyval.exprtype) = (yyvsp[-2].exprtype);
        (yyvsp[-2].exprtype)->setNext((yyvsp[0].exprtype));        //设置next
    }
#line 2847 "src/parser.cpp"
    break;

  case 108: /* ParaList: Exp  */
#line 1169 "src/parser.y"
          { (yyval.exprtype)=(yyvsp[0].exprtype);}
#line 2853 "src/parser.cpp"
    break;

  case 109: /* ParaList: %empty  */
#line 1170 "src/parser.y"
             { (yyval.exprtype)=nullptr;}
#line 2859 "src/parser.cpp"
    break;


#line 2863 "src/parser.cpp"

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

#line 1173 "src/parser.y"


int yyerror(char const* message)
{
    std::cerr<<message<<std::endl;
    return -1;
}
