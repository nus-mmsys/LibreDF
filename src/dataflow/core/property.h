/*
 *   LibreDF
 *   Copyright (C) A. Shafiei
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DF_PROPERTY_H_
#define DF_PROPERTY_H_

#include <iostream>
#include <string>
#include <map>

namespace df {
  
  /*!
   * \class Property
   * Property is a map containing the attribute name and the attribute value
   */
  
  class Property {

  private:
    std::map<std::string, std::string>  props; /**< The map containing the message (key, value). */
    
  public:

    /*!
     * Get list of all properties.
     *
     */
    std::map<std::string, std::string> getProps();
    
    /*!
     * Get the message value by passing the key
     *
     * \param key the key to retrieve the message value
     */
    std::string getProp(const std::string & key);

    /*!
     * Get the message key by passing the value
     *
     * \param val the value
     *
     */
    std::string getKey(const std::string & val);
 
    /*!
     * Get the message value (int) by passing the key
     *
     * \param key the key to retrieve the message value
     */
    int getPropInt(const std::string & key);
 
    /*!
     * Get the message value (bool) by passing the key
     *
     * \param key the key to retrieve the message value
     */
    bool getPropBool(const std::string & key);

    /*!
     * Get the message value (float) by passing the key
     *
     * \param key the key to retrieve the message value
     */
    float getPropFloat(const std::string & key);
 
    /*!
     * Searched for a key
     *
     * \param key the key to retrieve the message value
     *
     * \return 
     * 	true if the key is found and false otherwise
     */
    bool propEmpty(const std::string & key);
     
    /*!
     * Set the message of any type by key and value
     *
     * \param key the key of the message
     * \param val the value of the message
     *
     */
    template <typename T>
    void setProp(const std::string & key, const T & val) {
      std::string valstr = std::to_string(val);
      props.insert(std::make_pair(key, valstr));
    }
    
    /*!
     * Replace the value of the key if key exists and 
     * insert otherwise.
     *
     * \param key the key of the message
     * \param val the value of the message
     *
     */
    template <typename T>
    void replaceProp(const std::string & key, const T & val) {
      props.erase(key);
      setProp<T>(key, val);
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
     * Replace the value of the key if it exists and insert otherwise.
     *
     * \param key the key of the message
     * \param val the string value of the message
     *
     */ 
    void replaceProp(const std::string & key, const std::string& val);

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

#endif /* DF_PROPERTY_H_ */
