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

#ifndef KENSHO_AST_SOURCELOCATIONAWARE_HPP_
#define KENSHO_AST_SOURCELOCATIONAWARE_HPP_

#include <stdint.h>

namespace kensho {
namespace ast {

	class SourceLocationAware {
	private:
		uint32_t line;
		uint32_t offset;
	public:
		SourceLocationAware() : line(0), offset(0) {};

		uint32_t getLine() {
			return line;
		}

		uint32_t getOffset() {
			return offset;
		}

		void setSourcePosition(uint32_t line, uint32_t offset) {
			this->line = line;
			this->offset = offset;
		}

		virtual ~SourceLocationAware() {};
	};

}} // end ns

#endif /* KENSHO_AST_SOURCELOCATIONAWARE_HPP_ */
