#include "drop_table_executor.h"
#include "sql/stmt/drop_table_stmt.h"
#include "event/session_event.h"
#include "session/session.h"
#include "storage/db/db.h"


RC DropTableExecutor::execute(SQLStageEvent *sql_event) {
  DropTableStmt *drop_stmt = (DropTableStmt *)sql_event->stmt();
  return sql_event->session_event()->session()->get_current_db()->drop_table( drop_stmt->table_name().c_str());
}
