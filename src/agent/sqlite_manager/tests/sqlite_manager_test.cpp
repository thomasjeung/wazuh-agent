#include <gtest/gtest.h>

#include <sqlite_manager.hpp>

class SQLiteManagerTest : public ::testing::Test
{
protected:
    const std::string m_dbName = "testdb.db";
    const std::string m_tableName = "TestTable";

    std::unique_ptr<sqlite_manager::SQLiteManager> m_db;

    void SetUp() override
    {
        m_db = std::make_unique<sqlite_manager::SQLiteManager>(m_dbName);
    }

    void TearDown() override {}

    void AddTestData()
    {

        using namespace sqlite_manager;
        EXPECT_NO_THROW(m_db->Remove(m_tableName));
        m_db->Insert(m_tableName,
                     {sqlite_manager::Col("Name", ColumnType::TEXT, "DummyData"),
                      sqlite_manager::Col("Status", ColumnType::TEXT, "DummyData")});
        m_db->Insert(m_tableName,
                     {sqlite_manager::Col("Name", ColumnType::TEXT, "MyTestName"),
                      sqlite_manager::Col("Status", ColumnType::TEXT, "MyTestValue")});
        m_db->Insert(m_tableName,
                     {sqlite_manager::Col("Name", ColumnType::TEXT, "DummyData2"),
                      sqlite_manager::Col("Status", ColumnType::TEXT, "DummyData2")});
        m_db->Insert(m_tableName,
                     {sqlite_manager::Col("Name", ColumnType::TEXT, "DummyData3"),
                      sqlite_manager::Col("Status", ColumnType::TEXT, "DummyData3")});
    }
};

TEST_F(SQLiteManagerTest, CreateTableTest)
{
    sqlite_manager::Col col1 {"Id", sqlite_manager::ColumnType::INTEGER, true, true, true};
    sqlite_manager::Col col2 {"Name", sqlite_manager::ColumnType::TEXT, true, false};
    sqlite_manager::Col col3 {"Status", sqlite_manager::ColumnType::TEXT, true, false};
    sqlite_manager::Col col4 {"Module", sqlite_manager::ColumnType::TEXT, false, false};
    sqlite_manager::Col col5 {"Orden", sqlite_manager::ColumnType::INTEGER, false, false, false};

    EXPECT_NO_THROW(m_db->CreateTable(m_tableName, {col1, col2, col3, col4, col5}));
}

TEST_F(SQLiteManagerTest, InsertTest)
{
    sqlite_manager::Col col1 {"Name", sqlite_manager::ColumnType::TEXT, "ItemName1"};
    sqlite_manager::Col col2 {"Status", sqlite_manager::ColumnType::TEXT, "ItemStatus1"};

    EXPECT_NO_THROW(m_db->Insert(m_tableName, {col1, col2}));
    EXPECT_NO_THROW(m_db->Insert(m_tableName,
                                 {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "ItemName2"),
                                  sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "ItemStatus2")}));
    EXPECT_NO_THROW(m_db->Insert(m_tableName,
                                 {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "ItemName3"),
                                  sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "ItemStatus3"),
                                  sqlite_manager::Col("Module", sqlite_manager::ColumnType::TEXT, "ItemModule3")}));

    EXPECT_NO_THROW(m_db->Insert(m_tableName,
                                 {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "ItemName4"),
                                  sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "ItemStatus4"),
                                  sqlite_manager::Col("Module", sqlite_manager::ColumnType::TEXT, "ItemModule4"),
                                  sqlite_manager::Col("Orden", sqlite_manager::ColumnType::INTEGER, "16")}));
}

TEST_F(SQLiteManagerTest, GetCountTest)
{
    EXPECT_NO_THROW(m_db->Remove(m_tableName));
    int count = m_db->GetCount(m_tableName);
    EXPECT_EQ(count, 0);

    sqlite_manager::Col col1 {"Name", sqlite_manager::ColumnType::TEXT, "ItemName1"};
    sqlite_manager::Col col2 {"Status", sqlite_manager::ColumnType::TEXT, "ItemStatus1"};
    EXPECT_NO_THROW(m_db->Insert(m_tableName, {col1, col2}));

    count = m_db->GetCount(m_tableName);
    EXPECT_EQ(count, 1);

    EXPECT_NO_THROW(m_db->Insert(m_tableName, {col1, col2}));

    count = m_db->GetCount(m_tableName);
    EXPECT_EQ(count, 2);
}

static void DumpResults(std::vector<sqlite_manager::Row>& ret)
{
    std::cout << "---------- " << ret.size() << " rows returned. ----------" << '\n';
    for (const auto& row : ret)
    {
        for (const auto& field : row)
        {
            std::cout << "[" << field.m_name << ": " << field.m_value << "]";
        }
        std::cout << '\n';
    }
}

TEST_F(SQLiteManagerTest, SelectTest)
{
    AddTestData();

    std::vector<sqlite_manager::Col> cols;

    // all fields, no selection criteria
    std::vector<sqlite_manager::Row> ret = m_db->Select(m_tableName, cols);

    DumpResults(ret);
    EXPECT_NE(ret.size(), 0);

    // all fields with selection criteria
    ret = m_db->Select(m_tableName,
                       cols,
                       {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "MyTestName"),
                        sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "MyTestValue")});

    DumpResults(ret);
    EXPECT_NE(ret.size(), 0);

    // only Name field no selection criteria
    cols.clear();
    cols.emplace_back("Name", sqlite_manager::ColumnType::TEXT, "MyTestName");
    ret.clear();
    ret = m_db->Select(m_tableName, cols);

    DumpResults(ret);
    EXPECT_NE(ret.size(), 0);

    // only Name field with selection criteria
    cols.clear();
    cols.emplace_back("Name", sqlite_manager::ColumnType::TEXT, "MyTestName");
    ret.clear();
    ret = m_db->Select(m_tableName,
                       cols,
                       {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "MyTestName"),
                        sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "MyTestValue")});

    DumpResults(ret);
    EXPECT_NE(ret.size(), 0);
}

TEST_F(SQLiteManagerTest, RemoveTest)
{
    AddTestData();

    int count = m_db->GetCount(m_tableName);
    EXPECT_EQ(count, 4);

    // Remove a single record
    EXPECT_NO_THROW(m_db->Remove(m_tableName,
                                 {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "MyTestName"),
                                  sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "MyTestValue")}));
    count = m_db->GetCount(m_tableName);
    EXPECT_EQ(count, 3);

    // Remove remaining records
    EXPECT_NO_THROW(m_db->Remove(m_tableName));
    count = m_db->GetCount(m_tableName);
    EXPECT_EQ(count, 0);
}

TEST_F(SQLiteManagerTest, UpdateTest)
{
    AddTestData();
    EXPECT_NO_THROW(m_db->Update(m_tableName,
                                 {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "Updated name"),
                                  sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "Updated status")},
                                 {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "MyTestName")}));

    auto ret =
        m_db->Select(m_tableName, {}, {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "Updated name")});
    DumpResults(ret);
    EXPECT_EQ(ret.size(), 1);

    EXPECT_NO_THROW(m_db->Update(m_tableName,
                                 {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "Updated name2"),
                                  sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "Updated status2")},
                                 {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "Updated name"),
                                  sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "Updated status")}));

    ret =
        m_db->Select(m_tableName, {}, {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "Updated name2")});
    DumpResults(ret);
    EXPECT_EQ(ret.size(), 1);
}

TEST_F(SQLiteManagerTest, TransactionTest)
{
    {
        auto transaction = m_db->BeginTransaction();

        m_db->Insert(m_tableName,
                     {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "TransactionName"),
                      sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "TransactionStatus")});

        m_db->Insert(m_tableName,
                     {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "TransactionName2"),
                      sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "TransactionStatus2")});
        EXPECT_NO_THROW(m_db->RollbackTransaction(transaction));
    }

    // since we rolled back the transaction we should find nothing
    auto ret = m_db->Select(
        m_tableName, {}, {sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "TransactionStatus2")});

    EXPECT_EQ(ret.size(), 0);

    {
        auto transaction = m_db->BeginTransaction();

        m_db->Insert(m_tableName,
                     {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "TransactionName"),
                      sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "TransactionStatus")});

        m_db->Insert(m_tableName,
                     {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "TransactionName2"),
                      sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "TransactionStatus2")});
    }

    // since transaction obejct ran out of scope without being committed we should find nothing
    ret = m_db->Select(
        m_tableName, {}, {sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "TransactionStatus2")});

    EXPECT_EQ(ret.size(), 0);

    {
        auto transaction = m_db->BeginTransaction();

        m_db->Insert(m_tableName,
                     {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "TransactionName"),
                      sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "TransactionStatus")});

        m_db->Insert(m_tableName,
                     {sqlite_manager::Col("Name", sqlite_manager::ColumnType::TEXT, "TransactionName2"),
                      sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "TransactionStatus2")});
        EXPECT_NO_THROW(m_db->CommitTransaction(transaction));
    }

    // since we commited the transaction we should find something
    ret = m_db->Select(
        m_tableName, {}, {sqlite_manager::Col("Status", sqlite_manager::ColumnType::TEXT, "TransactionStatus2")});

    EXPECT_EQ(ret.size(), 1);
}

TEST_F(SQLiteManagerTest, DropTableTest)
{
    sqlite_manager::Col col1 {"Id", sqlite_manager::ColumnType::INTEGER, true, true, true};
    sqlite_manager::Col col2 {"Name", sqlite_manager::ColumnType::TEXT, true, false};
    sqlite_manager::Col col3 {"Status", sqlite_manager::ColumnType::TEXT, true, false};
    sqlite_manager::Col col4 {"Module", sqlite_manager::ColumnType::TEXT, false, false};
    sqlite_manager::Col col5 {"Orden", sqlite_manager::ColumnType::INTEGER, false, false, false};

    EXPECT_NO_THROW(m_db->CreateTable("DropMe", {col1, col2, col3, col4, col5}));
    EXPECT_NO_THROW(m_db->DropTable("DropMe"));

    auto ret = m_db->Select("DropMe", {}, {});

    EXPECT_EQ(ret.size(), 0);
}
