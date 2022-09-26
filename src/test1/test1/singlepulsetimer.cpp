#include <iostream>
#include "singlepulsetimer.h"
#include "logger.h"

SinglePulseTimer::SinglePulseTimer()
{
  SetStopTimer(false);
  cb = nullptr;

  LOG_INFO("SinglePulseTimer::SinglePulseTimer Constructor executed");
}

SinglePulseTimer::~SinglePulseTimer()
{
  PrematureFinish();
  if (th.joinable()) {
    th.join();
  }

  LOG_INFO("SinglePulseTimer::~SinglePulseTimer() Descctructor executed");
}

void SinglePulseTimer::SetSinglePulseTimer(CBtype callBack, std::time_t interval)
{
   th = std::thread(&SinglePulseTimer::threadFunc,this, callBack, interval);
}

void SinglePulseTimer::threadFunc(CBtype callBack, std::time_t interval)
{
  LOG_INFO("SinglePulseTimer::threadFunc entered");

  std::unique_lock<std::mutex> lk(cv_m);

  auto end_time = std::chrono::steady_clock::now();
  auto before = end_time;
  end_time += std::chrono::seconds(interval);

  SetStopTimer(false);
  bool cond = cv.wait_until(lk, end_time, [this]{return GetStopTimer();});
  auto thistime = std::chrono::steady_clock::now();
  callBack(cond);
  SetStopTimer(false);

  LOG_INFO("SinglePulseTimer::threadFunc exited");
}
