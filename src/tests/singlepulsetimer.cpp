#include <iostream>
#include "singlepulsetimer.h"
#include "Logger.h"

SinglePulseTimer::SinglePulseTimer():
  stopTimer(false),
  expired(false)
{
  LOG_INFO("SinglePulseTimer::SinglePulseTimer() Constructor executed");
}

SinglePulseTimer::SinglePulseTimer(CBtype callback, void* obj, std::time_t interval):
  stopTimer(false),
  expired(false)
{
  LOG_INFO("SinglePulseTimer::SinglePulseTimer(CBtype callback, void* obj, std::time_t interval) Constructor executed");
  setSinglePulseTimer(callback,obj,interval);
}


SinglePulseTimer::SinglePulseTimer(std::string& value, CBtype callback, void* obj, std::time_t interval)
{
  LOG_INFO("SinglePulseTimer::SinglePulseTimer(std::string& value, CBtype callback, void* obj, std::time_t interval) Constructor executed");
  setId(value);
  setSinglePulseTimer(callback,obj,interval);
}

SinglePulseTimer::~SinglePulseTimer()
{
  PrematureFinish();
  if (th.joinable()) {
    th.join();
  }

  LOG_INFO("SinglePulseTimer::~SinglePulseTimer() Destructor executed");
}

void SinglePulseTimer::setSinglePulseTimer(CBtype callback, void* obj, std::time_t interval)
{
  th = std::thread(mem_fn(&SinglePulseTimer::threadFunc), this, callback, obj, interval);
}

void SinglePulseTimer::threadFunc(CBtype callback, void* obj, std::time_t interval)
{
  LOG_INFO("SinglePulseTimer<" + getId() + ">::threadFunc entered "+ getId());

  std::unique_lock<std::mutex> lk(cv_m);

  auto end_time = std::chrono::steady_clock::now();
  end_time += std::chrono::seconds(interval);

  setStopTimer(false);
  setExpired(false);
  bool cond = cv.wait_until(lk, end_time, [this]{return getStopTimer();});

  setStopTimer(false);

  // if callbck function is supplied, call it now.
  // if not supplied, use isExpired() to see how the timer has exited
  setExpired(!cond);
  if (callback) {
    callback(obj,cond);
  }

  LOG_INFO("SinglePulseTimer<" + getId() + ">::threadFunc exited");
}

///////////////////





