#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class userException : public std::exception
{
public:
    userException(const char *errorMessage);
    virtual const char *what() const noexcept;

private:
    const char * message;
};

#endif // EXCEPTIONS_H
