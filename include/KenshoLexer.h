/** \file
 *  This C header file was generated by $ANTLR version 3.2 Sep 23, 2009 12:02:23
 *
 *     -  From the grammar source file : f:/dev/projects/kensho/docs/Kensho.g
 *     -                            On : 2009-10-08 13:19:57
 *     -                 for the lexer : KenshoLexerLexer *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The lexer KenshoLexer has the callable functions (rules) shown below,
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
 * a parser context typedef pKenshoLexer, which is returned from a call to KenshoLexerNew().
 *
 * As this is a generated lexer, it is unlikely you will call it 'manually'. However
 * the methods are provided anyway.
 * * The methods in pKenshoLexer are  as follows:
 *
 *  -  void      pKenshoLexer->T_BOOL(pKenshoLexer)
 *  -  void      pKenshoLexer->T_BYTE(pKenshoLexer)
 *  -  void      pKenshoLexer->T_SHORT(pKenshoLexer)
 *  -  void      pKenshoLexer->T_INT(pKenshoLexer)
 *  -  void      pKenshoLexer->T_CHAR(pKenshoLexer)
 *  -  void      pKenshoLexer->T_LONG(pKenshoLexer)
 *  -  void      pKenshoLexer->T_FLOAT(pKenshoLexer)
 *  -  void      pKenshoLexer->T_DOUBLE(pKenshoLexer)
 *  -  void      pKenshoLexer->T_VOID(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_ADD(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_SUB(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_MUL(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_DIV(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_NOT(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_BIT_NOT(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_AND(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_OR(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_REM(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_BIT_AND(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_BIT_OR(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_XOR(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_SHIFT_L(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_SHIFT_R(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_USHIFT_R(pKenshoLexer)
 *  -  void      pKenshoLexer->CMP_EQ(pKenshoLexer)
 *  -  void      pKenshoLexer->CMP_NEQ(pKenshoLexer)
 *  -  void      pKenshoLexer->CMP_LT(pKenshoLexer)
 *  -  void      pKenshoLexer->CMP_GT(pKenshoLexer)
 *  -  void      pKenshoLexer->CMP_LTE(pKenshoLexer)
 *  -  void      pKenshoLexer->CMP_GTE(pKenshoLexer)
 *  -  void      pKenshoLexer->OP_ASSIGN(pKenshoLexer)
 *  -  void      pKenshoLexer->K_IF(pKenshoLexer)
 *  -  void      pKenshoLexer->K_ELSE(pKenshoLexer)
 *  -  void      pKenshoLexer->K_WHILE(pKenshoLexer)
 *  -  void      pKenshoLexer->LITERAL_TRUE(pKenshoLexer)
 *  -  void      pKenshoLexer->LITERAL_FALSE(pKenshoLexer)
 *  -  void      pKenshoLexer->K_NATIVE(pKenshoLexer)
 *  -  void      pKenshoLexer->K_RETURN(pKenshoLexer)
 *  -  void      pKenshoLexer->K_NEW(pKenshoLexer)
 *  -  void      pKenshoLexer->K_DELETE(pKenshoLexer)
 *  -  void      pKenshoLexer->K_THIS(pKenshoLexer)
 *  -  void      pKenshoLexer->K_STRUCT(pKenshoLexer)
 *  -  void      pKenshoLexer->K_STATIC(pKenshoLexer)
 *  -  void      pKenshoLexer->BRACE_L(pKenshoLexer)
 *  -  void      pKenshoLexer->BRACE_R(pKenshoLexer)
 *  -  void      pKenshoLexer->PAREN_L(pKenshoLexer)
 *  -  void      pKenshoLexer->PAREN_R(pKenshoLexer)
 *  -  void      pKenshoLexer->COMMA(pKenshoLexer)
 *  -  void      pKenshoLexer->SEMICOLON(pKenshoLexer)
 *  -  void      pKenshoLexer->LETTER(pKenshoLexer)
 *  -  void      pKenshoLexer->DIGIT(pKenshoLexer)
 *  -  void      pKenshoLexer->POSDIGIT(pKenshoLexer)
 *  -  void      pKenshoLexer->HEXDIGIT(pKenshoLexer)
 *  -  void      pKenshoLexer->EXPONENT(pKenshoLexer)
 *  -  void      pKenshoLexer->ESCSEQ(pKenshoLexer)
 *  -  void      pKenshoLexer->WS(pKenshoLexer)
 *  -  void      pKenshoLexer->LINE_COMMENT(pKenshoLexer)
 *  -  void      pKenshoLexer->BLOCK_COMMENT(pKenshoLexer)
 *  -  void      pKenshoLexer->LITERAL_OCT(pKenshoLexer)
 *  -  void      pKenshoLexer->LITERAL_HEX(pKenshoLexer)
 *  -  void      pKenshoLexer->LITERAL_INT(pKenshoLexer)
 *  -  void      pKenshoLexer->LITERAL_FLOAT(pKenshoLexer)
 *  -  void      pKenshoLexer->ID(pKenshoLexer)
 *  -  void      pKenshoLexer->LITERAL_CHAR(pKenshoLexer)
 *  -  void      pKenshoLexer->Tokens(pKenshoLexer)
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

#ifndef	_KenshoLexer_H
#define _KenshoLexer_H
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
typedef struct KenshoLexer_Ctx_struct KenshoLexer, * pKenshoLexer;



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

/** Context tracking structure for KenshoLexer
 */
struct KenshoLexer_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_LEXER    pLexer;


     void (*mT_BOOL)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mT_BYTE)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mT_SHORT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mT_INT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mT_CHAR)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mT_LONG)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mT_FLOAT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mT_DOUBLE)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mT_VOID)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_ADD)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_SUB)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_MUL)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_DIV)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_NOT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_BIT_NOT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_AND)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_OR)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_REM)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_BIT_AND)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_BIT_OR)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_XOR)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_SHIFT_L)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_SHIFT_R)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_USHIFT_R)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mCMP_EQ)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mCMP_NEQ)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mCMP_LT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mCMP_GT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mCMP_LTE)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mCMP_GTE)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mOP_ASSIGN)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mK_IF)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mK_ELSE)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mK_WHILE)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mLITERAL_TRUE)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mLITERAL_FALSE)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mK_NATIVE)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mK_RETURN)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mK_NEW)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mK_DELETE)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mK_THIS)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mK_STRUCT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mK_STATIC)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mBRACE_L)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mBRACE_R)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mPAREN_L)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mPAREN_R)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mCOMMA)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mSEMICOLON)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mLETTER)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mDIGIT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mPOSDIGIT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mHEXDIGIT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mEXPONENT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mESCSEQ)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mWS)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mLINE_COMMENT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mBLOCK_COMMENT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mLITERAL_OCT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mLITERAL_HEX)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mLITERAL_INT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mLITERAL_FLOAT)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mID)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mLITERAL_CHAR)	(struct KenshoLexer_Ctx_struct * ctx);
     void (*mTokens)	(struct KenshoLexer_Ctx_struct * ctx);    const char * (*getGrammarFileName)();
    void	    (*free)   (struct KenshoLexer_Ctx_struct * ctx);
        
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pKenshoLexer KenshoLexerNew         (pANTLR3_INPUT_STREAM instream);
ANTLR3_API pKenshoLexer KenshoLexerNewSSD      (pANTLR3_INPUT_STREAM instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the lexer will work with.
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

/* End of token definitions for KenshoLexer
 * =============================================================================
 */
/** \} */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
