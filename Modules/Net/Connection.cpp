//
// Created by corentin on 21/02/18.
//

#include "Connection.hpp"

Connection::Connection(boost::asio::ip::tcp::socket s, ConnectionManager &manager, zia::api::Net::Callback &cb)
        : _manager(manager), _sock(std::move(s)), _callback(cb)
{
    _implSock = new zia::api::ImplSocket(_sock);
    for (auto &it : _buffer)
        it = 0;
}

Connection::~Connection()
{
    if (_implSock)
        delete(_implSock);
}

void Connection::start() {
    doRead();
}

void Connection::stop() {
    _sock.close();
}

bool Connection::doSend(const zia::api::Net::Raw& resp)
{
    auto self(shared_from_this());
    _sock.async_write_some(boost::asio::buffer(resp),
                           [this, self](boost::system::error_code ec, std::size_t)
                           {
                               if (!ec)
                               {
                                   //TODO parsing keepAlive => Don't close sock and call doRead

                                   // Initiate graceful connection closure.
                                   boost::system::error_code ignored_ec;
                                   _sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                                                  ignored_ec);
                                   std::cout << "-------- Disconnection ------" << std::endl;
                               }

                               if (ec != boost::asio::error::operation_aborted)
                               {
                                   _manager.stop(shared_from_this());
                               }
                           });

    return true;
}

void Connection::doRead()
{
    auto self = shared_from_this();
    _sock.async_read_some(
            boost::asio::buffer(_buffer),
            [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
            {
                if (!ec)
                {
                    //TODO => Parsing requete (Good => Callback, Bad => Nothing, Else => read (incomplete request)

                    zia::api::Net::Raw      data;
                    zia::api::NetInfo       info;

                    generateNetInfo(info);
                    std::transform(_buffer.begin(), _buffer.begin() + bytes_transferred, std::back_inserter(data),
                                   [](char c) {
                                       return static_cast<std::byte >(c);
                                   });
                    _callback(data, info);
                    for (unsigned int i = 0; i < bytes_transferred; i++)
                        _buffer[i] = 0;
                }
                else if (ec != boost::asio::error::operation_aborted)
                {
                    _manager.stop(shared_from_this());
                }
            });
}

void Connection::generateNetInfo(zia::api::NetInfo &info)
{
    zia::api::NetIp ip;

    ip.str = _sock.remote_endpoint().address().to_string();
    info.sock = _implSock;
    info.ip = ip;
    info.port = _sock.remote_endpoint().port();
}

const zia::api::ImplSocket *const Connection::getImplSock() const {
    return _implSock;
}
