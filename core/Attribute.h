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

#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

#include <map>

namespace tmf {
  
  /*!
   * \class Attribute
   * Attribute is a map containing the attribute name and the attribute value
   */
  
  class Attribute {
  private:
    std::map<std::string, std::string>  props; /**< The map containing the message (key, value). */
    
  public:
    /*!
     * Get the message value by passing the key
     *
     * \param key the key to retrieve the message value
     */
    std::string getProp(const std::string & key);
    
    /*!
     * Set the message of any type by key and value
     *
     * \param key the key of the message
     * \param val the value of the message
     *
     */
    template <typename T>
    void setProp(const std::string & key, const T& val) {
      std::string valstr = std::to_string(val);
      props.insert(std::make_pair(key, valstr));
    }
    
    /*!
     * Set the message of string type by key and value
     *
     * \param key the key of the message
     * \param val the string value of the message
     *
     */ 
    void setProp(const std::string & key, const std::string& val);
    
    /*!
     * Set the message of char sequence type by key and value
     *
     * \param key the key of the message
     * \param val the char sequence value of the message
     *
     */ 
    void setProp(const std::string & key, const char* val);
  };
  
}

#endif /* ATTRIBUTE_H_ */
