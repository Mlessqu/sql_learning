#pragma once


namespace fela
{
    enum class CliCommand : int
    {
        create_account = 0,
        log_in,
        log_out
    };

    struct ParsedData
    {
        CliCommand command_{};
        std::optional<std::string> username_{};
        std::optional<std::string> password_{};
        std::optional<std::string> hash_token_{};
    };
    enum class DatabaseStatus
    {
        ok,
        db_error,
        not_found,
    };
    struct DbResult
    {

        DatabaseStatus status_;
        //acc table
        std::optional<int> acc_id_{};
        std::optional<std::string> acc_user_{};
        std::optional<std::string> acc_pass_hash_{};
        std::optional<std::string> creation_time_{};
        //session table
        std::optional<int> session_id_{};
        std::optional<int> account_id_ref_{};
        std::optional<std::string> exp_date_{};
        std::optional<std::string> session_token_{};

    };
}