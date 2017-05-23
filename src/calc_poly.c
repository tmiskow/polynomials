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
				assert(false);
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
				assert(false);
			}
		}
	}

	StackDestroy(&stack);

	return 0;
}

FuncResult CommandExecute(ParserCommand command, poly_coeff_t parameter, Stack *stack)
{
	// TODO
	parameter = parameter;

	switch (command)
	{
		case CALC_ADD:
			return CalcAdd(stack);
			break;

		case CALC_PRINT:
			return CalcPrint(stack);
			break;

		default:
			assert(false);
			return FUNC_ERROR;
			break;
	}
}
