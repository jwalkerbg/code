#ifndef SINGLEPULSETIMER_H
#define SINGLEPULSETIMER_H

#include <ctime>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

typedef std::function<void (bool)> CBtype;

class SinglePulseTimer {
public:
  SinglePulseTimer();
  SinglePulseTimer(CBtype callBack, std::time_t interval);
  ~SinglePulseTimer();

  // Activates timer for the specified time interval and call specified callback.
  void SetSinglePulseTimer(CBtype callBack, std::time_t interval);

private:
  // call back function to be called at the end of timer interval or if prematurely stopped
  void (*cb)(void);

public:
  // use GetThreadObject() to access th object (needed for join / joinable par example)
  std::thread& GetThreadObject() { return th; }
private:
  std::thread th;
  void threadFunc(CBtype callBack, std::time_t interval);

public:
  void SetStopTimer(bool what) { stopTimer = what; }
  bool GetStopTimer() { return stopTimer; }
private:
  bool stopTimer;

public:
  void PrematureFinish() { SetStopTimer(true); cv.notify_one(); };
private:
  std::condition_variable cv;
  std::mutex cv_m;
};

#endif // SINGLEPULSETIMER_H
