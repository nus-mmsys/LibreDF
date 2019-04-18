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

#ifndef DF_INPUTPORT_VECTOR_H
#define DF_INPUTPORT_VECTOR_H

#include "input_port.h"

namespace df {
  
  /*!
   * \class InputPortVector
   *
   */
  
  template <typename T>
  class InputPortVector: public IPort {
    
  private:
    std::map<int, InputPort<T> *> inputs;
    std::vector<int> portNumbers;

  public:
	  
    InputPortVector<T>(std::string name) : IPort(name) {
        port_cap = std::string(typeid(T).name());
    }
    
    int arity() {
	    return inputs.size();
    }

    InputPort<T> * at(int i) {
	    return inputs[i];
    }

    void setArity(int r) {
	for (int i=0; i<r; i++) {
		InputPort<T> * in = new InputPort<T>(name+"."+std::to_string(i));
		inputs.insert(std::make_pair(i,in));
	}
    }

    void addArity(int r) {
	auto insize = inputs.size();
	for (int i=0; i<r; i++) {
		InputPort<T> * in = new InputPort<T>(name+"."+std::to_string(i+insize));
		inputs.insert(std::make_pair(i+insize, in));
	}
    }

    virtual void accept() {
	//Fix this for dynamic port creation.
	//Use getFreePort.
    	 while (!portNumbers.empty()) {
		sock->accept();
		int p = portNumbers.back();
    		portNumbers.pop_back();
		sock->recvsend("port", std::to_string(p));
		sock->clnclose();
	}
    }

    virtual void listen(int portnb) {
	distributed = true;
        sock->listen(portnb);

	int i = 1;
       	for (auto in : inputs) {
		int p = portnb + i;
		portNumbers.insert(portNumbers.begin(), p);
		in.second->listen(p);
	    	i++;
	}
    }

    virtual void startAccept() {
	    for (auto in : inputs)
		    in.second->startAccept();
            taccept = std::thread(&InputPortVector<T>::accept, this);
    }

    virtual void waitAccept() {
	    for (auto in : inputs)
		    in.second->waitAccept();
	    taccept.join();
    }

    InputPort<T> * getFreePort(int idx) {
	    InputPort<T> * ip = nullptr;
	    if (inputs.find(idx) != inputs.end())
		    ip = inputs[idx];
	    else {
		    ip = new InputPort<T>(name+"."+std::to_string(idx));
		    inputs.insert(std::make_pair(idx, ip));
	    }
	    increaseLinked();
	    return ip;
    }

    void setBuffer(BufferInfc * b, int inpidx, int i) {
	    getFreePort(inpidx)->setBuffer(b,inpidx,i);
    }

    void unsetBuffer(int inpidx) {
	    if (inputs.find(inpidx) != inputs.end()) {
		InputPort<T> * in = inputs[inpidx];
	    	in->unsetBuffer(inpidx);
		inputs.erase(inpidx);
		delete in;
	    }
    }

    virtual void clearBuffer() {
	//Not tested.
	for (auto c : inputs)
		c.second->clearBuffer();
    }

    virtual ~InputPortVector() {
    }
    
  };
  
}
#endif // DF_INPUTPORT_VECTOR_H
