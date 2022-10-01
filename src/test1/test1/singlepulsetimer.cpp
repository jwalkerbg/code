#include <iostream>
#include "singlepulsetimer.h"
#include "logger.h"

SinglePulseTimer::SinglePulseTimer():
  stopTimer(false),
  expired(false)
{
  LOG_INFO("SinglePulseTimer::SinglePulseTimer() Constructor executed");
}

SinglePulseTimer::SinglePulseTimer(CBtype callBack, void* obj, std::time_t interval):
  stopTimer(false),
  expired(false)
{
  LOG_INFO("SinglePulseTimer::SinglePulseTimer(CBtype callBack, void* obj, std::time_t interval) Constructor executed");
  setSinglePulseTimer(callBack,obj,interval);
}


SinglePulseTimer::SinglePulseTimer(std::string& value, CBtype callBack, void* obj, std::time_t interval)
{
  LOG_INFO("SinglePulseTimer::SinglePulseTimer(std::string& value, CBtype callBack, void* obj, std::time_t interval) Constructor executed");
  setId(value);
  setSinglePulseTimer(callBack,obj,interval);
}

SinglePulseTimer::~SinglePulseTimer()
{
  PrematureFinish();
  if (th.joinable()) {
    th.join();
  }

  LOG_INFO("SinglePulseTimer::~SinglePulseTimer() Destructor executed");
}

void SinglePulseTimer::setSinglePulseTimer(CBtype callBack, void* obj, std::time_t interval)
{
   th = std::thread(&SinglePulseTimer::threadFunc, this, callBack, obj, interval);
}

void SinglePulseTimer::threadFunc(CBtype callBack, void* obj, std::time_t interval)
{
  LOG_INFO("SinglePulseTimer::threadFunc entered");

  std::unique_lock<std::mutex> lk(cv_m);

  auto end_time = std::chrono::steady_clock::now();
  end_time += std::chrono::seconds(interval);

  SetStopTimer(false);
  SetExpired(false);
  bool cond = cv.wait_until(lk, end_time, [this]{return GetStopTimer();});

  SetStopTimer(false);

  // if callbck function is supplied, call it now.
  // if not supplied, use isExpired() to see how the timer has exited
  SetExpired(!cond);
  if (callBack) {
    callBack(obj,cond);
  }

  LOG_INFO("SinglePulseTimer::threadFunc exited");
}
