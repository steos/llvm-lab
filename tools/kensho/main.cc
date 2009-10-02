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

#include <iostream>
#include <vector>
#include <string>

// always include antlr last!
#include <parser.hpp>

	using namespace kensho;

	enum ExitCode {
		EXIT_OK = 0,
		EXIT_USAGE_ERROR = -1,
		EXIT_PARSE_ERROR = -2,
		EXIT_ERROR = -99
	};

	enum Mode {
		MODE_DUMP_AST = 0,
		MODE_DUMP_IR = 1,
		MODE_RUN = 2
	};

	struct Options {
		std::vector<std::string> files;
		Mode mode;
		bool optimize;
		bool mem2reg;
	};

	void printUsage() {
		std::cout
			<< "Usage: kensho [Options] file...\n"
			<< "Options:\n"
			<< "  -ast           dump the AST to stdout and exit\n"
			<< "  -ir            dump the LLVM IR to stdout and exit\n"
			<< "  -o             enable optimizations\n"
			<< "  -no-mem2reg    disable stack to register promotion\n"

			<< "\n(c) 2009 Stefan Oestreicher"
		;
	}

	Options parse_args(int argc, char** argv) {
		Options op;
		op.mode = MODE_DUMP_IR;
		op.optimize = false;
		op.mem2reg = false;

		for (int i = 1; i < argc; i++) {
			if (std::strcmp(argv[i], "-ast") == 0) {
				op.mode = MODE_DUMP_AST;
			}
			else if (std::strcmp(argv[i], "-ir") == 0) {
				op.mode = MODE_DUMP_IR;
			}
			else if (std::strcmp(argv[i], "-o") == 0) {
				op.optimize = true;
			}
			else if (std::strcmp(argv[i], "-no-mem2reg") == 0) {
				op.mem2reg = false;
			}
			else {
				op.files.push_back(std::string(argv[i]));
			}
		}

		return op;
	}

	int main(int argc, char** argv)
	{
		try {
			if (argc == 1) {
				printUsage();
				return EXIT_OK;
			}

			Options op = parse_args(argc, argv);

			if (op.files.size() == 0) {
				std::cerr << "Error: Missing input files\n";
				printUsage();
				return EXIT_USAGE_ERROR;
			}

			const std::string file = op.files.at(0);
			Parser parser(file);
			antlr::ast_t ast = parser.parse();

			if (op.mode == MODE_DUMP_AST) {
				parser.dumpNode(ast.tree);
				return EXIT_OK;
			}

			parser.createTreeParser();
			antlr::treeast_t treeAst = parser.parseTree();
			ast::ModuleBuilder mb("default", treeAst.functions);
			mb.build();

			if (op.mode == MODE_DUMP_IR) {
				mb.getModule()->dump();
				return EXIT_OK;
			}

			// TODO JIT it or emit bitcode or assembly
		}
		catch (ParseError& e) {
			std::cerr << e.getMessage();
			if (e.getLine() > 0) {
				std::cerr << " on line " << e.getLine();
			}
			std::cerr << "\n";
			return EXIT_PARSE_ERROR;
		}
		catch (Error& e) {
			std::cerr << e.getMessage() << "\n";
			return EXIT_ERROR;
		}

		return EXIT_OK;
	}
