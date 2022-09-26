#ifndef SINGLEPULSETIMER_H
#define SINGLEPULSETIMER_H

#include <ctime>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

typedef std::function<void (void* obj, bool)> CBtype;

class SinglePulseTimer {
public:
  SinglePulseTimer();
  SinglePulseTimer(CBtype callBack, void* obj, std::time_t interval);
  ~SinglePulseTimer();

  // Activates timer for the specified time interval and call specified callback.
  void setSinglePulseTimer(CBtype callBack, void* obj, std::time_t interval);

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

#endif // SINGLEPULSETIMER_H
