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

std::string Edge::get_name() {
	return name;
}

void Edge::set_source(Actor * src) {
	source = src;
}
void Edge::set_sink(Actor * snk) {
	sink = snk;
}
void Edge::set_visited(bool v) {
	visited = v;
}
bool Edge::get_visited() {
	return visited;
}
Actor * Edge::get_source_actor() {
	return source;
}
Actor * Edge::get_sink_actor() {
	return sink;
}
void Edge::set_source_rate(int r) {
	src_rate = r;
}
void Edge::set_sink_rate(int r) {
	snk_rate = r;
}
int Edge::get_source_rate() {
	return src_rate;
}
int Edge::get_sink_rate() {
	return snk_rate;
}

