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

#include <kensho/ast/Type.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/ast/Struct.hpp>
#include <kensho/error.hpp>

using namespace kensho;

	const llvm::Type* mapToLLVM(int32_t type) {
		switch (type) {
			case ast::TyVoid:
				return llvm::Type::VoidTy;
			case ast::TyBool:
				return llvm::Type::Int1Ty;
			case ast::TyByte:
				return llvm::Type::Int8Ty;
			case ast::TyShort:
				return llvm::Type::Int16Ty;
			case ast::TyInt:
			case ast::TyChar:
				return llvm::Type::Int32Ty;
			case ast::TyLong:
				return llvm::Type::Int64Ty;
			case ast::TyFloat:
				return llvm::Type::FloatTy;
			case ast::TyDouble:
				return llvm::Type::DoubleTy;
		}

		return NULL;
	}

	bool ast::Type::isVoid() {
		return ( token != -1 && token == ast::TyVoid ) ||
			( llvmType != NULL && llvmType == llvm::Type::VoidTy );
	}

	bool ast::Type::isPrimitive() {
		return (token != -1 && token != ast::TyPtr) ||
			( llvmType != NULL && llvmType->isPrimitiveType());
	}

	void ast::Type::assemble() {
		if (token == -1 || token == ast::TyPtr) {
			Type* userType = mb.getUserType(name);
			if (userType == NULL) {
				throw(ParseError("undeclared type " + name));
			}
			llvmType = userType->getAssemblyType();
		}
		else {
			llvmType = mapToLLVM(token);
		}
		assert(llvmType != NULL && "type not handled in mapping");
	}

