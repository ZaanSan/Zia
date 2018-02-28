#include "Php.hpp"

bool    Php::NotFound(zia::api::HttpDuplex &http)
{
    http.resp.status = zia::api::http::common_status::not_found;
    http.resp.headers.insert(std::make_pair("content-length", std::to_string(0)));
    http.resp.headers.insert(std::make_pair("content-type", "text/html"));
    http.resp.headers.insert(std::make_pair("server", "Zia 1.0"));
    return true;
}

#ifdef WIN32

std::string Php::ExecCmd(const char *cmd, zia::api::HttpDuplex &http)
{
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
  if (!pipe) throw std::runtime_error("popen() failed!");
  while (!feof(pipe.get()))
  {
      if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
          result += buffer.data();
  }
  if (result == ("Could not open input file: " + http.req.uri + "\n"))
    {
      NotFound(http);
      return ("");
    }
  return result;
}

#endif

#ifndef WIN32

std::string Php::ExecCmd(const char *cmd, zia::api::HttpDuplex &http)
{
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  if (!pipe) throw std::runtime_error("popen() failed!");
  while (!feof(pipe.get()))
  {
      if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
          result += buffer.data();
  }
  if (result == ("Could not open input file: " + http.req.uri + "\n"))
    {
      NotFound(http);
      return ("");
    }
  return result;
}

#endif

bool Php::CheckExtension(zia::api::HttpDuplex &http)
{
  std::size_t last_slash_pos =  http.req.uri.find_last_of("/");
  std::size_t last_dot_pos =  http.req.uri.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
      extension =  http.req.uri.substr(last_dot_pos + 1);
  else
    extension = "";
  return (extension == "php");
}

bool Php::FillBody(zia::api::HttpDuplex &http)
{
  if (!CheckExtension(http))
    return (true);
  int length;
  std::string line;
  std::string tmp = "php " + http.req.uri + " 2>&1";
  tmp = ExecCmd(tmp.c_str(), http);
  if (tmp.empty())
    return (true);
  std::istringstream   ret(tmp);

  http.resp.status = zia::api::http::common_status::ok;
  http.resp.body.clear();
  while (std::getline(ret, line))
  {
      line += "\n";
      length += line.size();
      std::transform(line.begin(), line.end(), std::back_inserter(http.resp.body),
                     [](unsigned char c) { return std::byte(c); });
  }
  http.resp.headers.insert(std::make_pair("content-type", "text/html"));
  http.resp.headers.insert(std::make_pair("content-length", std::to_string(length)));
  http.resp.headers.insert(std::make_pair("server", "Zia 1.0"));
  return (true);
}

bool Php::exec(zia::api::HttpDuplex &http)
{
    return FillBody(http);
}

bool Php::config(const zia::api::Conf &conf)
{
    return true;
}

#ifndef WIN32
extern "C"
{
zia::api::Module *create()
{
    return new Php();
}
};
#else
extern "C" {
zia::api::Module __declspec(dllexport) *create()
{
    return new Php();
}
};
#endif //ifndef WIN32

//GESTION ERREUR - Fichier Php ? - Pas de fichier ? - quoid d'autre ?
