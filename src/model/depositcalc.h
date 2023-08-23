#ifndef CALCULATOR_MODEL_DEPOSITCALC_H_
#define CALCULATOR_MODEL_DEPOSITCALC_H_

#include <list>

#include "structs.h"

namespace smartcalc {

namespace model {

static constexpr double kKeyRate = 7.5;
static constexpr double kTaxTreshold = 1000000 * kKeyRate / 100;

using namespace smartcalc::common;

class DepositCalc final {
 public:
  DepositCalc() = default;
  void SetDepositData(const deposit_input &data) noexcept;
  [[nodiscard]] deposit_input GetDepositData() const noexcept;
  [[nodiscard]] deposit_output Calculate() noexcept;
  [[nodiscard]] deposit_output Calculate(const deposit_input &data) noexcept;
  void SetStartDate(const Date &date) noexcept;
  void SetAmount(const double &amount) noexcept;
  void SetTerm(const size_t &term) noexcept;
  void SetDepositRate(const double &rate) noexcept;
  void SetTaxRate(const double &rate) noexcept;
  void SetPaymentType(const dep_payment_t &type) noexcept;
  void SetCapitalizeState(const bool &state) noexcept;
  void SetDepositOperations(const std::list<std::pair<Date, double>> &ops);

 private:
  [[nodiscard]] Date GetDepositEnd() noexcept;
  void DateCorrection(Date *date) noexcept;
  void SetNextPaymentDay(Date *date) noexcept;
  [[nodiscard]] double GetDayrate(const Date &date) noexcept;
  void PaymentHandling(double *payment_value) noexcept;
  void DepOpsHandling(const Date &date) noexcept;
  void TaxHandling(double *prev_sum_for_tax) noexcept;
  [[nodiscard]] double GetCurrInterest() noexcept;
  void FinalInterestHandling(const double &payment_value) noexcept;

  Date start_date_ = Date(1, 1, 2000);
  double amount_ = 100000;
  size_t term_ = 12;
  double dep_rate_ = 12;
  double tax_rate_ = 12;
  dep_payment_t payment_type_ = dep_payment_t::kEnd;
  bool is_capitalize_ = true;
  std::list<std::pair<Date, double>> ops_;
  double interest_ = 0;
  double tax_summ_ = 0;
  double deposit_ = 0;
};

}  // namespace model

}  // namespace smartcalc

#endif  //  CALCULATOR_MODEL_DEPOSITCALC_H_