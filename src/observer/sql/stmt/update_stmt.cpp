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
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "common/log/log.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/db/db.h"
#include "storage/field/field_meta.h"
#include <utility>
#include <vector>

UpdateStmt::~UpdateStmt() { delete filter_stmt_; }

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{
  RC     rc    = RC::SUCCESS;
  Table *table = db->find_table(update.relation_name.c_str());
  if (table == nullptr) {
    LOG_WARN("Table not found. table=%s", update.relation_name.c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  FilterStmt                         *filter_stmt = nullptr;
  unordered_map<std::string, Table *> tables{{update.relation_name, table}};
  rc = FilterStmt::create(db, table, &tables, update.conditions.data(), update.conditions.size(), filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("Failed to create filter stmt. rc=%d", rc);
    return rc;
  }

  vector<Field> update_fields;
  for (const FieldMeta &field : *table->table_meta().field_metas()) {
    if (strcmp(field.name(), update.attribute_name.c_str()) == 0) {
      update_fields.push_back(Field(table, const_cast<FieldMeta *>(&field)));
      break;
    }
  }
  if (update_fields.empty()) {
    LOG_WARN("Field not found. field=%s", update.attribute_name.c_str());
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  stmt = new UpdateStmt(table, std::move(update_fields), const_cast<Value *>(&update.value), 1, filter_stmt);
  return RC::SUCCESS;
}
