/** @file
   Interfejs kalkulatora

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/

#ifndef __CALC_H__
#define __CALC_H__

#include "stack.h"
#include "error.h"

/**
 * Wstawia na wierzchołek stosu wielomian tożsamościowo równy zeru.
 * @param[in] stack : stos kalkulatora
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult CalcZero(Stack *stack);

/**
 * Sprawdza, czy wielomian na wierzchołku stosu jest współczynnikiem.
 * Jeżeli tak, to wypisuje na standardowe wyjście `1`.
 * W przeciwnym wypadku wypisuje `0`.
 * @param[in] stack : stos kalkulatora
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult CalcIsCoeff(Stack *stack);

/**
 * Sprawdza, czy wielomian na wierzchołku stosu jest tożsamościowo równy zeru.
 * Jeżeli tak, to wypisuje na standardowe wyjście `1`.
 * W przeciwnym wypadku wypisuje `0`.
 * @param[in] stack : stos kalkulatora
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult CalcIsZero(Stack *stack);

/**
 * Wstawia na wierzchołek stosu kopię wielomianu z wierzchołka.
 * @param[in] stack : stos kalkulatora
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult CalcClone(Stack *stack);

/**
 * Usuwa dwa wielomiany z wierzchu stosu, a na ich miejsce wstawia ich sumę.
 * @param[in] stack : stos kalkulatora
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult CalcAdd(Stack *stack);

/**
 * Usuwa dwa wielomiany z wierzchu stosu, a na ich miejsce wstawia ich iloczyn.
 * @param[in] stack : stos kalkulatora
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult CalcMul(Stack *stack);

/**
 * Zamienia wielomian z wierzchołka na wielomian do niego przeciwny.
 * @param[in] stack : stos kalkulatora
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult CalcNeg(Stack *stack);

/**
 * Usuwa dwa wielomiany z wierzchu stosu, a na ich miejsce wstawia ich różnicę.
 * @param[in] stack : stos kalkulatora
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult CalcSub(Stack *stack);

/**
 * Sprawdza, czy dwa wielomiany z wierzchu stosu są równe.
 * Jeżeli tak, to wypisuje na standardowe wyjście `1`.
 * W przeciwnym wypadku wypisuje `0`.
 * @param[in] stack : stos kalkulatora
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult CalcIsEq(Stack *stack);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu.
 * Dla wielomianu tożsamościowo równego zeru, wypisuje `-1`.
 * @param[in] stack : stos kalkulatora
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult CalcDeg(Stack *stack);

/**
 * Wypisuje na standardowe wyjście stopień wielomianu ze względu na zmienną o danym indeksie.
 * Dla wielomianu tożsamościowo równego zeru, wypisuje `-1`.
 * @param[in] stack : stos kalkulatora
 * @param[in] var_idx : indeks zmiennej
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult CalcDegBy(Stack *stack, unsigned var_idx);

/**
 * Zamienia wielomian z wierzchołka na wartość wielomianu w punkcie @p x.
 * @param[in] stack : stos kalkulatora
 * @param[in] x
 * @return status zakończenia funkcji inforumjący o sukcesie lub błędzie
 */
FuncResult CalcAt(Stack *stack, poly_coeff_t x);

/**
 * Wypisuje na standardowe wyjście wielomian z wierzchołka stosu.
 * @param[in] stack : stos kalkulatora
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult CalcPrint(Stack *stack);

/**
 * Usuwa wielomian z wierzchołka stosu.
 * @param[in] stack : stos kalkulatora
 * @return status zakończenia funkcji informujący o sukcesie lub błędzie
 */
FuncResult CalcPop(Stack *stack);

#endif /* __CALC_H__ */
