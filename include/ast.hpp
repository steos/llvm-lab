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

#ifndef AST_HPP_
#define AST_HPP_

namespace kensho {
namespace ast {

	/*
	 * manages the compilation of an AST
	 */
	class Compiler;

	/*
	 * the interface every AST node has to implement
	 */
	class Node {
	public:
		virtual llvm::Value* emit(Compiler& com) = 0;
		virtual ~Node();
	};

	class Variable : public Node {

	};

	class VariableDefinition : public Node {

	};

	class UnaryExpression : public Node {

	};

	class BinaryExpression : public Node {

	};

	class Literal : public Node {

	};

	class FunctionSignature : public Node {

	};

	class Function : public Node {

	};
}}


#endif /* AST_HPP_ */
