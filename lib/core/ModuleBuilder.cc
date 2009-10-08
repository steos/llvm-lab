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

#include <kensho/ast/ModuleBuilder.hpp>
#include <kensho/ast/VariableDefinition.hpp>
#include <kensho/ast/Callable.hpp>
#include <kensho/ast/Struct.hpp>

#include <llvm/ModuleProvider.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Transforms/Scalar.h>

using namespace kensho;

	void ast::ModuleBuilder::declareFunction(Callable* fun) {
		funScope.addSymbol(fun->getName(), fun);
	}

	void ast::ModuleBuilder::emitDefinitions() {
		int numFuns = functions.size();
		int numStructs = structs.size();
		for (int i = 0; i < numStructs; ++i) {
			Struct* st = structs.at(i);
			st->emitDefinition(*this);
		}
		for (int i = 0; i < numFuns; ++i) {
			Callable* cb = functions.at(i);
			cb->emitDefinition(*this);
		}
	}

	void ast::ModuleBuilder::emitImplementations() {
		int numFuns = functions.size();
		int numStructs = structs.size();
		for (int i = 0; i < numStructs; ++i) {
			Struct* st = structs.at(i);
			st->emit(*this);
		}
		for (int i = 0; i < numFuns; ++i) {
			Callable* cb = functions.at(i);
			llvm::Value* value = cb->emit(*this);
			Function* fun = dynamic_cast<Function*>(cb);
			if (fun) {
				llvm::Function* llvmFun = llvm::cast<llvm::Function>(value);
				llvm::verifyFunction(*llvmFun);
				fpm->run(*llvmFun);
			}
		}
	}

	void ast::ModuleBuilder::build(bool mem2reg, bool optimize) {
		initEngine(mem2reg, optimize);
		// pass 1: emit function definitions and type declarations
		// and put all that stuff into the symbol table
		emitDefinitions();
		// pass 2: emit function implementations
		emitImplementations();
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
		OperatorToken type, llvm::Value* left, llvm::Value* right)
	{
		switch (type) {
			case OpAdd:
				return irBuilder.CreateAdd(left, right, "add");
			case OpSub:
				return irBuilder.CreateSub(left, right, "sub");
			case OpMul:
				return irBuilder.CreateMul(left, right, "mul");
			case OpDiv:
				return irBuilder.CreateSDiv(left, right, "div");
			case OpAnd:
			case OpBitAnd:
				return irBuilder.CreateAnd(left, right, "and");
			case OpOr:
			case OpBitOr:
				return irBuilder.CreateOr(left, right, "or");
			case OpXor:
				return irBuilder.CreateXor(left, right, "xor");

			case OpEq:
				return irBuilder.CreateICmpEQ(left, right, "eq");
			case OpNeq:
				return irBuilder.CreateICmpNE(left, right, "neq");
			case OpLt:
				return irBuilder.CreateICmpSLT(left, right, "lt");
			case OpLte:
				return irBuilder.CreateICmpSLE(left, right, "lte");
			case OpGt:
				return irBuilder.CreateICmpSGT(left, right, "gt");
			case OpGte:
				return irBuilder.CreateICmpSGE(left, right, "gte");

			case OpShiftL:
				return irBuilder.CreateShl(left, right, "shl");
			case OpShiftR:
				return irBuilder.CreateLShr(left, right, "lshr");
			case OpUShiftR:
				return irBuilder.CreateAShr(left, right, "ashr");
			case OpRem:
				return irBuilder.CreateSRem(left, right, "rem");

			default:
				assert(false && "operator not handled");
		}
	}
