#pragma once
#include <cstdint>
#include <limits>
#include "./utils.h"

namespace fulgurdb {
const int NONE_INLINE_PTR_SIZE = 8;
enum TYPE_ID {
  TINYINT_ID = 0,
  SMALLINT_ID,
  MEDIUMINT_ID,
  INT_ID,
  BIGINT_ID,
  DECIMAL_ID,
  FLOAT_ID,
  DOUBLE_ID,
  VARCHAR_ID,
  MEDIUMTEXT_ID,
  YEAR_ID,
  DATE_ID,
  TIME_ID,
  DATETIME_ID,
  TIMESTAMP_ID,
  TYPE_ID_UPBOUND
};
const int FURGUR_TYPE_NUMS = TYPE_ID_UPBOUND;

// MVTO metadata in record header
const uint64_t INVALID_TRANSACTION_ID = 0;
const uint64_t INVALID_READ_TIMESTAMP = 0;
const uint64_t INVALID_TIMESTAMP = 0;
const uint64_t MAX_TIMESTAMP = std::numeric_limits<uint64_t>::max();

// epoch-based transaction id
const uint64_t INVALID_EPOCH_ID = std::numeric_limits<uint64_t>::max();

//@brief visibility status of mvcc records
enum RecordVersionVisibility {
  INVALID = 0,
  INVISIBLE,
  DELETED,
  VISIBLE
};

}
