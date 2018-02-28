#include <iostream>
#include "api/conf.h"
#include "api/http.h"
#include "api/module.h"
#include "api/net.h"
#include "Core/Core.hpp"

int main()
{
    Core&    ziaCore = Core::Instance();

    try
    {
        ziaCore.run();
    }
    catch (const std::exception &e)
    {
        std::cout << "ZiaException : " << e.what() << std::endl;
    }
    return 0;
}