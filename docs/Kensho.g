/* This file is part of Kensho.
 * Kensho is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Kensho is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Kensho.  If not, see <http://www.gnu.org/licenses/>.
 */
 
grammar Kensho;

options 
{
	language 		= C;
	output 			= AST;
	ASTLabelType 	= pANTLR3_BASE_TREE; 
}

tokens 
{
	// types
	T_BOOL 			= 'bool';
	T_BYTE 			= 'byte';
	T_SHORT 		= 'short';
	T_INT 			= 'int';
	T_CHAR 			= 'char';
	T_LONG 			= 'long';
	T_FLOAT 		= 'float';
	T_DOUBLE 		= 'double';
	T_VOID 			= 'void';
	
	// operators
	OP_ADD 			= '+';
	OP_SUB 			= '-';
	OP_MUL 			= '*';
	OP_DIV 			= '/';
	OP_NOT 			= '!';
	OP_BIT_NOT 		= '~';
	OP_AND			= '&&';
	OP_OR			= '||';
	OP_REM			= '%';
	
	OP_BIT_AND		= '&';
	OP_BIT_OR		= '|';
	OP_XOR			= '^';
	OP_SHIFT_L		= '<<';
	OP_SHIFT_R		= '>>';
	OP_USHIFT_R		= '>>>';
	
	CMP_EQ			= '==';
	CMP_NEQ			= '!=';
	CMP_LT			= '<';
	CMP_GT			= '>';
	CMP_LTE			= '<=';
	CMP_GTE			= '>=';
	
	OP_ASSIGN		= '=';

	// TODO
	// OP_DOT			= '.';
	
	// keywords
	K_IF			= 'if';
	K_ELSE			= 'else';
	K_WHILE			= 'while';
	LITERAL_TRUE	= 'true';
	LITERAL_FALSE	= 'false';
	K_NATIVE		= 'native';
	K_RETURN		= 'return';
	K_NEW			= 'new';
	K_DELETE		= 'delete';
	K_THIS			= 'this';
	K_STRUCT		= 'struct';
	K_STATIC		= 'static';
	
	// misc
	BRACE_L			= '{';
	BRACE_R			= '}';
	PAREN_L			= '(';
	PAREN_R			= ')';
	COMMA			= ',';
	SEMICOLON		= ';';
	
	// imaginary
	FUNDEF;
	FUNSIG;
	ARGDEF;
	BINOP;
	LIT;
	UNOP;
	VARDEF;
	CALL;
	CAST;
	KENNIDEF;
	ELSEIF;
	STRUCTFUN;
	MODS;
	CTOR;
	DTOR;
}

@parser::preincludes {
	#include <kensho/error.hpp>
	char* antlrTokenName(int type);
}

@parser::includes
{
	#include <vector>
	#include <cassert>
	#include <iostream>
	
	extern "C" void kenshoAntlrErrorReporter(pANTLR3_BASE_RECOGNIZER rec, pANTLR3_UINT8* tokens);
}

@parser::apifuncs {
	RECOGNIZER->displayRecognitionError = kenshoAntlrErrorReporter;
}

@parser::members
{
	extern "C" void kenshoAntlrErrorReporter(pANTLR3_BASE_RECOGNIZER rec, pANTLR3_UINT8* tokens) {
		assert(rec->state->exception->message != NULL);
	
		std::string err((char*)rec->state->exception->message);
		std::string exp(antlrTokenName(rec->state->exception->expecting));
		
		throw(kensho::ParseError(
			err + ", expected " + exp, 
			rec->state->exception->line,
			rec->state->exception->charPositionInLine
		));
	}

	char* antlrTokenName(int type) {
		return (char*)KenshoParserTokenNames[type];
	}
	
	bool isRightAssociative(ANTLR3_UINT32 type) {
		switch (type) {
			case OP_ASSIGN:
				return true;
		}
		return false;
	}
	
	ANTLR3_UINT32 getOperatorPrecedence(ANTLR3_UINT32 type) {
		switch (type) {
			// note: lowest number binds most tightly i.e. has highest precedence
			case OP_MUL:
			case OP_DIV:		
			case OP_REM:		return 1;
			
			case OP_SUB:		return 2;
			case OP_ADD:		return 3;
			
			case OP_SHIFT_L:
			case OP_SHIFT_R:
			case OP_USHIFT_R:	return 4;
			
			case CMP_LT:
			case CMP_LTE:
			case CMP_GT:	
			case CMP_GTE:		return 5;
				
			case CMP_EQ:
			case CMP_NEQ:		return 6;
			
			case OP_BIT_AND:	return 7;
			case OP_XOR:		return 8;
			case OP_BIT_OR:		return 9;
				
			case OP_AND:		return 10;
			case OP_OR:			return 11;
			
			case OP_ASSIGN:		return 12;
		
			default:
				// missing switch case
				assert(false);
				return 0;
		}
	}
	
	int findPivot(std::vector<pANTLR3_COMMON_TOKEN> ops, int start, int stop) {
		int pivot = start;
		ANTLR3_UINT32 pivotRank = getOperatorPrecedence(ops.at(pivot)->getType(ops.at(pivot)));
		for (int i = start; i <= stop; i++) {
			ANTLR3_UINT32 type = ops.at(i)->getType(ops.at(i));
			ANTLR3_UINT32 currentRank = getOperatorPrecedence(type);
			bool rightAssoc = isRightAssociative(type);
			if (currentRank > pivotRank || (currentRank == pivotRank && rightAssoc)) {
				pivot = i;
				pivotRank = currentRank;
			}
		}
		return pivot;
	}
	
	pANTLR3_BASE_TREE createPrecedenceTree(
		pKenshoParser ctx,
		std::vector<pANTLR3_BASE_TREE> expressions,
		std::vector<pANTLR3_COMMON_TOKEN> operators,
		int start, 
		int stop) {
		if (stop == start) {
			return expressions.at(start);
		}
		assert(expressions.size() > 0);
		assert(operators.size() > 0);
		assert(expressions.size() == operators.size() + 1);
		
		int pivot = findPivot( operators, start, stop - 1 );
		pANTLR3_COMMON_TOKEN rootTok = ADAPTOR->createToken(ADAPTOR, BINOP, (uint8_t*)"BINOP");
		pANTLR3_BASE_TREE root = (pANTLR3_BASE_TREE)ADAPTOR->nilNode(ADAPTOR);
		root = (pANTLR3_BASE_TREE)ADAPTOR->becomeRootToken(
			ADAPTOR, rootTok, root);	
		ADAPTOR->addChildToken(ADAPTOR, root, operators.at(pivot));
		ADAPTOR->addChild(ADAPTOR, root, createPrecedenceTree(
			ctx, expressions, operators, start, pivot));
		ADAPTOR->addChild(ADAPTOR, root, createPrecedenceTree(
			ctx, expressions, operators, pivot + 1, stop));
		return root;
	} 
	
	pANTLR3_BASE_TREE createPrecedenceTree(
		pKenshoParser ctx,
		std::vector<pANTLR3_BASE_TREE> expressions,
		std::vector<pANTLR3_COMMON_TOKEN> operators) {
		return createPrecedenceTree(ctx, expressions, operators, 0, expressions.size() - 1);
	}
}
	
program
	:	( function | kenniFunction | structDecl )*
	;
	
kenniFunction
	:	t=K_NATIVE functionType ID PAREN_L ( type ( COMMA type )* )? PAREN_R SEMICOLON
	->	^(KENNIDEF[$t] functionType ID type*) 
	;
	
function
	:	signature t=BRACE_L statement* BRACE_R
		-> ^(FUNDEF[$t] signature statement*)
	;	
	
signature
	:	functionType ID t=PAREN_L params? PAREN_R
		-> ^(FUNSIG[$t] functionType ID params?) 
	;
	
functionType
	:	T_VOID | type
	;	
		
params
	:	type t=ID ( COMMA type ID )*
		-> ^(ARGDEF[$t] type ID)+
	;
	
structDecl
	:	K_STRUCT ID BRACE_L structBodyDecl* BRACE_R
		-> ^(K_STRUCT ID structBodyDecl*)
	;
	
structBodyDecl
	:	variable SEMICOLON!
	|	structFunction
	|	structCtor
	|	structDtor
	;
	
structCtor
	:	K_NEW PAREN_L params? PAREN_R block
		->	^(CTOR params? block)
	;
	
structDtor
	:	K_DELETE block -> ^(DTOR block)
	;
	
structFunction
	:	structFunMods? signature t=BRACE_L statement* BRACE_R
	->	^(STRUCTFUN[$t] ^(MODS structFunMods)? signature statement*) 
	;
	
structFunMods
	:	K_STATIC
	;

statement
	:	variable SEMICOLON!
	|	expression SEMICOLON!
	|	ifStat
	|	whileStat
	|	returnStatement SEMICOLON!
	|	deleteStatement SEMICOLON!
	;
	
returnStatement
	:	K_RETURN expression?
		->	^(K_RETURN expression?)
	;
	
deleteStatement
	:	K_DELETE^ ID
	;
	
block
	:	BRACE_L! statement* BRACE_R!
	;	
	
ifStat
	:	K_IF PAREN_L expression PAREN_R block 
		elseIfStat*
		elseStat?
		
	->	^(K_IF expression block 
			^(ELSEIF elseIfStat)* 
			^(K_ELSE elseStat)? )
	;
	
elseIfStat
	:	K_ELSE! K_IF! PAREN_L! expression PAREN_R! block
	;

elseStat
	:	K_ELSE! block
	;

whileStat
	:	K_WHILE^ PAREN_L! expression PAREN_R! block
	;	
	
variable
	:	type t=ID ( OP_ASSIGN expression )?
		-> 	^(VARDEF[$t] type ID) 
			^(BINOP OP_ASSIGN ID expression)? 
	;

args
	:	expression ( COMMA! expression )*
	;

type
	:	T_BOOL
	|	T_BYTE
	|	T_SHORT
	|	T_CHAR
	|	T_INT
	|	T_LONG
	|	T_FLOAT
	|	T_DOUBLE
	|	ID
	;

expression
@init
{
	std::vector<pANTLR3_BASE_TREE> expressions;
	std::vector<pANTLR3_COMMON_TOKEN> operators;
}
	:	left=primary { expressions.push_back($left.tree); } 
		( 
			binop right=primary
			{ 
				operators.push_back($binop.start); 
				expressions.push_back($right.tree); 
			} 
		)*
		
		-> { createPrecedenceTree(ctx, expressions, operators) }
	;	
	
primary
	:	atom
	|	parenOrCastExpr
	|	K_NEW ID PAREN_L args? PAREN_R -> ^(K_NEW ID args?)
	;
	
castExpr
	:	PAREN_L type PAREN_R atom -> ^(CAST type atom)
	;	
	
atom
	:	literal -> ^(LIT literal)
	|	call
	|	ID
	|	unop atomOrParenExpr -> ^(UNOP unop atomOrParenExpr)
	;
	
atomOrParenExpr
	:	atom | parenExpr
	;
	
parenOrCastExpr	
	:	( PAREN_L type PAREN_R atom ) => castExpr
	|	parenExpr
	;
	
parenExpr
	:	PAREN_L! expression PAREN_R!
	;
	
call
	:	ID PAREN_L args? PAREN_R
		-> ^(CALL ID args?)
	;

literal
	:	 
		LITERAL_INT
	|	LITERAL_OCT
	|	LITERAL_HEX
	|	LITERAL_FLOAT
	|	LITERAL_TRUE
	|	LITERAL_FALSE
	|	LITERAL_CHAR
	;
	
unop
	:	OP_ADD 
	| 	OP_SUB 
	| 	OP_NOT 
	| 	OP_BIT_NOT
	;
	
binop
	:	OP_ADD 
	| 	OP_SUB 
	| 	OP_MUL 
	| 	OP_DIV
	|	OP_REM
	|	OP_ASSIGN
	|	OP_AND
	|	OP_OR
	|	OP_XOR
	|	OP_BIT_AND
	|	OP_BIT_OR
	|	OP_SHIFT_L
	|	OP_SHIFT_R
	|	OP_USHIFT_R
	|	OP_USHIFT_L
	|	CMP_EQ
	|	CMP_NEQ
	|	CMP_GT
	|	CMP_GTE
	|	CMP_LT
	|	CMP_LTE
	;

	
/****************************************************************************

	lexer rules

 ****************************************************************************/
 
fragment
LETTER	
	:	'a'..'z' | 'A'..'Z'
	;

fragment
DIGIT
	:	'0'..'9'
	;
	
fragment
POSDIGIT
	:	'1'..'9'
	;
	
fragment
HEXDIGIT
	:	DIGIT | 'a'..'f' | 'A'..'F'
	;
	
fragment
EXPONENT
	:	( 'e' | 'E' ) ( '+' | '-' )? DIGIT+
	;
	
fragment
ESCSEQ
	:	'\\' ( 't' | 'n' | 'r' | '\\' | '\'' )
	;
	
WS	
	:	(' ' | '\t' | '\n' | '\r') 
		{ $channel=HIDDEN; }
	;
	
LINE_COMMENT
	:	( '#' | '//' ) ~('\r'|'\n')* '\r'? '\n'
		{ $channel = HIDDEN; }
	;
	
BLOCK_COMMENT
	:	'/*' ( options { greedy=false; } : . )* '*/' 
		{ $channel = HIDDEN; }
	;
	
LITERAL_OCT
	:	'0' ( '0'..'7' )+ 
	;
	
LITERAL_HEX
	:	'0x' ( HEXDIGIT )+
	;
	
LITERAL_INT
	:	( DIGIT | POSDIGIT DIGIT+ ) ( 'l' | 'L' )?
	;

LITERAL_FLOAT
	:	( POSDIGIT DIGIT* | ( POSDIGIT DIGIT+ | DIGIT )? '.' DIGIT+ ) 
		EXPONENT? ( 'f' | 'F' | 'd' | 'D' )?
	;
		
ID	
	:	LETTER ( LETTER | DIGIT )*
	;
	
LITERAL_CHAR
	:	'\'' ( ESCSEQ | ~('\'' | '\r' | '\n' | '\t' | '\\' )) '\''
	;
