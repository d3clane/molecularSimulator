#ifndef UTILS_EXCEPTIONS_HPP
#define UTILS_EXCEPTIONS_HPP

#include <exception>
#include <memory>
#include <iostream>

namespace Utils
{

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

    //void* operator new(size_t size) = delete;
};

} // namespace Utils

#endif // UTILS_EXCEPTIONS_HPP