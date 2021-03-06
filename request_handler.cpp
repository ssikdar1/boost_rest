//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"

#include <iostream>

#include <jsoncpp/json/json.h>

namespace http {
namespace server {

request_handler::request_handler(const std::string& doc_root)
  : doc_root_(doc_root)
{
}

/***
Christopher M. Kohlhoff's original server functionality to serve
files as func
***/
void request_handler::serve_file(const request& req, reply& rep)
{
  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.uri, request_path))
  {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  // Determine the file extension.
  std::size_t last_slash_pos = request_path.find_last_of("/");
  std::size_t last_dot_pos = request_path.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
  {
    extension = request_path.substr(last_dot_pos + 1);
  }

  // Open the file to send back.
  std::string full_path = doc_root_ + request_path;
  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!is)
  {
    rep = reply::stock_reply(reply::not_found);
    return;
  }

  // Fill out the reply to be sent to the client.
  rep.status = reply::ok;
  char buf[512];
  while (is.read(buf, sizeof(buf)).gcount() > 0)
    rep.content.append(buf, is.gcount());
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = std::to_string(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = mime_types::extension_to_type(extension);
}



/**
SHAN
This is probably the function where we need to add out handler magic
**/
void request_handler::handle_request(const request& req, reply& rep)
{
  std::cout << "request_handler::handle_request() called " << std::endl;
  std::cout << req.uri << std::endl;
  std::cout << req.method << std::endl;

  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.uri, request_path))
  {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  std::cout << "request_path: " << request_path << std::endl;


  //request_handler::serve_file(req, rep);
  //TODO what is the best way to dispatch uri's here?
  if(req.uri == "/v0/foo")
  {
    std::cout << "PRINT OMG" << std::endl;

    // Fill out the reply to be sent to the client.
    rep.status = reply::ok;

    Json::Value body(Json::objectValue);
    body["platform"] = "linux";
    body["deviceId"] = "623bce38-a1f4-11e6-bb6c-3417eb9985a6";
    body["version"] = "17.2.3";
   
    //better way to do this?
    Json::FastWriter fastWriter;
    rep.content = fastWriter.write(body);

    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = std::to_string(rep.content.size());
  }
  else if(req.uri == "/v0/bar")
  {
    std::cout << "PRINT OMG" << std::endl;

    // Fill out the reply to be sent to the client.
    rep.status = reply::bad_request;

    Json::Value body(Json::objectValue);
   
    //better way to do this?
    Json::FastWriter fastWriter;
    rep.content = fastWriter.write(body);

    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = std::to_string(rep.content.size());
  }
  else 
  {}
	

  std::cout << "response code: " << rep.status  << std::endl;
  std::cout << "response body: " << rep.content  << std::endl;

}

bool request_handler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} // namespace server
} // namespace http
