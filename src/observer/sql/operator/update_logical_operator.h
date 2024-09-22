#pragma once

#include "sql/operator/logical_operator.h"

class Field;

class UpdateLogicalOperator : public LogicalOperator
{
public:
  UpdateLogicalOperator(Table *table, Field *field, Value *values, int value_amount)
      : table_(table), fields_(field), values_(values), value_amount_(value_amount)
  {}
  LogicalOperatorType type() const override { return LogicalOperatorType::UPDATE; }

  Table     *table() const { return table_; }
  Field *fields() const { return fields_; }
  Value     *values() const { return values_; }
  int        value_amount() const { return value_amount_; }

private:
  Table *table_;
  Field *fields_;
  Value *values_;
  int    value_amount_;
};