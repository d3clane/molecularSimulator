#ifndef UTILS_EXCEPTIONS_HPP
#define UTILS_EXCEPTIONS_HPP

#include <exception>
#include <memory>
#include <iostream>

namespace Utils
{

#define EXCEPTION_WITH_REASON_CREATE_NEXT_EXCEPTION(REASON, PREV_EXCEPTION) \
    Utils::ExceptionWithReason::createNextException(REASON, PREV_EXCEPTION, __func__, __FILE__, __LINE__)

class ExceptionWithReason : public std::exception
{
    std::unique_ptr<char> reasonString_;
    std::unique_ptr<ExceptionWithReason> prevException_;


public:
    ExceptionWithReason() noexcept;

    const char* what() const noexcept override;

    static ExceptionWithReason* createNextException(
        const char* reason, std::unique_ptr<ExceptionWithReason>&& prevException
    ) noexcept;

    static ExceptionWithReason* createNextException(
        const char* reason, std::unique_ptr<ExceptionWithReason>&& prevException,
        const char* funcWithErr, const char* fileWithErr, const size_t lineWithErr
    ) noexcept;
};

} // namespace Utils

#endif // UTILS_EXCEPTIONS_HPP