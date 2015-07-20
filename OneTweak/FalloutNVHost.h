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
        PrintLog(__FUNCTION__);

		config.ReadConfig();

        Run();
    }

    ~FalloutNVHost(){PrintLog(__FUNCTION__);};

protected:
	const OneTweakConfig& GetConfig() const
	{
		return config;
	}

private:
	FalloutNVConfig config;
};