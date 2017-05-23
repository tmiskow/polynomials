/** @file
   TODO

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date TODO
*/

#include "error.h"
#include <stdio.h>

void ErrorParserPoly(int row, int col)
{
	printf("ERROR %d %d\n", row, col);
}

void ErrorParserCommand(int row)
{
	printf("ERROR %d WRONG COMMAND\n", row);
}

void ErrorParserValue(int row)
{
	printf("ERROR %d WRONG VALUE\n", row);
}

void ErrorParserVariable(int row)
{
	printf("ERROR %d WRONG VARIABLE\n", row);
}
