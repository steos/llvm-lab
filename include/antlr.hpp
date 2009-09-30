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

#include <error.hpp>

#include <KenshoLexer.h>
#include <KenshoParser.h>

namespace kensho {
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
	inline std::string text(node_t node) {
		string_t text = node->getText(node);
		if (text != NULL) {
			return std::string((char*)text->chars);
		}
		int type = node->getType(node);
		switch (type) {
			case LIT:
				return "LIT";
			case UNOP:
				return "UNOP";
			case BINOP:
				return "BINOP";
		}
		return "[UNKNOWN]";
	}

	/*
	 * exception class for antlr errors
	 */
	class ParseError : public kensho::Error {
	protected:
		const int32_t line;
	public:
		ParseError(const std::string& message) :
			kensho::Error(message), line(0) {};

		ParseError(const std::string& message, int32_t line) :
			kensho::Error(message), line(line) {};

		int32_t getLine() const {
			return line;
		}
	};

	/*
	 * facade for antlr c functions
	 */
	class Parser {
	private:
		istream_t input;
		tokstream_t tokenStream;
		lexer_t lexer;
		parser_t parser;

		void free()
		{
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
		Parser(const std::string& file)
		{
			input = antlr3AsciiFileStreamNew((pANTLR3_UINT8)file.c_str());
			lexer = ::KenshoLexerNew(input);
			if (lexer == NULL) {
				free();
				throw(ParseError("out of memory while allocating lexer"));
			}
			tokenStream = antlr3CommonTokenStreamSourceNew(
				ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
			if (tokenStream == NULL) {
				free();
				throw(ParseError("out of memory while allocating parser"));
			}
			parser = ::KenshoParserNew(tokenStream);
			if (parser == NULL) {
				free();
				throw(ParseError("failed to allocate parser"));
			}
		};

		ast_t parse()
		{
			ast_t ast = parser->program(parser);

			if (parser->pParser->rec->state->errorCount > 0) {
				throw(ParseError("The parser returned with errors"));
			}
			else if (ast.tree == NULL) {
				throw(ParseError("The lexer returned with errors"));
			}

			return ast;
		}

		lexer_t getLexer() {
			return lexer;
		}

		parser_t getParser() {
			return parser;
		}

		void dumpNode(node_t node, std::string indent) {
			if (node == NULL) {
				std::cout << indent << "[null]\n";
				return;
			}
			if (!node->isNilNode(node)) {
				std::cout << indent << text(node) << "\n";
			}
			int count = node->getChildCount(node);
			for (int i = 0; i < count; ++i) {
				std::string subIndent = indent;
				subIndent += "  ";
				dumpNode((node_t)node->getChild(node, i), subIndent);
			}
		}

		void dumpNode(node_t node) {
			dumpNode(node, "");
		}

		~Parser() {
			free();
		}
	};
}}


#endif /* ANTLR_HPP_ */
