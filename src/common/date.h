#ifndef CALCULATOR_COMMON_DATE_H_
#define CALCULATOR_COMMON_DATE_H_

#include <cstddef>

namespace smartcalc {

namespace common {

struct Date {
  Date(size_t day, size_t month, size_t year)
      : day_(day), month_(month), year_(year) {}

  void NextDay() noexcept {
    if (IsEndOfYear()) {
      ++year_;
      month_ = 1;
      day_ = 1;
    } else if (IsEndOfMonth()) {
      day_ = 1;
      ++month_;
    } else {
      ++day_;
    }
  }

  void AddWeek() noexcept {
    for (int i = 0; i < 7; i++) NextDay();
  }

  void AddMonth() noexcept {
    ++month_;
    if (month_ > 12) {
      month_ -= 12;
      ++year_;
    }
    if (month_ == 2 && day_ >= 28) {
      if (IsYearLeap()) {
        if (day_ > 29) day_ = 29;
      } else {
        if (day_ > 28) day_ = 28;
      }
    } else if ((month_ == 4 || month_ == 6 || month_ == 9 || month_ == 11) &&
               day_ > 30) {
      day_ = 30;
    }
  }

  void AddYear() noexcept {
    ++year_;
    if (month_ == 2 && !IsYearLeap() && day_ > 28) day_ = 28;
  }

  bool IsEndOfMonth() const noexcept {
    bool status = false;
    if (month_ == 4 || month_ == 6 || month_ == 9 || month_ == 11) {
      if (day_ == 30) status = true;
    } else if (month_ == 2) {
      if (IsYearLeap()) {
        if (day_ == 29) status = true;
      } else {
        if (day_ == 28) status = true;
      }
    } else {
      if (day_ == 31) status = true;
    }
    return status;
  }

  int IsYearLeap() const noexcept {
    int status = false;
    if (year_ % 400 == 0) {
      status = true;
    } else if (year_ % 100 == 0) {
      status = false;
    } else if (year_ % 4 == 0) {
      status = true;
    }
    return status;
  }

  bool IsEndOfYear() const noexcept { return day_ == 31 && month_ == 12; }

  bool IsEqual(const Date& other) const noexcept {
    return day_ == other.day_ && month_ == other.month_ && year_ == other.year_;
  }

  size_t day_;
  size_t month_;
  size_t year_;
};

}  // namespace common

}  // namespace smartcalc

#endif  //  CALCULATOR_COMMON_DATE_H_