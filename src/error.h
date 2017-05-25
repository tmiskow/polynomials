/** @file
   Interfejs obsługi błędów

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/

#ifndef __ERROR_H__
#define __ERROR_H__

/**
 * Typ wyliczeniowy reprezentujący status zakończenia funkcji.
 */
typedef enum FuncResult
{
	FUNC_SUCCESS, ///< informacja o tym, że funkcja zakończyła się sukcesem
	FUNC_ERROR ///< informacja o tym, że funkcja zakończyła się błędem
} FuncResult;

/**
 * Wypisuje na standardowe wyjście błędów komunikat o błędzie parsowania wielomianu.
 * @param[in] row : numer wiersza
 * @param[in] col : numer kolumny
 */
void ErrorParserPoly(int row, int col);

/**
 * Wypisuje na standardowe wyjście błędów komunikat o błędzie parsowania komendy.
 * @param[in] row : numer wiersza
 */
void ErrorParserCommand(int row);

/**
 * Wypisuje na standardowe wyjście błędów komunikat o błędzie parsowania parametru komendy `CALC_AT`.
 * @param[in] row : numer wiersza
 */
void ErrorParserValue(int row);

/**
 * Wypisuje na standardowe wyjście błędów komunikat o błędzie parsowania parametru komendy `CALC_DEG_BY`.
 * @param[in] row : numer wiersza
 */
void ErrorParserVariable(int row);

/**
 * Wypisuje na standardowe wyjście błędów komunikat o za małej liczbie wielomianów na stosie.
 * @param[in] row : numer wiersza
 */
void ErrorStackUnderflow(int row);

#endif /* __ERROR_H__ */
