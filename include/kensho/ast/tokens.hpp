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

#ifndef KENSHO_AST_TOKENS_HPP_
#define KENSHO_AST_TOKENS_HPP_

namespace kensho {
namespace ast {

	enum TypeToken {
		TyVoid 		= 1,
		TyBool 		= 2,
		TyByte 		= 3,
		TyShort 	= 4,
		TyInt 		= 5,
		TyLong 		= 6,
		TyFloat 	= 7,
		TyDouble 	= 8,
		TyPtr 		= 9,
		TyChar		= 10
	};

	enum OperatorToken {
		OpAdd		= 1,
		OpSub		= 2,
		OpMul		= 3,
		OpDiv		= 4,
		OpRem		= 5,
		OpAnd		= 6,
		OpBitAnd	= 7,
		OpOr		= 8,
		OpBitOr		= 9,
		OpXor		= 10,
		OpShiftL	= 11,
		OpShiftR	= 12,
		OpUShiftR = 13,
		OpEq		= 14,
		OpNeq		= 15,
		OpGt		= 16,
		OpLt		= 17,
		OpGte		= 18,
		OpLte		= 19,
		OpNot		= 20,
		OpBitNot	= 21,
		OpAssign	= 22
	};

	enum LiteralToken {
		LitInt		= 1,
		LitOct		= 2,
		LitHex		= 3,
		LitFloat	= 4,
		LitChar		= 5,
		LitTrue 	= 6,
		LitFalse 	= 7
	};

}}

#endif /* KENSHO_AST_TOKENS_HPP_ */
