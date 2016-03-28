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

#include "ConfigurationManager.h"

using namespace std;

ConfigurationManager::ConfigurationManager(const string& json, const string& mode)
{
  std::string content;
  if (mode == "file") {
    std::ifstream ifs(json);
    content.assign((std::istreambuf_iterator<char>(ifs)),
                       (std::istreambuf_iterator<char>()));
  }
  else if (mode == "json") {
    content = json;
  } else {
    cout << "Mode is not defined." << endl;
    return;
  }

  content.erase(std::remove_if(content.begin(), content.end(),
    [](char c) { return c == '{' || c == '}' || c == '\n' || c == ' ' || c == '\"'; } ),
    content.end());

  std::string line;
  std::stringstream ss(content);
  while( std::getline(ss, line, ',') )
  {
    std::istringstream is_line(line);
    std::string key;
    if( std::getline(is_line, key, ':') )
    {   
      std::string value;
      if( std::getline(is_line, value) ) 
	data[key] = value;
    }
  }
}

string ConfigurationManager::getValue(const string& key)
{
  auto it = data.find(key);
  if (it!=data.end())
    return data[key];

  cout << key + " is not found in the configuration file " << endl;
  return "";
}

ConfigurationManager::~ConfigurationManager()
{

}
