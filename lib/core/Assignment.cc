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

#include <kensho/ast/Assignment.hpp>
#include <kensho/ast/Variable.hpp>
#include <kensho/ast/VariableDefinition.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/error.hpp>

using namespace kensho;

	void ast::Assignment::assemble(ModuleBuilder& mb) {
		const llvm::Type* typeRight = NULL;

		// emit right side
		llvm::Value* valRight = right->emit(mb);
		typeRight = valRight->getType();

		Variable* var = dynamic_cast<Variable*>(left);
		if (var == NULL) {
			throw(ParseError("can only assign to variables",
				getLine(), getOffset()));
		}

		Symbol* sym = mb.getSymbol(var->getName());

		if (sym == NULL) {
			throw(ParseError("variable " + var->getName() + " is not declared",
				getLine(), getOffset()));
		}

		MutableSymbol* vardef = dynamic_cast<MutableSymbol*>(sym);

		assert(vardef != NULL && "can only assign to mutable symbols");

		const llvm::Type* varType = vardef->getType()->getAssemblyType();
		if (varType != typeRight) {
			try {
				llvm::Value* castVal;
				castVal = Type::cast(valRight, varType, mb);
				assert(castVal != NULL);
				valRight = castVal;
			}
			catch (ParseError& err) {
				throw(ParseError(err.getMessage(), getLine(), getOffset()));
			}
		}

		value = vardef->assembleStore(valRight, mb);
	}
