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

#include "edge.h"

using namespace df;

Edge::Edge() {
	name = "";
	visited = false;
	source = nullptr;
	sink = nullptr;
}

Edge::Edge(std::string edgename) : Edge() {
	name = edgename;
}

std::string Edge::getName() {
	return name;
}

void Edge::setSource(Actor * src) {
	source = src;
}
void Edge::setSink(Actor * snk) {
	sink = snk;
}
void Edge::setVisited(bool v) {
	visited = v;
}
bool Edge::getVisited() {
	return visited;
}
Actor * Edge::getSourceActor() {
	return source;
}
Actor * Edge::getSinkActor() {
	return sink;
}
void Edge::setSourceRate(int r) {
	src_rate = r;
}
void Edge::setSinkRate(int r) {
	snk_rate = r;
}
int Edge::getSourceRate() {
	return src_rate;
}
int Edge::getSinkRate() {
	return snk_rate;
}
