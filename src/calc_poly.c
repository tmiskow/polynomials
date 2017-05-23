/** @file
   TODO

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date TODO
*/

// DEBUG
#include "poly.h"
#include "stack.h"
#include "parser.h"
#include "calc.h"
#include "error.h"

#include <stdio.h>
#include <limits.h>

/** TODO */
ParserResult CommandExecute(CalcCommand command, poly_coeff_t parameter, Stack *stack);

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
			CalcCommand command;
			poly_coeff_t parameter;
			ParserResult parser_result = ParseLineCommand(&command, &parameter, row);

			if (parser_result == PARSER_SUCCESS)
			{
				if (CommandExecute(command, parameter, &stack) == PARSER_ERROR)
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
			ParserResult parser_result = ParseLinePoly(&p, row);

			if (parser_result == PARSER_SUCCESS)
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

ParserResult CommandExecute(CalcCommand command, poly_coeff_t parameter, Stack *stack)
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
			return PARSER_ERROR;
			break;
	}
}
