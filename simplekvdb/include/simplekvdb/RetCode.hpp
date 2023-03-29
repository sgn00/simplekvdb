#pragma once

namespace simplekvdb {

enum class RetCode : int {
    SUCCESS = 0,
    SUCCESS_AND_EXISTED = 1,
    FAILURE = 2,
    DOES_NOT_EXIST = 3,
    WRONG_TYPE = 4
};

}