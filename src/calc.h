/** @file
   TODO

   @author Tomasz Miśków <tm385898@students.mimuw.edu.pl>
   @copyright Uniwersytet Warszawski
   @date TODO
*/

#ifndef __CALC_H__
#define __CALC_H__

#include "stack.h"
#include "error.h"

/** TODO */
FuncResult CalcZero(Stack *stack);

/** TODO */
FuncResult CalcIsCoeff(Stack *stack);

/** TODO */
FuncResult CalcIsZero(Stack *stack);

/** TODO */
FuncResult CalcClone(Stack *stack);

/** TODO */
FuncResult CalcAdd(Stack *stack);

/** TODO */
FuncResult CalcMul(Stack *stack);

/** TODO */
FuncResult CalcNeg(Stack *stack);

/** TODO */
FuncResult CalcSub(Stack *stack);

/** TODO */
FuncResult CalcIsEq(Stack *stack);

/** TODO */
FuncResult CalcDeg(Stack *stack);

/** TODO */
FuncResult CalcDegBy(Stack *stack, unsigned var_idx);

/** TODO */
FuncResult CalcAt(Stack *stack, poly_coeff_t x);

/** TODO */
FuncResult CalcPrint(Stack *stack);

/** TODO */
FuncResult CalcPop(Stack *stack);

#endif /* __CALC_H__ */
