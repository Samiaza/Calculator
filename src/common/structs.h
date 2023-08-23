#ifndef CALCULATOR_COMMON_STRUCTS_H_
#define CALCULATOR_COMMON_STRUCTS_H_

#include <list>
#include <vector>

#include "date.h"

namespace smartcalc {

namespace common {

typedef enum class CreditType { kAnnuit = 0, KDiff } credit_t;

typedef enum class DepositPaymentType {
  kDay = 0,
  kWeek,
  kMonth,
  kYear,
  kEnd
} dep_payment_t;

typedef struct CreditCalculatorInput {
  double amount;
  size_t term;
  double rate;
  credit_t type;
} credit_input;

typedef struct CreditCalculatorOutput {
  std::vector<double> payments;
  double interest;
  double total;
} credit_output;

typedef struct DepositCalculatorInput {
  Date start_date{0, 0, 0};
  double amount;
  size_t term;
  double dep_rate;
  double tax_rate;
  dep_payment_t payment_type;
  bool is_capitalize;
  std::list<std::pair<Date, double>> ops;
} deposit_input;

typedef struct DepositCalculatorOutput {
  double interest;
  double tax;
  double deposit;
} deposit_output;

}  // namespace common

}  // namespace smartcalc

#endif  //  CALCULATOR_COMMON_STRUCTS_H_
