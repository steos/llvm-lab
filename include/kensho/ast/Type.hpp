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
#include <kensho/ast/SourceLocationAware.hpp>
#include <llvm/Type.h>
#include <llvm/Value.h>
#include <llvm/DerivedTypes.h>

namespace kensho {
namespace ast {

	class ModuleBuilder;

	/*
	 * Basic type class
	 */
	class Type : public SourceLocationAware {
	protected:

		const llvm::Type* llvmType;

		Type() : llvmType(NULL) {};

		Type(const llvm::Type* type) : llvmType(type) {};

	public:

		virtual const llvm::Type* getAssemblyType() {
			return llvmType;
		}

		virtual llvm::Value* getDefaultValue() const;

		virtual bool isPrimitive() const = 0;

		virtual bool isVoid() const = 0;

		static llvm::Value* cast(llvm::Value* value, const llvm::Type* ty, ModuleBuilder& mb);

		virtual ~Type() {};
	};

	/*
	 * Represents a primitive type
	 */
	class PrimitiveType : public Type {
	private:

		TypeToken token;

		PrimitiveType(TypeToken token, const llvm::Type* type) :
			Type(type), token(token) {};

	public:

		static Type* create(TypeToken token);

		bool isPrimitive() const{
			return true;
		}

		bool isVoid() const {
			return token == TyVoid;
		}
	};

	/*
	 * Represents a struct type
	 */
	class StructType : public Type {
	private:

		std::string name;
		std::vector<Type*> types;
		const llvm::Type* abstract;
		void assemble();

	public:
		StructType(std::string& name) :	name(name) {};

		StructType(std::string& name, std::vector<Type*> types) :
			name(name), types(types) {};

		void addMemberType(Type* type) {
			types.push_back(type);
		}

		const llvm::Type* getAbstractType() {
			return abstract;
		}

		std::string getName() {
			return name;
		}

		const llvm::Type* getAssemblyType() {
			if (llvmType == NULL) {
				assemble();
			}
			assert(llvmType != NULL);
			return llvmType;
		}

		bool isPrimitive() const {
			return false;
		}

		bool isVoid() const {
			return false;
		}
	};

}} // end ns


#endif /* KENSHO_AST_TYPE_HPP_ */
