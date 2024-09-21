#include "date_type.h"
#include "common/type/attr_type.h"
#include "common/type/data_type.h"
#include "common/value.h"
#include <utility>

int DateType::compare(const Value &left, const Value &right) const {
  return left.get_int() - right.get_int();
}

RC DateType::set_value_from_str(Value &val, const string &data) const {
  return DataType::type_instance(AttrType::CHARS)->cast_to(Value(data.c_str()), AttrType::DATES, val);
}

RC DateType::to_string(const Value &val, string &result) const {
  int num = val.get_int();
  string res;
  res += std::to_string(num / 10000);
  res += "-";
  res += std::to_string(num / 100 % 100);
  res += "-";
  res += std::to_string(num % 100);

  result = std::move(res);
  return RC::SUCCESS;
}
