/** @file
   TODO

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date TODO
*/

#ifndef __PARSER_H__
#define __PARSER_H__

/** TODO */
typedef enum ParserResult
{
	PARSER_SUCCESS,
	PARSER_ERROR,
	PARSER_END_OF_FILE
} ParserResult;

/** TODO */
typedef enum CalcCommand
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
} CalcCommand;

/** TODO */
bool ParserIsCommand();

/** TODO */
bool ParserIsEndOfFile();

/** TODO */
ParserResult ParseLinePoly(Poly *p, int row);

/** TODO */
ParserResult ParseLineCommand(CalcCommand *command, long *parameter, int row);

#endif /* __PARSER_H__ */
