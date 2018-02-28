//
// Created by corentin on 21/02/18.
//

#include "ZiaNet.hpp"

ZiaNet::ZiaNet()
        : _service(1), _acceptor(_service), _signals(_service), _socket(_service)
{
    _signals.add(SIGINT);
    _signals.add(SIGTERM);
#if defined(SIGQUIT)
    _signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
    doAwaitStop();
}

ZiaNet::~ZiaNet()
= default;

bool    ZiaNet::config(const zia::api::Conf &conf)
{
    auto it = conf.at("port");
    int port;

    port = std::get<long long>(it.v);
    std::cout << "Starting development server at http://127.0.0.1:" << port << "\nQuit the server with CONTROL-C." << std::endl;
    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
    boost::asio::ip::tcp::resolver resolver(_service);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), static_cast<unsigned short>(port));
    _acceptor.open(endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(endpoint);
    _acceptor.listen();
    return true;
}

bool    ZiaNet::run(Callback cb)
{
    boost::system::error_code ec;

    _cb = cb;
    doAccept();
    _service.run(ec);
    if (ec)
    {
        std::cout << "Can't run server : " << ec.message() << std::endl;
        return false;
    }
    return true;
}

bool    ZiaNet::send(zia::api::ImplSocket *sock, const Raw &resp)
{
    return _manager.getConnectionByImplSock(sock)->doSend(resp);
}

bool    ZiaNet::stop()
{
    _acceptor.close();
    _manager.stopAll();
    exit(0);
    return true;
}

void    ZiaNet::doAccept()
{
    //socket = new boost::asio::ip::tcp::socket(_service);
    _acceptor.async_accept(_socket, [this](boost::system::error_code ec)
             {
                 if (!_acceptor.is_open())
                 {
                     return;
                 }
                 if (!ec)
                 {
                     std::cout << "------- New Connection -------" << std::endl;
                     _manager.start(std::make_shared<Connection>(std::move(_socket), _manager, _cb));
                 }
                 else
                 {
                     std::cout << "Can't accept connection : " << ec.message() << std::endl;
                 }
                 doAccept();
             });
}


void    ZiaNet::doAwaitStop()
{
    _signals.async_wait(
            [this](boost::system::error_code ec, int signo)
            {
                stop();
            }
    );
}

#ifndef WIN32
extern "C"
{
zia::api::Net *create()
{
    return new ZiaNet();
}
};
#else
extern "C" {
zia::api::Net __declspec(dllexport) *create()
    {
        return new ZiaNet();
    }
}
#endif //ifndef WIN32
