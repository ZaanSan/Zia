//
// Created by corentin on 21/02/18.
//

#include "ConnectionManager.hpp"

ConnectionManager::ConnectionManager()
= default;

ConnectionManager::~ConnectionManager()
= default;

void    ConnectionManager::start(connectionPtr c)
{
    _connections.insert(c);
    c->start();
}

void    ConnectionManager::stop(connectionPtr c)
{
    _connections.erase(c);
    c->stop();
}

void    ConnectionManager::stopAll()
{
    for (auto &c : _connections)
    {
        c->stop();
    }
    _connections.clear();
}

const connectionPtr &ConnectionManager::getConnectionByImplSock(zia::api::ImplSocket *sock)
{
    for (auto &connection : _connections)
    {
        if ((connection->getImplSock() == sock))
            return connection;
    }
}
