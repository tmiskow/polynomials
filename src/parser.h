/** @file
   TODO

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date TODO
*/

#ifndef __PARSER_H__
#define __PARSER_H__

#include "error.h"

/** TODO */
typedef enum ParserCommand
{
	CALC_ZERO,
	CALC_IS_COEFF,
	CALC_IS_ZERO,
	CALC_CLONE,
	CALC_ADD,
	CALC_MUL,
	CALC_NEG,
	CALC_SUB,
	CALC_IS_EQ,
	CALC_DEG,
	CALC_DEG_BY,
	CALC_AT,
	CALC_PRINT,
	CALC_POP,
	CALC_WRONG_COMMAND
} ParserCommand;

/** TODO */
bool ParserIsCommand();

/** TODO */
bool ParserIsEndOfFile();

/** TODO */
FuncResult ParseLinePoly(Poly *p, int row);

/** TODO */
FuncResult ParseLineCommand(ParserCommand *command, long *parameter, int row);

#endif /* __PARSER_H__ */
