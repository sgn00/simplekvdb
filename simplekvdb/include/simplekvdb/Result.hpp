#pragma once
#include <string>
#include <unordered_map>
#include <variant>

namespace simplekvdb {
    class Result {
    public:
        enum class Status : int {
            OK,
            Error
        };

        enum class ErrorCode : int {
            None,
            KeyNotFound,
            FieldNotFound,
            WrongType,
            DBFull
        };

        Result(Status status, ErrorCode error_code = ErrorCode::None)
            : status_(status), error_code_(error_code) {}

        template <typename T>
        Result(Status status, T value)
            : status_(status), value_(std::move(value)) {}

        Status status() const { return status_; }
        ErrorCode error_code() const { return error_code_; }

        template <typename T>
        bool holds_type() const {
            return std::holds_alternative<T>(value_);
        }

        template <typename T>
        const T& value() const { return std::get<T>(value_); }

    private:
        Status status_;
        ErrorCode error_code_;
        std::variant<std::monostate, std::string, int> value_;
    };
}

