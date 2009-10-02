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

#ifndef MODULEBUILDER_HPP_
#define MODULEBUILDER_HPP_

#include <llvm/Value.h>
#include <llvm/Module.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/PassManager.h>
#include <map>

namespace kensho {
namespace ast {

	class Callable;
	class Symbol;

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
		llvm::FunctionPassManager* fpm;
		llvm::ExecutionEngine* engine;
		void initEngine(bool mem2reg, bool optimize);
	public:
		ModuleBuilder(std::string name, std::vector<Callable*>* functions) :
			module(name), functions(functions) {};
		llvm::IRBuilder<>& getIRBuilder();
		llvm::Module* getModule();
		void declareSymbol(Symbol* symbol);
		bool isDeclared(std::string name);
		Symbol* getSymbol(std::string name);
		void build(bool mem2reg, bool optimize);
		llvm::Value* createBinaryOperator(
			uint32_t type, llvm::Value* left, llvm::Value* right);
		llvm::ExecutionEngine* getEngine();
		~ModuleBuilder() {};
	};

	inline llvm::ExecutionEngine* ModuleBuilder::getEngine() {
		return engine;
	}

	inline Symbol* ModuleBuilder::getSymbol(std::string name) {
		return symbols[name];
	}

	inline bool ModuleBuilder::isDeclared(std::string name) {
		return symbols.count(name) > 0;
	}

	inline llvm::Module* ModuleBuilder::getModule() {
		return &module;
	}

	inline llvm::IRBuilder<>& ModuleBuilder::getIRBuilder() {
		return irBuilder;
	}

}}

#endif /* MODULEBUILDER_HPP_ */
