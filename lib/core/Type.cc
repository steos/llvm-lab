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
#include <llvm/Constants.h>

using namespace kensho;

	llvm::Value* ast::Type::getDefaultValue() const {
		assert(llvmType != NULL && "assemble the type first");
		// initialize to 0, i.e. false for bools,
		// 0 for ints and null for pointers
		return llvm::Constant::getNullValue(llvmType);
	}

	llvm::Value* ast::Type::cast(llvm::Value* value, const llvm::Type* dst, ModuleBuilder& mb) {
		const llvm::Type* src = value->getType();
		if (src->isInteger() && dst->isInteger()) {
			uint32_t srcBits = src->getPrimitiveSizeInBits();
			uint32_t dstBits = dst->getPrimitiveSizeInBits();
			if (srcBits > dstBits) {
				throw(ParseError("cannot truncate integer type"));
			}
			else if (srcBits < dstBits) {
				if (srcBits == 1) {
					throw(ParseError("cannot cast boolean to numeric type"));
				}
				return mb.getIRBuilder().CreateSExt(value, dst);
			}
			else {
				assert(false);
			}
		}
		else if (src->isFloatingPoint() && dst->isInteger()) {
			throw(ParseError("cannot cast floating point to integer"));
		}
		else if (src->isFloatingPoint() && dst->isFloatingPoint()) {
			if (src == llvm::Type::DoubleTy && dst == llvm::Type::FloatTy) {
				throw(ParseError("cannot cast double to float"));
			}
			else {
				return mb.getIRBuilder().CreateFPExt(value, dst);
			}
		}
		else {
			throw(ParseError("type mismatch in binary expression"));
		}

		return NULL;
	}

	ast::Type* ast::PrimitiveType::create(TypeToken token) {
		static PrimitiveType Void(TyVoid, llvm::Type::VoidTy);
		static PrimitiveType Bool(TyBool, llvm::Type::Int1Ty);
		static PrimitiveType Byte(TyByte, llvm::Type::Int8Ty);
		static PrimitiveType Short(TyShort, llvm::Type::Int16Ty);
		static PrimitiveType Int(TyInt, llvm::Type::Int32Ty);
		static PrimitiveType Long(TyLong, llvm::Type::Int64Ty);
		static PrimitiveType Double(TyDouble, llvm::Type::DoubleTy);
		static PrimitiveType Float(TyFloat, llvm::Type::FloatTy);
		switch (token) {
			case ast::TyVoid:
				return &Void;
			case ast::TyBool:
				return &Bool;
			case ast::TyByte:
				return &Byte;
			case ast::TyShort:
				return &Short;
			case ast::TyInt:
			case ast::TyChar:
				return &Int;
			case ast::TyLong:
				return &Long;
			case ast::TyFloat:
				return &Float;
			case ast::TyDouble:
				return &Double;
			default:
				assert(false);
		}
	}

	void ast::StructType::assemble() {
		assert(types.size() > 0 && "struct type cannot be empty");
		llvm::PATypeHolder oty = llvm::OpaqueType::get();
		std::vector<const llvm::Type*> llvmTypes;
		for (uint32_t i = 0; i < types.size(); ++i) {
			if (types[i]->isPrimitive()) {
				llvmTypes.push_back(types[i]->getAssemblyType());
			}
			else {
				// we have a struct type as member so we need
				// to watch out for it either being this type or
				// referencing this type in which case we have a
				// recursive or cyclic declaration and must
				// perform type unification
				StructType* ty = dynamic_cast<StructType*>(types[i]);
				assert(ty != NULL);
				// it's a reference to this struct so we have a recursive type
				// that means we have to use an opaque type now and resolve it later
				if (ty->getName() == name) {
					llvmTypes.push_back(llvm::PointerType::getUnqual(oty));
				}
				// it's a reference to another struct type but this other
				// type may contain a reference to this type again in which
				// case we deal with a cyclic declaration.
				// The moment we call getAssemblyType on such a type we invoke
				// this method again unless we set the llvmType to a non-null value.
				else {
					llvmType = oty;
					llvmTypes.push_back(llvm::PointerType::getUnqual(
						ty->getAssemblyType()));
				}
			}
		}

		llvmType = llvm::StructType::get(llvmTypes);

		// in case we have a cyclic or recursive declaration we need
		// to perform type unification
		llvm::cast<llvm::OpaqueType>(oty.get())->refineAbstractTypeTo(llvmType);
		llvmType = llvm::cast<llvm::StructType>(oty.get());
	}

