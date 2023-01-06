#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <ctime>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <string.h>
#include <limits.h>
#include <regex>

#include "singlepulsetimer.h"
#include "Logger.h"

using namespace std;

//#define USE_STATIC_MEMBER_FUNC
//#define USE_NEW_DELETE
//#define USE_STR_TO_INT
//#define USE_REGEX
//#define USE_STRINGS
//#define USE_VECTOR_OF_STRINGS
//#define USE_SEARCH_FOR_SET_OF_STRINGS
//#define USE_STOI
//#define USE_SPT
//#define USE_SPT_CLASS
//#define USE_TIME_FUNCTIONS
//#define USE_TIME_INCREMENT
//#define USE_CLASS_CONS_DEST
//#define USE_SPT_TEMPLATIZED_CLASS
//#define USE_GET_LOCAL_AND_UTC_TIME
//#define USE_DYNAMIC_POINTER_CAST
//#define USE_TEST_sendCorrectedEndTime
//#define USE_SYSTEM_CALL
//#define USE_TEST_STRING_COMPARE
#define USE_CATCH_OUTPUT

#if defined(USE_STATIC_MEMBER_FUNC)
class CMyClass
{
public:
    CMyClass() { SetMember(10); }
    int m_Member;
    void SetMember(int Member) { m_Member = Member; }
    int GetMember() { return m_Member; }

    static char Greeting[100];

};

char CMyClass::Greeting[100] = "I am static member.";

#endif  // defined(USE_STATIC_MEMBER_FUNC)

#if defined(USE_SEARCH_FOR_SET_OF_STRINGS)

typedef std::pair<std::string::iterator,int> SubstringPosition;

int FindSubstring(const std::string& str, std::string substr[]);

#endif  // defined(USE_SEARCH_FOR_SET_OF_STRINGS)

#if defined(USE_SPT)

// maximum time to stay in active mode [minutes]
#define BH_MAX_ACTIVE_TIME (1*10)
// Guard timer time = (BH_MAX_BAKHOOR_TIME * 1.1)
#define BH_GUARD_TIMER_TIME ((BH_MAX_ACTIVE_TIME*110)/100)

class WithTimer {
public:
  WithTimer();
  ~WithTimer();

  // guard timer if UI crashes to reset states active and sensors
    void setGuardTimer();
    void setGuardTimer(std::function<void (void)>callBack, std::time_t bhInterval);
    void guardTimerThread(std::function<void (void)> callBack, std::time_t bhInterval);
    std::thread worker;
    std::condition_variable cv;
    std::mutex sgtm;
    bool GuardTimer;

    // callback function when the guard timer expires
    void reset();
    void stopper();
};

#endif  // defined(USE_SPT)

#if defined(USE_SPT_CLASS)

typedef std::function<void (bool how)> TCallback;

class SptUser {
public:
  SptUser() {};
  ~SptUser() {};

  // static class wrapper around object callback function
  static void cbWrapper(void* obj, bool how);
  // the real object callback function
  void callback(bool how);
  static std::mutex cblock;
};

std::mutex SptUser::cblock;

void SptUser::cbWrapper(void* obj, bool how)
{
  std::unique_lock<std::mutex> lk(cblock);
  std::stringstream str;
  str << "SptUser::cbWrapper(void* obj, bool how) " << how;
  LOG_INFO(str.str());

  SptUser* thisobject = static_cast<SptUser*>(obj);
  if (thisobject) {
    thisobject->callback(how);
  }
  else {
    // do the work here
  }

}

void SptUser::callback(bool how)
{
  std::stringstream str;
  str << "SptUser::objectFunc() " << "The real work is done here, because object context is known." << std::endl;
  str << "********** The timer " << (how ? "exited prematurely" : "exipred") << ".";
  LOG_INFO(str.str());
}

#endif  // defined(USE_SPT_CLASS)

#if defined(USE_TIME_INCREMENT)
void TimeIncrement();
#endif  // defined(USE_TIME_INCREMENT)
#if defined(USE_CLASS_CONS_DEST)
void class_cons_dest();
#endif  // defined(USE_CLASS_CONS_DEST)

#if defined(USE_SPT_TEMPLATIZED_CLASS)
void use_spt_templatized();
#endif  // defined(USE_SPT_TEMPLATIZED_CLASS)

#if defined(USE_GET_LOCAL_AND_UTC_TIME)
void GetLocalAndUTCTime();
#endif  // defined(USE_GET_LOCAL_AND_UTC_TIME)

#if defined(USE_DYNAMIC_POINTER_CAST)
void DynamicCast();
#endif  // defined(USE_DYNAMIC_POINTER_CAST)

#if defined(USE_TEST_sendCorrectedEndTime)
void Test_sendCorrectedEndTime();
#endif  // defined(USE_TEST_sendCorrectedEndTime)

#if defined(USE_SYSTEM_CALL)
void Test_systemCall();
#endif  // defined(USE_SYSTEM_CALL)

#if defined(USE_TEST_STRING_COMPARE)
void Test_StringCompare();
#endif // defined(USE_TEST_STRING_COMPARE)

#if defined(USE_CATCH_OUTPUT)
void Test_CatchOutput();
#endif // defined(USE_CATCH_OUTPUT)

int main()
{

#if defined(USE_STATIC_MEMBER_FUNC)
    CMyClass ob;

    cout << "From object " << ob.Greeting << endl;
    cout << "From class  " << CMyClass::Greeting << endl;
    cout << "From object " << ob.GetMember() << endl;
    //cout << "From class " << CMyClass::GetMember() << endl; // illegal
#endif // defined(USE_STATIC_MEMBER_FUNC)

#if defined(USE_NEW_DELETE)
    char * p = new char[100];
    char * q = nullptr;

    strcpy(p,"Hello, I stored data here");

    cout << p << endl;

    delete[] p;
#endif  // defined(USE_NEW_DELETE)

#if defined(USE_STR_TO_INT)
    char ptr0[] = { "1235" };
    int ival = 0;

    char* ptr = &ptr0[0];
    while (*ptr != '\0') {
        if (*ptr < '0' && *ptr > '9') {
            break;
        } else {
            ival = ival * 10 + *ptr++ - '0';
        }
    }

    cout << "ival = " << ival << endl;
#endif // defined(USE_STR_TO_INT)

#if defined(USE_REGEX)
// regex

    std::string s = "Some people, when confronted with a problem, think "
        "\"I know, I'll use regular expressions.\" "
        "Now they have two problems.";

    std::regex self_regex("REGULAR EXPRESSIONS",
            std::regex_constants::ECMAScript | std::regex_constants::icase);
    if (std::regex_search(s, self_regex)) {
        std::cout << "Text contains the phrase 'regular expressions'\n";
    }

    std::regex word_regex("\\w+");
    auto words_begin =
        std::sregex_iterator(s.begin(), s.end(), word_regex);
    auto words_end = std::sregex_iterator();

    std::cout << "Found "
              << std::distance(words_begin, words_end)
              << " words\n";

    const int N = 6;
    std::cout << "Words longer than " << N << " characters:\n";
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        if (match_str.size() > N) {
            std::cout << "  " << match_str << '\n';
        }
    }

    std::regex long_word_regex("(\\w{7,})");
    std::string new_s = std::regex_replace(s, long_word_regex, "[$&]");
    std::cout << new_s << '\n';

    std::regex AddDelGetSet(R"XYZ(^(SET|GET|DEL|ADD){1}\/)XYZ");
    std::string topic = "SET/PanelAPI/Version";
    std::smatch Operation_match;

    if (std::regex_search(topic,Operation_match,AddDelGetSet)) {
      std::cout << "Prefix: '" << Operation_match.prefix() << "'\n";
      for (size_t i = 0; i < Operation_match.size(); ++i)
          std::cout << i << ": " << Operation_match[i] << '\n';
      std::cout << "Suffix: '" << Operation_match.suffix() << "\'\n\n";
    }


#endif  // defined(USE_REGEX)

#if defined(USE_SIZEOF)
    void *p = NULL;

//#pragma pack(1)
    struct xx {
        int a;
    };
    struct xx xxa[10];
#pragma pack()
#define sizeofA(arr) (sizeof(arr)/sizeof(arr[0]))

    std::cout << "sizeof(void *) = " << sizeof(p) << std::endl;
    std::cout << sizeof(5) << std::endl;
    std::cout << sizeof(xxa) << std::endl;
    std::cout << sizeofA(xxa) << std::endl;

#endif  // defined(USE_SIZEOF)

#if defined(USE_STRINGS)

    std::string str;

    str = "";

    std::cout << "str.size() of \"\" = " << str.size() << std::endl;

#endif  // defined(USE_STRINGS)

#if defined(USE_VECTOR_OF_STRINGS)

    const std::string s1 = "s1 string";
    const std::string s2 = "s2 string";
    const std::string s3 = "s3 string";

    const std::vector<std::string>v1 = { s1, s2, s3 };

    // with for cycle
    for (const std::string &s : v1) {
        std::cout << s << ", ";
    }
    std::cout << std::endl;

    std::cout << "indexed access" << std::endl;

    for (int i = 0; i < v1.size(); ++i) {
        std::cout << v1[i] << std::endl;
    }

#endif  // defined(USE_VECTOR_OF_STRINGS)

#if defined(USE_SEARCH_FOR_SET_OF_STRINGS)
    std::string str = "blala blala @Smoke xfcgd @HeatSensor dfgd";
    std::string substr[] = { "@HeatSensor", "@Smoke", "" };

    int ix = FindSubstring(str,substr);
    std::cout << "This string has it: " << ix << ": " << substr[ix] << std::endl;

#endif  // defined(USE_SEARCH_FOR_SET_OF_STRINGS)

#if defined(USE_STOI)

    int value = -1;

    const char* pstr = "123456";
    std::string str;
    str = pstr;
    str[3] = '7';
    std::cout << "pstr = " << pstr << std::endl;
    try {
      value = stoi(str);
    }
    catch(std::invalid_argument ex) {
      std::cout << "Exception = " << ex.what() << std::endl;
      value = 0;
    }
    catch (std::out_of_range ex) {
      std::cout << "Exception = " << ex.what() << std::endl;
      if ('-' == str[0]) {
        value = INT_MIN;
      }
      else {
        value = INT_MAX;
      }
    }

    std::cout << "value = " << value << std::endl;

#endif  // defined(USE_STOI)

#if defined(USE_SPT)
    WithTimer bh;

    //bh.cv.notify_one();
    //bh.cv.notify_one();
    //bh.cv.notify_one();
    //bh.cv.notify_one();

    LOG_INFO("before bh.setGuardTimer();");
    bh.setGuardTimer();
    LOG_INFO("after bh.setGuardTimer();");

    // comment or uncomment next line to see how the timer waits its full time or is
    // terminated after about 3 seconds (the time stopper() sleep before notifying)
    std::thread th1(&WithTimer::stopper,&bh);

    //LOG_INFO("main is joined to th1");
    if (th1.joinable()) th1.join();
    LOG_INFO("main is joined to bh.worker");
    if (bh.worker.joinable()) bh.worker.join();

    LOG_INFO("Activate bh timer again");

    bh.setGuardTimer();
    LOG_INFO("main is joined to bh.worker");
    if (bh.worker.joinable()) bh.worker.join();

    LOG_INFO("main continues to \"return 0\"");

#endif  // defined(USE_SPT)

#if defined(USE_SPT_CLASS)

    SptUser sptu;
    SptUser sptv;

    // start via constructor
    std::string sputut = "sptut";
    std::string sputvt = "sptvt";
    SinglePulseTimer sptut(sputut, SptUser::cbWrapper,&sptu,7);
    SinglePulseTimer sptvt(sputvt, SptUser::cbWrapper,&sptv,7);

    // postponed start via method
    // SinglePulseTimer sptut;
    // sptut.setSinglePulseTimer(SptUser::cbWrapper,&sptu,7);

    // comment following two lines to see timeouted exit
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    // sptut.prematureFinish();

    if (sptut.getThreadObject().joinable()) {
      sptut.getThreadObject().join();
    }
    if (sptvt.getThreadObject().joinable()) {
      sptvt.getThreadObject().join();
    }

    {
      std::stringstream str;
      str << "main(): Polling sptut timer: It " << (sptut.isExprired() ? "expired" : "exited prematurely") << ".";
      LOG_INFO(str.str());
    }
    {
      std::stringstream str;
      str << "main(): Polling sptvt timer: It " << (sptvt.isExprired() ? "expired" : "exited prematurely") << ".";
      LOG_INFO(str.str());
    }

    sptut.setSinglePulseTimer(SptUser::cbWrapper,&sptu,5);

    std::this_thread::sleep_for(std::chrono::seconds(8));

    if (sptut.getThreadObject().joinable()) {
      sptut.getThreadObject().join();
    }

    sptut.setSinglePulseTimer(SptUser::cbWrapper,&sptu,5);

    if (sptut.getThreadObject().joinable()) {
      sptut.getThreadObject().join();
    }

    Logger::LogMessage("************ Templatized timer examples *******************");

    std::string sptutTid = "sptutT";
    SinglePulseTimerT<class SptUser> sptutT;
    sptutT.setId(sptutTid);

    sptutT.setSinglePulseTimer(&sptu,&sptu.callback,5);

    if (sptutT.getThreadObject().joinable()) {
      sptutT.getThreadObject().join();
    }
#endif  // defined(USE_SPT_CLASS)

#if defined(USE_TIME_FUNCTIONS)

    time_t tmnow;
    tmnow = std::time(nullptr);

    struct tm lf = *std::gmtime(&tmnow);

    {
      std::stringstream str;
      str << "lf.tm_hour: " << lf.tm_hour << std::endl
          << "lf.tm_min: " << lf.tm_min << std::endl
          << "lf.tm_sec: " << lf.tm_sec << std::endl;
      LOG_INFO(str.str());
    }

    std::tm local = *std::localtime(&tmnow);

    {
      std::stringstream str;
      str
          << "local - 03::30: " << tmnow - (3*60*60+30*60) << std::endl
          << "local: " << tmnow << std::endl
          << "local.tm_hour: " << local.tm_hour << std::endl
          << "local.tm_min: " << local.tm_min << std::endl
          << "local.tm_sec: " << local.tm_sec << std::endl;
      LOG_INFO(str.str());
    }

    std::tm bkStart{};
    bkStart.tm_hour = 5;
    bkStart.tm_min = 0;
    bkStart.tm_sec = 0;

    std::tm bkEnd{};
    bkEnd.tm_hour = 23;
    bkEnd.tm_min = 59;
    bkEnd.tm_sec = 59;

    // check to see if local is in [bkStart .. bkEnd]
    int bkStart_i = bkStart.tm_hour * (60*60) + bkStart.tm_min * 60 + bkStart.tm_sec;
    int bkEnd_i = bkEnd.tm_hour * (60*60) + bkEnd.tm_min * 60 + bkEnd.tm_sec;
    int local_i = local.tm_hour * (60*60) + local.tm_min * 60 + local.tm_sec;

    if ((local_i >= bkStart_i) && (local_i <= bkEnd_i)) {
      LOG_INFO("We are in!");
    }
    else {
      LOG_INFO("We are out!");
    }

// lets test std:tm --> std::time_t

    std::tm tms;
    tms.tm_hour = 2;
    tms.tm_min = 0;
    tms.tm_sec = 0;
    tms.tm_mday = 5;
    tms.tm_mon = 2; // March
    tms.tm_year = 2022 - 1900;
    tms.tm_isdst = 0; // no daylight - standard time

    std::time_t tm_local = mktime(&tms);
    std::tm gmt = *std::gmtime(&tm_local);
    std::tm lct = *std::localtime(&tm_local);

    std::time_t gmt_calculated = mktime(&gmt);

    std::cout << "gmt_calculated: " << gmt_calculated << " tmv_2_local: " << tm_local << std::endl;

//    std::cout << "UTC:  " << std::put_time(std::gmtime(&tmv_1_t), "%c %Z") << std::endl;
//    std::cout << "UTC:  " << std::put_time(std::localtime(&tmv_1_t), "%c %Z") << std::endl;

#endif  // defined(USE_TIME_FUNCTIONS)

#if defined(USE_TIME_INCREMENT)

    TimeIncrement();

#endif  // defined(USE_TIME_INCREMENT)

#if defined(USE_CLASS_CONS_DEST)

  class_cons_dest();

#endif  // defined(USE_CLASS_CONS_DEST)

#if defined(USE_SPT_TEMPLATIZED_CLASS)

  use_spt_templatized();

#endif  // defined(USE_SPT_TEMPLATIZED_CLASS)

#if defined(USE_GET_LOCAL_AND_UTC_TIME)
  GetLocalAndUTCTime();
#endif  // defined(USE_GET_LOCAL_AND_UTC_TIME)

#if defined(USE_DYNAMIC_POINTER_CAST)
  DynamicCast();
#endif  // defined(USE_DYNAMIC_POINTER_CAST)

#if defined(USE_TEST_sendCorrectedEndTime)
  Test_sendCorrectedEndTime();
#endif  // defined(USE_TEST_sendCorrectedEndTime)

#if defined(USE_SYSTEM_CALL)
  Test_systemCall();
#endif  // defined(USE_SYSTEM_CALL)

#if defined(USE_TEST_STRING_COMPARE)
  Test_StringCompare();
#endif // defined(USE_TEST_STRING_COMPARE)

#if defined(USE_CATCH_OUTPUT)
  Test_CatchOutput();
#endif // defined(USE_CATCH_OUTPUT)

  std::cout << "Stop" << std::endl;

  return 0;
}

#if defined(USE_SEARCH_FOR_SET_OF_STRINGS)

int FindSubstring(const std::string& str, std::string substr[])
{
  int r = -1;
  for (int i = 0; substr[i].empty() == false; ++i) {
    auto it = std::search(str.begin(), str.end(), substr[i].begin(), substr[i].end());
    if (it != str.end()) {
      r = i; //index of found key
      break;
    }
  }
  return r;
}

#endif  // defined(USE_SEARCH_FOR_SET_OF_STRINGS)

#if defined(USE_SPT)

WithTimer::WithTimer() :
  stopGuardTimer(false)
{
  LOG_INFO("WithTimer::WithTimer() Constructor executed");
}

WithTimer::~WithTimer()
{
  // Tell the timer to exit
  {
    std::lock_guard<std::mutex> lk(sgtm);
    stopGuardTimer = true;
  }
  cv.notify_one();

  LOG_INFO("WithTimer: Waiting Timer thread to exit...");
  if (worker.joinable()) {
    worker.join();
  }

  LOG_INFO("WithTimer::~WithTimer() Destructor executed");
}

// guard timer if UI crashes to reset states
void WithTimer::reset()
{
  LOG_INFO("WithTimer::reset() executed");
}

// This function activates guard timer. This timer will unconditionally call reset function
// to return to idle state
void WithTimer::setGuardTimer()
{
  setGuardTimer(std::bind(&WithTimer::reset, this),BH_GUARD_TIMER_TIME);
}

void WithTimer::setGuardTimer(std::function<void(void)>callBack, std::time_t bhInterval)
{
  worker = std::thread(&WithTimer::guardTimerThread,this, callBack, bhInterval);
  // some people use here detach. we will try to not use it.
}

void WithTimer::guardTimerThread(std::function<void(void)> callBack, std::time_t bhInterval)
{
  LOG_INFO("-t " "@/MTEST/GUARD:" "-m " "{\"$tag\":\"Guard Timer thread started\"}");
  LOG_INFO(bhInterval);

  std::unique_lock<std::mutex> lk(sgtm);

  auto end_time = std::chrono::steady_clock::now();
  auto before = end_time;
  end_time += chrono::seconds(bhInterval);
  auto ff = chrono::seconds(bhInterval);
  LOG_INFO(ff.count());

  while (!stopGuardTimer) {
    //if (!cv.wait_until(lk, end_time, [this]{return stopGuardTimer;})) {
    bool cond = cv.wait_until(lk, end_time, [this]{return stopGuardTimer;});
    auto thistime = std::chrono::steady_clock::now();
    if (cond || (thistime >= end_time)) {
      callBack();
      stopGuardTimer = false;
      break;
    }
  }

  auto after = std::chrono::steady_clock::now();
  auto diff = after - before;
  LOG_INFO(diff.count());
  LOG_INFO("-t" "@/MTEST/GUARD" "-m" "{\"$tag\":\"Guard Timer thread ended\"}");
}

void WithTimer::stopper()
{
  LOG_INFO("WithTimer::stopper() entered")
  std::this_thread::sleep_for(std::chrono::seconds(4));

  LOG_INFO("WithTimer::stopper() unlocks");
  // Tell the timer to exit
  {
    std::lock_guard<std::mutex> lk(sgtm);
    stopGuardTimer = true;
  }
  LOG_INFO("Multiple cv.notify()");
  cv.notify_one();
  cv.notify_one();
  cv.notify_one();
  cv.notify_one();
  LOG_INFO("WithTimer::stopper() exited");
}

///////////////////

#endif  // defined(USE_SPT)

#if defined(USE_TIME_INCREMENT)

// Bukhoor maximum active time
#define BUKHOOR_MIN_ACTIVE_TIME (5*60)
#define BUKHOOR_MAX_ACTIVE_TIME (15*60)
#define BUKHOOR_INCREASE_STEP (5*60)
void TimeIncrement()
{
  std::time_t bhInterval = 15 * 60; // seconds

  auto start_time = std::chrono::steady_clock::now();
  auto end_time = start_time + std::chrono::seconds(bhInterval);

  std::cout << "Old Bukhoor time is " << std::chrono::duration_cast<std::chrono::seconds>(end_time-start_time).count() << std::endl;

  this_thread::sleep_for(std::chrono::seconds(3));

  auto this_time = std::chrono::steady_clock::now();
  auto remaining_time = end_time - this_time;
  std::cout << "remaining_time is " << std::chrono::duration_cast<std::chrono::seconds>(remaining_time).count() << std::endl;
  auto added_time = std::chrono::seconds(BUKHOOR_MAX_ACTIVE_TIME) - remaining_time;

  std::cout << "added time is " << std::chrono::duration_cast<std::chrono::seconds>(added_time).count() << std::endl;

  auto old_end_time = end_time;
  if (added_time >= std::chrono::seconds(BUKHOOR_INCREASE_STEP)) {
    end_time += std::chrono::seconds(BUKHOOR_INCREASE_STEP);
  }
  else {
    end_time += added_time;
  }

  std::cout << "New Bukhoor time is " << std::chrono::duration_cast<std::chrono::seconds>(end_time-start_time).count() << std::endl;

  std::cout << std::endl;

  std::cout << "old_end_time is " << old_end_time.time_since_epoch().count() << std::endl;
  std::cout << "end_time is " << end_time.time_since_epoch().count() << std::endl;

  std::cout << std::chrono::microseconds(std::chrono::seconds(1)).count() << std::endl;

  std::cout << "steady clock to seconds conversion" << std::endl;

  auto tp1 = std::chrono::steady_clock::now();
  this_thread::sleep_for(std::chrono::seconds(4));
  auto tp2 = std::chrono::steady_clock::now();

  std::cout << " tp2 - tp1: " << tp2.time_since_epoch().count() - tp1.time_since_epoch().count() << std::endl;

  auto int_s = std::chrono::duration_cast<std::chrono::seconds>(tp2 - tp1);

  std::cout << " tp2 - tp1 in seconds: " << int_s.count() << std::endl;
}

#endif  // defined(USE_TIME_INCREMENT)

#if defined(USE_CLASS_CONS_DEST)

class Inner {
public:
  Inner() { LOG_INFO("Constructor Inner"); }
  ~Inner() { LOG_INFO("Destructor Inner"); }
  void Print() { LOG_INFO("Inner print"); };
};

class Owner {
public:
  Owner() { LOG_INFO("Constructor Owner"); }
  ~Owner() { LOG_INFO("Destructor Owner"); }
  void Print() { LOG_INFO("Owner print"); };

  Inner inner;
};

void class_cons_dest()
{
  Owner obj;

  obj.Print();
  obj.inner.Print();
}

#endif  // defined(USE_CLASS_CONS_DEST)

#if defined(USE_SPT_TEMPLATIZED_CLASS)

class ClassWithTimer {
public:
  ClassWithTimer() {};
  ~ClassWithTimer() {};

  // callback to be called at the end
  void callback(bool how);
};

void ClassWithTimer::callback(bool how)
{
  std::string str("void ClassWithTimer::callback(bool how): ");
  Logger::LogMessage(str + (how ? "premature" : "timeout"));
}

//std::mutex ClassWithTimer::cblock;


void use_spt_templatized()
{
  ClassWithTimer obj;
  SinglePulseTimerT<ClassWithTimer> timer;

  auto tp1 = std::chrono::steady_clock::now();
  timer.setSinglePulseTimer(&obj,obj.callback,5);
  if (timer.getThreadObject().joinable()) {
    timer.getThreadObject().join();
  }
  auto tp2 = std::chrono::steady_clock::now();

  auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1);
  std::cout << " tp2 - tp1 in milliseconds: " << int_ms.count() << std::endl;
}

#endif  // defined(USE_SPT_TEMPLATIZED_CLASS)

#if defined(USE_GET_LOCAL_AND_UTC_TIME)

std::time_t local_to_utc(std::time_t local);

void GetLocalAndUTCTime()
{
  std::time_t utc = std::time(nullptr);
  std::time_t endtime = utc + 300;
  std::cout << "          endtime: " << endtime << std::endl <<
               "              UTC: " << utc << std::endl <<
               "             diff: " << std::setw(10) << abs(endtime - utc) << std::endl;
}

std::time_t local_to_utc(std::time_t local)
{
  auto local_field = *std::gmtime(&local);
  local_field.tm_isdst = -1;
  auto utc = std::mktime(&local_field);
  return utc;
}

#endif  // defined(USE_GET_LOCAL_AND_UTC_TIME)

#if defined(USE_DYNAMIC_POINTER_CAST)

struct Base
{
    int a;
    virtual void f() const { std::cout << "I am base!\n";}
    virtual ~Base(){}
};

struct Derived : Base
{
    void f() const override
    { std::cout << "I am derived!\n"; }
    ~Derived(){}
};

void DynamicCast()
{
  auto basePtr = std::make_shared<Base>();
  std::cout << "Base pointer says: ";
  basePtr->f();

  auto basePtr2 = basePtr;

  auto derivedPtr = std::make_shared<Derived>();
  std::cout << "Derived pointer says: ";
  derivedPtr->f();

  // static_pointer_cast to go up class hierarchy
  basePtr = std::static_pointer_cast<Base>(derivedPtr);
  std::cout << "Base pointer to derived says: ";
  basePtr->f();

  // dynamic_pointer_cast to go down/across class hierarchy
  auto downcastedPtr = std::dynamic_pointer_cast<Derived>(basePtr);
  if(downcastedPtr)
  {
      std::cout << "Downcasted pointer says: ";
      downcastedPtr->f();
  }

  auto downcastedPtr2 = std::dynamic_pointer_cast<Base>(basePtr2);
  if(downcastedPtr2)
  {
      std::cout << "DowncastedPtr2 pointer says: ";
      downcastedPtr2->f();
  }

  // All pointers to derived share ownership
  std::cout << "Pointers to underlying derived: "
          << derivedPtr.use_count()
          << "\n";

}

#endif  // defined(USE_DYNAMIC_POINTER_CAST)

#if defined(USE_TEST_sendCorrectedEndTime)
void Test_sendCorrectedEndTime()
{
  int value = 166094509;
  std::stringstream str;
  str << "{\"$inp\":\"Local\",\"EndTime\":" << value << "}";

  std::cout << "payload: " << str.str().c_str() << std::endl;
}
#endif  // defined(USE_TEST_sendCorrectedEndTime)

#if defined(USE_SYSTEM_CALL)
void Test_systemCall()
{
  int r;
   r = ::system("ping 8.8.8.8");
   std::cout << "r = " << r << std::endl;

   r = ::system("ping 200.1.100.4");
   std::cout << "r = " << r << std::endl;

}
#endif  // defined(USE_SYSTEM_CALL)

#if defined(USE_TEST_STRING_COMPARE)
void Test_StringCompare()
{
  std::string res = "Passed";

  int r = res.compare("Passed");
  std::cout << "Compare result = " << r << std::endl;
}
#endif // defined(USE_TEST_STRING_COMPARE)

#if defined(USE_CATCH_OUTPUT)

std::string GetStdoutFromCommand(string cmd)
{
  string data;
  FILE * stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
  //cmd.append(" 2>&1");

  stream = popen(cmd.c_str(), "r");
  if (stream) {
    while (!feof(stream))
      if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    pclose(stream);
  }
  return data;
}


void Test_CatchOutput()
{
  std::string ls = GetStdoutFromCommand("dir ");
  cout << "LS: " << ls << endl;

}
#endif // defined(USE_CATCH_OUTPUT)


