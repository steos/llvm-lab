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

#include <kensho/ast/UnaryExpression.hpp>
#include <kensho/ast/Literal.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/ast/tokens.hpp>

using namespace kensho;

	void ast::UnaryExpression::assemble(ast::ModuleBuilder& mb) {
		Literal* lit = dynamic_cast<Literal*>(expression);
		// if the expression is a literal float or integer
		// and the unary operator is a + or - it's not an operator
		// but simply a sign so we must treat it as a literal
		if (lit != NULL && (lit->getTokenType() == LitInt
			|| lit->getTokenType() == LitFloat)
			&& (token == OpAdd || token == OpSub)) {
			if (token == OpSub) {
				std::string& text = lit->getText();
				text.insert(text.begin(), '-');
			}
			// ignore OP_ADD
			value = lit->emit(mb);
		}
		else {
			llvm::Value* val = expression->emit(mb);
			if (token == OpNot || token == OpBitNot) {
				value = mb.getIRBuilder().CreateNot(val, "not");
			}
			else if (token == OpSub) {
				// sub from 0 depending on type
				value = mb.getIRBuilder().CreateNeg(val, "neg");
			}
			else {
				// we can ignore OP_ADD
				value = val;
			}
		}
	}
