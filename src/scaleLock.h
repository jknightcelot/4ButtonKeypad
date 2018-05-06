//=================================
// include guard
//#ifndef __SCALELOCK_H_INCLUDED__
//#define __SCALELOCK_H_INCLUDED__

//=================================
// forward declared dependencies
//class Foo;

//=================================
// included dependencies
#include <Arduino.h>
#include <AceButton.h>

namespace scale_lock {
  void handleKeypadEvent(AceButton* button, uint8_t eventType, uint8_t buttonState);
}
