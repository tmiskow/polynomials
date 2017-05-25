/** @file
   Interfejs parsera

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/

#ifndef __PARSER_H__
#define __PARSER_H__

#include "error.h"

/**
 * Typ wyliczeniowy reprezentujący komendy dla kalkulatora.
 */
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

/**
 * Sprawdza, czy następny wiersz parser będzie interpretować jako komendę.
 * Jeżeli wiersz rozpoczyna się od litery, to parser stara się zinterpretować komendę.
 * W przeciwnym wypadku, interpretuje go jako wielomian.
 * @return Czy następny wiersz będzie interpretowany jako komenda?
 */
bool ParserLineIsCommand();

/**
 * Sprawdza, czy następny wiersz jest końcem pliku.
 * @return Czy następny wiersz jest końcem pliku?
 */
bool ParserLineIsEndOfFile();

/**
 * Parsuje wielomian w kolejnym wierszu.
 * @param[in] p : wskaźnik na wielomian do nadpisania
 * @param[in] row : numer wiersza
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult ParseLinePoly(Poly *p, int row);

/**
 * Parsuje komendę w kolejnym wierszu.
 * @param[in] command : wskaźnik na komendę do nadpisania
 * @param[in] parameter : wskaźnik na parametr komendy do nadpisania
 * @param[in] row : numer wiersza
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult ParseLineCommand(ParserCommand *command, long *parameter, int row);

#endif /* __PARSER_H__ */
