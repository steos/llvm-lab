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
		assembleStore(type->getDefaultValue(), mb);
	}

	llvm::Value* ast::StructVariableDefinition::assembleLoad(ModuleBuilder& mb) {
		if (!mb.getSymbolScope().hasContext()) {
			throw(ParseError("instance variable " + name + " not availabe in static scope"));
		}
		llvm::Value* ptr = mb.getSymbolScope().getContextPointer();
		llvm::Value* varPtr = mb.getIRBuilder().CreateStructGEP(ptr, index, name.c_str());
		return mb.getIRBuilder().CreateLoad(varPtr, name.c_str());
	}

	llvm::Value* ast::StructVariableDefinition::assembleStore(llvm::Value* val, ModuleBuilder& mb) {
		if (!mb.getSymbolScope().hasContext()) {
			throw(ParseError("instance variable " + name + " not availabe in static scope"));
		}
		llvm::Value* ptr = mb.getSymbolScope().getContextPointer();
		llvm::Value* varPtr = mb.getIRBuilder().CreateStructGEP(ptr, index, name.c_str());
		return mb.getIRBuilder().CreateStore(val, varPtr);
	}

	void ast::Struct::assemble(ModuleBuilder& mb) {
		mb.installFunctionProvider(this);

		// declare variables and insert initializations into the constructor
		for (uint32_t i = 0; i < variables.size(); ++i) {
			StructVariableDefinition* def = variables[i];
//			StructVariableDefinition* var = new StructVariableDefinition(
//				this, def->getName(), def->getType(), i);
//			symbols.declareSymbol(var);
			ctor->insertBodyNode(def);
		}

		mb.getSymbolScope().push();
		ctor->emit(mb);
		mb.getSymbolScope().pop();

		// TODO emit dtor

		for (uint32_t i = 0; i < functions.size(); ++i) {
			mb.getSymbolScope().push();
			functions.at(i)->emit(mb);
			mb.getSymbolScope().pop();
		}

		mb.uninstallFunctionProvider();
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
		AbstractFunction::emitDefinition(mb);
	}

	llvm::FunctionType* ast::StructFunction::createType(std::vector<const llvm::Type*>& params) {
		if (defStatic == false) {
			params.insert(params.begin(), parent->getType()->getAssemblyType());
		}
		return AbstractFunction::createType(params);
	}

	llvm::Value* ast::StructFunction::assembleCall(std::vector<llvm::Value*>& args, ModuleBuilder& mb) {
		if (defStatic == false && mb.getSymbolScope().hasContext()) {
			args.insert(args.begin(), mb.getSymbolScope().getContextPointer());
		}
		return AbstractFunction::assembleCall(args, mb);
	}

	void ast::StructFunction::assembleParameters(llvm::Function* fun,
		llvm::Function::arg_iterator begin, ModuleBuilder& mb) {
		if (defStatic == false) {
			begin->setName("this");
			contextPtr = begin;
			++begin;
		}
		AbstractFunction::assembleParameters(fun, begin, mb);
		if (defStatic == false) {
			assert(contextPtr != NULL);
			mb.getSymbolScope().setContext(new StructContext(parent, contextPtr));
		}
	}

	void ast::StructFunction::assemble(ModuleBuilder& mb) {
		AbstractFunction::assemble(mb);
		if (defStatic == false) {
			mb.getSymbolScope().unsetContext();
		}
		mb.getSymbolScope().clearSymbols();
	}
