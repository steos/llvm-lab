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

#ifndef KENSHO_AST_FUNCTION_HPP_
#define KENSHO_AST_FUNCTION_HPP_

#include <kensho/ast/AbstractFunction.hpp>
#include <kensho/ast/Type.hpp>
#include <kensho/ast/ScopeProvider.hpp>
#include <string>
#include <vector>

namespace kensho {
namespace ast {

	/*
	 * A top-level function, i.e. it provides it's own symbol scope
	 */
	class Function : public AbstractFunction, public ScopeProvider {
	private:

		Scope symbols;

	public:

		Function(std::string name, Type* type) :
			AbstractFunction(name, type) {};

		Function(std::string name, Type* type, std::vector<Buildable*> body) :
			AbstractFunction(name, type, body) {};

		virtual void assemble(ModuleBuilder& mb);

		virtual Scope& getScope() {
			return symbols;
		}
	};
}}

#endif /* KENSHO_AST_FUNCTION_HPP_ */
