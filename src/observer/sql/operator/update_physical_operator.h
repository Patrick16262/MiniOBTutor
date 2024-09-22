#pragma once

#include "sql/operator/physical_operator.h"

class Table;
class Field;
class Value;

class UpdatePhysicalOperator : public PhysicalOperator
{
public:
  UpdatePhysicalOperator(Table *table, Field *fields, Value *values, int value_amount)
      : table_(table), fields_(fields), values_(values), value_amount_(value_amount)
  {}
  PhysicalOperatorType type() const override { return PhysicalOperatorType::UPDATE; };

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

private:
  Table *table_;
  Field *fields_;
  Value *values_;
  int value_amount_;
};
