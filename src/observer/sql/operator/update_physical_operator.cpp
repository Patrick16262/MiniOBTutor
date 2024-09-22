
#include "update_physical_operator.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "sql/operator/physical_operator.h"
#include "storage/record/record.h"
#include "storage/trx/trx.h"
#include <memory>
#include <utility>
#include <vector>

RC UpdatePhysicalOperator::open(Trx *trx)
{
  RC                                 rc    = RC::SUCCESS;
  std::unique_ptr<PhysicalOperator> &child = children_.front();
  vector<Record>                     records;

  rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  while ((rc = child->next()) == RC::SUCCESS) {
    Tuple *tuple = child->current_tuple();
    if (tuple == nullptr) {
      LOG_ERROR("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
    Record   &record    = row_tuple->record();

    for (int i = 0; i < value_amount_; i++) {
      if ((rc = fields_[i].set_value(record, values_[i])) != RC::SUCCESS) {
        LOG_WARN("failed to set value: %s", strrc(rc));
        return rc;
      }
    }

    records.push_back(std::move(record));
  }

  if (rc != RC::RECORD_EOF) {
    LOG_ERROR("failed to get next record: %s", strrc(rc));
    return rc;
  } else if ((rc = child->close()) != RC::SUCCESS) {
    LOG_ERROR("failed to close child operator: %s", strrc(rc));
    return rc;
  }

  for (auto &record : records) {
    if ((rc =trx->delete_record(table_, record)) != RC::SUCCESS) {
      LOG_ERROR("failed to delete record: %s", strrc(rc));
      return rc;
    } else if ((rc = trx->insert_record(table_, record)) != RC::SUCCESS) {
      LOG_ERROR("failed to insert record: %s", strrc(rc));
      return rc;
    }
  }

  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next() { return RC::RECORD_EOF; }

RC UpdatePhysicalOperator::close() { return RC::SUCCESS; }
