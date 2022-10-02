#ifndef SINGLEPULSETIMER_H
#define SINGLEPULSETIMER_H

#include <ctime>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "Logger.h"

typedef std::function<void (void* obj, bool)> CBtype;

class SinglePulseTimer {
public:
  SinglePulseTimer();
  SinglePulseTimer(std::string& value) { setId(value); }
  SinglePulseTimer(CBtype callBack, void* obj, std::time_t interval);
  SinglePulseTimer(std::string& value, CBtype callBack, void* obj, std::time_t interval);
  ~SinglePulseTimer();

  // Activates timer for the specified time interval and call specified callback.
  void setSinglePulseTimer(CBtype callBack, void* obj, std::time_t interval);

public:
  void setId(std::string& value) { id = value; }
  const std::string& getId() { return id;  }
private:
  std::string id;

public:
  // use GetThreadObject() to access th object (needed for join / joinable par example)
  std::thread& GetThreadObject() { return th; }
private:
  std::thread th;
  void threadFunc(CBtype callBack, void* obj, std::time_t interval);

public:
  void SetStopTimer(bool what) { stopTimer = what; }
  bool GetStopTimer() { return stopTimer; }
private:
  bool stopTimer;

public:
  bool isExprired() { return expired; }
private:
  void SetExpired(bool what) { expired = what; }
  bool expired;

public:
  void PrematureFinish() { SetStopTimer(true); cv.notify_one(); };
private:
  std::condition_variable cv;
  std::mutex cv_m;
};

/////////////////////////////////

template <class T>
class SinglePulseTimerT {
public:
  SinglePulseTimerT(): stopTimer(false), expired(false) { Logger::LogMessage("SinglePulseTimer::SinglePulseTimer() Constructor executed"); };
  SinglePulseTimerT(std::string& value) { setId(value); }
  ~SinglePulseTimerT() { };

public:
  void setId(std::string& value) { id = value; }
  const std::string& getId() { return id;  }
private:
  std::string id;

public:
  //typedef std::function<void (T::*)(bool)> callBackType;
  using callBackType = void (T::*)(bool);
  // Activates timer for the specified time interval and call specified callback.
  void setSinglePulseTimer(T* obj, callBackType callBack, std::time_t interval)
  {
    th = std::thread(&SinglePulseTimerT::threadFunc, this, obj, callBack, interval);
  }

public:
  // use GetThreadObject() to access th object (needed for join / joinable par example)
  std::thread& GetThreadObject() { return th; }
private:
  std::thread th;
  void threadFunc(T* obj, callBackType callBack, std::time_t interval)
  {
    Logger::LogMessage("SinglePulseTimer<" + getId() + ">::threadFunc entered "+ getId());

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
      (obj->*callBack)(cond);
    }

    Logger::LogMessage("SinglePulseTimer<" + getId() + ">::threadFunc exited");
  }

public:
  void SetStopTimer(bool what) { stopTimer = what; }
  bool GetStopTimer() { return stopTimer; }
private:
  bool stopTimer;

public:
  bool isExprired() { return expired; }
private:
  void SetExpired(bool what) { expired = what; }
  bool expired;

public:
  void PrematureFinish() { SetStopTimer(true); cv.notify_one(); };
private:
  std::condition_variable cv;
  std::mutex cv_m;
};

#endif // SINGLEPULSETIMER_H
