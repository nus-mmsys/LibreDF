/*
 *   libdataflow
 *   Copyright (C) 2018
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

#ifndef DF_OUTPUTPORT_VECTOR_H
#define DF_OUTPUTPORT_VECTOR_H

#include "output_port.h"

namespace df {
  
  /*!
   * \class OutputPortVector
   *
   */
  
  template <typename T>
  class OutputPortVector : public OPort {
    
  private:
    std::vector<OutputPort<T> *> outputs;

  public:
    
    OutputPortVector<T>(const std::string & name) : OPort(name) {
    }

    int arity() {
	    return outputs.size();
    }

    OutputPort<T> * at(int i) {
	    return outputs[i];
    }

    void setArity(int r) {
	for (int i=0; i<r; i++) {
		OutputPort<T> * out = new OutputPort<T>(name+"."+std::to_string(i));
		outputs.push_back(out);
	}
    }

    virtual int connectPort(std::string host, int portnb) { 
    	for (auto out : outputs) {
		if (out->getLinked() < 1) {
			out->connectPort(host, portnb);
			break;
		}
	}
	return 0;
    }

    virtual int connectPort(IPort* n) {
	//TODO
	return 0;
    }

    virtual int connectPort(IPort* n, int index) {
	//TODO
	return 0;
    }
    
    virtual ~OutputPortVector<T>() {
    }

  };
  
}
#endif // DF_OUTPUTPORT_VECTOR_H
