#pragma once
#include <string>
#include <unordered_map>
#include <variant>

namespace simplekvdb {
class Result {
 public:
  enum class Status : int { OK = 0, Error = 1 };

  enum class ErrorCode : int {
    None = 0,
    KeyNotFound = 1,
    FieldNotFound = 2,
    WrongType = 3,
    DBFull = 4
  };

  Result(Status status, ErrorCode error_code = ErrorCode::None)
      : status_(status), error_code_(error_code) {}

  template <typename T>
  Result(Status status, T value) : status_(status), value_(std::move(value)) {}

  Status status() const { return status_; }
  ErrorCode error_code() const { return error_code_; }

  template <typename T>
  bool holds_type() const {
    return std::holds_alternative<T>(value_);
  }

  template <typename T>
  const T& value() const {
    return std::get<T>(value_);
  }

 private:
  Status status_;
  ErrorCode error_code_;
  std::variant<std::monostate, std::string, int> value_;
};
}  // namespace simplekvdb
