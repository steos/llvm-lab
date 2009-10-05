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

#include <kensho/ast/Struct.hpp>
#include <kensho/ast/ModuleBuilder.hpp>

#include <llvm/TypeSymbolTable.h>

using namespace kensho;

	void ast::Struct::assemble(ModuleBuilder& mb) {
		// TODO
		// assert(false && "Struct::assemble not yet implemented");
	}

	void ast::Struct::emitDefinition(ModuleBuilder& mb) {
		assemblyType = llvm::StructType::get(variables, false);
		mb.getModule()->getTypeSymbolTable().insert(name, assemblyType);
		// emit function definitions
		for (uint32_t i = 0; i < functions.size(); ++i) {
			functions.at(i)->emitDefinition(mb);
		}
	}

	void::ast::Struct::Function::emitDefinition(ModuleBuilder& mb) {
		function->setName(parent->name + "." + function->getName());
		if (staticDef == false) {
			function->prependParameter("this", parent->assemblyType);
		}
		function->emitDefinition(mb);
	}
