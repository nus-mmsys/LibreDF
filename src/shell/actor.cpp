/*
 *   TDF
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

#include "actor.h"

Port::Port() {
	name = "";
	rate = 1;
	actor = nullptr;
	edge = nullptr;
}

Port::Port(string portname) : Port() {
	name = portname;
}

void Port::set_actor(Actor * ac) {
	actor = ac;
}

void Port::set_edge(Edge * ed) {
	edge = ed;
}

Actor * Port::get_actor() {
	return actor;
}

Edge * Port::get_edge() {
	return edge;
}

void Port::set_rate(int r) {
	rate = r;
}

int Port::get_rate() {
	return rate;
}

Edge::Edge() {
	name = "";
	visited = false;
	source = nullptr;
	sink = nullptr;
}

Edge::Edge(string edgename) : Edge() {
	name = edgename;
}

string Edge::get_name() {
	return name;
}

void Edge::connect_source(Port * sourceport) {
	source = sourceport;
	source->set_edge(this);
}
void Edge::connect_sink(Port * sinkport) {
	sink = sinkport;
	sink->set_edge(this);
}
void Edge::set_visited(bool v) {
	visited = v;
}
bool Edge::get_visited() {
	return visited;
}
Actor * Edge::get_source_actor() {
	return source->get_actor();
}
Actor * Edge::get_sink_actor() {
	return sink->get_actor();
}
void Edge::set_source_rate(int r) {
	source->set_rate(r);
}
void Edge::set_sink_rate(int r) {
	sink->set_rate(r);
}
int Edge::get_source_rate() {
	return source->get_rate();
}
int Edge::get_sink_rate() {
	return sink->get_rate();
}
Actor::Actor() {
	name = "";
	type = "";
	firing = 0;
	num = 0;
	den = 1;
}
Actor::Actor(string actorname, string actortype) : Actor() {
	name = actorname;
	type = actortype;
}
void Actor::set_name(string actorname) {
	name = actorname;
}
void Actor::set_type(string actortype) {
	type = actortype;
}
string Actor::get_name() {
	return name;
}
string Actor::get_type() {
	return type;
}
void Actor::set_firing(int firingcount) {
	firing = firingcount;
	num = firingcount;
	den = 1;
}

int Actor::get_firing() {
	return firing;
}

void Actor::set_numden(int numcount, int dencount) {
	num = numcount;
	den = dencount;
}

int Actor::get_num() {
	return num;
}

int Actor::get_den() {
	return den;
}

void Actor::set_visited(bool v) {
	visited = v;
}

bool Actor::get_visited() {
	return visited;
}

Port * Actor::create_iport() {
	Port * iport = new Port();
	iports.push_back(iport);
	iport->set_actor(this);
	return iport;
}

Port * Actor::create_oport() {
	Port * oport = new Port();
	oports.push_back(oport);
	oport->set_actor(this);
	return oport;
}
vector<Edge *> Actor::get_iedges() {
	vector<Edge *> iedges;
	for (auto ip : iports)
		iedges.push_back(ip->get_edge());
	return iedges;
}
vector<Edge *> Actor::get_oedges() {
	vector<Edge *> oedges;
	for (auto op : oports)
		oedges.push_back(op->get_edge());
	return oedges;
}

void Actor::set_prop(const string & p, const string & v) {
	properties.insert(std::make_pair(p,v));
}

string Actor::get_prop(const string & p) {
	auto key = properties.find(p);
  
  	if (key == properties.end())
    		return "";
  
  	return properties[p];

}

map<string, string> Actor::get_props() {
	return properties;
}

void Schedule::insert_order(vector<Actor *> order) 
{
	for (auto o : order) {
		Schedule * sac = new ScheduledActor(o);
		children.push_back(sac);
	}
}

string Schedule::to_string()
{
	string sstr;
	for (auto c : children) {
		sstr = sstr + " " + c->to_string();
	}
	sstr.erase(0,1);
	return sstr;
}

string ScheduledActor::to_string() {
	return actor->get_name() + "^" + std::to_string(actor->get_firing());
}

ScheduledActor::ScheduledActor(Actor * ac) { 
	actor = ac; 
}

