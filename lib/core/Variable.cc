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

#include <kensho/ast/Variable.hpp>
#include <kensho/ast/VariableDefinition.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/error.hpp>

using namespace kensho;

	void ast::Variable::assemble(ast::ModuleBuilder& mb) {
		if (!mb.isDeclared(name)) {
			throw(ParseError("variable " + name + " is not declared",
				getLine(), getOffset()));
		}

		VariableDefinition* var = mb.getSymbol(name);
		if (var == NULL) {
			throw(ParseError("variable " + name + " was not declared",
				getLine(), getOffset()));
		}

		value = mb.getIRBuilder().CreateLoad(var->getValue(), name.c_str());
	}
