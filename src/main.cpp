#include "cmw.hpp"

int main() {
    cmw::Logger::initialize();
    CMW_TRACE("Started\n");
    CMW_FATAL("Yeet\n");
    CMW_ERROR("oof\n");
    cmw::Logger::finalize();
    return 0;
}
