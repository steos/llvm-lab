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

#ifndef KENSHO_SCOPE_HPP_
#define KENSHO_SCOPE_HPP_

#include <llvm/Type.h>
#include <string>
#include <map>

namespace kensho {

	/*
	 * helper class that represents the symbol scope
	 */
	template <typename Symbol>
	class Scope {
	private:
		uint32_t current;
		std::vector<std::map<std::string, Symbol> > symbolStack;
	public:
		Scope() {
			symbolStack.push_back(std::map<std::string, Symbol>());
			current = 0;
		};

		bool hasSymbol(std::string);

		Symbol getSymbol(std::string);

		void addSymbol(std::string, Symbol);

		void removeSymbol(std::string);

		void pushScope();

		void popScope();

		void popAll();

		void clearScope();

		void popAllAndClear();

	};

	template <typename Symbol>
	inline void Scope<Symbol>::popAll() {
		while (current > 0) {
			symbolStack.erase(symbolStack.end());
			current--;
		}
	}

	template <typename Symbol>
	inline void Scope<Symbol>::clearScope() {
		symbolStack[current].clear();
	}

	template <typename Symbol>
	inline void Scope<Symbol>::popAllAndClear() {
		popAll();
		clearScope();
	}

	template <typename Symbol>
	inline bool Scope<Symbol>::hasSymbol(std::string name) {
		for (int32_t i = symbolStack.size() - 1; i >= 0; --i) {
			if (symbolStack[i].count(name) > 0) {
				return true;
			}
		}
		return false;
	}

	template <typename Symbol>
	inline Symbol Scope<Symbol>::getSymbol(std::string name) {
		for (int32_t i = symbolStack.size() - 1; i >= 0; --i) {
			if (symbolStack[i][name]) {
				return symbolStack[i][name];
			}
		}
		return NULL;
	}

	template <typename Symbol>
	inline void Scope<Symbol>::addSymbol(std::string name, Symbol type) {
		symbolStack[current][name] = type;
	}

	template <typename Symbol>
	inline void Scope<Symbol>::removeSymbol(std::string name) {
		symbolStack[current].erase(name);
	}

	template <typename Symbol>
	inline void Scope<Symbol>::pushScope() {
		symbolStack.push_back(std::map<std::string, Symbol>());
		current++;
	}

	template <typename Symbol>
	inline void Scope<Symbol>::popScope() {
		if (current > 0) {
			symbolStack.erase(symbolStack.end());
			current--;
		}
		else {
			assert(false && "cannot pop top scope");
		}
	}

} // end ns

#endif /* KENSHO_SCOPE_HPP_ */
