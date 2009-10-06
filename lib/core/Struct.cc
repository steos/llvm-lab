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

#include <kensho/ast/Struct.hpp>
#include <kensho/ast/ModuleBuilder.hpp>

#include <llvm/AbstractTypeUser.h>
#include <llvm/DerivedTypes.h>

#include <antlr.hpp>

using namespace kensho;

	void ast::Struct::assemble(ModuleBuilder& mb) {

		for (uint32_t i = 0; i < functions.size(); ++i) {
			functions.at(i)->emit(mb);
		}
	}

	void ast::Struct::emitTypeDefinition(ModuleBuilder& mb) {
		std::vector<const llvm::Type*> types;
		llvm::PATypeHolder opaqueTy = llvm::OpaqueType::get();
		bool recursive = false;
		for (uint32_t i = 0; i < variables.size(); ++i) {
			VariableDefinition* var = variables.at(i);
			// if a member of the struct is the struct itself we have
			// a recursive type and need to do some more
			// work to construct the corresponding llvm type
			if (var->getText() == name) {
				recursive = true;
				types.push_back(llvm::PointerType::getUnqual(opaqueTy));
			}
			else {
				const llvm::Type* ty = var->toAssemblyType(mb);
				types.push_back(ty);
			}
		}
		llvm::StructType* ty = llvm::StructType::get(types);
		// we need to perform type unification if the type is recursive
		if (recursive) {
			llvm::cast<llvm::OpaqueType>(opaqueTy.get())
				->refineAbstractTypeTo(ty);
			ty = llvm::cast<llvm::StructType>(opaqueTy.get());
		}
		mb.getModule()->addTypeName(name, ty);
		mb.declareUserType(this);

		// we only work with references, i.e. pointers
		assemblyType = llvm::PointerType::getUnqual(ty);
	}

	void ast::Struct::emitConstructorDefinition(ModuleBuilder& mb) {
		if (ctor == NULL) {
			ctor = new Struct::Function(this,
				new ast::Function("new", T_VOID, std::vector<Node*>()), false);
		}

		ctor->emitDefinition(mb);
	}

	void ast::Struct::emitDestructorDefinition(ModuleBuilder& mb) {
		ast::Function* dtorFun = new ast::Function("delete", T_VOID, dtorBody);
		dtor = new Struct::Function(this, dtorFun, false);
		dtor->emitDefinition(mb);
	}

	void ast::Struct::emitDefinition(ModuleBuilder& mb) {
		emitTypeDefinition(mb);
		emitConstructorDefinition(mb);
		if (hasDestructor()) {
			emitDestructorDefinition(mb);
		}
		// emit function definitions
		for (uint32_t i = 0; i < functions.size(); ++i) {
			functions.at(i)->emitDefinition(mb);
		}
	}

	void::ast::Struct::Function::emitDefinition(ModuleBuilder& mb) {
		function->setName(parent->name + "." + function->getName());
		if (staticDef == false) {
			function->prependParameter("this", parent->assemblyType);
		}
		function->emitDefinition(mb);
	}

	void ast::Struct::Function::emit(ModuleBuilder& mb) {
		function->emit(mb);
	}
