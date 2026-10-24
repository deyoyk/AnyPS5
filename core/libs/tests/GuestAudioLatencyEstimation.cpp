#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

extern "C" {
int APS5_VABI sceAudioLatencyEstimationAddData();
int APS5_VABI sceAudioLatencyEstimationGetDelayTime();
int APS5_VABI sceAudioLatencyEstimationInitialize();
int APS5_VABI sceAudioLatencyEstimationRelease();
}

static void Require(bool value) { if (!value) std::abort(); }

template <typename TCall>
static bool ReportsNotImplemented(const char* name, TCall call) {
    try {
        call();
    } catch (const std::runtime_error& error) {
        return std::string(error.what()) == std::string(name) + " not implemented";
    }
    return false;
}

namespace {

void TestLifecycle() {
    Require(sceAudioLatencyEstimationInitialize() == 0);
    Require(sceAudioLatencyEstimationRelease() == 0);
    Require(sceAudioLatencyEstimationRelease() == 0);
    Require(sceAudioLatencyEstimationInitialize() == 0);
    Require(sceAudioLatencyEstimationInitialize() == 0);
    Require(sceAudioLatencyEstimationRelease() == 0);
}

void TestUnimplementedThrow() {
    Require(ReportsNotImplemented("sceAudioLatencyEstimationAddData", [] { return sceAudioLatencyEstimationAddData(); }));
    Require(ReportsNotImplemented("sceAudioLatencyEstimationGetDelayTime", [] { return sceAudioLatencyEstimationGetDelayTime(); }));
}

}

int main(int argc, char** argv) {
    Require(argc == 2);
    Require(std::strcmp(argv[1], "lifecycle") == 0);
    TestLifecycle();
    TestUnimplementedThrow();
}
