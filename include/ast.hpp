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

#include <cassert>
#include <llvm/Value.h>
#include <llvm/Type.h>

// always include antlr last!
#include <antlr.hpp>

namespace kensho {
namespace ast {

	/*
	 * forward declaration of the ModuleBuilder class which
	 * manages the compilation of a module (i.e. is in charge
	 * of the symbol table etc.)
	 */
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
		Node() : value(NULL) {};
		llvm::Value* emit(ModuleBuilder& mb);
		llvm::Value* getValue();
		void setSourcePosition(int32_t, int32_t);
		int32_t getLine();
		int32_t getOffset();
		virtual ~Node() {};
	};

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

	/*
	 * abstract class every symbol has to extend
	 */
	class Symbol : public Node {
	protected:
		std::string name;
		const llvm::Type* type;
	protected:
		virtual void assemble(ModuleBuilder&) = 0;
	public:
		Symbol(std::string name, const llvm::Type* type) :
			name(name), type(type) {};
		std::string getName();
		const llvm::Type* getType();
		virtual ~Symbol() {};
	};

	inline const llvm::Type* Symbol::getType() {
		return type;
	}

	inline std::string Symbol::getName() {
		return name;
	}

	/*
	 * Abstract base class for nodes that result in a callable
	 * function, i.e. captures the name, return type and parameter types
	 * and is implemented by the function node as well as native function nodes
	 * and possibly future variations of a callable object like closures.
	 * Callable extends Symbol so we can easily support first class functions.
	 */
	class Callable : public Symbol {
	protected:
		std::vector<const llvm::Type*> parameterTypes;
		virtual void assemble(ModuleBuilder& mb) = 0;
	public:
		Callable(std::string name, const llvm::Type* type) :
			Symbol(name, type) {};
		virtual void addParameter(const llvm::Type* type);
		virtual ~Callable() {};
	};

	inline void Callable::addParameter(const llvm::Type* type) {
		parameterTypes.push_back(type);
	}

	/*
	 * represents a variable declaration
	 */
	class VariableDefinition : public Symbol {
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		VariableDefinition(std::string name, const llvm::Type* type) :
			Symbol(name, type) {};
	};

	/*
	 * this node represents access to a variable
	 */
	class Variable : public Node {
	private:
		VariableDefinition* def;
	protected:
		virtual void assemble(ModuleBuilder&);
	public:
		Variable(VariableDefinition* def) : def(def) {};
	};

	/*
	 * TODO unary expression
	 */
	class UnaryExpression : public Node {

	};

	/*
	 * a binary expression node
	 */
	class BinaryExpression : public Node {
	private:
		antlr::token_t token;
	protected:
		virtual void assemble(ModuleBuilder&);
	public:
		BinaryExpression(antlr::token_t token) : token(token) {};
	};

	/*
	 * a literal, i.e. integer, float, boolean or character literal
	 */
	class Literal : public Node {
	private:
		antlr::token_t token;
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		Literal(antlr::token_t token) : token(token) {};
	};

	/*
	 * a function declaration node
	 */
	class Function : public Callable {
	private:
		std::vector<std::string> parameterNames;
		std::vector<Node*> body;
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		Function(std::string name, const llvm::Type* type) :
			Callable(name, type) {};
		virtual void addParameter(std::string name, const llvm::Type* type);
		void addBodyNode(Node*);
	};

	inline void Function::addParameter(std::string name, const llvm::Type* type) {
		parameterNames.push_back(name);
		parameterTypes.push_back(type);
	}

	inline void Function::addBodyNode(Node* node) {
		body.push_back(node);
	}
}}


#endif /* AST_HPP_ */
