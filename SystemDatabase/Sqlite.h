#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

struct sqlite3;
struct sqlite3_stmt;

// Thin RAII layer over the SQLite C API, used by the System Database repositories.
// Every failure throws DatabaseError: the System Database treats a failed statement as a broken
// store or a programming error, not as an expected outcome (lookups that may miss return
// std::optional instead).
namespace s100::sdb
{
    class DatabaseError : public std::runtime_error
    {
    public:
        DatabaseError(int code, const std::string& message);

        // SQLite primary or extended result code (SQLITE_CONSTRAINT_UNIQUE, ...), 0 when the
        // error did not come from SQLite.
        [[nodiscard]] int code() const noexcept { return code_; }

    private:
        int code_;
    };

    class Connection final
    {
    public:
        // Opens (creating when missing) a database file. The path may contain any Unicode
        // characters; it is handed to SQLite as UTF-8.
        [[nodiscard]] static Connection open(const std::filesystem::path& file);
        [[nodiscard]] static Connection openInMemory();

        Connection(Connection&& other) noexcept;
        Connection& operator=(Connection&& other) noexcept;
        Connection(const Connection&) = delete;
        Connection& operator=(const Connection&) = delete;
        ~Connection();

        // Runs one or more statements that take no parameters and return no rows.
        void execute(std::string_view sql);

        [[nodiscard]] std::int64_t lastInsertRowId() const;
        // Rows modified by the most recent INSERT, UPDATE or DELETE.
        [[nodiscard]] int changes() const;

        [[nodiscard]] sqlite3* handle() const noexcept { return db_; }

    private:
        explicit Connection(sqlite3* db) noexcept : db_(db) {}
        static Connection openWith(const std::string& utf8Name);

        sqlite3* db_ = nullptr;
    };

    // A prepared statement. Parameters and columns are 1-based and 0-based respectively, as in
    // the C API.
    class Statement final
    {
    public:
        Statement(Connection& connection, std::string_view sql);
        Statement(Statement&& other) noexcept;
        Statement& operator=(Statement&& other) noexcept;
        Statement(const Statement&) = delete;
        Statement& operator=(const Statement&) = delete;
        ~Statement();

        Statement& bind(int index, std::int64_t value);
        Statement& bind(int index, int value);
        Statement& bind(int index, double value);
        Statement& bind(int index, std::string_view value);
        Statement& bind(int index, std::span<const std::uint8_t> value);
        Statement& bindNull(int index);

        template <typename T>
        Statement& bind(int index, const std::optional<T>& value)
        {
            return value ? bind(index, *value) : bindNull(index);
        }

        // Advances to the next row. Returns false when the statement has finished.
        [[nodiscard]] bool step();
        // Executes a statement that returns no rows.
        void run();
        void reset();

        [[nodiscard]] bool isNull(int column) const;
        [[nodiscard]] std::int64_t int64(int column) const;
        [[nodiscard]] int integer(int column) const;
        [[nodiscard]] double real(int column) const;
        [[nodiscard]] std::string text(int column) const;
        [[nodiscard]] std::vector<std::uint8_t> blob(int column) const;

        [[nodiscard]] std::optional<std::int64_t> optionalInt64(int column) const;
        [[nodiscard]] std::optional<int> optionalInteger(int column) const;
        [[nodiscard]] std::optional<double> optionalReal(int column) const;

    private:
        void check(int result) const;

        sqlite3* db_ = nullptr;
        sqlite3_stmt* statement_ = nullptr;
    };

    // BEGIN IMMEDIATE on construction; ROLLBACK on destruction unless commit() was called.
    // Transactions do not nest.
    class Transaction final
    {
    public:
        explicit Transaction(Connection& connection);
        Transaction(const Transaction&) = delete;
        Transaction& operator=(const Transaction&) = delete;
        ~Transaction();

        void commit();

    private:
        Connection* connection_;
        bool open_ = true;
    };
}
