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
	#include <kensho/error.hpp>
	
	#include <vector>
}

@includes {
	#include <kensho/ast/Literal.hpp>
}

program returns [kensho::ast::ModuleBuilder* builder] 
scope {
	kensho::ast::ModuleBuilder* mb;
}
@init {
	$program::mb = new kensho::ast::ModuleBuilder("default");
}
@after { $builder = $program::mb; }
	:	( 	function {
			$program::mb->addFunction($function.node);
		} 
		| 	kenniFunction { 
				$program::mb->addFunction($kenniFunction.node); 
			} 
		|	structDecl {
				$program::mb->addStruct($structDecl.node);			
			}
		)*
	;
	
kenniFunction returns [kensho::ast::Callable* node]
	:	^(KENNIDEF 
			t=functionType 
			n=ID {
				std::string name((char*)$n->getText($n)->chars);
				$node = new kensho::ast::Callable(name, $t.ty);
				$node->setSourcePosition($n.line, $n.pos);
			}
			( type {
				$node->addParameter($type.ty);
			})*
		) 
	;
	
structDecl returns [kensho::ast::Struct* node]
	:	^(K_STRUCT 
			n=ID {
				std::string name((char*)$n->getText($n)->chars);
				$node = new kensho::ast::Struct(name);
				$node->setSourcePosition($n.line, $n.pos);
			} 
			( structBodyDecl[$node] )*
		)
	;
	
structBodyDecl[kensho::ast::Struct* parent]
	:	variable {
		$parent->addVariableDefinition($variable.node);
	}
	|	structFunction[$parent]
	|	ctor=structCtor[$parent->getName()] {
			if ($parent->getConstructor() != NULL) {
				throw(kensho::ParseError("more than one constructor defined in struct " 
					+ $parent->getName()));
			}
			$parent->setConstructor($ctor.node);
		}
	|	structDtor[$parent]
	;
	
structCtor[std::string parentName] returns [kensho::ast::Function* node]
@init { uint32_t paramCount = 0; }
	:	^(CTOR {
			$node = new kensho::ast::Function("new", $program::mb->createType(T_VOID, ""));
		}
			params[$node, paramCount++]* 
			( statement {
				$node->addBodyNode($statement.node);
			})*
		)
	;
	
structDtor[kensho::ast::Struct* parent]
	:	^(DTOR {
			if ($parent->hasDestructor()) {
				throw(kensho::ParseError("more than one destructor defined in struct "
					+ $parent->getName()));
			}
		}
			( stat=statement {
				$parent->addDestructorBodyNode($stat.node);
			})* 
		)
	;
	
structFunction[kensho::ast::Struct* parent]
@init {
	bool staticDef = false;
}
@after {
	$parent->addFunction($sig.node, staticDef);
}
	:	^(STRUCTFUN 
			( structFunMods { if ($structFunMods.staticDef) staticDef = true; } )* 
			sig=signature 
			( statement {
				$sig.node->addBodyNode($statement.node);
			})*
		) 
	;
	
structFunMods returns [bool staticDef]
	:	^( MODS K_STATIC { $staticDef = true; } )
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
				$node = new kensho::ast::Function(name, $t.ty);
				$node->setSourcePosition($n.line, $n.pos);
			} 
			params[$node, paramCount++]*
		) 
	;
	
functionType returns [kensho::ast::Type* ty]
	:	T_VOID {
			$ty = $program::mb->createType(T_VOID, "");
		} 
	| 	type { $ty = $type.ty; }
	;	
		
params[kensho::ast::Function* node, uint32_t index]
	:	^(ARGDEF type n=ID) {
			std::string name((char*)$n->getText($n)->chars);
			$node->addParameter(name, $type.ty);
		}
	;

statement returns [kensho::ast::Node* node]
	:	variable { $node = $variable.node; }
	|	expression { $node = $expression.node; }
	|	ifStat { $node = $ifStat.node; }
	|	whileStat { $node = $whileStat.node; }
	|	returnStatement { $node = $returnStatement.node; }
	|	deleteStatement { $node = $deleteStatement.node; }
	;
	
deleteStatement returns [kensho::ast::Delete* node]
	:	^(K_DELETE n=ID) {
			$node = new kensho::ast::Delete(std::string((char*)$n->getText($n)->chars));
		}
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
				$node->setSourcePosition(tok->getLine(tok), tok->getCharPositionInLine(tok));
			})?
		)
	;	
	
ifStat returns [kensho::ast::Conditional* node]
	:	^(K_IF 
			ex=expression {
				$node = new kensho::ast::Conditional($ex.node);
				pANTLR3_COMMON_TOKEN tok = $ex.tree->getToken($ex.tree);
				$node->setSourcePosition(tok->getLine(tok), tok->getCharPositionInLine(tok));
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
				$node->setSourcePosition(tok->getLine(tok), tok->getCharPositionInLine(tok));
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
				$node->setSourcePosition(tok->getLine(tok), tok->getCharPositionInLine(tok));
			} 
			( s=statement {
				$node->addBodyNode($s.node);
			})*
		)
	;	
	
variable returns [kensho::ast::VariableDefinition* node]
@after {
	std::string name((char*)$n->getText($n)->chars);
	std::string text((char*)$t.tree->getText($t.tree)->chars);
	$node = new kensho::ast::VariableDefinition(name, $t.ty);
	$node->setSourcePosition($n.line, $n.pos);
}
	:	^(VARDEF t=type n=ID) 
	;
	
type returns [kensho::ast::Type* ty]
@after	{
	std::string text;
	pANTLR3_STRING str = $type.tree->getText($type.tree);
	if (str != NULL) {
		text = std::string((char*)str->chars); 
	}
	$ty = $program::mb->createType(
		$type.tree->getType($type.tree), text
	);
}
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
	|	ctorCall {
			$node = $ctorCall.node;
		}
	|	^(UNOP unop unex=expression) {
			pANTLR3_COMMON_TOKEN tok = $unop.tree->getToken($unop.tree);
			$node = new kensho::ast::UnaryExpression(tok->getType(tok), $unex.node); 
			$node->setSourcePosition(tok->getLine(tok), tok->getCharPositionInLine(tok));
		}
	|	^(CAST type castex=expression) {
			pANTLR3_COMMON_TOKEN tok = $type.tree->getToken($type.tree);
			$node = new kensho::ast::Cast(tok->getType(tok), $castex.node);
			$node->setSourcePosition(tok->getLine(tok), tok->getCharPositionInLine(tok));
		}
	|	^(BINOP binop left=expression right=expression) {
			pANTLR3_COMMON_TOKEN tok = $binop.tree->getToken($binop.tree);
			$node = new kensho::ast::BinaryExpression(
				tok->getType(tok), $left.node, $right.node
			);
			$node->setSourcePosition(tok->getLine(tok), tok->getCharPositionInLine(tok));
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
	
ctorCall returns [kensho::ast::ConstructorCall* node]
	:	^(K_NEW 
			name=ID {
				std::string nameStr((char*)$name->getText($name)->chars);
				$node = new kensho::ast::ConstructorCall(nameStr);
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
	$node->setSourcePosition(token->getLine(token), token->getCharPositionInLine(token));
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