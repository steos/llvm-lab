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

#ifndef KENSHO_AST_FUNCTION_HPP_
#define KENSHO_AST_FUNCTION_HPP_

#include <kensho/ast/Callable.hpp>
#include <kensho/ast/Type.hpp>
#include <kensho/ast/ScopeProvider.hpp>
#include <string>
#include <vector>

namespace kensho {
namespace ast {

	/*
	 * a function declaration node
	 */
	class Function : public Callable, public ScopeProvider {
	protected:
		std::vector<std::string> parameterNames;
		std::vector<Buildable*> body;
		Scope symbols;
	public:
		Function(std::string name, Type* type) :
			Callable(name, type) {};
		Function(std::string name, Type* type, std::vector<Buildable*> body) :
			Callable(name, type), body(body) {};
		virtual void addParameter(std::string name, Type* type);
		virtual void prependParameter(std::string name, Type* type);
		void addBodyNode(Buildable*);
		virtual void assemble(ModuleBuilder& mb);

		virtual Scope& getScope() {
			return symbols;
		}
	};

	inline void Function::prependParameter(std::string name, Type* type) {
		Callable::prependParameter(type);
		parameterNames.insert(parameterNames.begin(), 1, name);
	}

	inline void Function::addParameter(std::string name, Type* type) {
		Callable::addParameter(type);
		parameterNames.push_back(name);
	}

	inline void Function::addBodyNode(Buildable* node) {
		body.push_back(node);
	}
}}

#endif /* KENSHO_AST_FUNCTION_HPP_ */
