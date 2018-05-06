// SimpleFSM.h
#ifndef SimpleFSM_H
#define SimpleFSM_H
#include <Arduino.h>
#include <String.h>
#include <functional>

class SimpleFSM
{
private:
  SimpleFSM *successState = NULL;
  SimpleFSM *failureState = NULL;
  String stateName;

public:
  SimpleFSM(String name);
  int setSuccess(SimpleFSM*);
  int setFailure(SimpleFSM*);
  SimpleFSM* getSuccess();
  String getName() const;

};

#endif
