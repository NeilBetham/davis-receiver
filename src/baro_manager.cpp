#include "baro_manager.h"

#include "baro.h"
#include "logging.h"

BaroManager::BaroManager(ReadingReporter& reading_reporter) : _reading_reporter(reading_reporter), _timer(*this, 5000, true) {
  _timer.start();
}

BaroManager::~BaroManager() {
  systick_delete_delegate(&_timer);
}

void BaroManager::timer_event() {
  if(!_baro_inited) { baro::init_barometer(); _baro_inited = true; }

  ReadingReport baro_reading;
  baro_reading.type = ReadingType::barometer;
  if(baro::read_pressure(baro_reading.value)) {
    _reading_reporter.handle_report(baro_reading);
    log_i("Barometer Reading: {} kPa", baro_reading.value);
  } else {
    log_w("Failed to read sensor data from barometer");
  }
}
