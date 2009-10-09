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

#ifndef KENSHO_AST_VARIABLEDEFINITION_HPP_
#define KENSHO_AST_VARIABLEDEFINITION_HPP_

#include <kensho/ast/Symbol.hpp>

namespace kensho {
namespace ast {

	/*
	 * represents a variable declaration
	 */
	class VariableDefinition : public Symbol {
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		VariableDefinition(std::string name, Type* type) :
			Symbol(name, type) {};

		void setName(std::string name);

		virtual Type* getType() {
			return type;
		}
	};

	inline void VariableDefinition::setName(std::string name) {
		this->name = name;
	}

}} // end ns

#endif /* KENSHO_AST_VARIABLEDEFINITION_HPP_ */
