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

#ifndef KENSHO_AST_BUILDABLE_HPP_
#define KENSHO_AST_BUILDABLE_HPP_

#include <kensho/ast/SourceLocationAware.hpp>

namespace kensho {
namespace ast {

	/*
	 * forward declaration of ModuleBuilder class
	 */
	class ModuleBuilder;

	/*
	 * Buildable is the most basic class everyone has to extend to
	 * participate in the assembly of a kensho module. It extends
	 * SourceLocationAware so we can track problems during code generation
	 * back to the source code position.
	 */
	class Buildable : public SourceLocationAware {
	public:
		/*
		 * assembles this Buildable
		 */
		virtual void assemble(ModuleBuilder&) = 0;

		/*
		 * determines whether this buildable represents a return statement
		 *
		 * This exists because we need to check that often and it should
		 * be faster this way than using dynamic_cast all the time.
		 */
		virtual bool isReturnStatement() {
			return false;
		}
	};

}} // end ns

#endif /* KENSHO_AST_BUILDABLE_HPP_ */
