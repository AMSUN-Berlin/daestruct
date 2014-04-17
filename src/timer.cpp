/*
 * Copyright (C) 2014 uebb.tu-berlin.de.
 *
 * This file is part of daestruct
 *
 * daestruct is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * daestruct is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with daestruct. If not, see <http://www.gnu.org/licenses/>.
 */

#include "daestruct/timer.h"
#include <boost/timer/timer.hpp>

extern "C" {

  struct daestruct_timer : public boost::timer::auto_cpu_timer {};

  struct daestruct_timer* daestruct_timer_new() {
    return new daestruct_timer();
  }

  void daestruct_timer_delete(struct daestruct_timer* t) {
    delete t;
  }
  
  void daestruct_timer_start(struct daestruct_timer* t) {
    t->start();
  }

  void daestruct_timer_stop(struct daestruct_timer* t) {
    t->stop();
  }

  void daestruct_timer_resume(struct daestruct_timer* t) {
    t->resume();
  }

  void daestruct_timer_report(struct daestruct_timer* t) {
    t->report();
  }
}
