#include "date_type.h"
#include "common/type/attr_type.h"
#include "common/type/data_type.h"
#include "common/value.h"
#include <utility>

string format(int number, int count) {
  string res(count, '0');
  for (int i = 0; i < count; i++) {
    res[count - i - 1] = std::to_string(number % 10)[0];
    number /= 10;
  }
  return res;
}

int DateType::compare(const Value &left, const Value &right) const {
  return left.get_int() - right.get_int();
}

RC DateType::set_value_from_str(Value &val, const string &data) const {
  return DataType::type_instance(AttrType::CHARS)->cast_to(Value(data.c_str()), AttrType::DATES, val);
}

RC DateType::to_string(const Value &val, string &result) const {
  int num = val.get_int();
  string res;
  res += format(num / 10000, 4);
  res += "-";
  res += format(num / 100 % 100, 2);
  res += "-";
  res += format(num % 100, 2);

  result = std::move(res);
  return RC::SUCCESS;
}
