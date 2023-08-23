#include "calccontroller.h"

#include <cmath>
#include <cstring>

namespace smartcalc {

namespace controller {

model::arg_type CalcController::CalculateMath(
    const model::expr_type &expression, const model::arg_type &arguments) {
  return calccore_->math_module.Calculate(expression, arguments);
}

model::arg_type CalcController::CalculateMath(
    const model::arg_type &arguments) {
  return calccore_->math_module.Calculate(arguments);
}

bool CalcController::CheckMathExpression(
    const model::expr_type &expression) noexcept {
  return calccore_->math_module.CheckExpression(expression);
}

model::deposit_output CalcController::CalculateDeposit(
    const deposit_input &data) noexcept {
  return calccore_->deposit_module.Calculate(data);
}

model::credit_output CalcController::CalculateCredit(
    const credit_input &data) noexcept {
  return calccore_->credit_module.Calculate(data);
}

}  // namespace controller

}  // namespace smartcalc
