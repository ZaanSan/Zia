//
// Created by corentin on 21/02/18.
//

#ifndef CPP_ZIA_CONNECTIONMANAGER_HPP
#define CPP_ZIA_CONNECTIONMANAGER_HPP

#ifdef WIN32
# define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
# define _HAS_AUTO_PTR_ETC 1
#endif // ifdef WIN32

#include <set>
#include <iostream>
#include "../../api/net.h"
#include "Connection.hpp"

class Connection;

typedef std::shared_ptr<Connection> connectionPtr;

class ConnectionManager {
public:
    ConnectionManager();
    ~ConnectionManager();
    ConnectionManager(const ConnectionManager &) = delete;
    ConnectionManager &operator=(const ConnectionManager &) = delete;
    void    start(connectionPtr);
    void    stop(connectionPtr);
    void    stopAll();
    const connectionPtr &getConnectionByImplSock(zia::api::ImplSocket *sock);

private:
    std::set<connectionPtr> _connections;
};


#endif //CPP_ZIA_CONNECTIONMANAGER_HPP
