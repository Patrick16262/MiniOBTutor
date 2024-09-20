#include "drop_table_stmt.h"

RC DropTableStmt::create(Db *db, const DropTableSqlNode &sql_node, Stmt *&stmt) {
  stmt = new DropTableStmt(sql_node.relation_name);
  return RC::SUCCESS;
}
