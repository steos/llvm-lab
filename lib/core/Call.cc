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

#include <kensho/ast/Call.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/ast/Callable.hpp>
#include <kensho/error.hpp>
#include <boost/lexical_cast.hpp>

using namespace kensho;

	void ast::Call::assemble(ast::ModuleBuilder& mb) {
		if (!mb.isFunctionDeclared(name)) {
			throw(ParseError("function " + name + " is not declared",
				getLine(), getOffset()));
		}

		Callable* fun = mb.getFunction(name);
		if (fun == NULL) {
			throw(ParseError("symbol " + name + " is not a function and cannot be called",
				getLine(), getOffset()));
		}

		uint32_t numParams = fun->countParameters();
		if (numParams != arguments.size()) {
			throw(ParseError("argument count mismatch in call to function " + name,
				getLine(), getOffset()));
		}

		std::vector<const llvm::Type*> expected = fun->getParameterTypes();
		std::vector<llvm::Value*> values;

		for (uint32_t i = 0; i < numParams; ++i) {
			Node* arg = arguments.at(i);
			llvm::Value* val = arg->emit(mb);
			if (expected.at(i) != val->getType()) {
				throw(ParseError("type mismatch in call to function " + name
					+ " for argument #" + boost::lexical_cast<std::string>(i),
					getLine(), getOffset()));
			}
			values.push_back(val);
		}

		value = mb.getIRBuilder().CreateCall(
			fun->getValue(), values.begin(), values.end());
	}
