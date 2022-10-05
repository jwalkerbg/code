/*
 * Mtest.h
 *
 *  Created on: Sep 02, 2022
 *      Author: imc
 */

#ifndef MTEST_H_
#define MTEST_H_

#include "QModel.h"

#if !defined(DEFAULT_QOS)
#define DEFAULT_QOS (1)
#endif  // !defined(DEFAULT_QOS)

#if !defined(LOG_INFO)
#define LOG_INFO(str) { std::cout << __FILE__ << ":" << __LINE__ << ": " << str << std::endl; }
#endif  // !defined(LOG_INFO)

/*
 * class MtestBase
 * Abstract base for deviced MTEST classes. Contains base functionality
 * for receiving MTEST messages and routing them to their handlers.
 * Handlers are abstract here and must be overridden in derived classes.
 */
class MtestBase {
protected:
  MtestBase(const std::string id, const std::string model);
  ~MtestBase();
private:
  MtestBase() = delete;   // no default constructor

public:
  std::string& GetMtestId() { return mtestId; }
private:
  std::string mtestId;

public:
  QModel& getModel() { return tags; }
  void publishMessage(const char* topic, const char* message, int qos = 0);
protected:
  QModel tags;

public:
  void setPublishEnabled(bool value) { publishEnabled = value; }
  bool getPublishEnabled() { return publishEnabled; }
private:
  bool publishEnabled;

public:
  void SetQoS(int QoS) { m_QoS = QoS; }
  int GetQoS() { return m_QoS; }
private:
  int m_QoS;

protected:
  static std::string strTopics[];
  void subscribe();
  // common callback for all derived objects
  void dispatch(const char *topic, const std::string &message);

protected:
  virtual void internalCallback(const char *topic, const std::string &message) = 0;
  virtual void setUp(int testId, VJson& payload) = 0;
  virtual void runTest(int testId, int run, VJson& payload) = 0;
  virtual void tearDown(int testId, VJson& payload) = 0;
  virtual void answer(int testId, VJson& payload) = 0;
};

///////////////////////

// This is sample class that can be used in applications.
// The test functions have to be filled with useful code and if needed
// to added other tests.

// Usualy, and object of this class is created as embedded object of the class
// that have to be tested. This class has to be made friend of the class under test.
// A pointer to a object of the class under test is passed as an argument owner_
// of the constructor.

class Mtest : public MtestBase {
public:
  Mtest(const std::string id, const std::string model, void* owner_);
  ~Mtest();
private:
  Mtest() = delete;   // no default constructor

  // pointer to the object that this object is an subelement
  void* owner;

  virtual void internalCallback(const char *topic, const std::string &message);

  virtual void setUp(int testId, VJson& payload);
  virtual void runTest(int testId, int run, VJson& payload);
  virtual void tearDown(int testId, VJson& payload);
  virtual void answer(int testId, VJson& payload);

// tests
// this is application specific part
// These tests may contain everithing that is useful
// Each of these functions is assigned testId (the first argument of runTest())

  void test_1();
  void test_2();
  void test_3();

  void test_default();

// helpers
// Here some helper functions may be defined that may be used by the tests

};

#endif  // MTEST_H_
