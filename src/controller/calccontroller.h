#ifndef CALCULATOR_CONTROLLER_CALCCONTROLLER_H_
#define CALCULATOR_CONTROLLER_CALCCONTROLLER_H_

#include "calccore.h"
#include "structs.h"

namespace smartcalc {

namespace controller {

using namespace smartcalc::common;

class CalcController final {
 public:
  CalcController() = delete;
  CalcController(CalcController&&) = delete;
  CalcController(CalcController const&) = delete;
  CalcController& operator=(CalcController&&) = delete;
  CalcController& operator=(CalcController const&) = delete;
  explicit CalcController(model::CalcCore* calccore) : calccore_(calccore) {}

  model::arg_type CalculateMath(const model::expr_type& expression,
                                const model::arg_type& arguments);
  model::arg_type CalculateMath(const model::arg_type& arguments);
  bool CheckMathExpression(const model::expr_type& arguments) noexcept;
  model::deposit_output CalculateDeposit(const deposit_input& data) noexcept;
  model::credit_output CalculateCredit(const credit_input& data) noexcept;

 private:
  model::CalcCore* calccore_;
};

}  // namespace controller

}  // namespace smartcalc

#endif  //  CALCULATOR_CONTROLLER_CALCCONTROLLER_H_
