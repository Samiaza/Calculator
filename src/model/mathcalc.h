#ifndef CALCULATOR_MODEL_MATHCALC_H_
#define CALCULATOR_MODEL_MATHCALC_H_

#include <list>
#include <map>
#include <stack>
#include <string>
#include <vector>

namespace smartcalc {

namespace model {

using expr_type = std::string;
using arg_type = std::vector<double>;

class MathCalc final {
 public:
  void SetExpression(const expr_type &expression) noexcept;
  [[nodiscard]] arg_type Calculate(const expr_type &expression,
                                   const arg_type &arguments);
  [[nodiscard]] arg_type Calculate(const arg_type &arguments);
  [[nodiscard]] bool CheckExpression(const expr_type &expression) noexcept;
  [[nodiscard]] bool CheckExpression() noexcept;

 private:
  enum class LexemType {
    kNull = 0,
    kX,
    kNumber,
    kSum,
    kSub,
    kMul,
    kDiv,
    kMod,
    kPow,
    kSin,
    kCos,
    kTan,
    kAsin,
    kAcos,
    kAtan,
    kSqrt,
    kLn,
    kLog,
    kUnaryMinus,
    kUnaryPlus,
    kOpeningBracket
  };

  enum class PriorityType { kNull = 0, kFunction, kAdd, kMult, kUnaryOp, kPow };

  using lexem_t = LexemType;
  using priority_t = PriorityType;
  using postfix_type = std::list<std::pair<lexem_t, double>>;
  using expr_iterator_type = std::string::iterator;
  using postfix_iterator_type = postfix_type::iterator;

  void Parse() noexcept;
  void ParseX() noexcept;
  void ParseOpeningBracket() noexcept;
  void ParseClosingBracket() noexcept;
  void ParseNumber() noexcept;
  void ParseFunction() noexcept;
  void ParseUnary() noexcept;
  void ParseBinary() noexcept;
  void ExecFunction() noexcept;
  void ExecUnary() noexcept;
  void ExecBinary() noexcept;
  void FromStackToPostfixed() noexcept;
  void ResetLexemStack() noexcept;

  static inline std::map<lexem_t, priority_t> priorities_ = {
      {lexem_t::kOpeningBracket, priority_t::kNull},
      {lexem_t::kX, priority_t::kNull},
      {lexem_t::kSin, priority_t::kFunction},
      {lexem_t::kCos, priority_t::kFunction},
      {lexem_t::kTan, priority_t::kFunction},
      {lexem_t::kAsin, priority_t::kFunction},
      {lexem_t::kAcos, priority_t::kFunction},
      {lexem_t::kAtan, priority_t::kFunction},
      {lexem_t::kSqrt, priority_t::kFunction},
      {lexem_t::kLn, priority_t::kFunction},
      {lexem_t::kLog, priority_t::kFunction},
      {lexem_t::kSum, priority_t::kAdd},
      {lexem_t::kSub, priority_t::kAdd},
      {lexem_t::kMul, priority_t::kMult},
      {lexem_t::kDiv, priority_t::kMult},
      {lexem_t::kMod, priority_t::kMult},
      {lexem_t::kUnaryMinus, priority_t::kUnaryOp},
      {lexem_t::kUnaryPlus, priority_t::kUnaryOp},
      {lexem_t::kPow, priority_t::kPow}};

  static inline std::map<std::string, lexem_t> func_types_ = {
      {"sin", lexem_t::kSin},   {"cos", lexem_t::kCos},
      {"tan", lexem_t::kTan},   {"asin", lexem_t::kAsin},
      {"acos", lexem_t::kAcos}, {"atan", lexem_t::kAtan},
      {"sqrt", lexem_t::kSqrt}, {"ln", lexem_t::kLn},
      {"log", lexem_t::kLog}};

  expr_type expression_;
  expr_iterator_type expr_it_;
  postfix_type postfixed_;
  postfix_iterator_type postf_it_;
  bool expression_is_valid_ = true;
  std::stack<double> inner_double_stack_;
  std::stack<lexem_t> inner_lexem_stack_;
};

}  // namespace model

}  // namespace smartcalc

#endif  //  CALCULATOR_MODEL_MATHCALC_H_