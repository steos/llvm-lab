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

#ifndef MODULEBUILDER_HPP_
#define MODULEBUILDER_HPP_

#include <llvm/Module.h>
#include <llvm/Support/IRBuilder.h>

namespace kensho {
namespace ast {

	class ModuleBuilder {
	private:
		llvm::Module module;
		llvm::IRBuilder<> irBuilder;
	public:
		ModuleBuilder(std::string name) : module(name) {};
		llvm::IRBuilder<>& getIRBuilder();
		~ModuleBuilder() {};
	};

	inline llvm::IRBuilder<>& ModuleBuilder::getIRBuilder() {
		return irBuilder;
	}

}} // end ns


#endif /* MODULEBUILDER_HPP_ */
