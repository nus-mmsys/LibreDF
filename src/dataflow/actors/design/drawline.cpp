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

#include "drawline.h"

using namespace df;
using namespace std;

ActorRegister<DrawLine> DrawLine::reg("DrawLine");

DrawLine::DrawLine(const string& name) : Actor(name) {
  input = createInputPort<df::Complex3D>("input");
  output = createOutputPort<df::Mat>("output");

}

void DrawLine::init() {
  
}

void DrawLine::run() {

  Complex3D * in = consume(input);
  Mat * out = produce(output);

  log("producing "+to_string(stepno));
  
  release(output);  
  release(input);
}


DrawLine::~DrawLine() {
  destroyPort(output);
}
