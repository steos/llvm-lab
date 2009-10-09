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

#include <kensho/ast/Callable.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/error.hpp>
#include <vector>

using namespace kensho;

	void ast::Callable::assemble(ast::ModuleBuilder& mb) {
		// noop
	}

	void ast::Callable::emitDefinition(ast::ModuleBuilder& mb) {
		if (mb.isFunctionDeclared(name)) {
			throw(ParseError("function " + name + " is already declared",
				getLine(), getOffset()));
		}
		std::vector<const llvm::Type*> paramTypes;
		std::vector<Type*>::iterator it;
		for (it = parameterTypes.begin(); it != parameterTypes.end(); ++it) {
			const llvm::Type* llvmTy = (*it)->getAssemblyType();
			paramTypes.push_back(llvmTy);
		}
		llvm::FunctionType* funtype = llvm::FunctionType::get(
			type->getAssemblyType(), paramTypes, false);
		llvm::Function* fun = llvm::Function::Create(
			funtype, llvm::Function::ExternalLinkage, name, mb.getModule());

		value = fun;
		mb.declareFunction(this);
	}
