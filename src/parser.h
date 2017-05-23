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
ParserResult ParseLinePoly(Poly *p);

#endif /* __PARSER_H__ */
