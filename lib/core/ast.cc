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

#include <ast.hpp>
#include <ModuleBuilder.hpp>
#include <error.hpp>
#include <llvm/Type.h>
#include <llvm/Function.h>
#include <iostream>
#include <KenshoLexer.h>

using namespace kensho;

	/*
	 * implementation of VariableDefinition
	 */
	void ast::VariableDefinition::assemble(ast::ModuleBuilder& mb) {
		if (mb.isDeclared(name)) {
			std::string err = "symbol ";
			err += name;
			err += " is already declared";
			throw(ParseError(err));
		}
		value = mb.getIRBuilder().CreateAlloca(assemblyType, 0, name.c_str());
		mb.declareSymbol(this);
	}

	void ast::ParameterDefinition::assemble(ast::ModuleBuilder& mb) {
		assert (mb.isDeclared(name) == false && "parameter already declared");
		VariableDefinition* vardef = new VariableDefinition(name, type);
		llvm::Value* ptr = vardef->emit(mb);
		value = mb.getIRBuilder().CreateStore(fun->getParameterValue(index), ptr);
	}

	/*
	 * implementation of Variable
	 */
	void ast::Variable::assemble(ast::ModuleBuilder& mb) {
		if (!mb.isDeclared(name)) {
			std::string err = "variable ";
			err += name;
			err += " is not declared";
			throw(ParseError(err));
		}

		Symbol* sym = mb.getSymbol(name);
		// currently we only allow variables
		VariableDefinition* var = dynamic_cast<VariableDefinition*>(sym);
		if (var == NULL) {
			std::string err = "symbol ";
			err += name;
			err += " is not a variable";
		}

		value = mb.getIRBuilder().CreateLoad(var->getValue(), name.c_str());
	}

	/*
	 * implementation of Literal
	 */
	void ast::Literal::assemble(ast::ModuleBuilder& mb) {
		switch (tokenType) {
			case LITERAL_TRUE:
				value = llvm::ConstantInt::get(llvm::Type::Int1Ty, 1);
				break;
			case LITERAL_FALSE:
				value = llvm::ConstantInt::get(llvm::Type::Int1Ty, 0);
				break;
			case LITERAL_INT: {
				char last = text[text.length() - 1];
				const llvm::Type* type = (last == 'l' || last == 'L') ?
						llvm::Type::Int64Ty : llvm::Type::Int32Ty;
				value = llvm::ConstantInt::get(
					type,
					type == llvm::Type::Int64Ty ?
						std::atol(text.c_str()) : std::atoi(text.c_str()),
					true
				);
				break;
			}
			case LITERAL_FLOAT: {
				char last = text[text.length() - 1];
				const llvm::Type* type = (last == 'd' || last == 'D') ?
					llvm::Type::DoubleTy : llvm::Type::FloatTy;
				value = llvm::ConstantFP::get(type, std::atof(text.c_str()));
				break;
			}
			case LITERAL_HEX:
			case LITERAL_OCT: {
				long int val = std::strtol(
					text.c_str(),
					NULL,
					tokenType == LITERAL_HEX ? 16 : 8
				);
				value = llvm::ConstantInt::get(llvm::Type::Int32Ty, val, true);
				break;
			}
			case LITERAL_CHAR: {
				char c;
				if (text[1] == '\\') {
					if (text[2] == 'n') {
						c = '\n';
					}
					else if (text[2] == 'r') {
						c = '\r';
					}
					else if (text[2] == 't') {
						c = '\t';
					}
				}
				else {
					c = text[1];
				}
				value = llvm::ConstantInt::get(llvm::Type::Int32Ty, c, true);
				break;
			}
			default:
				assert(false && "missing literal type in switch");
		}
	}

	/*
	 * implementation of BinaryExpression
	 */
	void ast::BinaryExpression::assemble(ast::ModuleBuilder& mb) {
		const llvm::Type* typeLeft = NULL;
		const llvm::Type* typeRight = NULL;

		// emit right side
		llvm::Value* valRight = right->emit(mb);
		typeRight = valRight->getType();

		// special handling for assignment
		if (token == OP_ASSIGN) {
			Variable* var = dynamic_cast<Variable*>(left);
			if (var == NULL) {
				throw(ParseError("can only assign to variables"));
			}
			if (!mb.isDeclared(var->getName())) {
				std::string err = "variable ";
				err += var->getName();
				err += " is not declared";
				throw(ParseError(err));
			}
			VariableDefinition* vardef = dynamic_cast<VariableDefinition*>(
				mb.getSymbol(var->getName()));
			assert(vardef != NULL);
			if (vardef->getAssemblyType() != typeRight) {
				std::string err = "type mismatch in variable assignment to ";
				err += var->getName();
				throw(ParseError(err));
			}
			value = mb.getIRBuilder().CreateStore(valRight, vardef->getValue());
			// we're done for variable assignments
			return;
		}

		llvm::Value* valLeft = left->emit(mb);
		typeLeft = valLeft->getType();

		if (typeLeft != typeRight) {
			// TODO
			throw(ParseError("type mismatch in expression"));
		}

		value = mb.createBinaryOperator(token, valLeft, valRight);
	}

	/*
	 * implementation of UnaryExpression
	 */
	void ast::UnaryExpression::assemble(ast::ModuleBuilder& mb) {
		Literal* lit = dynamic_cast<Literal*>(expression);
		// if the expression is a literal float or integer
		// and the unary operator is a + or - it's not an operator
		// but simply a sign so we must treat it as a literal
		if (lit != NULL && (lit->getTokenType() == LITERAL_INT
			|| lit->getTokenType() == LITERAL_FLOAT)
			&& (token == OP_ADD || token == OP_SUB)) {
			if (token == OP_SUB) {
				std::string& text = lit->getText();
				text.insert(text.begin(), '-');
			}
			// ignore OP_ADD
			value = lit->emit(mb);
		}
		else {
			llvm::Value* val = expression->emit(mb);
			if (token == OP_NOT || token == OP_BIT_NOT) {
				value = mb.getIRBuilder().CreateNot(val, "not");
			}
			else if (token == OP_SUB) {
				// sub from 0 depending on type
				value = mb.getIRBuilder().CreateNeg(val, "neg");
			}
			else {
				// we can ignore OP_ADD
				value = val;
			}
		}
	}

	/*
	 * implementation of Function
	 */
	void ast::Function::assemble(ast::ModuleBuilder& mb) {
		llvm::Function* fun = llvm::cast<llvm::Function>(value);
		assert(fun != NULL);

		// set parameter names
		llvm::Function::arg_iterator arg = NULL;
		int32_t i = 0;
		for (arg = fun->arg_begin(); arg != fun->arg_end(); ++arg) {
			std::string str = parameterNames.at(i++);
			arg->setName(str.c_str());
			parameterValues.push_back(arg);
		}

		// emit body
		llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create("entry", fun);
		mb.getIRBuilder().SetInsertPoint(entryBlock);

		uint32_t numStats = body.size();
		for (uint32_t i = 0; i < numStats; ++i) {
			body.at(i)->emit(mb);
			if (body.at(i)->isReturnStatement() && i < body.size() - 1) {
				throw(ParseError("unreachable code after return statement"));
			}
		}

		// if we have no return statement and the function
		// returns void we synthesize a return statement
		// otherwise bail out with error
		if (numStats > 0
			&& body.at(numStats - 1)->isReturnStatement() == false) {
			if (type == T_VOID) {
				Return* returnStat = new Return(NULL);
				returnStat->emit(mb);
			}
			else {
				std::string err = "missing return statement in non-void function ";
				err += name;
				throw(ParseError(err));
			}
		}
		else {
			Return* ret = dynamic_cast<Return*>(body.at(numStats - 1));
			// verify type match
			Node* ex = ret->getExpression();
			if (ex != NULL && type == T_VOID) {
				std::string err = "void function ";
				err += name;
				err += " cannot return non-void type";
				throw(ParseError(err));
			}
			else if (ex != NULL && ex->getValue()->getType() != assemblyType) {
				std::string err = "type mismatch in return statement in function ";
				err += name;
				throw(ParseError(err));
			}
		}
	}

	/*
	 * implementation of Call
	 */
	void ast::Call::assemble(ast::ModuleBuilder& mb) {
		if (!mb.isDeclared(name)) {
			std::string err = "function ";
			err += name;
			err += " is not declared";
			throw(ParseError(err));
		}

		Callable* fun = dynamic_cast<Callable*>(mb.getSymbol(name));
		if (fun == NULL) {
			std::string err = "symbol ";
			err += name;
			err += " is not a function and cannot be called";
			throw(ParseError(err));
		}

		uint32_t numParams = fun->countParameters();
		if (numParams != arguments.size()) {
			std::string err = "argument count mismatch in call to function ";
			err += name;
			throw(ParseError(err));
		}

		std::vector<const llvm::Type*> expected = fun->getParameterTypes();
		std::vector<llvm::Value*> values;

		for (uint32_t i = 0; i < numParams; ++i) {
			Node* arg = arguments.at(i);
			llvm::Value* val = arg->emit(mb);
			if (expected.at(i) != val->getType()) {
				std::string err = "type mismatch in call to function ";
				err += name;
				err += " for argument #";
				err += i;
				throw(ParseError(err));
			}
			values.push_back(val);
		}

		value = mb.getIRBuilder().CreateCall(
			fun->getValue(), values.begin(), values.end());
	}

	/*
	 * implementation of Cast
	 */
	void ast::Cast::assemble(ast::ModuleBuilder& mb) {
		// TODO
		assert(false && "Cast::assemble not yet implemented");
	}

	/*
	 * implementation of While
	 */
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
			body.at(i)->emit(mb);
		}
		// if last statement in the while is no return statement
		// we must emit an unconditional branch back to the while condition block
		if (false == body.at(stats - 1)->isReturnStatement()) {
			mb.getIRBuilder().CreateBr(cond);
		}

		// emit merge block
		fun->getBasicBlockList().push_back(merge);
		mb.getIRBuilder().SetInsertPoint(merge);

		value = merge;
	}

	/*
	 * implementation of Conditional
	 */
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
			throw(ParseError("non-boolean if-expression"));
		}

		// emit conditional branch
		mb.getIRBuilder().CreateCondBr(exval, trueBlock, firstAlt);

		// emit true block
		mb.getIRBuilder().SetInsertPoint(trueBlock);
		for (uint32_t i = 0; i < trueBody.size(); ++i) {
			trueBody.at(i)->emit(mb);
			if (trueBody.at(i)->isReturnStatement() && i < trueBody.size() - 1) {
				throw(ParseError("unreachable code after return statement"));
			}
		}
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
					throw(ParseError("non-boolean else-if-expression"));
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

				// emit branch body
				fun->getBasicBlockList().push_back(branchBlock);
				mb.getIRBuilder().SetInsertPoint(branchBlock);
				for (uint32_t i = 0; i < cond->trueBody.size(); ++i) {
					cond->trueBody.at(i)->emit(mb);
					if (cond->trueBody.at(i)->isReturnStatement() && i < cond->trueBody.size() - 1) {
						throw(ParseError("unreachable code after return statement"));
					}
				}
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
			fun->getBasicBlockList().push_back(falseBlock);
			mb.getIRBuilder().SetInsertPoint(falseBlock);
			for (uint32_t i = 0; i < falseBody.size(); ++i) {
				falseBody.at(i)->emit(mb);
				if (falseBody.at(i)->isReturnStatement() && i < falseBody.size() - 1) {
					throw(ParseError("unreachable code after return statement"));
				}
			}
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

		value = merge;
	}

	/*
	 * implementation of Callable
	 */
	void ast::Callable::assemble(ast::ModuleBuilder& mb) {
		// noop
	}

	void ast::Callable::emitDefinition(ast::ModuleBuilder& mb) {
		if (mb.isDeclared(name)) {
			std::string err = "symbol ";
			err += name;
			err += " is already declared";
			throw(ParseError(err));
		}
		llvm::FunctionType* funtype = llvm::FunctionType::get(
			assemblyType, parameterTypes, false);
		llvm::Function* fun = llvm::Function::Create(
			funtype, llvm::Function::ExternalLinkage, name, mb.getModule());

		value = fun;
		mb.declareSymbol(this);
	}

	/*
	 * implementation of Return
	 */
	void ast::Return::assemble(ast::ModuleBuilder& mb) {
		if (expression == NULL) {
			value = mb.getIRBuilder().CreateRetVoid();
			return;
		}
		value = mb.getIRBuilder().CreateRet(expression->emit(mb));
	}

	const llvm::Type* ast::toAssemblyType(uint32_t type)
	{
		switch (type) {
			case T_VOID:
				return llvm::Type::VoidTy;
			case T_BOOL:
				return llvm::Type::Int1Ty;
			case T_BYTE:
				return llvm::Type::Int8Ty;
			case T_SHORT:
				return llvm::Type::Int16Ty;
			case T_INT:
			case T_CHAR:
				return llvm::Type::Int32Ty;
			case T_LONG:
				return llvm::Type::Int64Ty;
			case T_FLOAT:
				return llvm::Type::FloatTy;
			case T_DOUBLE:
				return llvm::Type::DoubleTy;
		}

		return NULL;
	}

