#include <cmw.hpp>

#include "test/test.h"

int main() {
    CMW_TRY_RC_RETURN(cmw::Logger::initialize());
    CMW_TRACE("wow\n");
    CMW_FATAL("yeet\n");
    CMW_ERROR("oof\n");
    test();
    CMW_TRY_RC_RETURN(cmw::Logger::finalize());
    return 0;
}
