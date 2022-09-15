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

#ifndef DF_OUTPUTPORT_VECTOR_H
#define DF_OUTPUTPORT_VECTOR_H

#include "output_port.h"
#include <stack>

namespace df {
  
  /*!
   * \class OutputPortVector
   *
   */
  
  template <typename T>
  class OutputPortVector : public OPort {
    
  private:
    std::vector<OutputPort<T> *> outputs;
    std::stack<int> idx_stack;

  public:
    
    OutputPortVector<T>(const std::string & name) : OPort(name) {
        port_cap = std::string(typeid(T).name());
    }

    int arity() {
	    return outputs.size();
    }

    OutputPort<T> * at(int i) {
	    return outputs[i];
    }

    OutputPort<T> * getFreePort(int & outpidx) {
 	OutputPort<T> * op = nullptr;
	if (outpidx >=0 && outpidx < outputs.size()) {
		op = outputs[outpidx];
	} else {
		if (idx_stack.empty())
			outpidx = outputs.size();
		else {
			outpidx = idx_stack.top();
			idx_stack.pop();
		}
		op = new OutputPort<T>(name+"."+std::to_string(outpidx));
		outputs.push_back(op);
	}
	increaseLinked();
	return op;
    }

    virtual void setBufferSize(int s) {
	for (auto out : outputs) {
		out->setBufferSize(s);
	}
    }

    virtual int connectPort(std::string host, int portnb) { 
	int idx = -1;
	return getFreePort(idx)->connectPort(host, portnb);
    }

    virtual int connectPort(IPort* n) {
	int idx = -1;
	return getFreePort(idx)->connectPort(n);
    }

    virtual int disconnectPort(IPort* n, int outpidx, int inpidx) {
     	int idx = outpidx;
	
	if (outputs.size()==0)
		return -1;

	if (outpidx < 0)
		idx = outputs.size()-1;

	OutputPort<T> * op = nullptr;
	auto it = outputs.begin();
	for (; it!= outputs.end(); it++) {
		if ((*it)->getName() == name+"."+std::to_string(idx)) {
			op = *it;
			break;
		}
	}
	if (op != nullptr) {
 		n->unsetBuffer(inpidx);
		outputs.erase(it);
 		decreaseLinked();
 		delete op;
	}
	idx_stack.push(idx);
	return 0;
    }

    virtual int connectPort(IPort* n, int & outpidx, int & inpidx) {
	auto p = getFreePort(outpidx);
	int idx = outpidx;
	int res = p->connectPort(n,outpidx,inpidx);
	outpidx = idx;
	return res;
    }
    
    virtual int getOccupancy(int idx) {
	OutputPort<T> * out = nullptr;
	auto it = outputs.begin();
 	for (; it != outputs.end(); it++) {
	    if ((*it)->getName() == name+"."+std::to_string(idx)) {
	    	out = *it;
		break;
	    }
	}
	return out->getOccupancy(0);
    }

    virtual ~OutputPortVector<T>() {
    }

  };
  
}
#endif // DF_OUTPUTPORT_VECTOR_H
