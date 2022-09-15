/*
 *   LibreDF
 *   Copyright (C) A. Shafiei
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

#include "arithmetic.h"

using namespace std;
using namespace df;

Arithmetic::Arithmetic() {
	int j = 1;
	for (int i=2; i<=257; i++) {
		if (isPrime(i)) {
			nth[j] = i;
			j++;
		}	
	}
}

bool Arithmetic::isPrime(int n) {
	if (n==2) return true;
	if (n%2==0) return false;

	for (int i=3; i<= sqrt(n); i=i+2) {
		if (n%i==0)
			return false;
	}
	return true;
}

int Arithmetic::factor(int n) {
	for (int i=floor(sqrt(n)); i>0; i--) {
		if (n%i==0)
			return i;
	}
	return 1;
}

long int Arithmetic::nThPrime(int n) {
	if (nth.find(n) == nth.end())
		return 0;
	else
		return nth[n];
}
