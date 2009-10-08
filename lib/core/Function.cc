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

#include <kensho/ast/Function.hpp>
#include <kensho/ast/Return.hpp>
#include <kensho/ast/VariableDefinition.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/error.hpp>

#include <llvm/Value.h>
#include <llvm/Function.h>

using namespace kensho;

	void ast::Function::assemble(ast::ModuleBuilder& mb) {
		llvm::Function* fun = llvm::cast<llvm::Function>(value);
		assert(fun != NULL);
		uint32_t numParams = fun->getFunctionType()->getNumParams();
		assert(numParams == parameterNames.size());
		assert(numParams == parameterTypes.size());

		// emit body
		llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create("entry", fun);
		mb.getIRBuilder().SetInsertPoint(entryBlock);

		// set parameter names and emit declarations
		llvm::Function::arg_iterator arg = NULL;
		int32_t i = 0;
		for (arg = fun->arg_begin(); arg != fun->arg_end(); ++arg, ++i) {
			std::string str = parameterNames.at(i);
			arg->setName(str.c_str());
			VariableDefinition* vardef = new ast::VariableDefinition(
				str, parameterTypes.at(i));
			llvm::Value* ptr = vardef->emit(mb);
			mb.getIRBuilder().CreateStore(arg, ptr);
		}

		uint32_t numStats = body.size();
		for (uint32_t i = 0; i < numStats; ++i) {
			body.at(i)->assemble(mb);
			if (body.at(i)->isReturnStatement() && i < body.size() - 1) {
				throw(ParseError("unreachable code after return statement",
					getLine(), getOffset()));
			}
		}

		// if we have no return statement and the function
		// returns void we synthesize a return statement
		// otherwise bail out with error
		if (numStats == 0 || (numStats > 0
			&& body.at(numStats - 1)->isReturnStatement() == false)) {
			if (type->isVoid()) {
				Return* returnStat = new Return(NULL);
				returnStat->emit(mb);
			}
			else {
				throw(ParseError("missing return statement in non-void function " + name,
					getLine(), getOffset()));
			}
		}
		else {
			Return* ret = dynamic_cast<Return*>(body.at(numStats - 1));
			// verify type match
			Node* ex = ret->getExpression();
			if (ex != NULL && type->isVoid()) {
				throw(ParseError("void function " + name + " cannot return non-void type",
					getLine(), getOffset()));
			}
			else if (ex != NULL && ex->getValue()->getType() != type->getAssemblyType()) {
				throw(ParseError("type mismatch in return statement in function " + name,
					getLine(), getOffset()));
			}
		}

		// clear all symbols we declared in this function
		mb.clearSymbolScope();
	}
