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

#include <llvm/Value.h>

namespace kensho {
namespace ast {

	// forward declare ModuleBuilder
	class ModuleBuilder;

	/*
	 * the base class every AST node has to implement
	 */
	class Node {
	protected:
		llvm::Value* value;
		int32_t line;
		int32_t offset;
	protected:
		virtual void assemble(ModuleBuilder&) = 0;
	public:
		Node() : value(NULL), line(0), offset(0) {};
		llvm::Value* emit(ModuleBuilder& mb);
		llvm::Value* getValue();
		void setSourcePosition(int32_t, int32_t);
		int32_t getLine();
		int32_t getOffset();
		virtual bool isReturnStatement();
		virtual ~Node() {};
	};

	inline bool Node::isReturnStatement() {
		return false;
	}

	inline void Node::setSourcePosition(int32_t line, int32_t offset) {
		this->line = line;
		this->offset = offset;
	}

	inline int32_t Node::getLine() {
		return line;
	}

	inline int32_t Node::getOffset() {
		return offset;
	}

	inline llvm::Value* Node::getValue() {
		assert(value != NULL);
		return value;
	}

	inline llvm::Value* Node::emit(ModuleBuilder& mb) {
		assemble(mb);
		return getValue();
	}

}} // end ns

#endif /* KENSHO_AST_NODE_HPP_ */
