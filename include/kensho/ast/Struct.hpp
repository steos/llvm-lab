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
#include <kensho/ast/Function.hpp>
#include <kensho/ast/VariableDefinition.hpp>
#include <string>
#include <vector>
#include <map>

#include <llvm/DerivedTypes.h>

namespace kensho {
namespace ast {

	class Struct : public Buildable {
	public:
		class Function {
		private:
			Struct* parent;
			ast::Function* function;
			bool staticDef;
		protected:
			void assemble(ModuleBuilder&);
		public:
			Function(Struct* parent, ast::Function* func, bool staticDef) :
				parent(parent), function(func), staticDef(staticDef) {};
			void emitDefinition(ModuleBuilder& mb);
			void emit(ModuleBuilder& mb);
		};
		std::string name;
		const llvm::Type* assemblyType;
		std::vector<Struct::Function*> functions;
		std::vector<VariableDefinition*> variables;
		std::map<std::string, int> varmap;
		Struct::Function* ctor;
		Struct::Function* dtor;
		std::vector<Buildable*> dtorBody;
		void assembleType(ModuleBuilder&);
		void emitConstructorDefinition(ModuleBuilder&);
		void emitDestructorDefinition(ModuleBuilder&);
	public:
		Struct(std::string name) : name(name), assemblyType(NULL), ctor(NULL), dtor(NULL) {};
		virtual void assemble(ModuleBuilder& mb);
		void addFunction(ast::Function* func, bool staticDef) {
			functions.push_back(new Struct::Function(this, func, staticDef));
		}
		void addFunction(ast::Function* func) {
			addFunction(func, false);
		}
		void addVariableDefinition(VariableDefinition* vardef) {
			variables.push_back(vardef);
			// start the index at 1
			varmap[vardef->getName()] = variables.size();
		}
		void setConstructor(ast::Function* ctor) {
			this->ctor = new Struct::Function(this, ctor, false);
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

		Struct::Function* getConstructor() {
			return ctor;
		}

		bool hasDestructor() {
			return dtorBody.size() > 0;
		}

		std::string getName() {
			return name;
		}

		const llvm::Type* getAssemblyType() {
			return assemblyType;
		}
	};


}} // end ns

#endif /* KENSHO_AST_STRUCT_HPP_ */
