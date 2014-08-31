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

#include "Pipeline.h"

Pipeline::Pipeline(const string& name) {
	this->name = name;
	this->start = 0;
	this->status = PIPELINE_STOPPED;
}

Pipeline::~Pipeline() {

	for (set<Filter*>::iterator it = filters.begin(); it != filters.end(); ++it)
		 delete *it;

}

void Pipeline::connectFilters(Filter * inf, Filter * outf) {

	filters.insert(inf);
	filters.insert(outf);

	if (this->start == 0 && inf->inputPortNum() == 0)
		this->start = inf;

	if (this->start == 0 && outf->inputPortNum() == 0)
		this->start = outf;

	inf->connectFilter(outf);
}

//void Pipeline::setStarter(Filter *starter) {
//	this->start = starter;
//}



PipelineStatus Pipeline::init() {

	FilterStatus ret;
	if (start == NULL) {
		cerr << "Pipeline does not have enough filters to run.\n";
		return PIPELINE_STOPPED;
	}

	ret = start->initFilter(0);

	if (ret == FILTER_ERROR) {
		cerr << "Pipeline cannot initialize a filter.\n";
		return PIPELINE_STOPPED;
	}
	/*for (set<Filter*>::iterator it = filters.begin(); it != filters.end(); ++it) {

		ret = (*it)->initFilter(0);

		if (ret == FILTER_ERROR) {
			cerr << "Pipeline cannot initialize a filter.\n";
			return PIPELINE_STOPPED;
		}
	}
	*/
	return PIPELINE_RUNNING;

}

PipelineStatus Pipeline::run() {

	FilterStatus status;
	while(1) {

		status = start->executeFilter();

		switch(status) {
		case FILTER_SUCCESS:
		case FILTER_WAIT_FOR_INPUT:
			continue;
		case FILTER_ERROR:
			return PIPELINE_STOPPED;
		case FILTER_FINISHED:
			return PIPELINE_FINISHED;

		}

	}

	return PIPELINE_STATE_UNKNOWN;

}

