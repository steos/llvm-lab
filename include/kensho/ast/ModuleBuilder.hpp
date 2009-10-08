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
#include <kensho/ast/Type.hpp>
#include <kensho/ast/Symbol.hpp>
#include <kensho/ast/ScopeProvider.hpp>

#include <llvm/ADT/StringMap.h>
#include <llvm/Value.h>
#include <llvm/Module.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/PassManager.h>

#include <vector>

namespace kensho {
namespace ast {

	// forward declare referenced ast node classes
	class Callable;
	class Struct;

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
		ScopeProvider* symbolScopeProvider;
		llvm::StringMap<StructType*> types;
		llvm::StringMap<Callable*> funcs;
		llvm::FunctionPassManager* fpm;
		llvm::ExecutionEngine* engine;

		void initEngine(bool mem2reg, bool optimize);

		void emitStructFunctionDefinitions();

		void declareUserType(StructType* ty) {
			types[ty->getName()] = ty;
		}

	public:

		ModuleBuilder(std::string name) : module(name), symbolScopeProvider(NULL) {};

		void postProcessFunction(llvm::Function* fun);

		void declareFunction(Callable* fun);

		void emitDefinitions();

		void emitImplementations();

		bool isFunctionDeclared(std::string name) {
			return funcs.count(name) > 0;
		}

		Callable* getFunction(std::string name) {
			return funcs[name];
		}

		bool isUserTypeDeclared(std::string name) {
			return types.count(name) > 0;
		}

		StructType* getUserType(std::string name) {
			return types[name];
		}

		void build(bool mem2reg, bool optimize);

		llvm::Value* createBinaryOperator(
			OperatorToken type, llvm::Value* left, llvm::Value* right);

		llvm::ExecutionEngine* getEngine() {
			return engine;
		}

		void addFunction(Callable* fun) {
			functions.push_back(fun);
		}

		void addStruct(Struct* st);

		Type* createType(TypeToken token) {
			return PrimitiveType::create(token);
		}

		Type* createType(std::string name) {
			StructType* ty = types[name];
			if (ty == NULL) {
				ty = new StructType(name);
				types[name] = ty;
			}
			return ty;
		}

		Type* createType(TypeToken token, std::string name) {
			return token == TyPtr ?
				createType(name) : createType(token);
		}

		void installSymbolScopeProvider(ScopeProvider* sp) {
			symbolScopeProvider = sp;
		}

		void uninstallSymbolScopeProvider() {
			symbolScopeProvider = NULL;
		}

		ScopeProvider* getSymbolScopeProvider() {
			return symbolScopeProvider;
		}

		Scope& getSymbolScope() {
			assert(symbolScopeProvider != NULL);
			return symbolScopeProvider->getScope();
		}

		llvm::IRBuilder<>& getIRBuilder() {
			return irBuilder;
		}

		llvm::Module* getModule() {
			return &module;
		}

		~ModuleBuilder() {};
	};

}}

#endif /* KENSHO_AST_MODULEBUILDER_HPP_ */
