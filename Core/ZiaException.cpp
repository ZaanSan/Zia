//
// Created by corentin on 22/02/18.
//

#include "ZiaException.hpp"

ZiaException::ZiaException(const std::string &exception)
{
    this->exception = exception;
}

const char *ZiaException::what() const throw()
{
    return this->exception.c_str();
}
