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

#ifndef KENSHO_AST_UNARYEXPRESSION_HPP_
#define KENSHO_AST_UNARYEXPRESSION_HPP_

#include <kensho/ast/tokens.hpp>
#include <kensho/ast/Node.hpp>

namespace kensho {
namespace ast {

	/*
	 * an unary expression
	 */
	class UnaryExpression : public Node {
	private:
		OperatorToken token;
		Node* expression;
	protected:
		virtual void assemble(ModuleBuilder&);
	public:
		UnaryExpression(OperatorToken token, Node* expression) :
			token(token), expression(expression) {};
	};



}}


#endif /* KENSHO_AST_UNARYEXPRESSION_HPP_ */
