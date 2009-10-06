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

#include <kensho/ast/util.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/error.hpp>

#include <antlr.hpp>

using namespace kensho;

	/*
	 * converts a token type enum value to a llvm::Type*
	 */
	const llvm::Type* ast::toAssemblyType(uint32_t type)
	{
		switch (type) {
			case T_VOID:
				return llvm::Type::VoidTy;
			case T_BOOL:
				return llvm::Type::Int1Ty;
			case T_BYTE:
				return llvm::Type::Int8Ty;
			case T_SHORT:
				return llvm::Type::Int16Ty;
			case T_INT:
			case T_CHAR:
				return llvm::Type::Int32Ty;
			case T_LONG:
				return llvm::Type::Int64Ty;
			case T_FLOAT:
				return llvm::Type::FloatTy;
			case T_DOUBLE:
				return llvm::Type::DoubleTy;
		}

		assert(false);
		return NULL;
	}

	/*
	 * tries to cast the given value from the specified
	 * source type to destination type
	 */
	llvm::Value* ast::implicitTypeCast(
		const llvm::Type* src,
		const llvm::Type* dst,
		llvm::Value* value,
		ast::ModuleBuilder& mb)
	{
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
