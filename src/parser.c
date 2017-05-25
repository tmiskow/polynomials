/** @file
   Implementacja funkcji z parser.h i funkcji pomocniczych

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/

#include "poly.h"
#include "parser.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

/** Stała reprezentująca maksymalną długość komendy */
#define MAX_COMMAND_LENGTH 9

/** Typ wspołczynnikow wielomianu używany przez parser */
typedef unsigned long parser_coeff_t;

/** Typ wykładników wielomianu używany przez parser */
typedef long parser_exp_t;

/* DEKLARACJE FUNKCJI POMOCNICZYCH */

/**
 * Wczytuje znaki ze standardowego wejścia aż do napotkania znaku `\n`
 */
static void ParserSkipLine();

/**
 * Sprawdza, czy współcznnik mieści się w obsługiwanym zakresie.
 * Dopuszczalny zakres to `[LONG_MIN; LONG_MAX].
 * @param[in] parser_coeff : moduł wspołczynnika
 * @param[in] is_negative : Czy współczynnik jest ujemny?
 * @return Czy współcznnik mieście się w obsługiwanym zakresie?
 */
static bool ParserCoeffIsInRange(parser_coeff_t parser_coeff, bool is_negative);

/**
 * Sprawdza, czy wykładnik mieści się w obsługiwanym zakresie.
 * Dopuszczalny zakres to `[0; INT_MAX].
 * @param[in] parser_exp : wykładnik
 * @return Czy wykładnik mieście się w obsługiwanym zakresie?
 */
static bool ParserExpIsInRange(parser_exp_t parser_exp);

/**
 * Sprawdza, czy indeks zmiennej mieści się w obsługiwanym zakresie.
 * Dopuszczalny zakres to `[0; UINT_MAX].
 * @param[in] parser_var_index : indeks zmiennej
 * @return Czy indeks zmiennej mieście się w obsługiwanym zakresie?
 */
static bool ParserVarIndexIsInRange(unsigned long parser_var_index);

/**
 * Sprawdza, czy znak jest początkiem współczynnika.
 * @param[in] c : znak
 * @return Czy znak jest początkiem współczynnika?
 */
static bool CharIsStartOfCoeff(char c);

/**
 * Sprawdza, czy znak jest początkiem komendy.
 * @param[in] c : znak
 * @return Czy znak jest początkiem komendy?
 */
static bool CharIsStartOfCommand(char c);

/**
 * Konwertuje znak (zakładając, że reprezentuje cyfrę) na typ @p parser_coeff_t.
 * @param[in] c : znak
 * @return liczba typy @p parser_coeff_t
 */
static parser_coeff_t CharToCoeff(char c);

/**
 * Konwertuje znak (zakładając, że reprezentuje cyfrę) na typ @p parser_exp_t.
 * @param[in] c : znak
 * @return liczba typy @p parser_exp_t
 */
static parser_exp_t CharToExp(char c);

/**
 * Podgląda znak ze standardowego wejścia.
 * @return podejrzany znak
 */
static char CharPeek();

/**
 * Parsuje znak ze standardowego wejścia. Iteruje @p col o `1`.
 * @param[in] col : wskaźnik na liczbę kolumn do iterowania
 * @return sparsowany znak
 */
static char ParseChar(int *col);

/**
 * Parsuje współczynnik ze standardowego wejścia.
 * Iteruje @p col o liczbę wczytanych znaków.
 * @param[in] col : wskaźnik na liczbę kolumn do iterowania
 * @param[in] coeff : wskaźnik na współcznnik do nadpisania
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
static FuncResult ParseCoeff(int *col, poly_coeff_t *coeff);

/**
 * Parsuje wykładnik ze standardowego wejścia.
 * Iteruje @p col o liczbę wczytanych znaków.
 * @param[in] col : wskaźnik na liczbę kolumn do iterowania
 * @param[in] exp : wskaźnik na wykładnik do nadpisania
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
static FuncResult ParseMonoExp(int *col, poly_exp_t *exp);

/**
 * Parsuje jednomian ze standardowego wejścia.
 * Iteruje @p col o liczbę wczytanych znaków.
 * @param[in] col : wskaźnik na liczbę kolumn do iterowania
 * @param[in] m : wskaźnik na jednomian do nadpisania
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
static FuncResult ParseMono(int *col, Mono *m);

/**
 * Parsuje wielomian ze standardowego wejścia.
 * Iteruje @p col o liczbę wczytanych znaków.
 * @param[in] col : wskaźnik na liczbę kolumn do iterowania
 * @param[in] p : wskaźnik na wielomian do nadpisania
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
static FuncResult ParsePoly(int *col, Poly *p);

/**
 * Parsuje komendę z tablicy znaków.
 * @param[in] char_array : tablica znaków
 * @return sparsowana komenda
 */
static ParserCommand ParseCommandFromArray(char char_array[]);

/**
 * Parsuje indeks zmiennej (parametr komendy CALC_DEG_BY) ze standardowego wejścia.
 * @param[in] var_idx : wskaźnik na indeks zmiennej do nadpisania
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
static FuncResult ParseVarIdx(unsigned *var_idx);

/**
 * Parsuje wartość (parametr komendy CALC_AT) ze standardowego wejścia.
 * @param[in] coeff : wskaźnik na wartość do nadpisania
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
static FuncResult ParseValue(poly_coeff_t *coeff);

/* IMPLEMENTACJA FUNKCJI POMOCNICZYCH */

static void ParserSkipLine()
{
	char c;

	do
	{
	 	c = getchar();
	} while (c != '\n');
}

static bool ParserCoeffIsInRange(parser_coeff_t parser_coeff, bool is_negative)
{
	if (is_negative)
	{
		parser_coeff_t max = ((parser_coeff_t) LONG_MAX) + 1;
		return parser_coeff <= max;
	}
	else
	{
		return parser_coeff <= LONG_MAX;
	}
}

static bool ParserExpIsInRange(parser_exp_t parser_exp)
{
	return (0 <= parser_exp && parser_exp <= INT_MAX);
}

static bool ParserVarIndexIsInRange(unsigned long parser_var_index)
{
	return parser_var_index <= UINT_MAX;
}

static inline bool CharIsStartOfCoeff(char c)
{
	return isdigit(c) || c == '-';
}

static bool CharIsStartOfCommand(char c)
{
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static parser_exp_t CharToExp(char c)
{
	assert(isdigit(c));
	return (poly_coeff_t) c - '0';
}

static parser_coeff_t CharToCoeff(char c)
{
	assert(isdigit(c));
	return (poly_coeff_t) c - '0';
}

static char CharPeek()
{
	char c = getchar();
	ungetc(c, stdin);
	return c;
}

static char ParseChar(int *col)
{
	if (col)
	{
		(*col)++;
	}

	return getchar();
}

static FuncResult ParseCoeff(int *col, poly_coeff_t *coeff)
{
	if (CharIsStartOfCoeff(CharPeek()))
	{
		parser_coeff_t parser_coeff = 0;
		bool is_negative = false;

		if (CharPeek() == '-')
		{
			is_negative = true;
			ParseChar(col);

			if (!isdigit(CharPeek()))
			{
				return FUNC_ERROR;
			}
		}

		while (isdigit(CharPeek()))
		{
			parser_coeff = 10 * (parser_coeff) + CharToCoeff(CharPeek());

			if (!ParserCoeffIsInRange(parser_coeff, is_negative))
			{
				return FUNC_ERROR;
			}
			else
			{
				ParseChar(col);
			}
		}

		*coeff = (poly_coeff_t) parser_coeff;

		if (is_negative)
		{
			*coeff *= -1;
		}

		return FUNC_SUCCESS;
	}
	else
	{
		return FUNC_ERROR;
	}
}

static FuncResult ParseMonoExp(int *col, poly_exp_t *exp)
{
	if (!isdigit(CharPeek()))
	{
		return FUNC_ERROR;
	}
	else
	{
		parser_exp_t parser_exp = 0;

		while (isdigit(CharPeek()))
		{
			parser_exp = 10 * parser_exp + CharToExp(CharPeek());

			if (!ParserExpIsInRange(parser_exp))
			{
				return FUNC_ERROR;
			}
			else
			{
				ParseChar(col);
			}
		}

		*exp = parser_exp;
		return FUNC_SUCCESS;
	}
}

static FuncResult ParseMono(int *col, Mono *m)
{
	if (CharPeek() == '(')
	{
		ParseChar(col);
		Poly p;
		if (ParsePoly(col, &p) != FUNC_ERROR)
		{
			if (CharPeek() == ',')
			{
				ParseChar(col);
				poly_exp_t exp;

				if (ParseMonoExp(col, &exp) != FUNC_ERROR)
				{
					*m = MonoFromPoly(&p, exp);

					if (CharPeek() == ')')
					{
						ParseChar(col);
						return FUNC_SUCCESS;
					}
					else
					{
						return FUNC_ERROR;
					}
				}
				else
				{
					PolyDestroy(&p);
					return FUNC_ERROR;
				}
			}
			else
			{
				PolyDestroy(&p);
				return FUNC_ERROR;
			}
		}
		else
		{
			return FUNC_ERROR;
		}
	}
	else
	{
		return FUNC_ERROR;
	}
}

static FuncResult ParsePoly(int *col, Poly *p)
{
	if (CharIsStartOfCoeff(CharPeek()))
	{
		poly_coeff_t coeff;

		if (ParseCoeff(col, &coeff) == FUNC_ERROR)
		{
			return FUNC_ERROR;
		}
		else
		{
			*p = PolyFromCoeff(coeff);
			return FUNC_SUCCESS;
		}
	}
	else if (CharPeek() == '(')
	{
		unsigned size = 10;
		Mono* monos = (Mono*) calloc(size, sizeof(Mono));
		assert(monos);

		unsigned count = 0;

		FuncResult result = FUNC_SUCCESS;
		do
		{
			if (CharPeek() == '+')
			{
				ParseChar(col);
			}

			result = ParseMono(col, &(monos[count]));
			count++;

			if (count == size - 1)
			{
				size *= 2;
				monos = (Mono*) realloc(monos, size * sizeof(Mono));
				assert(monos);
			}
		} while (result == FUNC_SUCCESS && CharPeek() == '+');

		if (result == FUNC_SUCCESS)
		{
			*p = PolyAddMonos(count, monos);
		}
		else
		{
			*p = PolyZero();
		}
		free(monos);
		return result;
	}
	else
	{
		*p = PolyZero();
		return FUNC_ERROR;
	}
}

static ParserCommand ParseCommandFromArray(char char_array[])
{
	if (strcmp(char_array, "ZERO") == 0)
	{
		return CALC_ZERO;
	}
	else if (strcmp(char_array, "IS_COEFF") == 0)
	{
		return CALC_IS_COEFF;
	}
	else if (strcmp(char_array, "IS_ZERO") == 0)
	{
		return CALC_IS_ZERO;
	}
	else if (strcmp(char_array, "CLONE") == 0)
	{
		return CALC_CLONE;
	}
	else if (strcmp(char_array, "ADD") == 0)
	{
		return CALC_ADD;
	}
	else if (strcmp(char_array, "MUL") == 0)
	{
		return CALC_MUL;
	}
	else if (strcmp(char_array, "NEG") == 0)
	{
		return CALC_NEG;
	}
	else if (strcmp(char_array, "SUB") == 0)
	{
		return CALC_SUB;
	}
	else if (strcmp(char_array, "IS_EQ") == 0)
	{
		return CALC_IS_EQ;
	}
	else if (strcmp(char_array, "DEG") == 0)
	{
		return CALC_DEG;
	}
	else if (strcmp(char_array, "DEG_BY") == 0)
	{
		return CALC_DEG_BY;
	}
	else if (strcmp(char_array, "AT") == 0)
	{
		return CALC_AT;
	}
	else if (strcmp(char_array, "PRINT") == 0)
	{
		return CALC_PRINT;
	}
	else if (strcmp(char_array, "POP") == 0)
	{
		return CALC_POP;
	}
	else
	{
		return CALC_WRONG_COMMAND;
	}
}

static FuncResult ParseVarIdx(unsigned *var_idx)
{
	if (!isdigit(CharPeek()))
	{
		return FUNC_ERROR;
	}
	else
	{
		unsigned long parser_var_idx = 0;

		while (isdigit(CharPeek()))
		{
			parser_var_idx = 10 * parser_var_idx + CharToExp(CharPeek());

			if (!ParserVarIndexIsInRange(parser_var_idx))
			{
				return FUNC_ERROR;
			}
			else
			{
				ParseChar(NULL);
			}
		}

		*var_idx = parser_var_idx;
		return FUNC_SUCCESS;
	}
}

static FuncResult ParseValue(poly_coeff_t *value)
{
	if (CharIsStartOfCoeff(CharPeek()))
	{
		parser_coeff_t parser_coeff = 0;
		bool is_negative = false;

		if (CharPeek() == '-')
		{
			is_negative = true;
			ParseChar(NULL);

			if (!isdigit(CharPeek()))
			{
				return FUNC_ERROR;
			}
		}

		while (isdigit(CharPeek()))
		{
			char c = ParseChar(NULL);
			parser_coeff = 10 * (parser_coeff) + CharToCoeff(c);

			if (!ParserCoeffIsInRange(parser_coeff, is_negative))
			{
				return FUNC_ERROR;
			}
		}

		*value = (poly_coeff_t) parser_coeff;

		if (is_negative)
		{
			*value *= -1;
		}

		return FUNC_SUCCESS;
	}
	else
	{
		return FUNC_ERROR;
	}
}

/* IMPLEMENTACJA FUNKCJI GŁÓWNYCH */

bool ParserLineIsCommand()
{
	return CharIsStartOfCommand(CharPeek());
}

bool ParserLineIsEndOfFile()
{
	return CharPeek() == EOF;
}

FuncResult ParseLinePoly(Poly *p, int row)
{
	int col = 1;

	FuncResult result = ParsePoly(&col, p);

	if (result == FUNC_ERROR || CharPeek() != '\n')
	{
		PolyDestroy(p);
		ErrorParserPoly(row, col);
		ParserSkipLine();
		return FUNC_ERROR;
	}
	else
	{
		ParserSkipLine();
		return result;
	}
}

FuncResult ParseLineCommand(ParserCommand *command, poly_coeff_t *parameter, int row)
{
	char char_array[MAX_COMMAND_LENGTH] = "";

	unsigned i = 0;

	while (CharPeek() != ' ' && CharPeek() != '\n' && i < MAX_COMMAND_LENGTH - 1)
	{
		char_array[i] = ParseChar(NULL);
		i++;
	}

	*command = ParseCommandFromArray(char_array);

	if (*command == CALC_WRONG_COMMAND)
	{
		ParserSkipLine();
		ErrorParserCommand(row);
		return FUNC_ERROR;
	}
	else if (*command == CALC_DEG_BY || *command == CALC_AT)
	{
		if (CharPeek() == ' ')
		{
			ParseChar(NULL);

			unsigned int var_idx;

			switch (*command)
			{
				case CALC_DEG_BY:
					if (ParseVarIdx(&var_idx) == FUNC_ERROR || CharPeek() != '\n')
					{
						ParserSkipLine();
						ErrorParserVariable(row);
						return FUNC_ERROR;
					}
					else
					{
						*parameter = (poly_coeff_t) var_idx;
					}
					break;

				case CALC_AT:
					if (ParseValue(parameter) == FUNC_ERROR || CharPeek() != '\n')
					{
						ParserSkipLine();
						ErrorParserValue(row);
						return FUNC_ERROR;
					}
					break;
				default:
					assert(false);
					return FUNC_ERROR;
					break;
			}

			ParseChar(NULL);
			return FUNC_SUCCESS;
		}
		else
		{
			ParserSkipLine();
			ErrorParserCommand(row);
			return FUNC_ERROR;
		}
	}
	else if (CharPeek() != '\n')
	{
		ErrorParserCommand(row);
		ParserSkipLine();
		return FUNC_ERROR;
	}
	else
	{
		ParseChar(NULL);
		return FUNC_SUCCESS;
	}
}
