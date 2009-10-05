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

#ifndef KENSHO_AST_LITERAL_HPP_
#define KENSHO_AST_LITERAL_HPP_

#include <kensho/ast/Node.hpp>

namespace kensho {
namespace ast {

	/*
	 * a literal, i.e. integer, float, boolean or character literal
	 */
	class Literal : public Node {
	private:
		uint32_t tokenType;
		std::string text;
	protected:
		virtual void assemble(ModuleBuilder& mb);
	public:
		Literal(uint32_t tokenType, std::string text) : tokenType(tokenType), text(text) {};
		uint32_t getTokenType() {
			return tokenType;
		}
		std::string& getText() {
			return text;
		}
		static bool hasDoubleSuffix(std::string& str) {
			int last = str.length() - 1;
			return str[last] == 'd' || str[last] == 'D';
		}

		static bool hasFloatSuffix(std::string& str) {
			int last = str.length() - 1;
			return str[last] == 'f' || str[last] == 'F';
		}

		static bool hasLongSuffix(std::string& str) {
			int last = str.length() - 1;
			return str[last] == 'l' || str[last] == 'L';
		}
	};

}} // end ns


#endif /* KENSHO_AST_LITERAL_HPP_ */
