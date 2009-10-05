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

#ifndef KENSHO_AST_VARIABLE_HPP_
#define KENSHO_AST_VARIABLE_HPP_

#include <kensho/ast/Node.hpp>

namespace kensho {
namespace ast {

	/*
	 * this node represents access to a variable
	 */
	class Variable : public Node {
	private:
		std::string name;
	protected:
		virtual void assemble(ModuleBuilder&);
	public:
		Variable(std::string name) : name(name) {};
		std::string getName();
	};

	inline std::string Variable::getName() {
		return name;
	}

}} // end ns


#endif /* KENSHO_AST_VARIABLE_HPP_ */
