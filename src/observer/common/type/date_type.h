#pragma once

#include "data_type.h"

/**
 * @brief 日期类型
 * @ingroup DataType
 * @details 采用yyyyMMdd的四字节整数格式存储日期
 *         例如：2023年6月13日存储为整数20230613
 */

class DateType : public DataType
{
public:
  DateType() : DataType(AttrType::DATES) {}

  virtual ~DateType() = default;

  int compare(const Value &left, const Value &right) const override;

  RC set_value_from_str(Value &val, const string &data) const override;

  RC to_string(const Value &val, string &result) const override;
};