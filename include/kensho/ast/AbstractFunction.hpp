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

#ifndef KENSHO_AST_ABSTRACTFUNCTION_HPP_
#define KENSHO_AST_ABSTRACTFUNCTION_HPP_

#include <kensho/ast/Callable.hpp>

namespace kensho {
namespace ast {

	/*
	 * AbstractFunction is the base class for all
	 * functions that have an implementation.
	 */
	class AbstractFunction : public Callable {
	protected:

		std::vector<std::string> parameterNames;
		std::vector<Buildable*> body;
		std::vector<llvm::Value*> parameterValues;

		virtual void assembleParameters(llvm::Function*, ModuleBuilder&);

	public:

		AbstractFunction(std::string name, Type* type) :
			Callable(name, type) {};

		AbstractFunction(std::string name, Type* type, std::vector<Buildable*> body) :
			Callable(name, type), body(body) {};

		void addNamedParameter(std::string name, Type* type) {
			Callable::addParameter(type);
			parameterNames.push_back(name);
		}

		void prependNamedParameter(std::string name, Type* type) {
			Callable::prependParameter(type);
			parameterNames.insert(parameterNames.begin(), 1, name);
		}

		void addBodyNode(Buildable* node) {
			body.push_back(node);
		}

		virtual void assemble(ModuleBuilder& mb);
	};

}}

#endif /* KENSHO_AST_ABSTRACTFUNCTION_HPP_ */
