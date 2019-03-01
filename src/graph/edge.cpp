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

Rate::Rate(string s, int v) {
	symbol = s;
	val = v;
}
Rate::Rate(int v) : Rate("", v) { }
Rate::Rate() : Rate("", 1) { }

void Rate::set_symbol(string s) {
	symbol = s;
}
string Rate::get_symbol() {
	return symbol;
}
void Rate::set_value(int v) {
	val = v;
}
int Rate::get_value() {
	return val;
}

Edge::Edge() {
	name = "";
	visited = false;
	src_port = "";
	snk_port = "";
	src_actor = nullptr;
	snk_actor = nullptr;
	src_rate.set_value(1);
	snk_rate.set_value(1);
	init_tokens = 0;
	tokens = 0;
}

Edge::Edge(string edgename) : Edge() {
	name = edgename;
}

string Edge::get_name() {
	return name;
}

void Edge::set_source_port(string src) {
	src_port = src;
}

void Edge::set_sink_port(string snk) {
	snk_port = snk;
}

string Edge::get_source_port() {
	return src_port;
}

string Edge::get_sink_port() {
	return snk_port;
}

void Edge::set_visited(bool v) {
	visited = v;
}
bool Edge::get_visited() {
	return visited;
}

Actor * Edge::get_source_actor() {
	return src_actor;
}

Actor * Edge::get_sink_actor() {
	return snk_actor;
}

void Edge::set_source_actor(Actor * src) {
	src_actor = src;
}

void Edge::set_sink_actor(Actor * snk) {
	snk_actor = snk;
}

void Edge::set_source_rate(int r) {
	src_rate.set_value(r);
}

void Edge::set_source_param_rate(string r) {
	src_rate.set_symbol(r);
}

void Edge::set_sink_rate(int r) {
	snk_rate.set_value(r);
}

void Edge::set_sink_param_rate(string r) {
	snk_rate.set_symbol(r);
}

int Edge::get_source_rate() {
	return src_rate.get_value();
}

int Edge::get_sink_rate() {
	return snk_rate.get_value();
}

void Edge::set_init_tokens(int tokens) {
	init_tokens = tokens;
}

int Edge::get_init_tokens() {
	return init_tokens;
}

void Edge::set_tokens(int t) {
	tokens = t;
}

int Edge::get_tokens() {
	return tokens;
}
