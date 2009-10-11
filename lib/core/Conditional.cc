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

#include <kensho/ast/Conditional.hpp>
#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/error.hpp>

#include <llvm/Value.h>
#include <llvm/BasicBlock.h>

using namespace kensho;

	void ast::Conditional::assemble(ast::ModuleBuilder& mb) {
		llvm::Function* fun = mb.getIRBuilder().GetInsertBlock()->getParent();
		llvm::BasicBlock* trueBlock = llvm::BasicBlock::Create("if-true", fun);
		llvm::BasicBlock* merge = llvm::BasicBlock::Create("if-merge");
		llvm::BasicBlock* falseBlock = llvm::BasicBlock::Create("if-false");
		llvm::BasicBlock* firstAlt = merge;
		std::vector<llvm::BasicBlock*> branchBlocks;
		bool branch = false;
		bool hasElse = false;

		// setup false block
		if (falseBody.size() > 0) {
			hasElse = true;
			firstAlt = falseBlock;
		}

		// setup branch blocks (those are elseifs)
		if (branches.size() > 0) {
			branch = true;
			for (uint32_t i = 0; i < branches.size(); ++i) {
				branchBlocks.push_back(llvm::BasicBlock::Create("elseif"));
			}
			firstAlt = branchBlocks[0];
		}

		// verify that we have a boolean expression
		llvm::Value* exval = expression->emit(mb);
		if (exval->getType() != llvm::Type::Int1Ty) {
			throw(ParseError("non-boolean if-expression",
				getLine(), getOffset()));
		}

		// emit conditional branch
		mb.getIRBuilder().CreateCondBr(exval, trueBlock, firstAlt);

		// introduce true-block-scope
		mb.getSymbolScope().push();
		// emit true block
		mb.getIRBuilder().SetInsertPoint(trueBlock);
		for (uint32_t i = 0; i < trueBody.size(); ++i) {
			trueBody.at(i)->assemble(mb);
			if (trueBody.at(i)->isReturnStatement() && i < trueBody.size() - 1) {
				throw(ParseError("unreachable code after return statement",
					getLine(), getOffset()));
			}
		}
		// pop true-block-scope
		mb.getSymbolScope().pop();

		// we must only emit a branch to the next block
		// if the true body contained no return statement
		if (trueBody.size() > 0
			&& false == trueBody.at(trueBody.size() - 1)->isReturnStatement()) {
			mb.getIRBuilder().CreateBr(merge);
		}

		// emit branch blocks if we have any
		if (branch) {
			for (uint32_t i = 0; i < branches.size(); ++i) {
				Conditional* cond = branches.at(i);
				llvm::BasicBlock* block = branchBlocks.at(i);
				fun->getBasicBlockList().push_back(block);
				mb.getIRBuilder().SetInsertPoint(block);
				llvm::Value* val = cond->expression->emit(mb);
				if (val->getType() != llvm::Type::Int1Ty) {
					throw(ParseError("non-boolean else-if-expression",
						getLine(), getOffset()));
				}
				// if this is the last one we branch to
				// the final merge block, otherwise we branch
				// to the next block in the list
				llvm::BasicBlock* branchBlock = llvm::BasicBlock::Create("elseif-true");
				llvm::BasicBlock* nextAlt = NULL;
				if (i == branches.size() - 1) {
					nextAlt = hasElse ? falseBlock : merge;
				}
				else {
					nextAlt = branchBlocks.at(i + 1);
				}

				// emit conditional branch
				mb.getIRBuilder().CreateCondBr(val, branchBlock, nextAlt);

				// introduce elseif scope
				mb.getSymbolScope().push();
				// emit branch body
				fun->getBasicBlockList().push_back(branchBlock);
				mb.getIRBuilder().SetInsertPoint(branchBlock);
				for (uint32_t i = 0; i < cond->trueBody.size(); ++i) {
					cond->trueBody.at(i)->assemble(mb);
					if (cond->trueBody.at(i)->isReturnStatement()
						&& i < cond->trueBody.size() - 1) {
						throw(ParseError("unreachable code after return statement",
							getLine(), getOffset()));
					}
				}
				// remove elseif scope
				mb.getSymbolScope().pop();
				// we must only emit a branch to the next block
				// if the true body contained no return statement
				if (cond->trueBody.size() > 0
					&& false == cond->trueBody.at(cond->trueBody.size() - 1)->isReturnStatement()) {
					mb.getIRBuilder().CreateBr(merge);
				}
			}
		}

		// emit else block if we have one
		if (hasElse) {
			// introduce else scope
			mb.getSymbolScope().push();
			fun->getBasicBlockList().push_back(falseBlock);
			mb.getIRBuilder().SetInsertPoint(falseBlock);
			for (uint32_t i = 0; i < falseBody.size(); ++i) {
				falseBody.at(i)->assemble(mb);
				if (falseBody.at(i)->isReturnStatement() && i < falseBody.size() - 1) {
					throw(ParseError("unreachable code after return statement",
						getLine(), getOffset()));
				}
			}
			// drop else scope
			mb.getSymbolScope().pop();
			// we must only emit a branch to the next block
			// if the false body contained no return statement
			if (falseBody.size() > 0
				&& false == falseBody.at(falseBody.size() - 1)->isReturnStatement()) {
				mb.getIRBuilder().CreateBr(merge);
			}
		}

		// emit merge block
		fun->getBasicBlockList().push_back(merge);
		mb.getIRBuilder().SetInsertPoint(merge);
	}
