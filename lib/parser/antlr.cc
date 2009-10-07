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

#include <antlr.hpp>

	void antlr::Parser::free() {
		if (treeparser != NULL) {
			treeparser->free(treeparser);
			treeparser = NULL;
		}
		if (nodestream != NULL) {
			nodestream->free(nodestream);
			nodestream = NULL;
		}
		if (parser != NULL) {
			delete parser->errors;
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

	kensho::ast::ModuleBuilder* antlr::Parser::parse(const std::string& file)
	{
		init(file);

		ast = parser->program(parser);

		if (lexer->pLexer->rec->state->errorCount > 0) {
			throw(kensho::ParseError("The lexer returned with errors"));
		}
		if (parser->pParser->rec->state->errorCount > 0) {
			uint32_t num = parser->errors->size();
			std::string out;
			for (uint32_t i = 0; i < num; i++) {
				out += parser->errors->at(i);
				if (i + 1 < num) {
					out += "\n";
				}
			}
			throw(kensho::ParseError(out));
		}
		else if (ast.tree == NULL) {
			throw(kensho::ParseError("The lexer returned with errors"));
		}

		createTreeParser();
		treeast_t tree = parseTree();
		return tree.builder;
	}

	void antlr::Parser::dumpNode(antlr::node_t node, std::string indent) {
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

	void antlr::Parser::init(const std::string& file) {
		input = antlr3AsciiFileStreamNew((pANTLR3_UINT8)file.c_str());
		lexer = ::KenshoLexerNew(input);
		if (lexer == NULL) {
			free();
			throw(kensho::Error("out of memory while allocating lexer"));
		}
		tokenStream = antlr3CommonTokenStreamSourceNew(
			ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
		if (tokenStream == NULL) {
			free();
			throw(kensho::Error("out of memory while allocating parser"));
		}
		parser = ::KenshoParserNew(tokenStream);
		if (parser == NULL) {
			free();
			throw(kensho::Error("failed to allocate parser"));
		}
	}

	void antlr::Parser::createTreeParser() {
		nodestream = antlr3CommonTreeNodeStreamNewTree(ast.tree, ANTLR3_SIZE_HINT);
		if (nodestream == NULL) {
			throw(kensho::Error("failed to allocate node stream"));
		}
		treeparser = KenshoTreeParserNew(nodestream);
		if (treeparser == NULL) {
			throw(kensho::Error("failed to allocate tree parser"));
		}
	}

	antlr::treeast_t antlr::Parser::parseTree() {
		antlr::treeast_t ast = treeparser->program(treeparser);

		assert(treeparser->pTreeParser->rec->state->errorCount == 0
			&& "tree parser returned with errors");

		return ast;
	}

	void kenshoAntlrErrorReporter(pANTLR3_BASE_RECOGNIZER rec, pANTLR3_UINT8* tokens) {
		pANTLR3_PARSER parser;
		pANTLR3_EXCEPTION ex = rec->state->exception;
		std::string message;

		if (ex->streamName == NULL) {
			if (((pANTLR3_COMMON_TOKEN)(ex->token))->type == ANTLR3_TOKEN_EOF) {
				message += "Parse error at end of input";
			}
			else {
				message += "Parse error in unknown source";
			}
		}
		else {
			pANTLR3_STRING sname = ex->streamName->to8(ex->streamName);
			message += "Parse error in " + std::string((char*)sname->chars);
		}

		message += " on line " + boost::lexical_cast<std::string>(rec->state->exception->line);

		parser = (pANTLR3_PARSER)rec->super;

		message += " at offset " + boost::lexical_cast<std::string>(
			rec->state->exception->charPositionInLine);
		message += ": ";

		switch (ex->type) {
			case ANTLR3_UNWANTED_TOKEN_EXCEPTION: {
				message += "extraneous token";
				if (tokens != NULL) {
					if (ex->expecting == ANTLR3_TOKEN_EOF) {
						message += ", expected <EOF>";
					}
					else {
						message += ", expected " + std::string((char*)tokens[ex->expecting]);
					}
				}
				else {
					message += ", expected " + ex->expecting;
				}
			}
			break;
			case ANTLR3_MISSING_TOKEN_EXCEPTION: {
				message += "missing token";
				if (tokens != NULL) {
					if (ex->expecting == ANTLR3_TOKEN_EOF) {
						message += ", expected <EOF>";
					}
					else {
						message += ", expected " + std::string((char*)tokens[ex->expecting]);
					}
				}
				else {
					message += ", expected " + ex->expecting;
				}
			}
			break;
			case ANTLR3_RECOGNITION_EXCEPTION:
				message += "syntax error";
			break;
			case ANTLR3_MISMATCHED_TOKEN_EXCEPTION: {
				if (tokens == NULL) {
					message += "syntax error";
				}
				else {
					if (ex->expecting == ANTLR3_TOKEN_EOF) {
						message += "expected <EOF>";
					}
					else {
						message += "expected " + std::string((char*)(tokens[ex->expecting]));
					}
				}
			}
			break;
			case ANTLR3_NO_VIABLE_ALT_EXCEPTION:
				message += "cannot match to any predicted input";
			break;
			case ANTLR3_MISMATCHED_SET_EXCEPTION: {
				message += "unexpected token, expected one of ";
				pANTLR3_BITSET errBits = antlr3BitsetLoad(ex->expectingSet);
				pANTLR3_INT32 list = errBits->toIntList(errBits);
				// first element in the list is the number of elements
				int32_t num = list[0];
				for (int32_t i = 0; i < num; ++i) {
					message += std::string((char*)tokens[i + 1]);
					if (i < num - 1) {
						message += ", ";
					}
				}
			}
			break;
			case ANTLR3_EARLY_EXIT_EXCEPTION:
				message += "missing tokens";
			break;
			default:
				message += "syntax not recognized";
			break;
		}

		pKenshoParser impl = (pKenshoParser)parser->super;
		assert(impl != NULL);
		impl->errors->push_back(message);
	}
