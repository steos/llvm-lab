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
