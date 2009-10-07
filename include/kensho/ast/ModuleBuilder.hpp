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

#ifndef KENSHO_AST_MODULEBUILDER_HPP_
#define KENSHO_AST_MODULEBUILDER_HPP_

#include <kensho/ast/tokens.hpp>
#include <kensho/Scope.hpp>
#include <kensho/ast/Type.hpp>

#include <llvm/Value.h>
#include <llvm/Module.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/PassManager.h>

#include <map>
#include <vector>

namespace kensho {
namespace ast {

	// forward declare referenced ast node classes
	class Callable;
	class Struct;
	class VariableDefinition;

	/*
	 * The ModuleBuilder class
	 * manages the compilation of a module (i.e. is in charge
	 * of the symbol table etc.)
	 */
	class ModuleBuilder {
	private:
		llvm::Module module;
		llvm::IRBuilder<> irBuilder;
		std::vector<Callable*> functions;
		std::vector<ast::Struct*> structs;
		Scope<VariableDefinition*> symScope;
		Scope<Callable*> funScope;
		std::map<std::string, Type*> types;
		llvm::FunctionPassManager* fpm;
		llvm::ExecutionEngine* engine;
		void initEngine(bool mem2reg, bool optimize);
		void emitStructFunctionDefinitions();
	public:
		ModuleBuilder(std::string name) : module(name) {};

		llvm::IRBuilder<>& getIRBuilder();

		llvm::Module* getModule();

		void addFunction(Callable* fun) {
			functions.push_back(fun);
		}

		void addStruct(Struct* st) {
			structs.push_back(st);
		}

		Type* createType(TypeToken token, std::string name) {
			return new Type(*this, token, name);
		}

		Type* createType(const llvm::Type* ty) {
			return new Type(*this, ty);
		}

		void declareSymbol(VariableDefinition* symbol);

		void clearSymbolScope();

		void declareFunction(Callable* fun);

		bool isFunctionDeclared(std::string name);

		Callable* getFunction(std::string name);

		bool isDeclared(std::string name);

		void declareUserType(Type*);

		bool isUserTypeDeclared(std::string name);

		Type* getUserType(std::string name);

		VariableDefinition* getSymbol(std::string name);

		void build(bool mem2reg, bool optimize);

		llvm::Value* createBinaryOperator(
			OperatorToken type, llvm::Value* left, llvm::Value* right);

		llvm::ExecutionEngine* getEngine();

		void emitDefinitions();

		void emitImplementations();

		~ModuleBuilder() {};
	};

	inline void ModuleBuilder::clearSymbolScope() {
		symScope.popAllAndClear();
	}

	inline bool ModuleBuilder::isUserTypeDeclared(std::string name) {
		return types.count(name) > 0;
	}

	inline Type* ModuleBuilder::getUserType(std::string name) {
		return types[name];
	}

	inline void ModuleBuilder::declareUserType(Type* ty) {
		types[ty->getName()] = ty;
	}

	inline bool ModuleBuilder::isFunctionDeclared(std::string name) {
		return funScope.hasSymbol(name);
	}

	inline Callable* ModuleBuilder::getFunction(std::string name) {
		return funScope.getSymbol(name);
	}

	inline llvm::ExecutionEngine* ModuleBuilder::getEngine() {
		return engine;
	}

	inline VariableDefinition* ModuleBuilder::getSymbol(std::string name) {
		return symScope.getSymbol(name);
	}

	inline bool ModuleBuilder::isDeclared(std::string name) {
		return symScope.hasSymbol(name);
	}

	inline llvm::Module* ModuleBuilder::getModule() {
		return &module;
	}

	inline llvm::IRBuilder<>& ModuleBuilder::getIRBuilder() {
		return irBuilder;
	}

}}

#endif /* KENSHO_AST_MODULEBUILDER_HPP_ */
