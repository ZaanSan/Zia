//
// Created by corentin on 21/02/18.
//

#ifndef CPP_ZIA_CONNECTION_HPP
#define CPP_ZIA_CONNECTION_HPP

#ifdef WIN32
# define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
# define _HAS_AUTO_PTR_ETC 1
#endif // ifdef WIN32

#include <memory>
#include <boost/asio/ip/tcp.hpp>
#include "ConnectionManager.hpp"
#include "../../api/net.h"

struct zia::api::ImplSocket
{
    explicit ImplSocket(boost::asio::ip::tcp::socket &sock)
        : sock(sock)
    {
    }

    boost::asio::ip::tcp::socket &sock;
};

class ConnectionManager;

class Connection : public std::enable_shared_from_this<Connection>
{

private:
    ConnectionManager &_manager;
    boost::asio::ip::tcp::socket _sock;
    std::array<char, 8192> _buffer;
    zia::api::Net::Callback &_callback;
    zia::api::ImplSocket    *_implSock;

public:
    Connection(const Connection &) = delete;
    Connection &operator=(const Connection &) = delete;

    explicit Connection(boost::asio::ip::tcp::socket s, ConnectionManager &manager, zia::api::Net::Callback &cb);
    ~Connection();
    void    start();
    void    stop();
    bool    doSend(const zia::api::Net::Raw& resp);
    const zia::api::ImplSocket    * const getImplSock() const;

private:
    void    doRead();
    void    generateNetInfo(zia::api::NetInfo &info);
};

typedef std::shared_ptr<Connection> connectionPtr;

#endif //CPP_ZIA_CONNECTION_HPP
