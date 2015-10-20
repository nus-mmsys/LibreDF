/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2015  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "ConfigurationManager.h"

using namespace std;

ConfigurationManager::ConfigurationManager(const string& fname) : fileName(fname)
{
  cfile.open(fname, ios::in);
  std::string line;
  while( std::getline(cfile, line) )
  {
    std::istringstream is_line(line);
    std::string key;
    if( std::getline(is_line, key, '=') )
    {
      std::string value;
      if( std::getline(is_line, value) ) 
	data[key] = value;
    }
  }
  cfile.close();
}

string ConfigurationManager::getValue(const string& key)
{
  auto it = data.find(key);
  if (it!=data.end())
    return data[key];

  cout << key + " is not found in the configuration file " << fileName << endl;
  return "";
}

ConfigurationManager::~ConfigurationManager()
{

}
