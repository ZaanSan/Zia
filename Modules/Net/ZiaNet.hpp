//
// Created by corentin on 21/02/18.
//

#ifndef CPP_ZIA_ZIANET_HPP
#define CPP_ZIA_ZIANET_HPP

#ifdef WIN32
# define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
# define _HAS_AUTO_PTR_ETC 1
#endif // ifdef WIN32

#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>

#include "../../api/net.h"
#include "ConnectionManager.hpp"

class ZiaNet : public zia::api::Net
{
private:
    boost::asio::io_service         _service;
    boost::asio::ip::tcp::acceptor  _acceptor;
    boost::asio::signal_set         _signals;
    ConnectionManager               _manager;
    boost::asio::ip::tcp::socket    _socket;
    Callback                        _cb;

public:
    ZiaNet();
    ~ZiaNet() override;
    bool config(const zia::api::Conf& conf) override;
    bool run(Callback cb) override;
    bool send(zia::api::ImplSocket *sock, const Raw& resp) override;
    bool stop() override;

private:
    void    doAccept();
    void    doAwaitStop();
};

#endif //CPP_ZIA_ZIANET_HPP
