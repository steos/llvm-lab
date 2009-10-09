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
#include <kensho/ast/Variable.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/error.hpp>

#include <llvm/Value.h>

using namespace kensho;

	void ast::BinaryExpression::assemble(ast::ModuleBuilder& mb) {
		const llvm::Type* typeLeft = NULL;
		const llvm::Type* typeRight = NULL;

		// emit right side
		llvm::Value* valRight = right->emit(mb);
		typeRight = valRight->getType();
		llvm::Value* valLeft = left->emit(mb);
		typeLeft = valLeft->getType();

		if (typeLeft != typeRight) {
			try {
				llvm::Value* castval = NULL;
				if (typeLeft < typeRight) {
					castval = Type::cast(valLeft, typeRight, mb);
//					castval = implicitTypeCast(
//						typeLeft, typeRight, valLeft, mb);
				}
				else if (typeLeft > typeRight) {
//					castval = implicitTypeCast(
//						typeRight, typeLeft, valRight, mb);
					castval = Type::cast(valRight, typeLeft, mb);
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
