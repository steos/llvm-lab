/** \file
 *  This C header file was generated by $ANTLR version 3.1.3 Mar 18, 2009 10:09:25
 *
 *     -  From the grammar source file : f:/dev/projects/kensho/docs/Kensho.g
 *     -                            On : 2009-10-02 09:56:06
 *     -                for the parser : KenshoParserParser *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The parser KenshoParser has the callable functions (rules) shown below,
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
 * a parser context typedef pKenshoParser, which is returned from a call to KenshoParserNew().
 *
 * The methods in pKenshoParser are  as follows:
 *
 *  - KenshoParser_program_return      pKenshoParser->program(pKenshoParser)
 *  - KenshoParser_kenniFunction_return      pKenshoParser->kenniFunction(pKenshoParser)
 *  - KenshoParser_function_return      pKenshoParser->function(pKenshoParser)
 *  - KenshoParser_signature_return      pKenshoParser->signature(pKenshoParser)
 *  - KenshoParser_functionType_return      pKenshoParser->functionType(pKenshoParser)
 *  - KenshoParser_params_return      pKenshoParser->params(pKenshoParser)
 *  - KenshoParser_statement_return      pKenshoParser->statement(pKenshoParser)
 *  - KenshoParser_returnStatement_return      pKenshoParser->returnStatement(pKenshoParser)
 *  - KenshoParser_block_return      pKenshoParser->block(pKenshoParser)
 *  - KenshoParser_ifStat_return      pKenshoParser->ifStat(pKenshoParser)
 *  - KenshoParser_elseIfStat_return      pKenshoParser->elseIfStat(pKenshoParser)
 *  - KenshoParser_elseStat_return      pKenshoParser->elseStat(pKenshoParser)
 *  - KenshoParser_whileStat_return      pKenshoParser->whileStat(pKenshoParser)
 *  - KenshoParser_variable_return      pKenshoParser->variable(pKenshoParser)
 *  - KenshoParser_args_return      pKenshoParser->args(pKenshoParser)
 *  - KenshoParser_type_return      pKenshoParser->type(pKenshoParser)
 *  - KenshoParser_expression_return      pKenshoParser->expression(pKenshoParser)
 *  - KenshoParser_primary_return      pKenshoParser->primary(pKenshoParser)
 *  - KenshoParser_atom_return      pKenshoParser->atom(pKenshoParser)
 *  - KenshoParser_call_return      pKenshoParser->call(pKenshoParser)
 *  - KenshoParser_literal_return      pKenshoParser->literal(pKenshoParser)
 *  - KenshoParser_unop_return      pKenshoParser->unop(pKenshoParser)
 *  - KenshoParser_binop_return      pKenshoParser->binop(pKenshoParser)
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

#ifndef	_KenshoParser_H
#define _KenshoParser_H

	char* antlrTokenName(int type);

/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */

	#include <vector>
	#include <cassert>

 
#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct KenshoParser_Ctx_struct KenshoParser, * pKenshoParser;



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
typedef struct KenshoParser_program_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_program_return;

typedef struct KenshoParser_kenniFunction_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_kenniFunction_return;

typedef struct KenshoParser_function_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_function_return;

typedef struct KenshoParser_signature_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_signature_return;

typedef struct KenshoParser_functionType_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_functionType_return;

typedef struct KenshoParser_params_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_params_return;

typedef struct KenshoParser_statement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_statement_return;

typedef struct KenshoParser_returnStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_returnStatement_return;

typedef struct KenshoParser_block_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_block_return;

typedef struct KenshoParser_ifStat_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_ifStat_return;

typedef struct KenshoParser_elseIfStat_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_elseIfStat_return;

typedef struct KenshoParser_elseStat_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_elseStat_return;

typedef struct KenshoParser_whileStat_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_whileStat_return;

typedef struct KenshoParser_variable_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_variable_return;

typedef struct KenshoParser_args_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_args_return;

typedef struct KenshoParser_type_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_type_return;

typedef struct KenshoParser_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_expression_return;

typedef struct KenshoParser_primary_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_primary_return;

typedef struct KenshoParser_atom_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_atom_return;

typedef struct KenshoParser_call_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_call_return;

typedef struct KenshoParser_literal_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_literal_return;

typedef struct KenshoParser_unop_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_unop_return;

typedef struct KenshoParser_binop_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;
   
}
    KenshoParser_binop_return;



/** Context tracking structure for KenshoParser
 */
struct KenshoParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_PARSER   pParser;


     KenshoParser_program_return (*program)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_kenniFunction_return (*kenniFunction)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_function_return (*function)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_signature_return (*signature)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_functionType_return (*functionType)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_params_return (*params)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_statement_return (*statement)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_returnStatement_return (*returnStatement)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_block_return (*block)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_ifStat_return (*ifStat)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_elseIfStat_return (*elseIfStat)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_elseStat_return (*elseStat)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_whileStat_return (*whileStat)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_variable_return (*variable)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_args_return (*args)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_type_return (*type)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_expression_return (*expression)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_primary_return (*primary)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_atom_return (*atom)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_call_return (*call)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_literal_return (*literal)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_unop_return (*unop)	(struct KenshoParser_Ctx_struct * ctx);
     KenshoParser_binop_return (*binop)	(struct KenshoParser_Ctx_struct * ctx);
    // Delegated rules
    const char * (*getGrammarFileName)();
    void	    (*free)   (struct KenshoParser_Ctx_struct * ctx);
    /* @headerFile.members() */
    pANTLR3_BASE_TREE_ADAPTOR	adaptor;
    pANTLR3_VECTOR_FACTORY		vectors;
    /* End @headerFile.members() */
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pKenshoParser KenshoParserNew         (pANTLR3_COMMON_TOKEN_STREAM instream);
ANTLR3_API pKenshoParser KenshoParserNewSSD      (pANTLR3_COMMON_TOKEN_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the parser will work with.
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
#define CAST      56
#define EXPONENT      68
#define OP_AND      19
#define OP_ASSIGN      34
#define OP_OR      20
#define LETTER      65
#define OP_MUL      15
#define T_BOOL      4
#define T_DOUBLE      11
#define PAREN_L      44
#define CMP_EQ      28
#define ID      59
#define EOF      -1
#define T_VOID      12
#define OP_BIT_NOT      18
#define KENNIDEF      57
#define PAREN_R      45
#define T_LONG      9
#define CMP_LT      30
#define OP_SUB      14
#define T_CHAR      8
#define COMMA      46
#define T_INT      7
#define LITERAL_TRUE      38
#define BLOCK_COMMENT      72
#define OP_BIT_OR      22
#define OP_NOT      17
#define DIGIT      66
#define VARDEF      54
#define LITERAL_OCT      61
#define LITERAL_HEX      62
#define LITERAL_FALSE      39
#define CMP_GTE      33
#define ESCSEQ      69
#define FUNSIG      49
#define BRACE_L      42
#define LITERAL_FLOAT      63
#define BRACE_R      43
#define CMP_NEQ      29
#define T_BYTE      5
#define BINOP      51
#define LINE_COMMENT      71
#define LITERAL_INT      60
#define OP_ADD      13
#define LIT      52
#define T_FLOAT      10
#define K_WHILE      37
#define LITERAL_CHAR      64
#define OP_USHIFT_R      27
#define SEMICOLON      47
#define OP_DIV      16
#define OP_USHIFT_L      26
#define K_ELSE      36
#define CMP_GT      31
#define ARGDEF      50
#define K_RETURN      41
#define K_IF      35
#define ELSEIF      58
#define UNOP      53
#define OP_SHIFT_R      25
#define WS      70
#define OP_SHIFT_L      24
#define FUNDEF      48
#define OP_XOR      23
#define CMP_LTE      32
#define CALL      55
#define OP_BIT_AND      21
#define T_SHORT      6
#define K_NATIVE      40
#define HEXDIGIT      67
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for KenshoParser
 * =============================================================================
 */
/** \} */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
