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
#include <llvm/Type.h>
#include <string>
#include <vector>

namespace kensho {
namespace ast {

	/*
	 * a function declaration node
	 */
	class Function : public Callable {
	protected:
		std::vector<std::string> parameterNames;
		std::vector<Node*> body;
		virtual void assemble(ModuleBuilder& mb);
	public:
		Function(std::string name, uint32_t type) :
			Callable(name, type) {};
		Function(std::string name, uint32_t type, std::vector<Node*> body) :
			Callable(name, type), body(body) {};
		virtual void addParameter(std::string name, uint32_t type);
		virtual void prependParameter(std::string name, const llvm::Type* type);
		void addBodyNode(Node*);
		const llvm::Type* getParameterType(uint32_t offset);
	};

	inline const llvm::Type* Function::getParameterType(uint32_t offset) {
		assert(offset < parameterTypes.size());
		return parameterTypes.at(offset);
	}

	inline void Function::prependParameter(std::string name, const llvm::Type* type) {
		Callable::prependParameter(type);
		parameterNames.insert(parameterNames.begin(), 1, name);
	}

	inline void Function::addParameter(std::string name, uint32_t type) {
		Callable::addParameter(type);
		parameterNames.push_back(name);
	}

	inline void Function::addBodyNode(Node* node) {
		body.push_back(node);
	}
}}

#endif /* KENSHO_AST_FUNCTION_HPP_ */
