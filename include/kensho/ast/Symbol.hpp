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

#ifndef KENSHO_AST_SYMBOL_HPP_
#define KENSHO_AST_SYMBOL_HPP_

#include <kensho/ast/Type.hpp>
#include <kensho/ast/Node.hpp>
#include <llvm/Value.h>
#include <string>

namespace kensho {
namespace ast {

	/*
	 * Symbol is the base class for every node that
	 * represents a symbol in the symbol table. It extends
	 * Node because it has a value.
	 */
	class Symbol : public Node {
	protected:

		std::string name;
		Type* type;

	public:

		/*
		 * creates a new symbol with the given name and type
		 */
		Symbol(std::string name, Type* type) : name(name), type(type) {};

		/*
		 * retrieves the type of this symbol
		 */
		virtual Type* getType() {
			return type;
		}

		/*
		 * retrieves this symbols name
		 */
		virtual std::string getName() const {
			return name;
		}

		virtual bool isHidable() {
			return false;
		}

		virtual ~Symbol() {};
	};

	class MutableSymbol : public Symbol {
	public:

		MutableSymbol(std::string name, Type* type) :
			Symbol(name, type) {};

		virtual llvm::Value* assembleStore(llvm::Value*, ModuleBuilder&) = 0;

		virtual llvm::Value* assembleLoad(ModuleBuilder&) = 0;
	};

}}

#endif /* KENSHO_AST_SYMBOL_HPP_ */
