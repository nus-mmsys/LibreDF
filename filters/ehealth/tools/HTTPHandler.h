/*
 * 
 *  Tiny Multimedia Framework (tmf)
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This file is part of tmf.
 *
 *  tmf is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  tmf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with tmf.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <curl/curl.h>
#include <string>
#include <iostream>

class HTTPHandler
{
private:
  std::string url;
  CURL *curl;
  CURLcode res;
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

public:
  HTTPHandler();
  void setHost(std::string url);
  std::string sendHTTP(std::string message);
  ~HTTPHandler();
};

#endif // HTTPHANDLER_H
