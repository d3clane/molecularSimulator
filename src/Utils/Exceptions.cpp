#include "Utils/Exceptions.hpp"

namespace Utils
{

ExceptionWithReason::ExceptionWithReason() noexcept
{
}

ExceptionWithReason* ExceptionWithReason::createNextException(
    const char* reason, std::unique_ptr<ExceptionWithReason>&& prevException
) noexcept
{
    ExceptionWithReason* nextException = nullptr;
    try 
    {
        nextException = new ExceptionWithReason();
    }
    catch (...)
    {
        std::cerr << "CAUGHT SMTH IN CREATING\n";
    }

    if (!nextException)
        return prevException.release();
    
    nextException->prevException_.reset(prevException.release());
    nextException->reasonString_.reset(strdup(reason));

    return nextException;
}

const char* ExceptionWithReason::what() const noexcept
{
    const ExceptionWithReason* exceptionNow = this;

    auto& a = exceptionNow->reasonString_;
    
    printf("Exception occurred. Reasons:\n");

    size_t reasonId = 0;
    const char* reason = nullptr;
    
    while (exceptionNow)
    {
        reason = exceptionNow->reasonString_.get();

        if (reason) 
        {
            fprintf(stderr, "%zu. %s ", reasonId, reason);
            reasonId++;
        }

        exceptionNow = exceptionNow->prevException_.get();
    }

    return "";
}

} // namespace Utils