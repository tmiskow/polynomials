/** @file
   Interfejs stosu

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date 2017-05-25
*/

#ifndef __STACK_H__
#define __STACK_H__

#include "poly.h"

#include <stdbool.h>

/** Definicja typu struktury stosu wielomianów */
typedef struct Stack Stack;

/** Definicja typu struktury elementu stosu wielomianów */
typedef struct StackItem StackItem;

/**
 * Struktura przechowująca stos wielomianów.
 * Implementacja stosu jest oparta o listę.
 */
struct Stack
{
	StackItem* top_item; ///< wskaźnik na element z wierzchołka stosu
};

/**
 * Struktura przechowująca element stosu wielomianów.
 * Zawiera wielomian i wskaźnik na następny element.
 */
struct StackItem
{
	Poly poly; ///< wielomian
	StackItem* next_item; ///< wskaźnik na następny element
};

/**
 * Inicjalizuje stos.
 * @param[in] stack : stos
 */
void StackInit(Stack *stack);

/**
 * Usuwa elementy stosu z pamięci.
 * @param[in] stack : stos
 */
void StackDestroy(Stack *stack);

/**
 * Umieszcza wielomian na wierzchu stosu.
 * Stos przejmuje na własnośc wielomian @p p.
 * @param[in] stack : stos
 * @param[in] p : wielomian
 */
void StackPush(Stack *stack, const Poly *p);

/**
 * Zdejmuje wielomian z wierzchu stosu.
 * @param[in] stack : stos
 * @return wielomian z wierzchołka
 */
Poly StackPop(Stack *stack);

/**
 * Podgląda wielomian z wierzchu stosu.
 * @param[in] stack : stos
 * @return wielomian z wierzchołka
 */
Poly StackPeek(Stack *stack);

/**
 * Sprawdza, czy stos jest pusty.
 * @param[in] stack : stos
 * @return Czy stos jest pusty?
 */
bool StackIsEmpty(const Stack *stack);

#endif /* __STACK_H__ */
