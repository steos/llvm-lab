/** \file
 *  This C header file was generated by $ANTLR version 3.2 Sep 23, 2009 12:02:23
 *
 *     -  From the grammar source file : f:/dev/projects/kensho/docs/KenshoTreeParser.g
 *     -                            On : 2009-10-08 06:20:41
 *     -           for the tree parser : KenshoTreeParserTreeParser *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The tree parser KenshoTreeParser has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 * 
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pKenshoTreeParser, which is returned from a call to KenshoTreeParserNew().
 *
 * The methods in pKenshoTreeParser are  as follows:
 *
 *  - KenshoTreeParser_program_return      pKenshoTreeParser->program(pKenshoTreeParser)
 *  - KenshoTreeParser_kenniFunction_return      pKenshoTreeParser->kenniFunction(pKenshoTreeParser)
 *  - KenshoTreeParser_structDecl_return      pKenshoTreeParser->structDecl(pKenshoTreeParser)
 *  - KenshoTreeParser_structBodyDecl_return      pKenshoTreeParser->structBodyDecl(pKenshoTreeParser)
 *  - KenshoTreeParser_structCtor_return      pKenshoTreeParser->structCtor(pKenshoTreeParser)
 *  - KenshoTreeParser_structDtor_return      pKenshoTreeParser->structDtor(pKenshoTreeParser)
 *  - KenshoTreeParser_structFunction_return      pKenshoTreeParser->structFunction(pKenshoTreeParser)
 *  - KenshoTreeParser_structFunMods_return      pKenshoTreeParser->structFunMods(pKenshoTreeParser)
 *  - KenshoTreeParser_function_return      pKenshoTreeParser->function(pKenshoTreeParser)
 *  - KenshoTreeParser_signature_return      pKenshoTreeParser->signature(pKenshoTreeParser)
 *  - KenshoTreeParser_functionType_return      pKenshoTreeParser->functionType(pKenshoTreeParser)
 *  - KenshoTreeParser_params_return      pKenshoTreeParser->params(pKenshoTreeParser)
 *  - KenshoTreeParser_statement_return      pKenshoTreeParser->statement(pKenshoTreeParser)
 *  - KenshoTreeParser_deleteStatement_return      pKenshoTreeParser->deleteStatement(pKenshoTreeParser)
 *  - KenshoTreeParser_returnStatement_return      pKenshoTreeParser->returnStatement(pKenshoTreeParser)
 *  - KenshoTreeParser_ifStat_return      pKenshoTreeParser->ifStat(pKenshoTreeParser)
 *  - KenshoTreeParser_elseIfStat_return      pKenshoTreeParser->elseIfStat(pKenshoTreeParser)
 *  - KenshoTreeParser_elseStat_return      pKenshoTreeParser->elseStat(pKenshoTreeParser)
 *  - KenshoTreeParser_whileStat_return      pKenshoTreeParser->whileStat(pKenshoTreeParser)
 *  - KenshoTreeParser_variable_return      pKenshoTreeParser->variable(pKenshoTreeParser)
 *  - KenshoTreeParser_type_return      pKenshoTreeParser->type(pKenshoTreeParser)
 *  - KenshoTreeParser_expression_return      pKenshoTreeParser->expression(pKenshoTreeParser)
 *  - KenshoTreeParser_call_return      pKenshoTreeParser->call(pKenshoTreeParser)
 *  - KenshoTreeParser_ctorCall_return      pKenshoTreeParser->ctorCall(pKenshoTreeParser)
 *  - KenshoTreeParser_literal_return      pKenshoTreeParser->literal(pKenshoTreeParser)
 *  - KenshoTreeParser_unop_return      pKenshoTreeParser->unop(pKenshoTreeParser)
 *  - KenshoTreeParser_binop_return      pKenshoTreeParser->binop(pKenshoTreeParser)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD licence"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_KenshoTreeParser_H
#define _KenshoTreeParser_H

	#include <kensho/ast/tokens.hpp>
	#include <kensho/ast/Call.hpp>
	#include <kensho/ast/Variable.hpp>
	#include <kensho/ast/VariableDefinition.hpp>
	#include <kensho/ast/Function.hpp>
	#include <kensho/ast/Struct.hpp>
	#include <kensho/ast/BinaryExpression.hpp>
	#include <kensho/ast/UnaryExpression.hpp>
	#include <kensho/ast/Conditional.hpp>
	#include <kensho/ast/While.hpp>
	#include <kensho/ast/Return.hpp>
	#include <kensho/ast/Cast.hpp>
	#include <kensho/ast/ConstructorCall.hpp>
	#include <kensho/ast/Delete.hpp>
	#include <kensho/ast/Type.hpp>
	#include <kensho/ast/ModuleBuilder.hpp>
	#include <kensho/ast/Literal.hpp>
	#include <kensho/ast/Assignment.hpp>
	#include <kensho/error.hpp>
	#include <vector>

/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */
 
#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct KenshoTreeParser_Ctx_struct KenshoTreeParser, * pKenshoTreeParser;



#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule 
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif
typedef struct KenshoTreeParser_program_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::ModuleBuilder* builder;
}
    KenshoTreeParser_program_return;

typedef struct KenshoTreeParser_kenniFunction_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Callable* node;
}
    KenshoTreeParser_kenniFunction_return;

typedef struct KenshoTreeParser_structDecl_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Struct* node;
}
    KenshoTreeParser_structDecl_return;

typedef struct KenshoTreeParser_structBodyDecl_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoTreeParser_structBodyDecl_return;

typedef struct KenshoTreeParser_structCtor_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Function* node;
}
    KenshoTreeParser_structCtor_return;

typedef struct KenshoTreeParser_structDtor_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoTreeParser_structDtor_return;

typedef struct KenshoTreeParser_structFunction_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoTreeParser_structFunction_return;

typedef struct KenshoTreeParser_structFunMods_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    bool staticDef;
}
    KenshoTreeParser_structFunMods_return;

typedef struct KenshoTreeParser_function_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Function* node;
}
    KenshoTreeParser_function_return;

typedef struct KenshoTreeParser_signature_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Function* node;
}
    KenshoTreeParser_signature_return;

typedef struct KenshoTreeParser_functionType_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Type* ty;
}
    KenshoTreeParser_functionType_return;

typedef struct KenshoTreeParser_params_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoTreeParser_params_return;

typedef struct KenshoTreeParser_statement_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Node* node;
}
    KenshoTreeParser_statement_return;

typedef struct KenshoTreeParser_deleteStatement_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Delete* node;
}
    KenshoTreeParser_deleteStatement_return;

typedef struct KenshoTreeParser_returnStatement_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Return* node;
}
    KenshoTreeParser_returnStatement_return;

typedef struct KenshoTreeParser_ifStat_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Conditional* node;
}
    KenshoTreeParser_ifStat_return;

typedef struct KenshoTreeParser_elseIfStat_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Conditional* node;
}
    KenshoTreeParser_elseIfStat_return;

typedef struct KenshoTreeParser_elseStat_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoTreeParser_elseStat_return;

typedef struct KenshoTreeParser_whileStat_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::While* node;
}
    KenshoTreeParser_whileStat_return;

typedef struct KenshoTreeParser_variable_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::VariableDefinition* node;
}
    KenshoTreeParser_variable_return;

typedef struct KenshoTreeParser_type_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Type* ty;
}
    KenshoTreeParser_type_return;

typedef struct KenshoTreeParser_expression_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Node* node;
}
    KenshoTreeParser_expression_return;

typedef struct KenshoTreeParser_call_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Call* node;
}
    KenshoTreeParser_call_return;

typedef struct KenshoTreeParser_ctorCall_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::ConstructorCall* node;
}
    KenshoTreeParser_ctorCall_return;

typedef struct KenshoTreeParser_literal_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::Literal* node;
}
    KenshoTreeParser_literal_return;

typedef struct KenshoTreeParser_unop_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::OperatorToken op;
}
    KenshoTreeParser_unop_return;

typedef struct KenshoTreeParser_binop_return_struct
{
    pANTLR3_BASE_TREE       start;
    pANTLR3_BASE_TREE       stop;
    pANTLR3_BASE_TREE	tree;
   
    kensho::ast::OperatorToken op;
}
    KenshoTreeParser_binop_return;



/* ruleAttributeScopeDecl(scope)
 */
/* makeScopeSet() 
 */
 /** Definition of the program scope variable tracking
 *  structure. An instance of this structure is created by calling
 *  KenshoTreeParser_programPush().
 */
typedef struct  KenshoTreeParser_program_SCOPE_struct
{
    /** Function that the user may provide to be called when the
     *  scope is destroyed (so you can free pANTLR3_HASH_TABLES and so on)
     *
     * \param POinter to an instance of this typedef/struct
     */
    void    (ANTLR3_CDECL *free)	(struct KenshoTreeParser_program_SCOPE_struct * frame);
    
    /* =============================================================================
     * Programmer defined variables...
     */
    kensho::ast::ModuleBuilder* mb;

    /* End of programmer defined variables
     * =============================================================================
     */
} 
    KenshoTreeParser_program_SCOPE, * pKenshoTreeParser_program_SCOPE;

/** Context tracking structure for KenshoTreeParser
 */
struct KenshoTreeParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_TREE_PARSER	    pTreeParser;

    /* ruleAttributeScopeDef(scope)
     */
    /** Pointer to the  program stack for use by pKenshoTreeParser_programPush()
     *  and pKenshoTreeParser_programPop()
     */
    pANTLR3_STACK pKenshoTreeParser_programStack;
    ANTLR3_UINT32 pKenshoTreeParser_programStack_limit;
    pKenshoTreeParser_program_SCOPE   (*pKenshoTreeParser_programPush)(struct KenshoTreeParser_Ctx_struct * ctx);
    pKenshoTreeParser_program_SCOPE   pKenshoTreeParser_programTop;


     KenshoTreeParser_program_return (*program)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_kenniFunction_return (*kenniFunction)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_structDecl_return (*structDecl)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_structBodyDecl_return (*structBodyDecl)	(struct KenshoTreeParser_Ctx_struct * ctx, kensho::ast::Struct* parent);
     KenshoTreeParser_structCtor_return (*structCtor)	(struct KenshoTreeParser_Ctx_struct * ctx, std::string parentName);
     KenshoTreeParser_structDtor_return (*structDtor)	(struct KenshoTreeParser_Ctx_struct * ctx, kensho::ast::Struct* parent);
     KenshoTreeParser_structFunction_return (*structFunction)	(struct KenshoTreeParser_Ctx_struct * ctx, kensho::ast::Struct* parent);
     KenshoTreeParser_structFunMods_return (*structFunMods)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_function_return (*function)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_signature_return (*signature)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_functionType_return (*functionType)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_params_return (*params)	(struct KenshoTreeParser_Ctx_struct * ctx, kensho::ast::Function* node, uint32_t index);
     KenshoTreeParser_statement_return (*statement)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_deleteStatement_return (*deleteStatement)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_returnStatement_return (*returnStatement)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_ifStat_return (*ifStat)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_elseIfStat_return (*elseIfStat)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_elseStat_return (*elseStat)	(struct KenshoTreeParser_Ctx_struct * ctx, kensho::ast::Conditional* node);
     KenshoTreeParser_whileStat_return (*whileStat)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_variable_return (*variable)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_type_return (*type)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_expression_return (*expression)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_call_return (*call)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_ctorCall_return (*ctorCall)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_literal_return (*literal)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_unop_return (*unop)	(struct KenshoTreeParser_Ctx_struct * ctx);
     KenshoTreeParser_binop_return (*binop)	(struct KenshoTreeParser_Ctx_struct * ctx);
    // Delegated rules
    const char * (*getGrammarFileName)();
    void	    (*free)   (struct KenshoTreeParser_Ctx_struct * ctx);
    /* @headerFile.members() */
    pANTLR3_BASE_TREE_ADAPTOR	adaptor;
    pANTLR3_VECTOR_FACTORY		vectors;
    /* End @headerFile.members() */
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pKenshoTreeParser KenshoTreeParserNew         (pANTLR3_COMMON_TREE_NODE_STREAM instream);
ANTLR3_API pKenshoTreeParser KenshoTreeParserNewSSD      (pANTLR3_COMMON_TREE_NODE_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the tree parser will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif 
#define CAST      61
#define EXPONENT      79
#define OP_AND      19
#define OP_ASSIGN      34
#define OP_OR      20
#define LETTER      75
#define K_DELETE      43
#define OP_MUL      15
#define T_BOOL      4
#define T_DOUBLE      11
#define PAREN_L      49
#define CMP_EQ      28
#define ID      68
#define EOF      -1
#define T_VOID      12
#define OP_BIT_NOT      18
#define KENNIDEF      62
#define PAREN_R      50
#define T_LONG      9
#define CMP_LT      30
#define OP_SUB      14
#define CTOR      66
#define MODS      65
#define T_CHAR      8
#define COMMA      51
#define T_INT      7
#define LITERAL_TRUE      38
#define BLOCK_COMMENT      83
#define OP_BIT_OR      23
#define K_STRUCT      45
#define OP_NOT      17
#define DIGIT      76
#define VARDEF      59
#define LITERAL_OCT      70
#define LITERAL_HEX      71
#define LITERAL_FALSE      39
#define CMP_GTE      33
#define ESCSEQ      80
#define FUNSIG      54
#define BRACE_L      47
#define LITERAL_FLOAT      72
#define BRACE_R      48
#define CMP_NEQ      29
#define T_BYTE      5
#define BINOP      56
#define LINE_COMMENT      82
#define LITERAL_INT      69
#define OP_ADD      13
#define LIT      57
#define K_WHILE      37
#define T_FLOAT      10
#define OP_USHIFT_R      27
#define LITERAL_CHAR      73
#define OP_DIV      16
#define SEMICOLON      52
#define STRUCTFUN      64
#define OP_USHIFT_L      74
#define K_ELSE      36
#define CMP_GT      31
#define K_NEW      42
#define K_RETURN      41
#define ARGDEF      55
#define K_IF      35
#define ELSEIF      63
#define K_STATIC      46
#define OP_SHIFT_R      26
#define UNOP      58
#define WS      81
#define POSDIGIT      77
#define OP_SHIFT_L      25
#define FUNDEF      53
#define DTOR      67
#define OP_XOR      24
#define CMP_LTE      32
#define OP_BIT_AND      22
#define CALL      60
#define T_SHORT      6
#define K_NATIVE      40
#define OP_REM      21
#define K_THIS      44
#define HEXDIGIT      78
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for KenshoTreeParser
 * =============================================================================
 */
/** \} */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
