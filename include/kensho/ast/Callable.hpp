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
#include <kensho/ast/Type.hpp>

#include <kensho/ast/util.hpp>
#include <llvm/Type.h>

namespace kensho {
namespace ast {

	/*
	 * Base class for nodes that result in a callable
	 * function, i.e. captures the name, return type and parameter types
	 */
	class Callable : public Node {
	protected:
		std::string name;
		Type* type;
		std::vector<Type*> parameterTypes;
		virtual void assemble(ModuleBuilder& mb);
	public:
		Callable(std::string name, Type* type) :
			name(name), type(type) {};
		virtual void emitDefinition(ModuleBuilder& mb);
		virtual void addParameter(Type* type);
		virtual void prependParameter(Type*);
		std::vector<Type*> getParameterTypes();
		int countParameters();
		std::string getName();
		void setName(std::string name);
		const Type* getType();
		Type* getParameterType(uint32_t offset);
		virtual ~Callable() {};
	};

	inline Type* Callable::getParameterType(uint32_t offset) {
		assert(offset < parameterTypes.size());
		return parameterTypes.at(offset);
	}

	inline const Type* Callable::getType() {
		return type;
	}

	inline std::string Callable::getName() {
		return name;
	}

	inline void Callable::setName(std::string name) {
		this->name = name;
	}

	inline void Callable::prependParameter(Type* type) {
		parameterTypes.insert(parameterTypes.begin(), type);
	}

	inline void Callable::addParameter(Type* type) {
		parameterTypes.push_back(type);
	}

	inline std::vector<Type*> Callable::getParameterTypes() {
		return parameterTypes;
	}

	inline int Callable::countParameters() {
		return parameterTypes.size();
	}

}} // end ns

#endif /* KENSHO_AST_CALLABLE_HPP_ */
