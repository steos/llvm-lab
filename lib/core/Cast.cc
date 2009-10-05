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

#include <kensho/ast/Cast.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/error.hpp>

using namespace kensho;

	void ast::Cast::assemble(ast::ModuleBuilder& mb) {
		// emit expression to cast
		llvm::Value* val = expression->emit(mb);
		// do nothing if types match
		if (val->getType() == assemblyType) {
			return;
		}
		// case for integer to integer cast (truncation or extension)
		if (val->getType()->isInteger() && assemblyType->isInteger()) {
			uint32_t valBits = val->getType()->getPrimitiveSizeInBits();
			uint32_t dstBits = assemblyType->getPrimitiveSizeInBits();
			// special case for boolean => disallow
			if ((valBits == 1 && dstBits > 1) || (dstBits == 1 && valBits > 1)) {
				throw(ParseError("cannot cast numeric value to boolean",
					getLine(), getOffset()));
			}
			// truncation
			if (valBits > dstBits) {
				value = mb.getIRBuilder().CreateTrunc(val, assemblyType);
			}
			// extension
			else if (valBits < dstBits) {
				value = mb.getIRBuilder().CreateSExt(val, assemblyType);
			}
			else {
				// can't happen
				assert(false);
			}
		}
		// floating point to integer
		if (val->getType()->isFloatingPoint() && assemblyType->isInteger()) {
			// disallow bool
			if (assemblyType->getPrimitiveSizeInBits() == 1) {
				throw(ParseError("cannot cast floating point to boolean",
					getLine(), getOffset()));
			}
			value = mb.getIRBuilder().CreateFPToSI(val, assemblyType);
		}
		// integer to floating point
		if (val->getType()->isInteger() && assemblyType->isFloatingPoint()) {
			// disallow boolean
			if (val->getType()->getPrimitiveSizeInBits() == 1) {
				throw(ParseError("cannot cast boolean to floating point",
					getLine(), getOffset()));
			}
			value = mb.getIRBuilder().CreateSIToFP(val, assemblyType);
		}
		// floating point to floating point
		if (val->getType()->isFloatingPoint() && assemblyType->isFloatingPoint()) {
			uint32_t dstBits = assemblyType->getPrimitiveSizeInBits();
			uint32_t srcBits = val->getType()->getPrimitiveSizeInBits();
			// truncate
			if (srcBits < dstBits) {
				value = mb.getIRBuilder().CreateFPTrunc(val, assemblyType);
			}
			// extend
			else if (dstBits > srcBits) {
				value = mb.getIRBuilder().CreateFPExt(val, assemblyType);
			}
		}

		if (value == NULL) {
			throw(ParseError("cannot cast non-numeric type",
				getLine(), getOffset()));
		}
	}
