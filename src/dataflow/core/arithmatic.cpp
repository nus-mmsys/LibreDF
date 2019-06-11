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

#include "arithmatic.h"

using namespace std;
using namespace df;

Arithmatic::Arithmatic() {
	nth[1] = 2;
	nth[2] = 3;
	nth[3] = 5;
}

long int Arithmatic::nThPrime(int n) {
	if (nth.find(n) == nth.end())
		return 0;
	else
		return nth[n];
}
