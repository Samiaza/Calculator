#ifndef CALCULATOR_V2_MODEL_CALCCORE_H_
#define CALCULATOR_V2_MODEL_CALCCORE_H_

#include "creditcalc.h"
#include "depositcalc.h"
#include "mathcalc.h"

namespace smartcalc {

namespace model {

struct CalcCore {
  MathCalc math_module;
  CreditCalc credit_module;
  DepositCalc deposit_module;
};

}  // namespace model

}  // namespace smartcalc

#endif  //  CALCULATOR_V2_MODEL_CALCCORE_H_