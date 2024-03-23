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
  std::thread& getThreadObject() { return th; }
private:
  std::thread th;
  void threadFunc(CBtype callback, void* obj, std::time_t interval);

public:
  void setStopTimer(bool what) { stopTimer = what; }
  bool getStopTimer() { return stopTimer; }
private:
  bool stopTimer;

public:
  bool isExprired() { return expired; }
  void clearExpired() { expired = false; }
private:
  void setExpired(bool what) { expired = what; }
  bool expired;

public:
  void PrematureFinish() { setStopTimer(true); cv.notify_one(); };
private:
  std::condition_variable cv;
  std::mutex cv_m;
};

/////////////////////////////////

template <class T>
class SinglePulseTimerT {
public:
  SinglePulseTimerT(): stopTimer(false), expired(false) {
    Logger::LogMessage("SinglePulseTimerT::SinglePulseTimerT() Constructor executed");
  };
  SinglePulseTimerT(std::string& value): stopTimer(false), expired(false) {
    Logger::LogMessage("SinglePulseTimerT::SinglePulseTimerT() Constructor executed"); setId(value);
    setId(value);
  }
  SinglePulseTimerT(T* obj, CBtype callback, std::time_t interval): stopTimer(false), expired(false) {
    LOG_INFO("SinglePulseTimerT::SinglePulseTimer(T* obj, CBtype callBack, std::time_t interval) Constructor executed");
    setSinglePulseTimer(obj,callback,interval);
  }
  SinglePulseTimerT(std::string& value, T* obj, CBtype callBack, std::time_t interval): stopTimer(false), expired(false) {
    LOG_INFO("SinglePulseTimerT::SinglePulseTimer(std::string& value, T* obj, CBtype callBack, std::time_t interval) Constructor executed");
    setId(value);
    setSinglePulseTimer(obj,callBack,interval);
  }
  ~SinglePulseTimerT() { LOG_INFO("SinglePulseTimerT::~SinglePulseTimerT() Destructor executed"); };

public:
  void setId(std::string& value) { id = value; }
  const std::string& getId() { return id;  }
private:
  std::string id;

public:
  using callbackType = void (T::*)(bool);
  // Activates timer for the specified time interval and call specified callback.
  void setSinglePulseTimer(T* obj, callbackType callBack, std::time_t interval)
  {
    th = std::thread(&SinglePulseTimerT::threadFunc, this, obj, callBack, interval);
  }

public:
  // use GetThreadObject() to access th object (needed for join / joinable par example)
  std::thread& getThreadObject() { return th; }
private:
  std::thread th;
  void threadFunc(T* obj, callbackType callback, std::time_t interval)
  {
    Logger::LogMessage("SinglePulseTimerT<" + getId() + ">::threadFunc entered "+ getId());

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

    if ((nullptr != obj) && (nullptr != callback)) {
      (obj->*callback)(cond);
    }

    Logger::LogMessage("SinglePulseTimerT<" + getId() + ">::threadFunc exited");
  }

public:
  void setStopTimer(bool what) { stopTimer = what; }
  bool getStopTimer() { return stopTimer; }
private:
  bool stopTimer;

public:
  bool isExprired() { return expired; }
  void clearExpired() { expired = false; }
private:
  void setExpired(bool what) { expired = what; }
  bool expired;

public:
  void prematureFinish() { setStopTimer(true); cv.notify_one(); };
private:
  std::condition_variable cv;
  std::mutex cv_m;
};

#endif // SINGLEPULSETIMER_H
