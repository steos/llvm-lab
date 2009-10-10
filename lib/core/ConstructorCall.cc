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

#include <kensho/ast/ConstructorCall.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/ast/Callable.hpp>
#include <kensho/error.hpp>

using namespace kensho;

	void ast::ConstructorCall::assemble(ast::ModuleBuilder& mb) {
		StructType* sty = mb.getUserType(name);
		if (sty == NULL) {
			throw(ParseError("cannot construct undeclared type " + name,
				getLine(), getOffset()));
		}

		const llvm::Type* ty = sty->getAssemblyType();
		const llvm::PointerType* pty = llvm::cast<llvm::PointerType>(ty);
		assert(pty != NULL && "struct type assembly must be pointer type");
		const llvm::Type* ety = pty->getElementType();
		ptr = mb.getIRBuilder().CreateMalloc(ety, 0, std::string(name + "Ref").c_str());
		arguments.insert(arguments.begin(), new ContextPointer(ptr));

		name = name + ".new";

		Call::assemble(mb);

		value = ptr;
	}
