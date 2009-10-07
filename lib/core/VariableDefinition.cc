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

#include <kensho/ast/VariableDefinition.hpp>
#include <kensho/ast/Struct.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/error.hpp>
#include <kensho/ast/util.hpp>
#include <llvm/TypeSymbolTable.h>

using namespace kensho;

	void ast::VariableDefinition::assemble(ast::ModuleBuilder& mb) {
		if (mb.isDeclared(name)) {
			throw(ParseError("symbol " + name + " is already declared",
				getLine(), getOffset()));
		}
		const llvm::Type* ty = type->getAssemblyType();
		value = mb.getIRBuilder().CreateAlloca(ty, 0, name.c_str());
		mb.declareSymbol(this);
	}
