/** @file
   Główny plik kalkulatora wielomianów

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/

#include "stack.h"
#include "parser.h"
#include "calc.h"
#include "error.h"

#include <stdio.h>
#include <limits.h>

/**
 * Wykonuje komendę kalkulatora otrzymaną od parsera
 * @param[in] command : komenda otrzymana od parsera
 * @param[in] parameter : parametr wykorzystywant przy `CALC_DEG_BY` i `CALC_AT`
 * @param[in] stack : stos kalkulatora
 * @return status zakończenia funkcji inofrumjący o sukcesie lub błędzie
 */
FuncResult CommandExecute(ParserCommand command,
	                      poly_coeff_t parameter,
						  Stack *stack);

/**
* Główna funkcja kalkulatora.
* @return 0
*/
int main(void) {

	Stack stack;
	StackInit(&stack);

	int row = 0;

	while (!ParserLineIsEndOfFile())
	{
		row++;

		if (ParserLineIsCommand())
		{
			ParserCommand command;
			poly_coeff_t parameter;

			if (ParseLineCommand(&command, &parameter, row) == FUNC_SUCCESS)
			{
				if (CommandExecute(command, parameter, &stack) == FUNC_ERROR)
				{
					ErrorStackUnderflow(row);
				}
			}
		}
		else
		{
			Poly p;

			if (ParseLinePoly(&p, row) == FUNC_SUCCESS)
			{
				StackPush(&stack, &p);
			}
		}
	}

	StackDestroy(&stack);

	return 0;
}

FuncResult CommandExecute(ParserCommand command,
	                      poly_coeff_t parameter,
						  Stack *stack)
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
