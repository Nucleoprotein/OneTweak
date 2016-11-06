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

	// run OneTweak
	bool Run();

protected:
    // set DPI awareness to proper scale window
    void SetDPIAware();
};

extern std::unique_ptr<OneTweakHost> g_Host;