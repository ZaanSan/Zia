//
// Created by corentin on 22/02/18.
//

#ifndef CPP_ZIA_ZIAEXCEPTION_HPP
#define CPP_ZIA_ZIAEXCEPTION_HPP

#include <exception>
#include <string>

class ZiaException : public std::exception
{
public:
    explicit ZiaException(const std::string &exception);
    ~ZiaException() override = default;
    const char* what() const throw();

private:
    std::string exception;
};


#endif //CPP_ZIA_ZIAEXCEPTION_HPP
