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

#include <iostream>
#include <string>
#include "parser/parser.h"
#include "dataflow/core/df.h"

using namespace std;

int main(int argc, char * argv[]) {

    if (argc != 2) {
	cout << "usage: " << argv[0] << " <file.tmf>\n";
	exit(0);
    }
    std::chrono::high_resolution_clock::time_point start, end; 			
    Parser * parser = new Parser();
    parser->load_from_file(argv[1]);
    Graph * g = parser->get_graph();

    start = std::chrono::high_resolution_clock::now(); 
    vector<vector<string>> paths = g->hamiltonians();
    end = std::chrono::high_resolution_clock::now();
    for (auto path : paths) {
    	for (auto actor : path)
    		cout << actor << " ";
	cout << "\n";	
    }
    std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()  << " us\n"; 
    return 0;
}
