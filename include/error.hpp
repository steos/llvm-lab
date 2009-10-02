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

#ifndef ERROR_HPP_
#define ERROR_HPP_

#include <string>
#include <iostream>

namespace kensho {

	class Error {
	protected:
		const std::string message;
	public:
		Error(const std::string& message) :
			message(message) {};

		std::string getMessage() const {
			return message;
		}

		virtual ~Error() {};
	};

	/*
	 * exception class for parse errors
	 */
	class ParseError : public Error {
	protected:
		const int32_t line;
	public:
		ParseError(const std::string& message) :
			Error(message), line(0) {};

		ParseError(const std::string& message, int32_t line) :
			Error(message), line(line) {};

		int32_t getLine() const {
			return line;
		}
	};
}

#endif /* ERROR_HPP_ */
