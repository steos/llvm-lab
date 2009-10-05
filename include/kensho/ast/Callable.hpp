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

#ifndef KENSHO_AST_CALLABLE_HPP_
#define KENSHO_AST_CALLABLE_HPP_

#include <kensho/ast/Node.hpp>
#include <kensho/ast/util.hpp>
#include <llvm/Type.h>

namespace kensho {
namespace ast {

	/*
	 * Base class for nodes that result in a callable
	 * function, i.e. captures the name, return type and parameter types
	 * and is implemented by the function node as well as native function nodes
	 * and possibly future variations of a callable object like closures.
	 * Callable extends Symbol so we can easily support first class functions.
	 */
	class Callable : public Node {
	protected:
		std::string name;
		uint32_t type;
		const llvm::Type* assemblyType;
		std::vector<const llvm::Type*> parameterTypes;
		virtual void assemble(ModuleBuilder& mb);
	public:
		Callable(std::string name, uint32_t type) :
			name(name), type(type) {
			assemblyType = toAssemblyType(type);
			assert(assemblyType != NULL);
		};
		virtual void emitDefinition(ModuleBuilder& mb);
		virtual void addParameter(uint32_t type);
		virtual void prependParameter(const llvm::Type*);
		std::vector<const::llvm::Type*> getParameterTypes();
		int countParameters();
		std::string getName();
		void setName(std::string name);
		const llvm::Type* getType();
		virtual ~Callable() {};
	};

	inline const llvm::Type* Callable::getType() {
		return assemblyType;
	}

	inline std::string Callable::getName() {
		return name;
	}

	inline void Callable::setName(std::string name) {
		this->name = name;
	}

	inline void Callable::prependParameter(const llvm::Type* type) {
		parameterTypes.insert(parameterTypes.begin(), type);
	}

	inline void Callable::addParameter(uint32_t type) {
		const llvm::Type* asmt = toAssemblyType(type);
		assert(asmt != NULL);
		parameterTypes.push_back(asmt);
	}

	inline std::vector<const::llvm::Type*> Callable::getParameterTypes() {
		return parameterTypes;
	}

	inline int Callable::countParameters() {
		return parameterTypes.size();
	}

}} // end ns

#endif /* KENSHO_AST_CALLABLE_HPP_ */
