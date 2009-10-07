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

#include <kensho/ast/Literal.hpp>
#include <llvm/Type.h>
#include <llvm/Constants.h>
#include <llvm/Support/IRBuilder.h>
#include <boost/lexical_cast.hpp>

using namespace kensho;

	void ast::Literal::assemble(ast::ModuleBuilder& mb) {
		switch (tokenType) {
			case LitTrue:
				value = llvm::ConstantInt::get(llvm::Type::Int1Ty, 1);
				break;
			case LitFalse:
				value = llvm::ConstantInt::get(llvm::Type::Int1Ty, 0);
				break;
			case LitInt: {
				bool isLong = hasLongSuffix(text);
				const llvm::Type* type = isLong ?
						llvm::Type::Int64Ty : llvm::Type::Int32Ty;
				value = llvm::ConstantInt::get(
					type,
					isLong ?
						boost::lexical_cast<int64_t>(text) :
						boost::lexical_cast<int32_t>(text),
					true
				);
				break;
			}
			case LitFloat: {
				bool isFloat = hasFloatSuffix(text);
				const llvm::Type* type = hasFloatSuffix(text) ?
						llvm::Type::FloatTy : llvm::Type::DoubleTy;
				if (isFloat || hasDoubleSuffix(text)) {
					text.erase(text.length() - 1, 1);
				}

				value = llvm::ConstantFP::get(
					type,
					isFloat ?
						boost::lexical_cast<float>(text) :
						boost::lexical_cast<double>(text)
				);
				break;
			}
			case LitHex:
			case LitOct: {
				long int val = std::strtol(
					text.c_str(),
					NULL,
					tokenType == LitHex ? 16 : 8
				);
				value = llvm::ConstantInt::get(llvm::Type::Int32Ty, val, true);
				break;
			}
			case LitChar: {
				char c = text[1];
				if (c == '\\') {
					switch (text[2]) {
					case 'n':
						c = '\n';
						break;
					case 'r':
						c = '\r';
						break;
					case 't':
						c = '\t';
						break;
					default:
						assert(false && "unhandled escape sequence");
					}
				}
				value = llvm::ConstantInt::get(llvm::Type::Int32Ty, c, true);
				break;
			}
			default:
				assert(false && "missing literal type in switch");
		}
	}
