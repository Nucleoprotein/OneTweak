#pragma once

#include "OneTweak.h"

class FalloutNVConfig : public OneTweakConfig
{
public:
	FalloutNVConfig();
	virtual ~FalloutNVConfig();

	void OnReadConfig(CSimpleIniA* ini);
};


class FalloutNVHost : public OneTweakHost
{
public:
    FalloutNVHost(const NVSEInterface * nvse)
    {	
        _MESSAGE(__FUNCTION__);

		config.ReadConfig();
    }

    ~FalloutNVHost(){_MESSAGE(__FUNCTION__);};

protected:
	const OneTweakConfig& GetConfig() const override
	{
		return config;
	}

private:
	FalloutNVConfig config;
};