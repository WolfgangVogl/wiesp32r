#pragma once

#include "DataStore/DataStore.hpp"
#include "Webserver/Webserver.hpp"
#include "accessControl.hpp"

#include <Wiegand.h>

namespace wv::wiesp {

struct Global
{
    WIEGAND wiegand;

    unsigned long checkForModify{1};
    volatile bool allowModify{false};

    DataStore dateStore;
    AccessControl accessControl;
    Webserver webserver;
};

} // namespace wv::wiesp
