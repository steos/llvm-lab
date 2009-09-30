grammar Kensho;

options 
{
	language 		= C;
	output 			= AST;
	ASTLabelType 	= pANTLR3_BASE_TREE; 
}

tokens 
{
	T_BOOL 			= 'bool';
	T_BYTE 			= 'byte';
	T_SHORT 		= 'short';
	T_INT 			= 'int';
	T_CHAR 			= 'char';
	T_LONG 			= 'long';
	T_FLOAT 		= 'float';
	T_DOUBLE 		= 'double';
	T_VOID 			= 'void';
	
	OP_ADD 			= '+';
	OP_SUB 			= '-';
	OP_MUL 			= '*';
	OP_DIV 			= '/';
	OP_NOT 			= '!';
	OP_BIT_NOT 		= '~';
	OP_AND			= '&&';
	OP_OR			= '||';
	
	// TODO
	// OP_BIT_AND		= '&';
	// OP_BIT_OR		= '|';
	// OP_XOR			= '^';
	
	CMP_EQ			= '==';
	CMP_NEQ			= '!=';
	CMP_LT			= '<';
	CMP_GT			= '>';
	CMP_LTE			= '<=';
	CMP_GTE			= '>=';
	
	OP_ASSIGN		= '=';

	// TODO
	// OP_DOT			= '.';
	
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
}

@parser::includes
{
	#include <vector>
	#include <cassert>
}

@parser::members
{
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
			
			case OP_ADD:
			case OP_SUB:	return 2;
				
			case CMP_LT:
			case CMP_LTE:
			case CMP_GT:	
			case CMP_GTE:	return 3;
				
			case CMP_EQ:
			case CMP_NEQ:	return 4;	
				
			case OP_AND:	return 5;
			case OP_OR:		return 6;
			
			case OP_ASSIGN:	return 7;
		
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
		//pANTLR3_BASE_TREE objRoot = (pANTLR3_BASE_TREE)ADAPTOR->nilNode(ADAPTOR);
		//objRoot = (pANTLR3_BASE_TREE)ADAPTOR->becomeRootToken(
		//	ADAPTOR, operators.at(pivot), objRoot);
		//assert(objRoot != NULL);
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

fragment
LETTER	
	:	'a'..'z' | 'A'..'Z'
	;

fragment
DIGIT
	:	'0'..'9'
	;
	
fragment
HEXDIGIT
	:	DIGIT | 'a'..'f' | 'A'..'F'
	;
	
fragment
EXPONENT
	:	( 'e' | 'E' ) ( '+' | '-' )? DIGIT+
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
	:	DIGIT+ EXPONENT? ( 'l' | 'L' | 'f' | 'F' | 'd' | 'D' )?
	;

LITERAL_FLOAT
	:	DIGIT* '.' DIGIT+ EXPONENT? ( 'f' | 'F' | 'd' | 'D' )?
	;
		
ID	
	:	LETTER ( LETTER | DIGIT )*
	;
	
program
	:	function*
	;
	
function
	:	signature t=BRACE_L statement* BRACE_R
		-> ^(FUNDEF[$t, "FUNDEF"] signature statement*)
	;	
	
signature
	:	functionType ID t=PAREN_L params? PAREN_R
		-> ^(FUNSIG[$t, "FUNSIG"] functionType ID params?) 
	;
	
functionType
	:	T_VOID | type
	;	
		
params
	:	type t=ID ( COMMA type ID )*
		-> ^(ARGDEF[$t, "ARGDEF"] type ID)+
	;

statement
	:	variable SEMICOLON!
	|	expression SEMICOLON!
	;
	
variable
	:	type t=ID ( OP_ASSIGN expression )?
		-> 	^(VARDEF[$t, "VARDEF"] type ID) 
			^(BINOP OP_ASSIGN ID expression)? 
	;

args
	:	expression ( COMMA expression )*
	;

type
	:	T_BOOL
	|	T_BYTE
	|	T_SHORT
	|	T_INT
	|	T_LONG
	|	T_FLOAT
	|	T_DOUBLE
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
	|	unop atom -> ^(UNOP unop atom)
	;
	
atom
	:	literal -> ^(LIT literal)
	|	call
	|	ID
	|	PAREN_L! expression PAREN_R!
	;
	
call
	:	ID PAREN_L args? PAREN_R
	;

literal
	:	 
		LITERAL_INT
	|	LITERAL_OCT
	|	LITERAL_HEX
	|	LITERAL_FLOAT
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
	|	OP_ASSIGN
	|	OP_AND
	|	OP_OR
	|	CMP_EQ
	|	CMP_NEQ
	|	CMP_GT
	|	CMP_GTE
	|	CMP_LT
	|	CMP_LTE
	;
