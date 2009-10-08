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

#ifndef KENSHO_AST_SCOPE_HPP_
#define KENSHO_AST_SCOPE_HPP_

#include <kensho/ast/Symbol.hpp>

#include <string>
#include <vector>
#include <map>

namespace kensho {
namespace ast {

	class Scope {
	private:
		std::vector<std::map<std::string, Symbol*> > stack;
		uint32_t current;
	public:
		Scope() : current(0) {
			stack.push_back(std::map<std::string, Symbol*>());
		};

		void declareSymbol(Symbol* sym) {
			stack[current][sym->getName()] = sym;
		}

		bool isDeclared(const std::string& name) {
			int32_t last = stack.size() - 1;
			for (int32_t i = last; i >= 0; --i) {
				if (stack[i].count(name) > 0) {
					return true;
				}
			}
			return false;
		}

		Symbol* getSymbol(const std::string& name) {
			int32_t last = stack.size() - 1;
			for (int32_t i = last; i >= 0; --i) {
				std::map<std::string, Symbol*> map = stack[i];
				Symbol* sym = map[name];
				if (sym) {
					return sym;
				}
			}
			return NULL;
		}

		void push() {
			if (current + 1 == stack.size()) {
				stack.push_back(std::map<std::string, Symbol*>());
			}
			++current;
		}

		void pop() {
			assert(current > 0 && "cannot pop top scope");
			stack[current].clear();
			--current;
		}

		void popAll() {
			while (current > 0) {
				pop();
			}
		}
	};

}} // end ns

#endif /* KENSHO_AST_SCOPE_HPP_ */
