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

#include <ModuleBuilder.hpp>
#include <ast.hpp>

#include <llvm/ModuleProvider.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Transforms/Scalar.h>

#include <antlr.hpp>

using namespace kensho;

	void ast::ModuleBuilder::declareSymbol(ast::Symbol* symbol) {
		symbols[symbol->getName()] = symbol;
	}

	void ast::ModuleBuilder::build(bool mem2reg, bool optimize) {
		initEngine(mem2reg, optimize);
		int numFuns = functions->size();
		for (int i = 0; i < numFuns; ++i) {
			Callable* cb = functions->at(i);
			cb->emitDefinition(*this);
		}
		for (int i = 0; i < numFuns; ++i) {
			Callable* cb = functions->at(i);
			llvm::Value* value = cb->emit(*this);
			Function* fun = dynamic_cast<Function*>(cb);
			if (fun) {
				llvm::Function* llvmFun = llvm::cast<llvm::Function>(value);
				llvm::verifyFunction(*llvmFun);
				fpm->run(*llvmFun);
			}
		}
	}

	void ast::ModuleBuilder::initEngine(bool mem2reg, bool optimize) {
		llvm::ModuleProvider* provider = new llvm::ExistingModuleProvider(&module);
		engine = llvm::ExecutionEngine::create(&module);
		fpm = new llvm::FunctionPassManager(provider);
		fpm->add(new llvm::TargetData(*engine->getTargetData()));
		if (mem2reg) {
			fpm->add(llvm::createPromoteMemoryToRegisterPass());
			if (optimize) {
				fpm->add(llvm::createInstructionCombiningPass());
				fpm->add(llvm::createReassociatePass());
				fpm->add(llvm::createGVNPass());
				fpm->add(llvm::createCFGSimplificationPass());
			}
		}
	}

	llvm::Value* ast::ModuleBuilder::createBinaryOperator(
		uint32_t type, llvm::Value* left, llvm::Value* right)
	{
		switch (type) {
			case OP_ADD:
				return irBuilder.CreateAdd(left, right, "add");
			case OP_SUB:
				return irBuilder.CreateSub(left, right, "sub");
			case OP_MUL:
				return irBuilder.CreateMul(left, right, "mul");
			case OP_DIV:
				return irBuilder.CreateSDiv(left, right, "div");
			case OP_AND:
			case OP_BIT_AND:
				return irBuilder.CreateAnd(left, right, "and");
			case OP_OR:
			case OP_BIT_OR:
				return irBuilder.CreateOr(left, right, "or");
			case OP_XOR:
				return irBuilder.CreateXor(left, right, "xor");

			case CMP_EQ:
				return irBuilder.CreateICmpEQ(left, right, "eq");
			case CMP_NEQ:
				return irBuilder.CreateICmpNE(left, right, "neq");
			case CMP_LT:
				return irBuilder.CreateICmpSLT(left, right, "lt");
			case CMP_LTE:
				return irBuilder.CreateICmpSLE(left, right, "lte");
			case CMP_GT:
				return irBuilder.CreateICmpSGT(left, right, "gt");
			case CMP_GTE:
				return irBuilder.CreateICmpSGE(left, right, "gte");

			case OP_SHIFT_L:
				return irBuilder.CreateShl(left, right, "shl");
			case OP_SHIFT_R:
				return irBuilder.CreateLShr(left, right, "lshr");
			case OP_USHIFT_R:
				return irBuilder.CreateAShr(left, right, "ashr");

			default:
				assert(false && "operator not handled");
		}
	}
