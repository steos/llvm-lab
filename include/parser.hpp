/*
 * parser.hpp
 *
 *  Created on: 02.10.2009
 *      Author: stefan
 */

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <string>
#include <error.hpp>

// always include antlr last!
#include <antlr.hpp>

namespace kensho {

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
		antlr::istream_t input;
		antlr::tokstream_t tokenStream;
		antlr::lexer_t lexer;
		antlr::parser_t parser;

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

		antlr::ast_t parse()
		{
			antlr::ast_t ast = parser->program(parser);

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
				std::cout << indent << antlr::text(node, parser) << "\n";
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

		~Parser() {
			free();
		}
	};

} // end ns

#endif /* PARSER_HPP_ */
