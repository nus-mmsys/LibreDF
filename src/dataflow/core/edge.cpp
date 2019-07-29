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

#include "edge.h"

using namespace df;

Edge::Edge() {
	name = "";
	visited = false;
	src_actor = nullptr;
	snk_actor = nullptr;
	src_port = "";
	snk_port = "";
	src_rate = 1;
	snk_rate = 1;
	init_tokens = 0;
	tokens = 0;
}

Edge::Edge(std::string edgename) : Edge() {
	name = edgename;
}

std::string Edge::getName() {
	return name;
}

void Edge::setSource(Actor * src) {
	src_actor = src;
}

void Edge::setSink(Actor * snk) {
	snk_actor = snk;
}

void Edge::setInPort(IPort * ip) {
	iport = ip;
}

void Edge::setOutPort(OPort * op) {
	oport = op;
}

void Edge::setVisited(bool v) {
	visited = v;
}
bool Edge::getVisited() {
	return visited;
}
Actor * Edge::getSource() {
	return src_actor;
}
Actor * Edge::getSink() {
	return snk_actor;
}
IPort * Edge::getInPort() {
	return iport;
}
OPort * Edge::getOutPort() {
	return oport;
}
void Edge::setSourceRate(int r) {
	src_rate = r;
}
void Edge::setSinkRate(int r) {
	snk_rate = r;
}

std::string Edge::readPortName(std::string p, int & idx) {
  std::string res;
  auto bs = p.find('.');
  if (bs != std::string::npos) {
        res = p.substr(0,bs);
	try {
		idx = stoi(p.substr(bs+1,p.size()-bs-1));
	} catch(...) {
		std::cerr << "cannot parse port name "+p << std::endl;
		idx = -1;
	}
  } else {
  	res = p;
	idx = -1;
  }
  return res;
}

void Edge::setSourcePort(std::string p) {
  if (p!="")
  	src_port = readPortName(p, src_port_idx);
}

void Edge::setSinkPort(std::string p) {
  if (p!="")
      	snk_port = readPortName(p, snk_port_idx);
}

std::string Edge::getSourcePort() {
	return src_port;
}
std::string Edge::getSinkPort() {
	return snk_port;
}

int Edge::getSourcePortIndex() {
	return src_port_idx;
}
int Edge::getSinkPortIndex() {
	return snk_port_idx;
}

void Edge::setSourcePortIndex(int idx) {
	src_port_idx = idx;
}
void Edge::setSinkPortIndex(int idx) {
	snk_port_idx = idx;
}

int Edge::getSourceRate() {
	return src_rate;
}
int Edge::getSinkRate() {
	return snk_rate;
}
void Edge::setInitialTokens(int tokens) {
	init_tokens = tokens;
}
int Edge::getInitialTokens() {
	return init_tokens;
}
void Edge::setTokens(int t) {
	tokens = t;
}
int Edge::getTokens() {
	return tokens;
}
