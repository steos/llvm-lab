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
 
tree grammar KenshoTreeParser;

options 
{
	language 		= C;
	output 			= AST;
	ASTLabelType 	= pANTLR3_BASE_TREE; 
	tokenVocab		= Kensho;
}

@preincludes {
	#include <ast.hpp>
	#include <vector>
}

program returns [std::vector<kensho::ast::Callable*>* functions]
@init {
	$functions = new std::vector<kensho::ast::Callable*>();
}
	:	( 	function {
			$functions->push_back($function.node);
		} 
		| 	kenniFunction { 
				$functions->push_back($kenniFunction.node); 
			} 
		)*
	;
	
kenniFunction returns [kensho::ast::Callable* node]
	:	^(KENNIDEF 
			t=functionType 
			n=ID {
				std::string name((char*)$n->getText($n)->chars);
				$node = new kensho::ast::Callable(name, $t.tree->getType($t.tree));
				$node->setSourcePosition($n.line, $n.pos);
			}
			( type {
				$node->addParameter($type.tree->getType($type.tree));
			})*
		) 
	;
	
function returns [kensho::ast::Function* node]
	:	^(FUNDEF 
			signature { 
				$node = $signature.node;
			} 
			( statement {
				$node->addBodyNode($statement.node);
			})*
		)
	;	
	
signature returns [kensho::ast::Function* node]
@init { uint32_t paramCount = 0; }
	:	^(FUNSIG 
			t=functionType 
			n=ID {
				std::string name((char*)$n->getText($n)->chars);
				$node = new kensho::ast::Function(name, $t.tree->getType($t.tree));
				$node->setSourcePosition($n.line, $n.pos);
			} 
			params[$node, paramCount++]*
		) 
	;
	
functionType
	:	T_VOID | type
	;	
		
params[kensho::ast::Function* node, uint32_t index]
	:	^(ARGDEF type n=ID) {
			std::string name((char*)$n->getText($n)->chars);
			$node->addParameter(name, $type.tree->getType($type.tree));
			// add variable definition for the parameter
			$node->addBodyNode(new kensho::ast::ParameterDefinition(
				name, $node, index, $type.tree->getType($type.tree)
			));
		}
	;

statement returns [kensho::ast::Node* node]
	:	variable { $node = $variable.node; }
	|	expression { $node = $expression.node; }
	|	ifStat { $node = $ifStat.node; }
	|	whileStat { $node = $whileStat.node; }
	|	returnStatement { $node = $returnStatement.node; }
	;
	
returnStatement returns [kensho::ast::Return* node]
@after {
	if ($node == NULL) {
		$node = new kensho::ast::Return(NULL);
	}
}
	:	^(K_RETURN 
			( ex=expression { 
				$node = new kensho::ast::Return($ex.node); 
				pANTLR3_COMMON_TOKEN tok = $ex.tree->getToken($ex.tree);
				$node->setSourcePosition(tok->getLine(tok), tok->getStartIndex(tok));
			})?
		)
	;	
	
ifStat returns [kensho::ast::Conditional* node]
	:	^(K_IF 
			ex=expression {
				$node = new kensho::ast::Conditional($ex.node);
				pANTLR3_COMMON_TOKEN tok = $ex.tree->getToken($ex.tree);
				$node->setSourcePosition(tok->getLine(tok), tok->getStartIndex(tok));
			}
			( statement {
				$node->addTrueBodyNode($statement.node);
			})* 
			( elseIfStat {
				$node->addBranch($elseIfStat.node);
			})* 
			( elseStat[$node] )? 
		)
	;
	
elseIfStat returns [kensho::ast::Conditional* node]
	:	^(ELSEIF 
			ex=expression {
				$node = new kensho::ast::Conditional($ex.node);
				pANTLR3_COMMON_TOKEN tok = $ex.tree->getToken($ex.tree);
				$node->setSourcePosition(tok->getLine(tok), tok->getStartIndex(tok));
			}
			( statement {
				$node->addTrueBodyNode($statement.node);
			})*
		)
	;
	
elseStat[kensho::ast::Conditional* node]
	:	^(K_ELSE 
			( statement {
				$node->addFalseBodyNode($statement.node);
			})*
		)
	;

whileStat returns [kensho::ast::While* node]
	:	^(t=K_WHILE 
			ex=expression {
				$node = new kensho::ast::While($ex.node);
				pANTLR3_COMMON_TOKEN tok = $ex.tree->getToken($ex.tree);
				$node->setSourcePosition(tok->getLine(tok), tok->getStartIndex(tok));
			} 
			( s=statement {
				$node->addBodyNode($s.node);
			})*
		)
	;	
	
variable returns [kensho::ast::VariableDefinition* node]
@after {
	std::string name((char*)$n->getText($n)->chars);
	$node = new kensho::ast::VariableDefinition(name, $t.tree->getType($t.tree));
	$node->setSourcePosition($n.line, $n.pos);
}
	:	^(VARDEF t=type n=ID) 
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
	;

expression returns [kensho::ast::Node* node]
	:	name=ID { 
			$node = new kensho::ast::Variable(
				std::string((char*)$name->getText($name)->chars)
			); 
			$node->setSourcePosition($name.line, $name.pos);
		}
	|	^(LIT lit=literal) {
			$node = $lit.node;
		}
	|	call {
			$node = $call.node;
		}
	|	^(UNOP unop unex=expression) {
			pANTLR3_COMMON_TOKEN tok = $unop.tree->getToken($unop.tree);
			$node = new kensho::ast::UnaryExpression(tok->getType(tok), $unex.node); 
			$node->setSourcePosition(tok->getLine(tok), tok->getStartIndex(tok));
		}
	|	^(CAST type castex=expression) {
			pANTLR3_COMMON_TOKEN tok = $type.tree->getToken($type.tree);
			$node = new kensho::ast::Cast(tok->getType(tok), $castex.node);
			$node->setSourcePosition(tok->getLine(tok), tok->getStartIndex(tok));
		}
	|	^(BINOP binop left=expression right=expression) {
			pANTLR3_COMMON_TOKEN tok = $binop.tree->getToken($binop.tree);
			$node = new kensho::ast::BinaryExpression(
				tok->getType(tok), $left.node, $right.node
			);
			$node->setSourcePosition(tok->getLine(tok), tok->getStartIndex(tok));
		}
	;	
	
call returns [kensho::ast::Call* node]
	:	^(CALL 
			name=ID {
				std::string nameStr((char*)$name->getText($name)->chars);
				$node = new kensho::ast::Call(nameStr);   
				$node->setSourcePosition($name.line, $name.pos);
			} 
			( ex=expression {
				$node->addArgument($ex.node);
			})*
		)
	;

literal returns [kensho::ast::Literal* node]
@after
{
	pANTLR3_COMMON_TOKEN token = $literal.tree->getToken($literal.tree);
	$node = new kensho::ast::Literal(
		$literal.tree->getType($literal.tree),
		std::string((char*)token->getText(token)->chars)
	);
	$node->setSourcePosition(token->getLine(token), token->getStartIndex(token));
}
	:	LITERAL_INT
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