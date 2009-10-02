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
#include <llvm/Type.h>
#include <iostream>
#include <KenshoLexer.h>

using namespace kensho;


	/*
	 * implementation of VariableDefinition
	 */
	void ast::VariableDefinition::assemble(ast::ModuleBuilder& mb) {
		// TODO
	}

	/*
	 * implementation of Variable
	 */
	void ast::Variable::assemble(ast::ModuleBuilder& mb) {
		// TODO
	}

	/*
	 * implementation of Literal
	 */
	void ast::Literal::assemble(ast::ModuleBuilder& mb) {
		// TODO
	}

	/*
	 * implementation of BinaryExpression
	 */
	void ast::BinaryExpression::assemble(ast::ModuleBuilder& mb) {
		// TODO
	}

	/*
	 * implementation of UnaryExpression
	 */
	void ast::UnaryExpression::assemble(ast::ModuleBuilder& mb) {
		// TODO
	}

	/*
	 * implementation of Function
	 */
	void ast::Function::assemble(ast::ModuleBuilder& mb) {
		// TODO
	}

	/*
	 * implementation of Call
	 */
	void ast::Call::assemble(ast::ModuleBuilder& mb) {
		// TODO
	}

	/*
	 * implementation of Cast
	 */
	void ast::Cast::assemble(ast::ModuleBuilder& mb) {
		// TODO
	}

	/*
	 * implementation of While
	 */
	void ast::While::assemble(ast::ModuleBuilder& mb) {
		// TODO
	}

	/*
	 * implementation of Conditional
	 */
	void ast::Conditional::assemble(ast::ModuleBuilder& mb) {
		// TODO
	}

	/*
	 * implementation of Callable
	 */
	void ast::Callable::assemble(ast::ModuleBuilder& mb) {
		// TODO
	}

	/*
	 * implementation of ModuleBuilder
	 */
	void ast::ModuleBuilder::build() {
		int numFuns = functions->size();
		for (int i = 0; i < numFuns; ++i) {
			Callable* cb = functions->at(i);
			std::cout << "function \"" << cb->getName() << "\"\n";
			// TODO cb->emit(*this);
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
