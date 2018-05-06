// SimpleFSM - a Simple class for simple people.
// instanciate states which point to other success and failure states.

#include "SimpleFSM.h"

SimpleFSM::SimpleFSM (String name){
    stateName = name;
}

int SimpleFSM::setSuccess(SimpleFSM *success)
{
  successState = success;
  return 0;
}

SimpleFSM* SimpleFSM::getSuccess(){
  return successState;
}

String SimpleFSM::getName() const{
  return stateName;
}
