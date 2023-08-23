#include "depositcalc.h"

#include <cmath>
#include <cstring>

namespace smartcalc {

namespace model {

using namespace smartcalc::common;

void DepositCalc::SetDepositData(const deposit_input &data) noexcept {
  SetStartDate(data.start_date);
  SetAmount(data.amount);
  SetTerm(data.term);
  SetDepositRate(data.dep_rate);
  SetTaxRate(data.tax_rate);
  SetPaymentType(data.payment_type);
  SetCapitalizeState(data.is_capitalize);
  SetDepositOperations(data.ops);
}

deposit_input DepositCalc::GetDepositData() const noexcept {
  return {start_date_, amount_,       term_,          dep_rate_,
          tax_rate_,   payment_type_, is_capitalize_, ops_};
}

deposit_output DepositCalc::Calculate() noexcept {
  deposit_ = amount_;
  tax_summ_ = 0;
  interest_ = 0;
  Date curr_date = start_date_;
  Date end_date = GetDepositEnd();
  DateCorrection(&end_date);
  Date payment_date = curr_date;
  SetNextPaymentDay(&payment_date);
  double dayrate = GetDayrate(curr_date);
  double payment_value = 0;
  double prev_sum_for_tax = 0;
  while (!curr_date.IsEqual(end_date)) {
    curr_date.NextDay();
    payment_value += deposit_ * dayrate;
    if (curr_date.IsEqual(payment_date)) {
      SetNextPaymentDay(&payment_date);
      PaymentHandling(&payment_value);
    }
    DepOpsHandling(curr_date);
    if (curr_date.IsEndOfYear()) {
      Date tmp = curr_date;
      tmp.NextDay();
      dayrate = GetDayrate(tmp);
      TaxHandling(&prev_sum_for_tax);
    }
  }
  FinalInterestHandling(payment_value);
  TaxHandling(&prev_sum_for_tax);
  return {interest_, tax_summ_, deposit_};
}

deposit_output DepositCalc::Calculate(const deposit_input &data) noexcept {
  SetDepositData(data);
  return Calculate();
}

void DepositCalc::SetStartDate(const Date &date) noexcept {
  start_date_ = date;
}

void DepositCalc::SetAmount(const double &amount) noexcept { amount_ = amount; }

void DepositCalc::SetTerm(const size_t &term) noexcept { term_ = term; }

void DepositCalc::SetDepositRate(const double &rate) noexcept {
  dep_rate_ = rate;
}

void DepositCalc::SetTaxRate(const double &rate) noexcept { tax_rate_ = rate; }

void DepositCalc::SetPaymentType(const dep_payment_t &type) noexcept {
  payment_type_ = type;
}

void DepositCalc::SetCapitalizeState(const bool &state) noexcept {
  is_capitalize_ = state;
}

void DepositCalc::SetDepositOperations(
    const std::list<std::pair<Date, double>> &ops) {
  ops_ = ops;
}

Date DepositCalc::GetDepositEnd() noexcept {
  Date end_date = start_date_;
  for (size_t i = 0; i < term_; ++i) {
    end_date.AddMonth();
  }
  return end_date;
}

void DepositCalc::DateCorrection(Date *date) noexcept {
  while (!date->IsEndOfMonth() && start_date_.day_ > date->day_) {
    date->NextDay();
  }
}

void DepositCalc::SetNextPaymentDay(Date *date) noexcept {
  switch (payment_type_) {
    case dep_payment_t::kDay:
      date->NextDay();
      break;
    case dep_payment_t::kWeek:
      date->AddWeek();
      break;
    case dep_payment_t::kMonth:
      date->AddMonth();
      DateCorrection(date);
      break;
    case dep_payment_t::kYear:
      date->AddYear();
      DateCorrection(date);
      break;
    case dep_payment_t::kEnd:
      *date = GetDepositEnd();
      DateCorrection(date);
    default:
      break;
  }
}

double DepositCalc::GetDayrate(const Date &date) noexcept {
  return dep_rate_ / 100 / (date.IsYearLeap() ? 366 : 365);
}

void DepositCalc::PaymentHandling(double *payment_value) noexcept {
  if (is_capitalize_) {
    deposit_ += *payment_value;
  } else {
    interest_ += *payment_value;
  }
  *payment_value = 0;
}

void DepositCalc::DepOpsHandling(const Date &date) noexcept {
  for (auto op : ops_) {
    if (date.IsEqual(op.first)) {
      deposit_ += op.second;
      amount_ += op.second;
    }
  }
}

void DepositCalc::TaxHandling(double *prev_sum_for_tax) noexcept {
  double curr_profit = GetCurrInterest();
  double tmp_tax_sum = curr_profit - *prev_sum_for_tax - kTaxTreshold;
  if (tmp_tax_sum < 0) tmp_tax_sum = 0;
  tax_summ_ += tmp_tax_sum * tax_rate_ / 100;
  *prev_sum_for_tax = curr_profit;
}

double DepositCalc::GetCurrInterest() noexcept {
  return is_capitalize_ ? deposit_ - amount_ : interest_;
}

void DepositCalc::FinalInterestHandling(const double &payment_value) noexcept {
  if (is_capitalize_) {
    deposit_ += payment_value;
    interest_ = deposit_ - amount_;
  } else {
    interest_ += payment_value;
  }
}

}  // namespace model

}  // namespace smartcalc
