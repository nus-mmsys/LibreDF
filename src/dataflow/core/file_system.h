/*
 *   TMF
 *   Copyright (C) TMF Team
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

#ifndef DF_FILE_SYSTEM_H_
#define DF_FILE_SYSTEM_H_

#include <string>

namespace df {
	
  class FileSystem {

    private:
      std::string home_path; /**< The path for home folder */    
      std::string df_path; /**< The path for actors to use */
      std::string dfout_path; /**< The path for actors to use as output */
    
    public:
      FileSystem();
      std::string inPath();
      std::string outPath();
  };

}

#endif /* DF_FILE_SYSTEM_H_ */
