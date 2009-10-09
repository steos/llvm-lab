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
#include <kensho/ast/Type.hpp>

#include <llvm/AbstractTypeUser.h>
#include <llvm/DerivedTypes.h>

using namespace kensho;

	void ast::Struct::assemble(ModuleBuilder& mb) {

		for (uint32_t i = 0; i < functions.size(); ++i) {
			functions.at(i)->emit(mb);
		}
	}

	void ast::Struct::constructType(StructType* ty) {
		if (type == NULL) {
			std::vector<Type*> types;
			assert(variables.size() > 0);
			for (uint32_t i = 0; i < variables.size(); ++i) {
				ty->addMemberType(variables[i]->getType());
			}
			type = ty;
		}
	}

	void ast::Struct::emitConstructorDefinition(ModuleBuilder& mb) {
		if (ctor == NULL) {
			ctor = new Struct::Function(this,
				new ast::Function("new", PrimitiveType::create(TyVoid),
						std::vector<Buildable*>()), false);
		}
		// TODO initialize members to default values
		ctor->emitDefinition(mb);
	}

	void ast::Struct::emitDestructorDefinition(ModuleBuilder& mb) {
		ast::Function* dtorFun = new ast::Function(
			"delete", PrimitiveType::create(TyVoid), dtorBody);
		dtor = new Struct::Function(this, dtorFun, false);
		dtor->emitDefinition(mb);
	}

	void ast::Struct::emitDefinition(ModuleBuilder& mb) {
		mb.getModule()->addTypeName(name, type->getAssemblyType());
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
			function->prependParameter("this", parent->type);
		}
		function->emitDefinition(mb);
	}

	void ast::Struct::Function::emit(ModuleBuilder& mb) {
		function->emit(mb);
	}
