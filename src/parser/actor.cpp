/*
 *   TMF
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
}

Port::Port(string portname) : Port() {
	name = portname;
}

void Port::set_rate(int r) {
	rate = r;
}

int Port::get_rate() {
	return rate;
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
	return iport;
}

Port * Actor::create_oport() {
	Port * oport = new Port();
	oports.push_back(oport);
	return oport;
}

int Actor::iport_size() {
	return iports.size();
}

int Actor::oport_size() {
	return oports.size();
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

