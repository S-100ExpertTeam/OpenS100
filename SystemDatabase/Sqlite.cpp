#include "Sqlite.h"

#include <sqlite3.h>

#include <limits>
#include <utility>

namespace s100::sdb
{
    namespace
    {
        [[noreturn]] void fail(sqlite3* db, int result, std::string_view context)
        {
            std::string message(context);
            message += ": ";
            message += db ? sqlite3_errmsg(db) : sqlite3_errstr(result);
            const int code = db ? sqlite3_extended_errcode(db) : result;
            throw DatabaseError(code, message);
        }

        int checkedSize(std::size_t size)
        {
            if (size > static_cast<std::size_t>(std::numeric_limits<int>::max()))
            {
                throw DatabaseError(SQLITE_TOOBIG, "value too large to bind");
            }
            return static_cast<int>(size);
        }
    }

    DatabaseError::DatabaseError(int code, const std::string& message)
        : std::runtime_error(message), code_(code)
    {
    }

    // --- Connection ---------------------------------------------------------------------------

    Connection Connection::open(const std::filesystem::path& file)
    {
        const std::u8string name = file.u8string();
        return openWith(std::string(name.begin(), name.end()));
    }

    Connection Connection::openInMemory()
    {
        return openWith(":memory:");
    }

    Connection Connection::openWith(const std::string& utf8Name)
    {
        sqlite3* db = nullptr;
        const int result = sqlite3_open_v2(utf8Name.c_str(), &db,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, nullptr);
        if (result != SQLITE_OK)
        {
            std::string message = "cannot open database '" + utf8Name + "': ";
            message += db ? sqlite3_errmsg(db) : sqlite3_errstr(result);
            sqlite3_close_v2(db);
            throw DatabaseError(result, message);
        }
        sqlite3_extended_result_codes(db, 1);
        return Connection(db);
    }

    Connection::Connection(Connection&& other) noexcept
        : db_(std::exchange(other.db_, nullptr))
    {
    }

    Connection& Connection::operator=(Connection&& other) noexcept
    {
        if (this != &other)
        {
            sqlite3_close_v2(db_);
            db_ = std::exchange(other.db_, nullptr);
        }
        return *this;
    }

    Connection::~Connection()
    {
        sqlite3_close_v2(db_);
    }

    void Connection::execute(std::string_view sql)
    {
        char* error = nullptr;
        const int result = sqlite3_exec(db_, std::string(sql).c_str(), nullptr, nullptr, &error);
        if (result != SQLITE_OK)
        {
            std::string message = error ? error : sqlite3_errstr(result);
            sqlite3_free(error);
            throw DatabaseError(sqlite3_extended_errcode(db_), message);
        }
    }

    std::int64_t Connection::lastInsertRowId() const
    {
        return sqlite3_last_insert_rowid(db_);
    }

    int Connection::changes() const
    {
        return sqlite3_changes(db_);
    }

    // --- Statement ----------------------------------------------------------------------------

    Statement::Statement(Connection& connection, std::string_view sql)
        : db_(connection.handle())
    {
        const int result = sqlite3_prepare_v2(db_, sql.data(), checkedSize(sql.size()), &statement_, nullptr);
        if (result != SQLITE_OK)
        {
            fail(db_, result, "prepare failed");
        }
    }

    Statement::Statement(Statement&& other) noexcept
        : db_(other.db_), statement_(std::exchange(other.statement_, nullptr))
    {
    }

    Statement& Statement::operator=(Statement&& other) noexcept
    {
        if (this != &other)
        {
            sqlite3_finalize(statement_);
            db_ = other.db_;
            statement_ = std::exchange(other.statement_, nullptr);
        }
        return *this;
    }

    Statement::~Statement()
    {
        sqlite3_finalize(statement_);
    }

    void Statement::check(int result) const
    {
        if (result != SQLITE_OK)
        {
            fail(db_, result, "bind failed");
        }
    }

    Statement& Statement::bind(int index, std::int64_t value)
    {
        check(sqlite3_bind_int64(statement_, index, value));
        return *this;
    }

    Statement& Statement::bind(int index, int value)
    {
        check(sqlite3_bind_int(statement_, index, value));
        return *this;
    }

    Statement& Statement::bind(int index, double value)
    {
        check(sqlite3_bind_double(statement_, index, value));
        return *this;
    }

    Statement& Statement::bind(int index, std::string_view value)
    {
        check(sqlite3_bind_text(statement_, index, value.data(), checkedSize(value.size()), SQLITE_TRANSIENT));
        return *this;
    }

    Statement& Statement::bind(int index, std::span<const std::uint8_t> value)
    {
        // A zero-length blob still has to be stored as a blob, not NULL: pass a non-null pointer.
        static constexpr std::uint8_t empty = 0;
        const void* data = value.empty() ? &empty : value.data();
        check(sqlite3_bind_blob(statement_, index, data, checkedSize(value.size()), SQLITE_TRANSIENT));
        return *this;
    }

    Statement& Statement::bindNull(int index)
    {
        check(sqlite3_bind_null(statement_, index));
        return *this;
    }

    bool Statement::step()
    {
        const int result = sqlite3_step(statement_);
        if (result == SQLITE_ROW)
        {
            return true;
        }
        if (result == SQLITE_DONE)
        {
            return false;
        }
        fail(db_, result, "step failed");
    }

    void Statement::run()
    {
        while (step())
        {
        }
    }

    void Statement::reset()
    {
        sqlite3_reset(statement_);
        sqlite3_clear_bindings(statement_);
    }

    bool Statement::isNull(int column) const
    {
        return sqlite3_column_type(statement_, column) == SQLITE_NULL;
    }

    std::int64_t Statement::int64(int column) const
    {
        return sqlite3_column_int64(statement_, column);
    }

    int Statement::integer(int column) const
    {
        return sqlite3_column_int(statement_, column);
    }

    double Statement::real(int column) const
    {
        return sqlite3_column_double(statement_, column);
    }

    std::string Statement::text(int column) const
    {
        const auto* data = sqlite3_column_text(statement_, column);
        const int size = sqlite3_column_bytes(statement_, column);
        if (!data)
        {
            return {};
        }
        return std::string(reinterpret_cast<const char*>(data), static_cast<std::size_t>(size));
    }

    std::vector<std::uint8_t> Statement::blob(int column) const
    {
        const auto* data = static_cast<const std::uint8_t*>(sqlite3_column_blob(statement_, column));
        const int size = sqlite3_column_bytes(statement_, column);
        if (!data || size <= 0)
        {
            return {};
        }
        return std::vector<std::uint8_t>(data, data + size);
    }

    std::optional<std::int64_t> Statement::optionalInt64(int column) const
    {
        if (isNull(column))
        {
            return std::nullopt;
        }
        return int64(column);
    }

    std::optional<int> Statement::optionalInteger(int column) const
    {
        if (isNull(column))
        {
            return std::nullopt;
        }
        return integer(column);
    }

    std::optional<double> Statement::optionalReal(int column) const
    {
        if (isNull(column))
        {
            return std::nullopt;
        }
        return real(column);
    }

    // --- Transaction --------------------------------------------------------------------------

    Transaction::Transaction(Connection& connection)
        : connection_(&connection)
    {
        connection_->execute("BEGIN IMMEDIATE");
    }

    Transaction::~Transaction()
    {
        if (open_)
        {
            try
            {
                connection_->execute("ROLLBACK");
            }
            catch (const DatabaseError&)
            {
                // SQLite may already have rolled back (for example after SQLITE_FULL); nothing
                // more can be done in a destructor.
            }
        }
    }

    void Transaction::commit()
    {
        connection_->execute("COMMIT");
        open_ = false;
    }
}
