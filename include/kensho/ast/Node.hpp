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

#ifndef KENSHO_AST_NODE_HPP_
#define KENSHO_AST_NODE_HPP_

#include <kensho/ast/Buildable.hpp>
#include <llvm/Value.h>

namespace kensho {
namespace ast {

	/*
	 * Node is the base class of all Buildables that result in a
	 * value, i.e. can be used in an operation like assignment.
	 */
	class Node : public Buildable {
	protected:

		/*
		 * the assembly value of this node
		 */
		llvm::Value* value;

	public:

		/*
		 * retrieves the node value
		 */
		llvm::Value* getValue() {
			return value;
		}

		virtual llvm::Value* emit(ModuleBuilder& mb) {
			assemble(mb);
			assert(value != NULL);
			return value;
		}
	};

}} // end ns

#endif /* KENSHO_AST_NODE_HPP_ */
