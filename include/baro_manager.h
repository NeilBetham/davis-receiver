/**
 * @brief Responsible for reading pressure from the barometer and submitting to reading reporter
 */

#pragma once

#include "timer.h"
#include "reading_reporter.h"

class BaroManager {
public:
  BaroManager(ReadingReporter& reading_reporter);
  ~BaroManager();

  void timer_event();

private:
  ReadingReporter& _reading_reporter;
  Timer<BaroManager, &BaroManager::timer_event> _timer;
  bool _baro_inited = false;
};
