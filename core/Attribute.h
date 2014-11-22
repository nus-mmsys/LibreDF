/*
 * 
 *  Tiny Multimedia Framework
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

#include <map>

class Attribute {
private:
  std::map<std::string, std::string>  props; /**< The map containing the message (key, value). */
  
public:
  /*!
   * Get the message value by passing the key
   *
   * \param key the key to retrieve the message value
   */
  std::string getProp(const std::string & key) {
    auto k = props.find(key);
    
    if (k == props.end())
      return "";
    
    return props[key];
  }
  
  /*!
   * Set the string message by key and value
   *
   * \param key the key of the message
   * \param val the string value of the message
   *
   */
  template <typename T>
  void setProp(const std::string & key, const T& val) {
    std::string valstr = std::to_string(val);
    props.insert(std::make_pair(key, valstr));
  }
  
  void setProp(const std::string & key, const std::string& val) {
    props.insert(std::make_pair(key, val));
  }

  void setProp(const std::string & key, const char* val) {
    std::string valstr = std::string(val);
    props.insert(std::make_pair(key, valstr));
  }
};

#endif /* ATTRIBUTE_H_ */
