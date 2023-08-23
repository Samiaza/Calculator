#ifndef CALCULATOR_MODEL_CREDITCALC_H_
#define CALCULATOR_MODEL_CREDITCALC_H_

#include <vector>

#include "structs.h"

namespace smartcalc {

namespace model {

using namespace smartcalc::common;

class CreditCalc final {
 public:
  CreditCalc() = default;
  void SetCreditData(const credit_input &data) noexcept;
  [[nodiscard]] credit_input GetCreditData() const noexcept;
  [[nodiscard]] credit_output Calculate() noexcept;
  [[nodiscard]] credit_output Calculate(const credit_input &data) noexcept;
  void SetAmount(const double &amount) noexcept;
  void SetTerm(const size_t &term) noexcept;
  void SetRate(const double &rate) noexcept;
  void SetType(const credit_t &type) noexcept;

 private:
  double amount_ = 100000;
  size_t term_ = 12;
  double rate_ = 12;
  credit_t type_ = credit_t::kAnnuit;
  std::vector<double> payments_;
  double interest_ = 0;
  double total_ = 0;
};

}  // namespace model

}  // namespace smartcalc

#endif  //  CALCULATOR_MODEL_CREDITCALC_H_