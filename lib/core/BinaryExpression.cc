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

#include <kensho/ast/BinaryExpression.hpp>
#include <kensho/ast/VariableDefinition.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/error.hpp>

#include <llvm/Value.h>

#include <antlr.hpp>

using namespace kensho;

	void ast::BinaryExpression::assemble(ast::ModuleBuilder& mb) {
		const llvm::Type* typeLeft = NULL;
		const llvm::Type* typeRight = NULL;

		// emit right side
		llvm::Value* valRight = right->emit(mb);
		typeRight = valRight->getType();

		// special handling for assignment
		if (token == OP_ASSIGN) {
			Variable* var = dynamic_cast<Variable*>(left);
			if (var == NULL) {
				throw(ParseError("can only assign to variables",
					getLine(), getOffset()));
			}
			if (!mb.isDeclared(var->getName())) {
				throw(ParseError("variable " + var->getName() + " is not declared",
					getLine(), getOffset()));
			}
			VariableDefinition* vardef = dynamic_cast<VariableDefinition*>(
				mb.getSymbol(var->getName()));
			assert(vardef != NULL);
			if (vardef->getAssemblyType() != typeRight) {
				try {
					llvm::Value* castVal = implicitTypeCast(
						typeRight, vardef->getAssemblyType(), valRight, mb);

					assert(castVal != NULL);
					valRight = castVal;
				}
				catch (ParseError& err) {
					throw(ParseError(err.getMessage(), getLine(), getOffset()));
				}
			}
			value = mb.getIRBuilder().CreateStore(valRight, vardef->getValue());
			// we're done for variable assignments
			return;
		}

		llvm::Value* valLeft = left->emit(mb);
		typeLeft = valLeft->getType();

		if (typeLeft != typeRight) {
			try {
				llvm::Value* castval = NULL;
				if (typeLeft < typeRight) {
					castval = implicitTypeCast(
						typeLeft, typeRight, valLeft, mb);
				}
				else if (typeLeft > typeRight) {
					castval = implicitTypeCast(
						typeRight, typeLeft, valRight, mb);
				}
				assert(castval != NULL);
				valRight = castval;
			}
			catch (ParseError& err) {
				throw(ParseError(err.getMessage(), getLine(), getOffset()));
			}
		}

		value = mb.createBinaryOperator(token, valLeft, valRight);
	}
