/** @file
   TODO

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date TODO
*/

#ifndef __ERROR_H__
#define __ERROR_H__

/** TODO */
typedef enum FuncResult
{
	FUNC_SUCCESS,
	FUNC_ERROR
} FuncResult;

/** TODO */
void ErrorParserPoly(int row, int col);

/** TODO */
void ErrorParserCommand(int row);

/** TODO */
void ErrorParserValue(int row);

/** TODO */
void ErrorParserVariable(int row);

/** TODO */
void ErrorStackUnderflow(int row);

#endif /* __ERROR_H__ */
