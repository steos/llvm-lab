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

#include <string>

// Must include TreeParser first because
// it includes ast.hpp and this in turn includes
// llvm headers. Otherwise antlr3.h gets included
// before the llvm headers and this messes everything up, i.e.
// you'll get compile errors that look like types are missing
// from the llvm headers (antlr3c obviously redefines some
// vital stuff for whatever idiotic reason...)
#include <KenshoTreeParser.h>
#include <KenshoLexer.h>
#include <KenshoParser.h>

namespace antlr {

	typedef ::pANTLR3_STRING string_t;
	typedef ::pANTLR3_BASE_TREE node_t;
	typedef ::pANTLR3_COMMON_TOKEN token_t;
	typedef ::pKenshoLexer lexer_t;
	typedef ::pKenshoParser parser_t;
	typedef ::pKenshoTreeParser treeparser_t;
	typedef ::pANTLR3_INPUT_STREAM istream_t;
	typedef ::pANTLR3_COMMON_TOKEN_STREAM tokstream_t;
	typedef ::KenshoParser_program_return ast_t;
	typedef ::KenshoTreeParser_program_return treeast_t;
	typedef ::pANTLR3_COMMON_TREE_NODE_STREAM nodestream_t;

	// helper functions
	inline std::string text(antlr::node_t& node) {
		int type = node->getType(node);
		std::string label(antlrTokenName(type));
		antlr::string_t text = node->getText(node);
		if (text != NULL) {
			label += " [ ";
			label += (char*)text->chars;
			label += " ]";
		}
		return label;
	}
} // end ns

void kenshoAntlrErrorReporter(
	pANTLR3_BASE_RECOGNIZER rec, pANTLR3_UINT8* tokens);


#endif /* ANTLR_HPP_ */
