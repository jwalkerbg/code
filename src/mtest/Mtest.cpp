/*
 * Mtest.cpp
 *
 *  Created on: Aug 25, 2022
 *      Author: imc
 */

#include "logger.h"
#include "mqtttopic.h"

#include "Mtest.h"

MtestBase::MtestBase(const std::string id, const std::string model):
tags(model.c_str()),
publishEnabled(false)
{
  mtestId = id;
  SetQoS(DEFAULT_QOS);
  LOG_INFO("MtestBase Constructor executed");
}

MtestBase::~MtestBase()
{
  LOG_INFO("~MtestBase() Destructor executed");
}

void MtestBase::publishMessage(const char* topic, const char* message, int qos)
{
  if (getPublishEnabled()) {
    getModel().publishMessage(topic,message,qos);
  }
}

std::string MtestBase::strTopics[] = { "MTEST/SETUP", "MTEST/RUN", "MTEST/TEARDOWN", "MTEST/ANS", "" };

void MtestBase::subscribe()
{
  std::string s1 = std::string("@/MTEST/SETUP/" + GetMtestId());
  std::string s2 = std::string("@/MTEST/RUN/" + GetMtestId());
  std::string s3 = std::string("@/MTEST/TEARDOWN/" + GetMtestId());
  std::string s4 = std::string("@/MTEST/ANS/" + GetMtestId());

  tags.subscribe(s1.c_str(),GetQoS());
  tags.subscribe(s2.c_str(),GetQoS());
  tags.subscribe(s3.c_str(),GetQoS());
  tags.subscribe(s4.c_str(),GetQoS());

  std::stringstream str;
  str << "MTEST Subscription: " << s1.c_str();
  LOG_INFO((str.str()));

  str = std::stringstream(std::string());
  str << "MTEST Subscription: " << s2.c_str();
  LOG_INFO((str.str()));

  str = std::stringstream(std::string());
  str << "MTEST Subscription: " << s3.c_str();
  LOG_INFO((str.str()));

  str = std::stringstream(std::string());
  str << "MTEST Subscription: " << s4.c_str();
  LOG_INFO((str.str()));
}

// MtestBase::dispatch will handle MTEST messages. Topics are
// @/MTEST/SETUP/<MtestId>, @/MTEST/RUN/<MtestId>, @/MTEST/TEARDOWN/<MtestId>
// <MtestId> is unique identifier supplied to derived class object constructor.

// Note that the topics do not contain context information (if such information is needed and have to be used).
// The context information comes with the payload which is passed to Mtest::internalCallback via message argument.

// See format of the topics in MtestBase::subscribe() for more information.

// The format of payloads is ALWAYS valid JSON object and never scalar JSON types.
// There are several predefined fields in the payload:
//
//   "id":"integer" - numerical identifier of the test. May be used for indexing, in switch case variants or what else is appropriate. Obligatory field.
//   "tname":"string" - human readable name of the test. May be used or not in the code. Useful when reading logs. Optional field.
//   "repeat":"integer" - how may times the test to be executed. If not present, the test is executed one time.
//      0 means the test not to be executed. Negative numbers mean the test not to be executed. These may be used as temporary disabling the tests,
//      but to see them in the logs. Optional Field.
//
// Additional fields defining context are allowed and they may be as complex as the test objects under test need.
//
// Example: -t "@/MTEST/RUN" -m '{"id":1,"tname":"MyFirstMtest", "userdata0":"uservalue0", .... "userdataN":"uservalueN"}'
//
void MtestBase::dispatch(const char *topic, const std::string &message)
{
  LOG_INFO("Mtest::dispatch");

  std::string topicStr(topic);
  mqtt_topic mqttTopic(topicStr);
  VJson payload = tags.parse(message.c_str());

  int testId = payload["id"];
  int repeat;
  if (payload["repeat"].isNull()) {
    repeat = 1;
  }
  else {
    repeat = payload["repeat"];
    if (repeat < 0) {
      repeat = 0;
    }
  }

  int ix = FindSubstring(topicStr, MtestBase::strTopics);
  switch (ix) {
  case 0: // "@/MTEST/SETUP"
    LOG_INFO("@/MTEST/SETUP");
    setUp(testId,payload);
    break;
  case 1: // "@/MTEST/RUN"
    LOG_INFO("@/MTEST/RUN");
    for (int i = 0; i < repeat; ++i) {
      runTest(testId,i + 1,payload);
    }
    break;
  case 2: // "@MTEST/TEARDOWN"
    LOG_INFO("@/MTEST/TEARDOWN");
    tearDown(testId,payload);
    break;
  case 3: // "@MTEST/ANS"
    LOG_INFO("@/MTEST/ANS");
    break;
  default:
    LOG_INFO("Unknown topic received by MTEST");
    break;
  }
}

///////////////////

// Mtest class.
// This class is can be instantiated to only one object

Mtest::Mtest(const std::string id, const std::string model, void* owner_):
MtestBase(id,model),owner(owner_)
{
  tags.addReceiveCallback(this,&Mtest::internalCallback);
  subscribe();
  LOG_INFO("Mtest Constructor executed");
}

Mtest::~Mtest()
{
  LOG_INFO("~Mtest() Destructor executed");
}

void Mtest::internalCallback(const char *topic, const std::string &message)
{
  LOG_INFO("Mtest::internalCallback");
  // MtestBase::dispatch is called this way so as to receive pointer this of
  // the object of Mtest class. It is needed to call the virtual function in Mtest.
  MtestBase::dispatch(topic,message);
}

void Mtest::setUp(int testId, VJson& payload)
{
  std::stringstream str;
  std::string tname = payload["tname"];
  str << "Setup test " << testId << " tname = \"" << tname << "\"";
  LOG_INFO(str.str());
}

void Mtest::runTest(int testId, int run, VJson& payload)
{
  std::stringstream str;
  std::string tname = payload["tname"];
  str << "Running test " << testId << " run " << run << " tname = \"" << tname << "\"";
  LOG_INFO(str.str());

  switch (testId) {
  case 1:
    test_1();
    break;
  case 2:
    test_2();
    break;
  case 3:
    test_3();
    break;
  default:
    test_default();
    break;
  }
}

void Mtest::tearDown(int testId, VJson& payload)
{
  std::stringstream str;
  std::string tname = payload["tname"];
  str << "Tear down  test " << testId << " tname = \"" << tname << "\"";
  LOG_INFO(str.str());
}

void Mtest::answer(int testId, VJson& payload)
{
  std::stringstream str;
  std::string tname = payload["tname"];
  str << "Answer from test " << testId << " tname = \"" << tname << "\"";
  LOG_INFO(str.str());
}

  void Mtest::test_1()
  {
    LOG_INFO("test_1 excuted");
  }

  void Mtest::test_2()
  {
    LOG_INFO("test_2 excuted");
  }

  void Mtest::test_3()
  {
    LOG_INFO("test_3 excuted");
  }

  void Mtest::test_default()
  {
    LOG_INFO("test_default excuted");
  }

// helpers


// End of Mtest.cpp

