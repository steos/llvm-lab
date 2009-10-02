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
#include <map>
#include <llvm/Value.h>
#include <llvm/Type.h>
#include <llvm/Module.h>
#include <llvm/Support/IRBuilder.h>

namespace kensho {
namespace ast {

	// forward declare modulebuilder
	class ModuleBuilder;

	const llvm::Type* toAssemblyType(uint32_t type);

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
		uint32_t type;
		const llvm::Type* assemblyType;
	protected:
		virtual void assemble(ModuleBuilder&) = 0;
	public:
		Symbol(std::string name, uint32_t type) :
			name(name), type(type) {
			assemblyType = toAssemblyType(type);
			assert(assemblyType != NULL);
		};
		std::string getName();
		uint32_t getType();
		const llvm::Type* getAssemblyType();
		virtual ~Symbol() {};
	};

	inline uint32_t Symbol::getType() {
		return type;
	}

	inline std::string Symbol::getName() {
		return name;
	}

	inline const llvm::Type* Symbol::getAssemblyType() {
		return assemblyType;
	}

	/*
	 * Base class for nodes that result in a callable
	 * function, i.e. captures the name, return type and parameter types
	 * and is implemented by the function node as well as native function nodes
	 * and possibly future variations of a callable object like closures.
	 * Callable extends Symbol so we can easily support first class functions.
	 */
	class Callable : public Symbol {
	protected:
		std::vector<const llvm::Type*> parameterTypes;
		virtual void assemble(ModuleBuilder& mb);
	public:
		Callable(std::string name, uint32_t type) :
			Symbol(name, type) {};
		virtual void emitDefinition(ModuleBuilder& mb);
		virtual void addParameter(uint32_t type);
		std::vector<const::llvm::Type*> getParameterTypes();
		int countParameters();
		virtual ~Callable() {};
	};

	inline void Callable::addParameter(uint32_t type) {
		const llvm::Type* asmt = toAssemblyType(type);
		assert(asmt != NULL);
		parameterTypes.push_back(asmt);
	}

	inline std::vector<const::llvm::Type*> Callable::getParameterTypes() {
		return parameterTypes;
	}

	inline int Callable::countParameters() {
		return parameterTypes.size();
	}

	/*
	 * represents a variable declaration
	 */
	class VariableDefinition : public Symbol {
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		VariableDefinition(std::string name, uint32_t type) :
			Symbol(name, type) {};
	};

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

	/*
	 * a unary expression
	 */
	class UnaryExpression : public Node {
	private:
		uint32_t token;
		Node* expression;
	protected:
		virtual void assemble(ModuleBuilder&);
	public:
		UnaryExpression(uint32_t token, Node* expression) :
			token(token), expression(expression) {};
	};

	/*
	 * a binary expression node
	 */
	class BinaryExpression : public Node {
	private:
		uint32_t token;
		Node* left;
		Node* right;
	protected:
		virtual void assemble(ModuleBuilder&);
	public:
		BinaryExpression(uint32_t token, Node* left, Node* right) :
			token(token), left(left), right(right) {};

		Node* getLeft() {
			return left;
		}

		Node* getRight() {
			return right;
		}
	};

	/*
	 * a literal, i.e. integer, float, boolean or character literal
	 */
	class Literal : public Node {
	private:
		uint32_t tokenType;
		std::string text;
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		Literal(uint32_t tokenType, std::string text) : tokenType(tokenType), text(text) {};
		uint32_t getTokenType() {
			return tokenType;
		}
		std::string getText() {
			return text;
		}
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
		Function(std::string name, uint32_t type) :
			Callable(name, type) {};
		virtual void addParameter(std::string name, uint32_t type);
		void addBodyNode(Node*);
	};

	inline void Function::addParameter(std::string name, uint32_t type) {
		parameterNames.push_back(name);
		const llvm::Type* asmt = toAssemblyType(type);
		parameterTypes.push_back(asmt);
	}

	inline void Function::addBodyNode(Node* node) {
		body.push_back(node);
	}

	/**
	 * function call
	 */
	class Call : public Node {
	private:
		std::string name;
		std::vector<Node*> arguments;
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		Call(std::string name) : name(name) {};
		void addArgument(Node* node) {
			arguments.push_back(node);
		}
		int countArguments() {
			return arguments.size();
		}
	};

	/*
	 * type cast expression
	 */
	class Cast : public Node {
	private:
		int32_t type;
		const llvm::Type* assemblyType;
		Node* expression;
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		Cast(int32_t type, Node* expression) :
			type(type), expression(expression) {
			assemblyType = toAssemblyType(type);
			assert(assemblyType != NULL);
		};
	};

	/*
	 * while statement
	 */
	class While : public Node {
	private:
		Node* expression;
		std::vector<Node*> body;
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		While(Node* expression) : expression(expression) {};
		void addBodyNode(Node* node) {
			body.push_back(node);
		}
	};

	/*
	 * conditional statement, i.e. if-else
	 */
	class Conditional : public Node {
	private:
		Node* expression;
		std::vector<Node*> trueBody;
		std::vector<Node*> falseBody;
		std::vector<Conditional*> branches;
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		Conditional(Node* expression) : expression(expression) {};
		void addTrueBodyNode(Node* node) {
			trueBody.push_back(node);
		}
		void addFalseBodyNode(Node* node) {
			falseBody.push_back(node);
		}
		void addBranch(Conditional* c) {
			branches.push_back(c);
		}
	};

	class Return : public Node {
	private:
		Node* expression;
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		Return(Node* expression) : expression(expression) {};
		Node* getExpression() {
			return expression;
		}
	};

	/*
	 * The ModuleBuilder class
	 * manages the compilation of a module (i.e. is in charge
	 * of the symbol table etc.)
	 */
	class ModuleBuilder {
	private:
		llvm::Module module;
		llvm::IRBuilder<> irBuilder;
		std::vector<Callable*>* functions;
		std::map<std::string, Symbol*> symbols;
	public:
		ModuleBuilder(std::string name, std::vector<ast::Callable*>* functions) :
			module(name), functions(functions) {};
		llvm::IRBuilder<>& getIRBuilder();
		llvm::Module* getModule();
		void declareSymbol(Symbol* symbol);
		bool isDeclared(std::string name);
		Symbol* getSymbol(std::string name);
		void build();
		llvm::Value* createBinaryOperator(
			uint32_t type, llvm::Value* left, llvm::Value* right);

		~ModuleBuilder() {};
	};

	inline Symbol* ModuleBuilder::getSymbol(std::string name) {
		return symbols[name];
	}

	inline bool ModuleBuilder::isDeclared(std::string name) {
		return symbols.count(name) > 0;
	}

	inline void ModuleBuilder::declareSymbol(Symbol* symbol) {
		symbols[symbol->getName()] = symbol;
	}

	inline llvm::Module* ModuleBuilder::getModule() {
		return &module;
	}

	inline llvm::IRBuilder<>& ModuleBuilder::getIRBuilder() {
		return irBuilder;
	}
}}


#endif /* AST_HPP_ */
