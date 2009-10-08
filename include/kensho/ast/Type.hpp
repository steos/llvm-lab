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

#ifndef KENSHO_AST_TYPE_HPP_
#define KENSHO_AST_TYPE_HPP_

#include <kensho/ast/tokens.hpp>
#include <llvm/Type.h>
#include <llvm/Value.h>

namespace kensho {
namespace ast {

	class ModuleBuilder;

	class Type {
	private:
		ModuleBuilder& mb;
		const llvm::Type* llvmType;
		int32_t token;
		std::string name;

		void assemble();

	public:

		Type(ModuleBuilder& mb, TypeToken token, std::string name) :
			mb(mb), llvmType(NULL), token(token), name(name) {
			assert(token != -1);
		};

		Type(ModuleBuilder& mb, const llvm::Type* llvmType) :
			mb(mb), llvmType(llvmType), token(-1) {};

		Type(ModuleBuilder& mb, const llvm::Type* llvmType, std::string name) :
			mb(mb), llvmType(llvmType), token(-1), name(name) {};

		const llvm::Type* getAssemblyType() {
			if (llvmType == NULL) {
				assemble();
			}
			return llvmType;
		}

		llvm::Value* getDefaultValue();

		std::string getName() {
			return name;
		}

		bool isVoid();

		bool isPrimitive();
	};

}} // end ns


#endif /* KENSHO_AST_TYPE_HPP_ */
