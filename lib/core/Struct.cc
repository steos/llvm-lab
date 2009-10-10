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
#include <kensho/error.hpp>

#include <llvm/AbstractTypeUser.h>
#include <llvm/DerivedTypes.h>

#include <iostream>

using namespace kensho;

	void ast::StructVariableDefinition::assemble(ModuleBuilder& mb) {
		// noop: here we would allocate space for the variable
		// but since it's part of an aggregate type we have nothing to do
	}

	llvm::Value* ast::StructVariableDefinition::load(ModuleBuilder& mb) {
		// TODO retrieve context pointer and load a ptr to the variable
//		assert(false && "sorry, StructVariableDefinition::load not yet implemented");
		if (!mb.getSymbolScope().hasContextPointer()) {
			throw(ParseError("instance variable " + name + " not availabe in static scope"));
		}
		llvm::Value* ptr = mb.getSymbolScope().getContextPointer();
		llvm::Value* varPtr = mb.getIRBuilder().CreateStructGEP(ptr, index, name.c_str());
		return varPtr;
	}

	llvm::Value* ast::StructVariableDefinition::store(llvm::Value* val, ModuleBuilder& mb) {
		// TODO retrieve context pointer and store the value into the variable
//		assert(false && "sorry, StructVariableDefinition::store not yet implemented");
		if (!mb.getSymbolScope().hasContextPointer()) {
			throw(ParseError("instance variable " + name + " not availabe in static scope"));
		}
		llvm::Value* ptr = mb.getSymbolScope().getContextPointer();
		llvm::Value* varPtr = mb.getIRBuilder().CreateStructGEP(ptr, index, name.c_str());
		return mb.getIRBuilder().CreateStore(val, varPtr);
	}

	void ast::Struct::declareVariables(ModuleBuilder& mb) {
		for (uint32_t i = 0; i < variables.size(); ++i) {
			VariableDefinition* def = variables[i];
			StructVariableDefinition* var = new StructVariableDefinition(
				this, def->getName(), def->getType(), i);
			symbols.declareSymbol(var);
		}
	}

	void ast::Struct::assemble(ModuleBuilder& mb) {
		mb.installSymbolScopeProvider(this);
		declareVariables(mb);

		// TODO emit ctor and dtor implementations

		for (uint32_t i = 0; i < functions.size(); ++i) {
			functions.at(i)->emit(mb);
		}

		mb.uninstallSymbolScopeProvider();
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
			ctor = dynamic_cast<StructFunction*>(
				createFunction("new", PrimitiveType::create(TyVoid))
			);
		}
		// TODO initialize members to default values
		ctor->emitDefinition(mb);
	}

	void ast::Struct::emitDestructorDefinition(ModuleBuilder& mb) {
		dtor = createFunction("delete", PrimitiveType::create(TyVoid), dtorBody, false);
		dtor->emitDefinition(mb);
	}

	void ast::Struct::emitDefinition(ModuleBuilder& mb) {
		const llvm::Type* ty = type->getAssemblyType();
		mb.getModule()->addTypeName(name + "Ptr", ty);
		const llvm::PointerType* pty = llvm::cast<llvm::PointerType>(ty);
		mb.getModule()->addTypeName(name, pty->getElementType());
		emitConstructorDefinition(mb);
		if (hasDestructor()) {
			emitDestructorDefinition(mb);
		}
		// emit function definitions
		for (uint32_t i = 0; i < functions.size(); ++i) {
			functions.at(i)->emitDefinition(mb);
		}
	}

	void::ast::StructFunction::emitDefinition(ModuleBuilder& mb) {
		name = parent->getName() + "." + name;
		if (defStatic == false) {
			Type* ty = parent->getType();
			prependNamedParameter("this", ty);
		}
		AbstractFunction::emitDefinition(mb);
	}

	void ast::StructFunction::assembleParameters(llvm::Function* fun, ModuleBuilder& mb) {
		AbstractFunction::assembleParameters(fun, mb);
		if (defStatic == false) {
			assert(parameterValues.size() > 0);
			mb.getSymbolScope().installContextPointer(parameterValues.at(0));
		}
	}

	void ast::StructFunction::assemble(ModuleBuilder& mb) {
		AbstractFunction::assemble(mb);
		if (defStatic == false) {
			mb.getSymbolScope().uninstallContextPointer();
		}
	}
