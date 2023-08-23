#include "mathcalc.h"

#include <cmath>
#include <cstring>
#include <stdexcept>

namespace smartcalc {

namespace model {

void MathCalc::SetExpression(const expr_type &expression) noexcept {
  if (expression != expression_) {
    expression_ = expression;
    expression_is_valid_ = true;
    postfixed_.clear();
    Parse();
  }
}

void MathCalc::Parse() noexcept {
  ResetLexemStack();
  expr_it_ = expression_.begin();
  expr_iterator_type last = std::prev(expression_.end());
  if (expr_it_ != expression_.end() &&
      (std::strchr(")/*^%", *expr_it_) || std::strchr("(/*-+%^", *last) ||
       (std::isalpha(*last) && *last != 'x'))) {
    expression_is_valid_ = false;
  }
  while (expr_it_ != expression_.end() && expression_is_valid_) {
    if (std::isdigit(*expr_it_)) {
      ParseNumber();
    } else if (*expr_it_ == 'x') {
      ParseX();
    } else if (*expr_it_ == '(') {
      ParseOpeningBracket();
    } else if (*expr_it_ == ')') {
      ParseClosingBracket();
    } else if (std::isalpha(*expr_it_)) {
      ParseFunction();
    } else if (std::strchr("-+", *expr_it_) &&
               (expr_it_ == expression_.begin() ||
                *std::prev(expr_it_) == '(')) {
      ParseUnary();
    } else if (std::strchr("+-/*%^", *expr_it_)) {
      ParseBinary();
    } else {
      expression_is_valid_ = false;
    }
  }
  while (expression_is_valid_ && inner_lexem_stack_.empty() != true) {
    if (inner_lexem_stack_.top() == lexem_t::kOpeningBracket) {
      expression_is_valid_ = false;
    } else {
      FromStackToPostfixed();
    }
  }
  if (!expression_is_valid_) ResetLexemStack();
}

void MathCalc::ParseX() noexcept {
  ++expr_it_;
  if (expr_it_ == expression_.end() || std::strchr(")+-*/%^", *expr_it_)) {
    postfixed_.emplace_back(lexem_t::kX, 0);
  } else {
    expression_is_valid_ = false;
  }
}

void MathCalc::ParseNumber() noexcept {
  size_t begin_number_pos = expr_it_ - expression_.begin();
  size_t end_number_pos;
  double num = std::stod(expression_.substr(begin_number_pos), &end_number_pos);
  std::advance(expr_it_, end_number_pos);
  if (expr_it_ == expression_.end() || std::strchr(")+-*/%^", *expr_it_)) {
    postfixed_.emplace_back(lexem_t::kNumber, num);
  } else {
    expression_is_valid_ = false;
  }
}

void MathCalc::ParseFunction() noexcept {
  std::string func_name;
  while (isalpha(*expr_it_)) {
    func_name += *expr_it_++;
  }
  if (*expr_it_ == '(') {
    if (func_name == "sin" || func_name == "cos" || func_name == "tan" ||
        func_name == "asin" || func_name == "acos" || func_name == "atan" ||
        func_name == "sqrt" || func_name == "ln" || func_name == "log") {
      inner_lexem_stack_.push(func_types_[func_name]);
    } else {
      expression_is_valid_ = false;
    }
  } else {
    expression_is_valid_ = false;
  }
}

void MathCalc::ParseUnary() noexcept {
  ++expr_it_;
  if (!std::strchr(")+-*/%^", *expr_it_)) {
    inner_lexem_stack_.push(*(std::prev(expr_it_)) == '+'
                                ? lexem_t::kUnaryPlus
                                : lexem_t::kUnaryMinus);
  } else {
    expression_is_valid_ = false;
  }
}

void MathCalc::ParseBinary() noexcept {
  ++expr_it_;
  if (!std::strchr(")+-*/%^", *expr_it_)) {
    char op_char = *(std::prev(expr_it_));
    lexem_t op = op_char == '+'   ? lexem_t::kSum
                 : op_char == '-' ? lexem_t::kSub
                 : op_char == '*' ? lexem_t::kMul
                 : op_char == '/' ? lexem_t::kDiv
                 : op_char == '%' ? lexem_t::kMod
                                  : lexem_t::kPow;
    priority_t op_priority = priorities_[op];
    while (inner_lexem_stack_.empty() != true &&
           ((op_priority != priorities_[lexem_t::kPow] &&
             priorities_[inner_lexem_stack_.top()] >= op_priority) ||
            (op_priority == priorities_[lexem_t::kPow] &&
             priorities_[inner_lexem_stack_.top()] > op_priority))) {
      FromStackToPostfixed();
    }
    inner_lexem_stack_.push(op);
  } else {
    expression_is_valid_ = false;
  }
}

void MathCalc::ParseOpeningBracket() noexcept {
  ++expr_it_;
  if (!std::strchr(")*/%^", *expr_it_)) {
    inner_lexem_stack_.push(lexem_t::kOpeningBracket);
  } else {
    expression_is_valid_ = false;
  }
}

void MathCalc::ParseClosingBracket() noexcept {
  ++expr_it_;
  if (expr_it_ == expression_.end() || std::strchr(")+-*/%^", *expr_it_)) {
    while (inner_lexem_stack_.empty() != true &&
           inner_lexem_stack_.top() != lexem_t::kOpeningBracket) {
      FromStackToPostfixed();
    }
    if (inner_lexem_stack_.empty() != true &&
        inner_lexem_stack_.top() == lexem_t::kOpeningBracket) {
      inner_lexem_stack_.pop();
      if (inner_lexem_stack_.empty() != true &&
          priorities_[inner_lexem_stack_.top()] == priority_t::kFunction)
        FromStackToPostfixed();
    } else {
      expression_is_valid_ = false;
    }
  } else {
    expression_is_valid_ = false;
  }
}

arg_type MathCalc::Calculate(const expr_type &expression,
                             const arg_type &arguments) {
  SetExpression(expression);
  return Calculate(arguments);
}

arg_type MathCalc::Calculate(const arg_type &arguments) {
  ResetLexemStack();
  arg_type values(arguments.size());
  if (!expression_is_valid_) {
    // values.reserve(arguments.size());
    values.resize(0);
  }
  for (size_t i = 0; i < values.size(); ++i) {
    postf_it_ = postfixed_.begin();
    while (postf_it_ != postfixed_.end()) {
      if ((*postf_it_).first == lexem_t::kNumber) {
        inner_double_stack_.push((*postf_it_).second);
      } else if ((*postf_it_).first == lexem_t::kX) {
        inner_double_stack_.push(arguments[i]);
      } else if (priorities_[(*postf_it_).first] == priority_t::kFunction) {
        ExecFunction();
      } else if (priorities_[(*postf_it_).first] == priority_t::kUnaryOp) {
        ExecUnary();
      } else {
        ExecBinary();
      }
      ++postf_it_;
    }
    if (inner_double_stack_.empty() != true) {
      values[i] = inner_double_stack_.top();
      inner_double_stack_.pop();
      if (inner_double_stack_.empty() != true)
        throw std::invalid_argument("Your parsing is too bad!");
    }
  }
  if (!expression_is_valid_ && values.size()) values.resize(0);
  return values;
}

bool MathCalc::CheckExpression() noexcept { return expression_is_valid_; }

bool MathCalc::CheckExpression(const expr_type &expression) noexcept {
  SetExpression(expression);
  return expression_is_valid_;
}

void MathCalc::ExecFunction() noexcept {
  double arg = inner_double_stack_.top();
  inner_double_stack_.pop();
  lexem_t func = (*postf_it_).first;
  inner_double_stack_.push(func == lexem_t::kSin    ? std::sin(arg)
                           : func == lexem_t::kCos  ? std::cos(arg)
                           : func == lexem_t::kTan  ? std::tan(arg)
                           : func == lexem_t::kAsin ? std::asin(arg)
                           : func == lexem_t::kAcos ? std::acos(arg)
                           : func == lexem_t::kAtan ? std::atan(arg)
                           : func == lexem_t::kLn   ? std::log(arg)
                           : func == lexem_t::kLog  ? std::log10(arg)
                                                    : std::sqrt(arg));
}

void MathCalc::ExecUnary() noexcept {
  if ((*postf_it_).first == lexem_t::kUnaryMinus)
    inner_double_stack_.top() *= -1;
}

void MathCalc::ExecBinary() noexcept {
  double arg2 = inner_double_stack_.top();
  inner_double_stack_.pop();
  double arg1 = inner_double_stack_.top();
  inner_double_stack_.pop();
  lexem_t binary_op = (*postf_it_).first;
  inner_double_stack_.push(binary_op == lexem_t::kSum   ? arg1 + arg2
                           : binary_op == lexem_t::kSub ? arg1 - arg2
                           : binary_op == lexem_t::kMul ? arg1 * arg2
                           : binary_op == lexem_t::kDiv ? arg1 / arg2
                           : binary_op == lexem_t::kMod ? std::fmod(arg1, arg2)
                                                        : std::pow(arg1, arg2));
}

void MathCalc::FromStackToPostfixed() noexcept {
  postfixed_.emplace_back(inner_lexem_stack_.top(), 0);
  inner_lexem_stack_.pop();
}

void MathCalc::ResetLexemStack() noexcept {
  while (inner_lexem_stack_.empty() != true) {
    inner_lexem_stack_.pop();
  }
}

}  // namespace model

}  // namespace smartcalc
