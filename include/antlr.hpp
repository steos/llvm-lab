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

#ifndef ANTLR_HPP_
#define ANTLR_HPP_

#include <KenshoLexer.h>
#include <KenshoParser.h>

namespace antlr {

	typedef ::pANTLR3_STRING string_t;
	typedef ::pANTLR3_BASE_TREE node_t;
	typedef ::pANTLR3_COMMON_TOKEN token_t;
	typedef ::pKenshoLexer lexer_t;
	typedef ::pKenshoParser parser_t;
	typedef ::pANTLR3_INPUT_STREAM istream_t;
	typedef ::pANTLR3_COMMON_TOKEN_STREAM tokstream_t;
	typedef ::KenshoParser_program_return ast_t;

	// helper functions
	inline std::string text(antlr::node_t& node, antlr::parser_t& parser) {
		int type = node->getType(node);
		std::string label(antlrTokenName(type));
		antlr::string_t text = node->getText(node);
		if (text != NULL) {
			label += " [\"";
			label += (char*)text->chars;
			label += "\"]";
		}
		return label;
	}
}


#endif /* ANTLR_HPP_ */
