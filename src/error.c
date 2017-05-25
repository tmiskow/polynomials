/** @file
   Implementacja funkcji z error.h

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/

#include "error.h"
#include <stdio.h>

void ErrorParserPoly(int row, int col)
{
	fprintf(stderr, "ERROR %d %d\n", row, col);
}

void ErrorParserCommand(int row)
{
	fprintf(stderr, "ERROR %d WRONG COMMAND\n", row);
}

void ErrorParserValue(int row)
{
	fprintf(stderr, "ERROR %d WRONG VALUE\n", row);
}

void ErrorParserVariable(int row)
{
	fprintf(stderr, "ERROR %d WRONG VARIABLE\n", row);
}

void ErrorStackUnderflow(int row)
{
	fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", row);
}
