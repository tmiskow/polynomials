/** @file
   TODO

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date TODO
*/

#include "stack.h"
#include "parser.h"
#include "calc.h"
#include "error.h"

#include <stdio.h>
#include <limits.h>

/** TODO */
FuncResult CommandExecute(ParserCommand command, poly_coeff_t parameter, Stack *stack);

int main(void) {

	/* TODO */
	Stack stack;
	StackInit(&stack);

	int row = 0;

	while (!ParserIsEndOfFile())
	{
		row++;

		if (ParserIsCommand())
		{
			ParserCommand command;
			poly_coeff_t parameter;
			FuncResult parser_result = ParseLineCommand(&command, &parameter, row);

			if (parser_result == FUNC_SUCCESS)
			{
				if (CommandExecute(command, parameter, &stack) == FUNC_ERROR)
				{
					ErrorStackUnderflow(row);
				}
			}
			else
			{
				//assert(false);
			}
		}
		else
		{
			Poly p;
			FuncResult parser_result = ParseLinePoly(&p, row);

			if (parser_result == FUNC_SUCCESS)
			{
				StackPush(&stack, &p);
			}
			else
			{
				//assert(false);
			}
		}
	}

	StackDestroy(&stack);

	return 0;
}

FuncResult CommandExecute(ParserCommand command, poly_coeff_t parameter, Stack *stack)
{
	switch (command)
	{
		case CALC_ZERO:
			return CalcZero(stack);
			break;

		case CALC_IS_COEFF:
			return CalcIsCoeff(stack);
			break;

		case CALC_IS_ZERO:
			return CalcIsZero(stack);
			break;

		case CALC_CLONE:
			return CalcClone(stack);
			break;

		case CALC_ADD:
			return CalcAdd(stack);
			break;

		case CALC_MUL:
			return CalcMul(stack);
			break;

		case CALC_NEG:
			return CalcNeg(stack);
			break;

		case CALC_SUB:
			return CalcSub(stack);
			break;

		case CALC_IS_EQ:
			return CalcIsEq(stack);
			break;

		case CALC_DEG:
			return CalcDeg(stack);
			break;

		case CALC_DEG_BY:
			return CalcDegBy(stack, (unsigned) parameter);
			break;

		case CALC_AT:
			return CalcAt(stack, parameter);
			break;

		case CALC_PRINT:
			return CalcPrint(stack);
			break;

		case CALC_POP:
			return CalcPop(stack);
			break;

		default:
			return FUNC_ERROR;
			break;
	}
}
