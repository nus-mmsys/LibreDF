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

#ifndef DF_PARSER_ACTOR_H
#define DF_PARSER_ACTOR_H

#include <string>
#include <vector>
#include <map>

using namespace std;

/*!
 * Status enumeration class
 *  The status value to return in functions.
 */
enum class Status {
	OK,
	ERROR,
	END_OF_FILE,
};

/*!
 * Actor class
 *  An actor has a number of input and output ports.
 *
 */
class Actor {
private:
	string name; /**< Actor name */
	string type; /**< Actor type */
	bool visited; /**< Boolean to indicate if actor is visited for dfs algorithm */
	int firing; /**< Number of firings (solution) of the actor */
	int num; /**< Numinator of the fractional form of the firing of the actor */
	int den; /**< Denuminator of the fractional form of the firing of the actor */
	std::map<std::string, std::string> properties; /**< List of properties of actors */

public:
	/*!
	 * Actor constructor
	 *
	 */
	Actor();
	
	/* 
	 * Actor constructor with specified name and type
	 *
	 * \param actorname
	 * 	Name of the actor to create.
	 *
	 * \param actortype
	 * 	Type of the actor to create.
	 *
	 */
	Actor(string actorname, string actortype);
	
	/*!
	 * Set the name of the actor
	 *
	 * \param actorname
	 * 	The name of the actor to set
	 *
	 */
	void set_name(string actorname);
        
	/*!
	 * Set the type of the actor
	 *
	 * \param actortype
	 * 	The type of the actor to set
	 *
	 */
	void set_type(string actortype);

	/*!
	 * Get the name of the actor
	 *
	 * \return
	 * 	The name of the actor
	 *
	 */
	string get_name();

	/*!
	 * Get the type of the actor
	 *
	 * \return
	 * 	The type of the actor
	 *
	 */
	string get_type();

	/*!
	 * Set the number of firings of the actor
	 *
	 * \param firingcount
	 * 	The number of firing of the actor to set
	 *
	 */
	void set_firing(int firingcount);

	/*!
	 * Set the numinator and denuminator of the fractional number of firingis of the actor
	 *
	 * \param num
	 * 	The numinator
	 *
	 * \param den
	 * 	The denuminator
	 *
	 */
	void set_numden(int num, int den);

	/*!
	 * Get the number of firings of the actor
	 *
	 * \return
	 * 	The number of firing of the actor
	 *
	 */
	int get_firing();

	/*!
	 * Get the numinator of the fractional number of firingis of the actor
	 *
	 * \return
	 * 	The numinator
	 *
	 */
	int get_num();

	/*!
	 * Get the denuminator of the fractional number of firingis of the actor
	 *
	 * \return
	 * 	The denuminator
	 *
	 */
	int get_den();

	/*!
	 * Set the boolean value of visited.
	 *
	 * \param v
	 * 	The value of visited to set
	 *
	 */
	void set_visited(bool v);

	/*!
	 * Get the boolean value of visited.
	 *
	 * \return
	 * 	The value of visited
	 *
	 */
	bool get_visited();

	/*!
	 * Set a property of the actor
	 *
	 * \param p
	 * 	property name to set
	 * \param v
	 * 	property value to set
	 */
	void set_prop(const std::string & p, const std::string & v);

	/*!
	 * Get a property of the actor
	 *
	 * \param p
	 * 	property name to get
	 * \return
	 * 	property value
	 */
	std::string get_prop(const std::string & p);
	
	/*!
	 * Get a property key of the actor
	 *
	 * \param val
	 * 	property value
	 * \return
	 * 	property key
	 */
	std::string get_prop_key(const std::string & val);

	/*!
	 * Get the map of all properties
	 *
	 * \return
	 * 	map of all properties
	 */
	std::map<std::string, std::string> get_props();

};

/*!
 * Schedule class
 *  A schedule contains a number of subschedule.
 *  A subschedule can be also an actor.
 *
 */
class Schedule {

protected:
	int rep; /**< Number of repetition of the schedule. */
private:
	vector<Schedule *> children; /**< Subschedules of the schedule. */
public:
	/*!
	 * Add subschedules to the schedule.
	 *
	 * \param order
	 * 	List of references to the actors.
	 *
	 */
	void insert_order(vector<Actor *> order);

	/*!
	 * Returns the schedule in string format.
	 *
	 * \return
	 * 	Schedule in string format.
	 *
	 */
	virtual string to_string(); 
};

/*!
 * SchedulesActor class
 *  It is a subschedule of an schedule which is an actor.
 *
 */
class ScheduledActor : public Schedule {
private:
	Actor * actor; /**< Referenced actor. */
public:
	/*!
	 * Constructor
	 *
	 * \param ac
	 * 	Referenced actori.
	 *
	 */
	ScheduledActor(Actor * ac);
	
	/*!
	 * Returns the scheduled actor in string format.
	 *
	 * \return
	 * 	Scheduled actor in string format.
	 *
	 */
	virtual string to_string();
};

#endif
