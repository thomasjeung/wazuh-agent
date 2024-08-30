#include <sqlite_manager.hpp>

#include <fmt/format.h>
#include <iostream>
#include <map>

namespace sqlite_manager
{
    const std::map<ColumnType, std::string> MAP_COL_TYPE_STRING {
        {ColumnType::INTEGER, "INTEGER"}, {ColumnType::TEXT, "TEXT"}, {ColumnType::FLOAT, "REAL"}};
    const std::map<LogicalOperator, std::string> MAP_LOGOP_STRING {{LogicalOperator::AND, "AND"},
                                                                   {LogicalOperator::OR, "OR"}};

    ColumnType SQLiteManager::ColumnTypeFromSQLiteType(const int type) const
    {
        if (type == SQLite::INTEGER)
            return ColumnType::INTEGER;

        if (type == SQLite::FLOAT)
            return ColumnType::FLOAT;

        return ColumnType::TEXT;
    }

    SQLiteManager::SQLiteManager(const std::string& dbName)
        : m_dbName(dbName)
        , m_db(std::make_unique<SQLite::Database>(dbName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE))
    {
        m_db->exec("PRAGMA journal_mode=WAL;");
    }

    void SQLiteManager::CreateTable(const std::string& tableName, const std::vector<Col>& cols)
    {
        std::vector<std::string> fields;
        for (const Col& col : cols)
        {
            std::string field = fmt::format("{} {}{}{}{}",
                                            col.m_name,
                                            MAP_COL_TYPE_STRING.at(col.m_type),
                                            (col.m_primaryKey) ? " PRIMARY KEY" : "",
                                            (col.m_autoIncrement) ? " AUTOINCREMENT" : "",
                                            (col.m_notNull) ? " NOT NULL" : "");
            fields.push_back(field);
        }

        std::string queryString =
            fmt::format("CREATE TABLE IF NOT EXISTS {} ({})", tableName, fmt::format("{}", fmt::join(fields, ", ")));

        Execute(queryString);
    }

    void SQLiteManager::Insert(const std::string& tableName, const std::vector<Col>& cols)
    {
        std::vector<std::string> names;
        std::vector<std::string> values;

        for (const Col& col : cols)
        {
            names.push_back(col.m_name);
            if (col.m_type == ColumnType::TEXT)
            {
                values.push_back(fmt::format("'{}'", col.m_value));
            }
            else
                values.push_back(col.m_value);
        }

        std::string queryString =
            fmt::format("INSERT INTO {} ({}) VALUES ({})", tableName, fmt::join(names, ", "), fmt::join(values, ", "));

        Execute(queryString);
    }

    int SQLiteManager::GetCount(const std::string& tableName)
    {
        std::string queryString = fmt::format("SELECT COUNT(*) FROM {}", tableName);

        try
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            SQLite::Statement query(*m_db, queryString);
            int count = 0;

            if (query.executeStep())
            {
                count = query.getColumn(0).getInt();
            }
            else
            {
                std::cerr << "Error getting element count." << '\n';
            }
            return count;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error during GetCount operation: " << e.what() << '\n';
            return {};
        }
    }

    std::vector<Row> SQLiteManager::Select(const std::string& tableName,
                                           const std::vector<Col>& fields,
                                           const std::vector<Col>& selCriteria,
                                           LogicalOperator logOp)
    {
        std::string selectedFields;
        if (fields.empty())
        {
            selectedFields = "*";
        }
        else
        {
            std::vector<std::string> fieldNames;
            fieldNames.reserve(fields.size());

            for (auto& col : fields)
            {
                fieldNames.push_back(col.m_name);
            }
            selectedFields = fmt::format("{}", fmt::join(fieldNames, ", "));
        }

        std::string condition;
        if (!selCriteria.empty())
        {
            std::vector<std::string> conditions;
            for (auto& col : selCriteria)
            {
                if (col.m_type == ColumnType::TEXT)
                    conditions.push_back(fmt::format("{} = '{}'", col.m_name, col.m_value));
                else
                    conditions.push_back(fmt::format("{} = {}", col.m_name, col.m_value));
            }
            condition = fmt::format("WHERE {}", fmt::join(conditions, fmt::format(" {} ", MAP_LOGOP_STRING.at(logOp))));
        }

        std::string queryString = fmt::format("SELECT {} FROM {} {}", selectedFields, tableName, condition);

        // Do the actual query
        std::vector<Row> results;
        try
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            SQLite::Statement query(*m_db, queryString);
            while (query.executeStep())
            {
                int nColumns = query.getColumnCount();
                std::vector<Col> queryFields;
                queryFields.reserve(static_cast<size_t>(nColumns));
                for (int i = 0; i < nColumns; i++)
                {
                    sqlite_manager::Col field(query.getColumn(i).getName(),
                                              ColumnTypeFromSQLiteType(query.getColumn(i).getType()),
                                              query.getColumn(i).getString());
                    queryFields.push_back(field);
                }
                results.push_back(queryFields);
            }

            return results;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error during Retrieve operation: " << e.what() << '\n';
            return {};
        }
        return results;
    }

    void SQLiteManager::Remove(const std::string& tableName, const std::vector<Col>& selCriteria, LogicalOperator logOp)
    {
        // Build the query string
        std::string whereClause;
        if (!selCriteria.empty())
        {
            std::vector<std::string> critFields;
            for (auto& col : selCriteria)
            {
                if (col.m_type == ColumnType::TEXT)
                {
                    critFields.push_back(fmt::format("{}='{}'", col.m_name, col.m_value));
                }
                else
                {
                    critFields.push_back(fmt::format("{}={}", col.m_name, col.m_value));
                }
            }
            whereClause =
                fmt::format(" WHERE {}", fmt::join(critFields, fmt::format(" {} ", MAP_LOGOP_STRING.at(logOp))));
        }

        std::string queryString = fmt::format("DELETE FROM {}{}", tableName, whereClause);

        Execute(queryString);
    }

    void SQLiteManager::Update(const std::string& tableName,
                               const std::vector<Col>& fields,
                               const std::vector<Col>& selCriteria,
                               LogicalOperator logOp)
    {
        if (fields.empty())
        {
            std::cerr << "Error: Missing update fields" << '\n';
            throw;
        }

        // Build the query string
        std::vector<std::string> setFields;
        for (auto& col : fields)
        {
            if (col.m_type == ColumnType::TEXT)
            {
                setFields.push_back(fmt::format("{}='{}'", col.m_name, col.m_value));
            }
            else
            {
                setFields.push_back(fmt::format("{}={}", col.m_name, col.m_value));
            }
        }
        std::string updateValues = fmt::format("{}", fmt::join(setFields, ", "));

        std::string whereClause;
        if (!selCriteria.empty())
        {
            std::vector<std::string> conditions;
            for (auto& col : selCriteria)
            {
                if (col.m_type == ColumnType::TEXT)
                {
                    conditions.push_back(fmt::format("{}='{}'", col.m_name, col.m_value));
                }
                else
                {
                    conditions.push_back(fmt::format("{}={}", col.m_name, col.m_value));
                }
            }
            whereClause =
                fmt::format(" WHERE {}", fmt::join(conditions, fmt::format(" {} ", MAP_LOGOP_STRING.at(logOp))));
        }

        std::string queryString = fmt::format("UPDATE {} SET {}{}", tableName, updateValues, whereClause);

        // Do the actual query
        Execute(queryString);
    }

    void SQLiteManager::Execute(const std::string& query)
    {
        try
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_db->exec(query);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error during database operation: " << e.what() << '\n';
        }
    }

    void SQLiteManager::DropTable(const std::string& tableName)
    {
        std::string queryString = fmt::format("DROP TABLE {}", tableName);

        Execute(queryString);
    }

    SQLite::Transaction SQLiteManager::BeginTransaction()
    {
        return SQLite::Transaction(*m_db);
    }

    void SQLiteManager::CommitTransaction(SQLite::Transaction& transaction)
    {
        transaction.commit();
    }

    void SQLiteManager::RollbackTransaction(SQLite::Transaction& transaction)
    {
        transaction.rollback();
    }
} // namespace sqlite_manager
