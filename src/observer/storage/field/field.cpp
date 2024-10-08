/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/04/24.
//

#include "storage/field/field.h"
#include "common/log/log.h"
#include "common/value.h"
#include "storage/record/record.h"
#include <algorithm>
#include <cstring>

void Field::set_int(Record &record, int value)
{
  ASSERT(field_->type() == AttrType::INTS, "could not set int value to a non-int field");
  ASSERT(field_->len() == sizeof(value), "invalid field len");

  char *field_data = record.data() + field_->offset();
  memcpy(field_data, &value, sizeof(value));
}

int Field::get_int(const Record &record)
{
  Value value(field_->type(), const_cast<char *>(record.data() + field_->offset()), field_->len());
  return value.get_int();
}

const char *Field::get_data(const Record &record) { return record.data() + field_->offset(); }

void Field::set_data(Record &record, const char *data, int len)
{
  ASSERT(field_->len() >= len, "copy len too large");
  ASSERT(record.len() >= field_->offset() + field_->len(), "record is too short to store field data");
  char *field_data = record.data() + field_->offset();
  memset(field_data, 0, field_->len());
  memcpy(field_data, data, len);
}

RC Field::set_value(Record &record, const Value &value)
{
  Value real_value;

  if (value.attr_type() == field_->type()) {
    real_value = value;
  } else {
    RC rc = Value::cast_to(value, field_->type(), real_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to cast value: %s", strrc(rc));
      return rc;
    }
  }

  if (field_->offset() + field_->len() > record.len()) {
    LOG_WARN("record is too short to store field data");
    return RC::INVALID_ARGUMENT;
  }

  set_data(record, real_value.data(), std::min(real_value.length(), field_->len()));
  return RC::SUCCESS;
}
