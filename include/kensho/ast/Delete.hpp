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

#ifndef KENSHO_AST_DELETE_HPP_
#define KENSHO_AST_DELETE_HPP_

#include <kensho/ast/Node.hpp>

namespace kensho {
namespace ast {

	/**
	 * delete statement
	 */
	class Delete : public Node {
	private:
		std::string name;
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		Delete(std::string name) : name(name) {};
	};

}} // end ns


#endif /* KENSHO_AST_DELETE_HPP_ */
