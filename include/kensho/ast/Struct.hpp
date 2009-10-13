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

#ifndef KENSHO_AST_STRUCT_HPP_
#define KENSHO_AST_STRUCT_HPP_

#include <kensho/ast/Buildable.hpp>
#include <kensho/ast/AbstractFunction.hpp>
#include <kensho/ast/FunctionFactory.hpp>
#include <kensho/ast/VariableDefinition.hpp>
#include <kensho/ast/FunctionProvider.hpp>
#include <kensho/ast/Context.hpp>

#include <llvm/DerivedTypes.h>
#include <llvm/ADT/StringMap.h>

#include <string>
#include <vector>
#include <map>

namespace kensho {
namespace ast {

	// forward declare Struct
	class Struct;

	/*
	 * a struct function
	 */
	class StructFunction : public AbstractFunction {
	private:

		Struct* parent;
		bool defStatic;
		llvm::Value* contextPtr;

	protected:

		void assembleParameters(llvm::Function*, llvm::Function::arg_iterator begin, ModuleBuilder&);

	public:
		StructFunction(Struct* parent, std::string name, Type* type) :
			AbstractFunction(name, type), parent(parent) {};

		StructFunction(Struct* parent, std::string name, Type* type, std::vector<Buildable*> body) :
			AbstractFunction(name, type, body), parent(parent) {};

		virtual void emitDefinition(ModuleBuilder& mb);

		void assemble(ModuleBuilder&);

		llvm::Value* assembleCall(std::vector<llvm::Value*>& args, ModuleBuilder& mb);

		llvm::FunctionType* createType(std::vector<const llvm::Type*>& params);

		void setStatic(bool defStatic) {
			this->defStatic = defStatic;
		}

		bool isStatic() {
			return defStatic;
		}

		void insertBodyNode(Buildable* node) {
			body.insert(body.begin(), node);
		}
	};

	/*
	 * a struct member, i.e. variable
	 */
	class StructVariableDefinition : public MutableSymbol {
	private:
		Struct* parent;
		uint32_t index;
	public:
		StructVariableDefinition(Struct* parent, std::string name, Type* type, uint32_t index) :
			MutableSymbol(name, type), parent(parent), index(index) {};

		void assemble(ModuleBuilder& mb);

		llvm::Value* assembleStore(llvm::Value*, ModuleBuilder&);

		llvm::Value* assembleLoad(ModuleBuilder&);

		bool isHidable() {
			return true;
		}
	};

	/*
	 * a struct definition
	 */
	class Struct : public Buildable, public FunctionFactory,
		public FunctionProvider {
	public:

		std::string name;
		StructType* type;
		std::vector<StructFunction*> functions;
		std::vector<StructVariableDefinition*> variables;
		std::map<std::string, int> varmap;
		StructFunction* ctor;
		StructFunction* dtor;
		std::vector<Buildable*> dtorBody;
		llvm::StringMap<int> functionMap;

		void emitConstructorDefinition(ModuleBuilder&);
		void emitDestructorDefinition(ModuleBuilder&);

	public:

		Struct(std::string name) :
			name(name), type(NULL), ctor(NULL), dtor(NULL) {};

		void constructType(StructType* ty);

		virtual void assemble(ModuleBuilder& mb);

		StructFunction* createFunction(std::string name, Type* type,
			std::vector<Buildable*> body, bool defStatic)
		{
			StructFunction* fun = new StructFunction(this, name, type, body);
			fun->setStatic(defStatic);
			return fun;
		}

		AbstractFunction* createFunction(
			std::string name, Type* type, std::vector<Buildable*> body)
		{
			return dynamic_cast<AbstractFunction*>(
				createFunction(name, type, body, false)
			);
		}

		AbstractFunction* createFunction(std::string name, Type* type) {
			return createFunction(name, type, std::vector<Buildable*>());
		}

		void addFunction(StructFunction* fun) {
			functions.push_back(fun);
			functionMap[fun->getName()] = functions.size();
		}

		Callable* getFunction(const std::string& name) {
			int index = functionMap[name];
			if (index) {
				return functions.at(index - 1);
			}
			return NULL;
		}

		void addVariableDefinition(VariableDefinition* vardef) {
			// TODO create structvars in treeparser
			StructVariableDefinition* def = new StructVariableDefinition(
				this,
				vardef->getName(),
				vardef->getType(),
				variables.size()
			);
			variables.push_back(def);
			// start the index at 1
			varmap[vardef->getName()] = variables.size();
			delete vardef;
		}

		void setConstructor(StructFunction* ctor) {
			this->ctor = ctor;
		}

		void addDestructorBodyNode(Buildable* node) {
			dtorBody.push_back(node);
		}

		bool hasVariable(std::string name) {
			return varmap.count(name) > 0;
		}

		int getVariableIndex(std::string name) {
			int index = varmap[name];
			if (!index) {
				return -1;
			}
			return index - 1;
		}

		void emitDefinition(ModuleBuilder& mb);

		StructFunction* getConstructor() {
			return ctor;
		}

		bool hasDestructor() {
			return dtorBody.size() > 0;
		}

		std::string getName() {
			return name;
		}

		StructType* getType() {
			return type;
		}

		StructVariableDefinition* getVariable(uint32_t index) {
			assert(index < variables.size());
			return variables[index];
		}
	};

	class StructContext : public Context {
	protected:
		llvm::Value* contextPointer;
		Struct* def;
	public:
		StructContext() : contextPointer(NULL), def(NULL) {};

		StructContext(Struct* def) : contextPointer(NULL), def(def) {};

		StructContext(Struct* def, llvm::Value* ctx) : contextPointer(ctx), def(def) {};

		llvm::Value* getPointer() {
			return contextPointer;
		}

		Struct* getDefinition() {
			return def;
		}

		Symbol* getSymbol(const std::string& name) {
			int idx = def->getVariableIndex(name);
			if (idx > -1) {
				return def->getVariable(idx);
			}
			return NULL;
		}
	};

}} // end ns

#endif /* KENSHO_AST_STRUCT_HPP_ */
