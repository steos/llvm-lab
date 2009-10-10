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

#ifndef KENSHO_AST_CALL_HPP_
#define KENSHO_AST_CALL_HPP_

#include <kensho/ast/Node.hpp>
#include <string>
#include <vector>

namespace kensho {
namespace ast {

	class Callable;

	/**
	 * function call
	 */
	class Call : public Node {
	protected:
		std::string name;
		std::vector<Node*> arguments;
		virtual void assemble(ModuleBuilder& mb);
		virtual std::vector<llvm::Value*> prepareParameters(Callable* fun, ModuleBuilder& mb);
	public:
		Call(std::string name) : name(name) {};
		void addArgument(Node* node) {
			arguments.push_back(node);
		}
		int countArguments() {
			return arguments.size();
		}
	};



}} // end ns


#endif /* KENSHO_AST_CALL_HPP_ */
