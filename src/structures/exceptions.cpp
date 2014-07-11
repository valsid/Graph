#include "exceptions.h"

userException::userException(const char *errorMessage):
    message(errorMessage)
{}

const char *userException::what() const noexcept
{
    return message;
}
