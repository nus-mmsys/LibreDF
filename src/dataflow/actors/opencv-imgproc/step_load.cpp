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

#include "step_load.h"

using namespace df;
using namespace std;

ActorRegister<StepLoad> StepLoad::reg("StepLoad");

StepLoad::StepLoad(const std::string & name): df::Actor(name) {
  input = createInputPort<df::Mat>("input");
  output = createOutputPort<df::Mat>("output");
}

void StepLoad::init() {
  if (!propEmpty("init_period"))
    prd = getPropInt("init_period");
  else
    prd = 30;

  if (!propEmpty("step1_period"))
    step1 = getPropInt("step1_period");
  else
    step1 = 60;

  if (!propEmpty("step2_period"))
    step2 = getPropInt("step2_period");
  else
    step2 = 120;

}

void StepLoad::reinit() {
}

void StepLoad::run() {
  auto in = consume(input);
  auto out = produce(output);
  start_exec = timer.nowUs();
  out->set(*in->get());
  log("step load "+to_string(stepno));
  timer.sleep(prd);

  if(iterno>110) prd = step1;
  if(iterno>200) prd = step2;

  exec_dur = timer.nowUs() - start_exec;
  release(input);
  release(output);
}

StepLoad::~StepLoad() {
  destroyPort(input);
  destroyPort(output);
}

