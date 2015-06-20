#pragma once

#include "OneTweakConfig.h"
#include "OneTweakDriver.h"

// main host class
class OneTweakHost
{
public:
    // read config on object creation
    OneTweakHost();

    // clean-up
    virtual ~OneTweakHost();

	virtual const OneTweakConfig& GetConfig() const = 0;

protected:
    // set DPI awareness to proper scale window
    void SetProcessDPIAware();

    // run OneTweak
	void Run();
};

extern std::unique_ptr<OneTweakHost> g_Host;