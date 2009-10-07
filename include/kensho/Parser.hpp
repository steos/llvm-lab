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

namespace kensho {

	// forward declare ModuleBuilder class
	namespace ast {
		class ModuleBuilder;
	}

	/*
	 * The basic parse interface
	 */
	class Parser {
	public:
		virtual ast::ModuleBuilder* parse(const std::string& file) = 0;
		virtual bool canDumpAST() = 0;
		virtual void dumpAST() = 0;
		virtual ~Parser() {};
	};

} // end ns

#endif /* KENSHO_PARSER_HPP_ */
