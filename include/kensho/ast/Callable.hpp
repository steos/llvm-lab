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

#include <kensho/ast/Symbol.hpp>

#include <llvm/Type.h>

namespace kensho {
namespace ast {

	/*
	 * Base class for nodes that result in a callable
	 * function, i.e. captures the name, return type and parameter types
	 */
	class Callable : public Symbol {
	protected:

		std::vector<Type*> parameterTypes;

	public:

		Callable(std::string name, Type* type) :
			Symbol(name, type) {};

		virtual void prepareCall(std::vector<Node*>* args, ModuleBuilder& mb) {
			// default is noop
		}

		virtual void assemble(ModuleBuilder& mb);

		virtual void emitDefinition(ModuleBuilder& mb);

		virtual void addParameter(Type* type) {
			parameterTypes.push_back(type);
		}

		virtual void prependParameter(Type* type) {
			parameterTypes.insert(parameterTypes.begin(), type);
		}

		std::vector<Type*> getParameterTypes() {
			return parameterTypes;
		}

		int countParameters() {
			return parameterTypes.size();
		}

		Type* getParameterType(uint32_t offset) {
			assert(offset < parameterTypes.size());
			return parameterTypes.at(offset);
		}

		virtual ~Callable() {};
	};

}} // end ns

#endif /* KENSHO_AST_CALLABLE_HPP_ */
