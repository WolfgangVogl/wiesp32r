#pragma once

#ifndef UNIT_TEST

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#else

#error "You must NOT define UNIT_TEST to use this file"

#endif
