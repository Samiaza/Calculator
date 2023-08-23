#include "creditcalc.h"

#include <cmath>
#include <cstring>

namespace smartcalc {

namespace model {

void CreditCalc::SetCreditData(const credit_input &data) noexcept {
  SetAmount(data.amount);
  SetTerm(data.term);
  SetRate(data.rate);
  SetType(data.type);
}

credit_input CreditCalc::GetCreditData() const noexcept {
  return {amount_, term_, rate_, type_};
}

credit_output CreditCalc::Calculate() noexcept {
  double rate = rate_ / 100 / 12, term = static_cast<double>(term_);
  double total, interest;
  std::vector<double> payments;
  payments.resize(term_);
  if (type_ == credit_t::kAnnuit) {
    double annuit_p = 1.0 * amount_ * rate / (1 - std::pow(1 + rate, -term));
    total = annuit_p * term_;
    for (size_t i = 0; i < term_; i++) {
      payments[i] = annuit_p;
    }
  } else {
    total = 0;
    for (size_t i = 0; i < term_; i++) {
      payments[i] = (amount_ / term_) * (1 + (term_ - i) * rate);
      total += payments[i];
    }
  }
  interest = total - amount_;
  return {payments, interest, total};
}

credit_output CreditCalc::Calculate(const credit_input &data) noexcept {
  SetCreditData(data);
  return Calculate();
}

void CreditCalc::SetAmount(const double &amount) noexcept { amount_ = amount; }

void CreditCalc::SetTerm(const size_t &term) noexcept { term_ = term; }

void CreditCalc::SetRate(const double &rate) noexcept { rate_ = rate; }

void CreditCalc::SetType(const credit_t &type) noexcept { type_ = type; }

}  // namespace model

}  // namespace smartcalc
