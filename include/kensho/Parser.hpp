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

#ifndef KENSHO_PARSER_HPP_
#define KENSHO_PARSER_HPP_

#include <string>
#include <kensho/error.hpp>

// always include antlr last!
#include <antlr.hpp>

namespace kensho {

	/*
	 * facade for antlr c functions
	 */
	class Parser {
	private:
		antlr::istream_t input;
		antlr::tokstream_t tokenStream;
		antlr::lexer_t lexer;
		antlr::parser_t parser;
		antlr::ast_t ast;
		antlr::nodestream_t nodestream;
		antlr::treeparser_t treeparser;

		void free()
		{
			if (treeparser != NULL) {
				treeparser->free(treeparser);
				treeparser = NULL;
			}
			if (nodestream != NULL) {
				nodestream->free(nodestream);
				nodestream = NULL;
			}
			if (parser != NULL) {
				parser->free(parser);
				parser = NULL;
			}
			if (lexer != NULL) {
				lexer->free(lexer);
				lexer = NULL;
			}
			if (tokenStream != NULL) {
				tokenStream->free(tokenStream);
				tokenStream = NULL;
			}
			if (input != NULL) {
				input->close(input);
				input = NULL;
			}
		}

	public:
		Parser(const std::string& file) :
			nodestream(NULL), treeparser(NULL)
		{
			input = antlr3AsciiFileStreamNew((pANTLR3_UINT8)file.c_str());
			lexer = ::KenshoLexerNew(input);
			if (lexer == NULL) {
				free();
				throw(Error("out of memory while allocating lexer"));
			}
			tokenStream = antlr3CommonTokenStreamSourceNew(
				ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
			if (tokenStream == NULL) {
				free();
				throw(Error("out of memory while allocating parser"));
			}
			parser = ::KenshoParserNew(tokenStream);
			if (parser == NULL) {
				free();
				throw(Error("failed to allocate parser"));
			}
		};

		antlr::ast_t parse()
		{
			ast = parser->program(parser);

			if (lexer->pLexer->rec->state->errorCount > 0) {
				throw(ParseError("The lexer returned with errors"));
			}
			if (parser->pParser->rec->state->errorCount > 0) {
				throw(ParseError("The parser returned with errors"));
			}
			else if (ast.tree == NULL) {
				throw(ParseError("The lexer returned with errors"));
			}

			return ast;
		}

		antlr::lexer_t getLexer() {
			return lexer;
		}

		antlr::parser_t getParser() {
			return parser;
		}

		void dumpNode(antlr::node_t node, std::string indent) {
			if (node == NULL) {
				std::cout << indent << "[null]\n";
				return;
			}
			if (!node->isNilNode(node)) {
				std::cout << indent << antlr::text(node) << "\n";
			}
			int count = node->getChildCount(node);
			for (int i = 0; i < count; ++i) {
				std::string subIndent = indent;
				subIndent += "  ";
				dumpNode((antlr::node_t)node->getChild(node, i), subIndent);
			}
		}

		void dumpNode(antlr::node_t node) {
			dumpNode(node, "");
		}

		void createTreeParser() {
			if (ast.tree == NULL) {
				parse();
			}
			nodestream = antlr3CommonTreeNodeStreamNewTree(ast.tree, ANTLR3_SIZE_HINT);
			if (nodestream == NULL) {
				throw(Error("failed to allocate node stream"));
			}
			treeparser = KenshoTreeParserNew(nodestream);
			if (treeparser == NULL) {
				throw(Error("failed to allocate tree parser"));
			}
		}

		antlr::treeast_t parseTree() {
			antlr::treeast_t ast = treeparser->program(treeparser);

			assert(treeparser->pTreeParser->rec->state->errorCount == 0
				&& "tree parser returned with errors");

			return ast;
		}

		~Parser() {
			free();
		}
	};

} // end ns

#endif /* KENSHO_PARSER_HPP_ */
