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

#include <ast.hpp>
#include <error.hpp>
#include <llvm/Type.h>
#include <llvm/Function.h>
#include <iostream>
#include <KenshoLexer.h>

using namespace kensho;


	/*
	 * implementation of VariableDefinition
	 */
	void ast::VariableDefinition::assemble(ast::ModuleBuilder& mb) {
		// TODO
		assert(false && "VariableDefinition::assemble not yet implemented");
	}

	/*
	 * implementation of Variable
	 */
	void ast::Variable::assemble(ast::ModuleBuilder& mb) {
		// TODO
		assert(false && "Variable::assemble not yet implemented");
	}

	/*
	 * implementation of Literal
	 */
	void ast::Literal::assemble(ast::ModuleBuilder& mb) {
		// TODO
		assert(false && "Literal::assemble not yet implemented");
	}

	/*
	 * implementation of BinaryExpression
	 */
	void ast::BinaryExpression::assemble(ast::ModuleBuilder& mb) {
		// TODO
		assert(false && "BinaryExpression::assemble not yet implemented");
	}

	/*
	 * implementation of UnaryExpression
	 */
	void ast::UnaryExpression::assemble(ast::ModuleBuilder& mb) {
		// TODO
		assert(false && "UnaryExpression::assemble not yet implemented");
	}

	/*
	 * implementation of Function
	 */
	void ast::Function::assemble(ast::ModuleBuilder& mb) {
		// emit prototype
		Callable::assemble(mb);
		llvm::Function* fun = llvm::cast<llvm::Function>(value);
		assert(fun != NULL);
		// set parameter names
		llvm::Function::arg_iterator arg = NULL;
		int32_t i = 0;
		for (arg = fun->arg_begin(); arg != fun->arg_end(); ++arg) {
			arg->setName(parameterNames.at(i++).c_str());
		}
		// emit body
		llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create("entry", fun);
		mb.getIRBuilder().SetInsertPoint(entryBlock);

		int32_t numStats = body.size();
		for (int32_t i = 0; i < numStats; ++i) {
			body.at(i)->emit(mb);
		}

		// check if we have a return statement
		Return* ret = numStats > 0 ?
			dynamic_cast<Return*>(body.at(numStats - 1)) : NULL;
		// if we have no return statement and the function
		// returns void we synthesize a return statement
		// otherwise bail out with error
		if (ret == NULL) {
			if (type == T_VOID) {
				Return* returnStat = new Return(NULL);
				returnStat->emit(mb);
			}
			else {
				std::string err = "missing return statement in non-void function ";
				err += name;
				throw(ParseError(err));
			}
		}
		else {
			ret->emit(mb);
			// verify type match
			Node* ex = ret->getExpression();
			if (ex != NULL && type == T_VOID) {
				std::string err = "void function ";
				err += name;
				err += " cannot return non-void type";
				throw(ParseError(err));
			}
			else if (ex != NULL && ex->getValue()->getType() != assemblyType) {
				std::string err = "type mismatch in return statement in function ";
				err += name;
				throw(ParseError(err));
			}
		}
	}

	/*
	 * implementation of Call
	 */
	void ast::Call::assemble(ast::ModuleBuilder& mb) {
		// TODO
		assert(false && "Call::assemble not yet implemented");
	}

	/*
	 * implementation of Cast
	 */
	void ast::Cast::assemble(ast::ModuleBuilder& mb) {
		// TODO
		assert(false && "Cast::assemble not yet implemented");
	}

	/*
	 * implementation of While
	 */
	void ast::While::assemble(ast::ModuleBuilder& mb) {
		// TODO
		assert(false && "While::assemble not yet implemented");
	}

	/*
	 * implementation of Conditional
	 */
	void ast::Conditional::assemble(ast::ModuleBuilder& mb) {
		// TODO
		assert(false && "Conditional::assemble not yet implemented");
	}

	/*
	 * implementation of Callable
	 */
	void ast::Callable::assemble(ast::ModuleBuilder& mb) {
		llvm::FunctionType* funtype = llvm::FunctionType::get(
			assemblyType, parameterTypes, false);
		llvm::Function* fun = llvm::Function::Create(
			funtype, llvm::Function::ExternalLinkage, name, mb.getModule());

		value = fun;
	}

	/*
	 * implementation of Return
	 */
	void ast::Return::assemble(ast::ModuleBuilder& mb) {
		// TODO
		assert(false && "Return::assemble not yet implemented");
	}

	/*
	 * implementation of ModuleBuilder
	 */
	void ast::ModuleBuilder::build() {
		int numFuns = functions->size();
		for (int i = 0; i < numFuns; ++i) {
			Callable* cb = functions->at(i);
			//std::cout << "function \"" << cb->getName() << "\"\n";
			cb->emit(*this);
		}
	}

	const llvm::Type* ast::toAssemblyType(uint32_t type)
	{
		switch (type) {
			case T_VOID:
				return llvm::Type::VoidTy;
			case T_BOOL:
				return llvm::Type::Int1Ty;
			case T_BYTE:
				return llvm::Type::Int8Ty;
			case T_SHORT:
				return llvm::Type::Int16Ty;
			case T_INT:
			case T_CHAR:
				return llvm::Type::Int32Ty;
			case T_LONG:
				return llvm::Type::Int64Ty;
			case T_FLOAT:
				return llvm::Type::FloatTy;
			case T_DOUBLE:
				return llvm::Type::DoubleTy;
			default:
				assert(false && "type not handled");
		}

		return NULL;
	}
