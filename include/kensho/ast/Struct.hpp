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

#include <kensho/ast/Node.hpp>
#include <kensho/ast/Function.hpp>
#include <kensho/ast/VariableDefinition.hpp>
#include <string>
#include <vector>
#include <map>

#include <llvm/DerivedTypes.h>

namespace kensho {
namespace ast {

	class Struct : public Node {
	public:
		class Function {
		private:
			Struct* parent;
			ast::Function* function;
			bool staticDef;
		public:
			Function(Struct* parent, ast::Function* func, bool staticDef) :
				parent(parent), function(func), staticDef(staticDef) {};
			void emitDefinition(ModuleBuilder& mb);
		};
		std::string name;
		llvm::StructType* assemblyType;
		std::vector<Struct::Function*> functions;
		std::vector<const llvm::Type*> variables;
		std::map<std::string, int> varmap;
		ast::Function* ctor;
		std::vector<Node*> dtor;
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		Struct(std::string name) : name(name) {};
		void addFunction(ast::Function* func, bool staticDef) {
			functions.push_back(new Struct::Function(this, func, staticDef));
		}
		void addFunction(ast::Function* func) {
			addFunction(func, false);
		}
		void addVariableDefinition(VariableDefinition* vardef) {
			variables.push_back(vardef->getAssemblyType());
			// start the index at 1
			varmap[vardef->getName()] = variables.size();
		}
		void setConstructor(ast::Function* ctor) {
			this->ctor = ctor;
		}
		void addDestructorBodyNode(Node* node) {
			dtor.push_back(node);
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

		llvm::Value* emit(ModuleBuilder& mb) {
			assemble(mb);
			return NULL;
		}
	};


}} // end ns

#endif /* KENSHO_AST_STRUCT_HPP_ */
