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

using namespace kensho;

	void ast::ConstructorCall::assemble(ast::ModuleBuilder& mb) {
		assert(false && "ConstructorCall::assemble not yet implemented");
		/* TODO
		 * for "struct foo { int a; bool b; }" we have:
		 * %foo = type { i32, i1 }
		 * define void @foo.new(%foo* %this) {
		 * 		ret void
		 * }
		 *
		 * Kensho				LLVM
		 *
		 * foo bar;				%bar = alloca %foo* ; yields %foo**
		 * bar = new foo();		%tmp = malloc %foo	; yields %foo*
		 * 						store %foo* %tmp, %foo** %bar
		 * 						call void @foo.new(%foo* %bar)
		 *
		 * variables access then yields
		 * %bar1 = load %foo** %bar ; yields %foo*
		 *
		 * member access must only work on pointers to struct types:
		 * "bar.a = 3" must result in
		 * %a = getelementptr %foo* %bar1, i32 0, i32 0 ; yields i32*
		 * store i32 3, i32* %a
		 */
	}
