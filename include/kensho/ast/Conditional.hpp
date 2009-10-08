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

#ifndef KENSHO_AST_CONDITIONAL_HPP_
#define KENSHO_AST_CONDITIONAL_HPP_

#include <kensho/ast/Node.hpp>
#include <vector>

namespace kensho {
namespace ast {

	/*
	 * conditional statement, i.e. if-else
	 */
	class Conditional : public Buildable {
	private:
		Node* expression;
		std::vector<Buildable*> trueBody;
		std::vector<Buildable*> falseBody;
		std::vector<Conditional*> branches;
	public:
		Conditional(Node* expression) : expression(expression) {};

		virtual void assemble(ModuleBuilder& mb);

		void addTrueBodyNode(Buildable* node) {
			trueBody.push_back(node);
		}

		void addFalseBodyNode(Buildable* node) {
			falseBody.push_back(node);
		}

		void addBranch(Conditional* c) {
			branches.push_back(c);
		}
	};



}} // end ns

#endif /* KENSHO_AST_CONDITIONAL_HPP_ */
