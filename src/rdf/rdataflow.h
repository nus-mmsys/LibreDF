/*
 *   RDF
 *   Copyright (C) INRIA - Orange Labs
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

#ifndef RDATAFLOW_H
#define RDATAFLOW_H

#include <iostream>
#include "dataflow/core/df.h"
#include "rule.h"
#include "rdfgraph.h"

/*!
 * \class RDataflow
 * The class for RDF run time.
 *
 */
class RDataflow : public df::Dataflow {
private:
    int load;
    RDFGraph * rdfg;
    Rule * get_applicable_rule();
    void reconfigure();
public:

    RDataflow(std::string name);

    void log(std::string msg);

    void set_graph(RDFGraph * r);

    /*!
     * Run the RDF.
     *
     */
    virtual void run();

};

#endif
