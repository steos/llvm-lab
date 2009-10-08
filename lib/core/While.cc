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

#include <kensho/ast/While.hpp>
#include <kensho/ast/ModuleBuilder.hpp>

using namespace kensho;

	void ast::While::assemble(ast::ModuleBuilder& mb) {
		llvm::Function* fun = mb.getIRBuilder().GetInsertBlock()->getParent();

		// create basic block for while condition, body and merge
		llvm::BasicBlock* cond = llvm::BasicBlock::Create("while-cond");
		llvm::BasicBlock* bodyBlock = llvm::BasicBlock::Create("while-body");
		llvm::BasicBlock* merge = llvm::BasicBlock::Create("while-merge");

		// branch to while condition
		mb.getIRBuilder().CreateBr(cond);

		// insert condition block and make it the current insert point
		fun->getBasicBlockList().push_back(cond);
		mb.getIRBuilder().SetInsertPoint(cond);

		// emit while expression and conditional branch
		llvm::Value* exval = expression->emit(mb);
		mb.getIRBuilder().CreateCondBr(exval, bodyBlock, merge);

		// emit while body
		fun->getBasicBlockList().push_back(bodyBlock);
		mb.getIRBuilder().SetInsertPoint(bodyBlock);
		uint32_t stats = body.size();
		for (uint32_t i = 0; i < stats; ++i) {
			body.at(i)->assemble(mb);
		}
		// if last statement in the while is no return statement
		// we must emit an unconditional branch back to the while condition block
		if (false == body.at(stats - 1)->isReturnStatement()) {
			mb.getIRBuilder().CreateBr(cond);
		}

		// emit merge block
		fun->getBasicBlockList().push_back(merge);
		mb.getIRBuilder().SetInsertPoint(merge);
	}
