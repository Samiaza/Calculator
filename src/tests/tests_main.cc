#include <gtest/gtest.h>

#include <cmath>
#include <cstring>

#include "calccore.h"
#include "structs.h"

using namespace smartcalc::model;

TEST(CalcCore, math_calc_test_1) {
  MathCalc *calc = new MathCalc;
  ASSERT_EQ(calc->CheckExpression("*(1+2)4(cos(x*7-2)+sin(2*x))70^(-10)+5(-3)"),
            false);
  std::string expression = "sin(cos(tan(x)))";
  ASSERT_EQ(calc->CheckExpression("sin(cos(tan(x)))"), true);
  arg_type args;
  args.push_back(5.43);
  auto vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_NEAR(0.4008403056, vals[0], 1e-7);
  delete calc;
}

TEST(CalcCore, math_calc_test_2) {
  MathCalc *calc = new MathCalc;
  std::string expression = "asin(acos(atan(x)))";
  arg_type args;
  args.push_back(5.43);
  auto vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_EQ(std::isnan(vals[0]), true);
  ASSERT_EQ(vals.size(), 1);
  delete calc;
}

TEST(CalcCore, math_calc_test_3) {
  MathCalc *calc = new MathCalc;
  ASSERT_EQ(calc->CheckExpression("asin(acos/(atan(x)5))"), false);
  std::string expression = "asin(acos/(atan(x)))";
  arg_type args;
  args.push_back(5.43);
  auto vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);
  delete calc;
}

TEST(CalcCore, math_calc_test_4) {
  MathCalc *calc = new MathCalc;
  ASSERT_EQ(calc->CheckExpression("(1+2)4(cos(x*7-2)+sin(2*x))70^(-10)+5(-3)"),
            false);
  std::string expression = "*asin(acos(atan(x)))";
  arg_type args;
  args.push_back(-5.43);
  auto vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);
  delete calc;
}

TEST(CalcCore, math_calc_test_5) {
  MathCalc *calc = new MathCalc;
  ASSERT_EQ(calc->CheckExpression("asin(acos(4)3/(atan(x)5))"), false);
  std::string expression = "100.235+x-(x+10)";
  arg_type args;
  args.push_back(3.5);
  auto vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_EQ(vals.size(), 1);
  ASSERT_NEAR(90.2350, vals[0], 1e-7);
  delete calc;
}

TEST(CalcCore, math_calc_test_6) {
  MathCalc *calc = new MathCalc;
  ASSERT_EQ(calc->CheckExpression("5(asin(acos/(atan(x)5))"), false);
  std::string expression = "5-(x+10)";
  arg_type args;
  args.push_back(3.5);
  auto vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_EQ(vals.size(), 1);
  ASSERT_NEAR(-8.5000, vals[0], 1e-7);

  expression = "-(3)*(-x-(7*(-(-(-(-(-7)))))))";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_EQ(vals.size(), 1);
  ASSERT_NEAR(-136.5000, vals[0], 1e-7);

  expression = "(-8+x*5)/(3^2-0.4^(-x))";
  args[0] = 3.5;
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_EQ(vals.size(), 1);
  double check_val = -0.60489156502;
  ASSERT_NEAR(check_val, vals[0], 1e-7);

  expression = "(1+2)*4*cos(x*7-2)+sin(2*x)";
  args[0] = 3.5;
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_EQ(vals.size(), 1);
  check_val = -9.8226690824;
  ASSERT_NEAR(check_val, vals[0], 1e-7);

  delete calc;
}

TEST(CalcCore, math_calc_test_7) {
  MathCalc *calc = new MathCalc;
  std::string expression = "4^acos(x/4)/tan(2*x)";
  arg_type args;
  args.push_back(3.5);
  auto vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_EQ(vals.size(), 1);
  double check_val = 2.31214930869;
  ASSERT_NEAR(check_val, vals[0], 1e-7);

  expression = "ln(55/(2+x))";
  args[0] = 3.5;
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_EQ(vals.size(), 1);
  check_val = 2.30258509299;
  ASSERT_NEAR(check_val, vals[0], 1e-7);

  expression = "(1+2)*4*(cos(x*7-2)+sin(2*x))*70^(-10)+5*(-3)";
  args[0] = 3.5;
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_EQ(vals.size(), 1);
  check_val = -15;
  ASSERT_NEAR(check_val, vals[0], 1e-7);

  delete calc;
}

TEST(CalcCore, math_calc_test_8) {
  MathCalc *calc = new MathCalc;
  std::string expression = "asin(2/x)%(x)+atan(+x)";
  arg_type args;
  args.push_back(3.5);
  auto vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_EQ(vals.size(), 1);
  double check_val = std::fmod(asin(2 / 3.5), 3.5) + std::atan(3.5);
  ASSERT_NEAR(check_val, vals[0], 1e-7);

  expression = "1/2*3";
  args[0] = 3.5;
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_EQ(vals.size(), 1);
  check_val = 1.5;
  ASSERT_NEAR(check_val, vals[0], 1e-7);

  expression = "2^3^2";
  args[0] = 3.5;
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_EQ(vals.size(), 1);
  check_val = 512;
  ASSERT_NEAR(check_val, vals[0], 1e-7);

  expression = "-(3)(-x-(7(-(-(-(--7))))))";
  args[0] = 3.5;
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "-sqrt(x^log(5-x))+ln(55/(2+x))";
  args[0] = 3.5;
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), true);
  ASSERT_EQ(vals.size(), 1);
  check_val = 1.18597174919;
  ASSERT_NEAR(check_val, vals[0], 1e-7);

  delete calc;
}

TEST(CalcCore, math_calc_test_9) {
  MathCalc *calc = new MathCalc;
  std::string expression = "(1+2)4(cos(x*7-2)+sin(2*x))70^(-10)+5(-3)";
  arg_type args;
  args.push_back(-5.43);
  auto vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "-";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "(10/x)%";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "((10/x)%5";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "((10/x)%5)))";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  delete calc;
}

TEST(CalcCore, math_calc_test_10) {
  MathCalc *calc = new MathCalc;
  std::string expression = "((10*z/x)%5))";
  arg_type args;
  args.push_back(-5.43);
  auto vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "((10|*z/x)%5))";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "((10|*z/x;)%5))";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "((&10x5*1/x)%5))";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "((10x5*1/x)%5)";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  delete calc;
}

TEST(CalcCore, math_calc_test_11) {
  MathCalc *calc = new MathCalc;
  std::string expression = "((x5*1/x)%5)";
  arg_type args;
  args.push_back(-5.43);
  auto vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "(()%5)";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "tg((4)%5)";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "--tan((4)%5)";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "5--tan((4)%5)";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  delete calc;
}

TEST(CalcCore, math_calc_test_12) {
  MathCalc *calc = new MathCalc;
  std::string expression = "((5--tan((4)%5)";
  arg_type args;
  args.push_back(-5.43);
  auto vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "(()%5)";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "tg((4)%5)";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "-()-tan((4)%5)";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  expression = "5-1/()-tan((4)%5)";
  vals = calc->Calculate(expression, args);
  ASSERT_EQ(calc->CheckExpression(), false);
  ASSERT_EQ(vals.size(), 0);

  delete calc;
}

TEST(CalcCore, calc_credit_test_1) {
  CreditCalc calc;
  credit_input input = {1000000, 17, 15.3, credit_t::kAnnuit};
  credit_output output = calc.Calculate(input);
  ASSERT_NEAR(output.payments[0], 65801, 1e0);
  ASSERT_NEAR(output.interest, 118623, 1e0);
  ASSERT_NEAR(output.total, 1118623, 1e0);
  credit_input check = calc.GetCreditData();
  ASSERT_EQ(check.amount, 1000000);
  ASSERT_EQ(check.rate, 15.3);
  ASSERT_EQ(check.term, 17);
  ASSERT_EQ(check.type, credit_t::kAnnuit);
}

TEST(CalcCore, calc_credit_test_2) {
  CreditCalc calc;
  calc.SetAmount(1000000);
  calc.SetTerm(17);
  calc.SetRate(15.3);
  calc.SetType(credit_t::kAnnuit);
  credit_output output = calc.Calculate();
  ASSERT_NEAR(output.payments[0], 65801, 1e0);
  ASSERT_NEAR(output.interest, 118623, 1e0);
  ASSERT_NEAR(output.total, 1118623, 1e0);
}

TEST(CalcCore, calc_credit_test_3) {
  CreditCalc calc;
  credit_input input = {1137000, 49, 65.4, credit_t::KDiff};
  credit_output output = calc.Calculate(input);
  ASSERT_NEAR(output.payments[0], 85170, 1e0);
  ASSERT_NEAR(output.payments[48], 24468, 1e0);
  ASSERT_NEAR(output.interest, 1549162, 1e0);
  ASSERT_NEAR(output.total, 2686162, 1e0);
}

TEST(CalcCore, calc_credit_test_4) {
  CreditCalc calc;
  calc.SetAmount(1137000);
  calc.SetTerm(49);
  calc.SetRate(65.4);
  calc.SetType(credit_t::KDiff);
  credit_output output = calc.Calculate();
  ASSERT_NEAR(output.payments[0], 85170, 1e0);
  ASSERT_NEAR(output.payments[48], 24468, 1e0);
  ASSERT_NEAR(output.interest, 1549162, 1e0);
  ASSERT_NEAR(output.total, 2686162, 1e0);
}

TEST(CalcCore, calc_deposit_test_1) {
  DepositCalc calc;
  std::list<std::pair<Date, double>> ops = {{{22, 7, 2024}, 123456},
                                            {{22, 11, 2024}, -423456},
                                            {{22, 2, 2025}, 123456}};
  deposit_input input = {{29, 2, 2024},       1450000, 17, 15.3, 13,
                         dep_payment_t::kDay, false,   ops};
  deposit_output output = calc.Calculate(input);
  ASSERT_NEAR(output.deposit, 1273456.00, 1e0);
  ASSERT_NEAR(output.interest, 296294.19, 1e0);
  ASSERT_NEAR(output.tax, 19018.24, 1e0);
  deposit_input check = calc.GetDepositData();
  ASSERT_EQ(check.dep_rate, 15.3);
  ASSERT_EQ(check.term, 17);
  ASSERT_EQ(check.is_capitalize, false);
  ASSERT_EQ(check.ops.front().second, 123456);
  ASSERT_EQ(check.start_date.day_, 29);
  ASSERT_EQ(check.start_date.month_, 2);
  ASSERT_EQ(check.start_date.year_, 2024);
  ASSERT_EQ(check.payment_type, dep_payment_t::kDay);
  ASSERT_EQ(check.tax_rate, 13);
}

TEST(CalcCore, calc_deposit_test_2) {
  DepositCalc calc;
  std::list<std::pair<Date, double>> ops = {{{22, 7, 2024}, 123456},
                                            {{22, 11, 2024}, -423456},
                                            {{22, 2, 2025}, 123456}};
  deposit_input input = {{1, 2, 2024},        1450000, 17, 15.3, 13,
                         dep_payment_t::kDay, true,    ops};
  deposit_output output = calc.Calculate(input);
  ASSERT_NEAR(output.deposit, 1607721.12, 1e0);
  ASSERT_NEAR(output.interest, 334265.12, 1e0);
  ASSERT_NEAR(output.tax, 23954.47, 1e0);
}

TEST(CalcCore, calc_deposit_test_3) {
  DepositCalc calc;
  std::list<std::pair<Date, double>> ops = {{{22, 7, 2024}, 123456},
                                            {{22, 11, 2024}, -423456},
                                            {{22, 2, 2025}, 123456}};
  deposit_input input = {{29, 2, 2024},        1450000, 17, 15.3, 13,
                         dep_payment_t::kWeek, true,    ops};
  deposit_output output = calc.Calculate(input);
  ASSERT_NEAR(output.deposit, 1605053.95, 1e0);
  ASSERT_NEAR(output.interest, 331597.95, 1e0);
  ASSERT_NEAR(output.tax, 23607.73, 1e0);
}

TEST(CalcCore, calc_deposit_test_4) {
  DepositCalc calc;
  std::list<std::pair<Date, double>> ops = {{{22, 7, 2024}, 123456},
                                            {{22, 11, 2024}, -423456},
                                            {{22, 2, 2025}, 123456}};
  deposit_input input = {{29, 2, 2024},         1450000, 17, 15.3, 13,
                         dep_payment_t::kMonth, true,    ops};
  deposit_output output = calc.Calculate(input);
  ASSERT_NEAR(output.deposit, 1603256.32, 1e0);
  ASSERT_NEAR(output.interest, 329800.32, 1e0);
  ASSERT_NEAR(output.tax, 23374.04, 1e0);
}

TEST(CalcCore, calc_deposit_test_5) {
  DepositCalc calc;
  std::list<std::pair<Date, double>> ops = {{{22, 7, 2024}, 123456},
                                            {{22, 11, 2024}, -423456},
                                            {{22, 2, 2025}, 123456}};
  deposit_input input = {{29, 2, 2024},        1450000, 17, 15.3, 13,
                         dep_payment_t::kYear, true,    ops};
  deposit_output output = calc.Calculate(input);
  ASSERT_NEAR(output.deposit, 1583345.72, 1e2);
  ASSERT_NEAR(output.interest, 309889.72, 1e2);
  ASSERT_NEAR(output.tax, 30535.66, 1e1);
}

TEST(CalcCore, calc_deposit_test_6) {
  DepositCalc calc;
  std::list<std::pair<Date, double>> ops = {{{22, 7, 2024}, 123456},
                                            {{22, 11, 2024}, -423456},
                                            {{22, 2, 2025}, 123456}};
  deposit_input input = {{29, 2, 2024},       1450000, 17, 15.3, 13,
                         dep_payment_t::kEnd, true,    ops};
  deposit_output output = calc.Calculate(input);
  ASSERT_NEAR(output.deposit, 1569750.29, 1e2);
  ASSERT_NEAR(output.interest, 296294.29, 1e2);
  ASSERT_NEAR(output.tax, 28768.26, 1e1);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// sqrt(sin(x)^2)+5*2.71^(-(x^100))*cos(x)
// sqrt(cos(x))*cos(200*x)+sqrt(x^2)-3.1415/4*(4-x^2)^0.01
