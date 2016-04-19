#pragma once

#include "OneTweak.h"

class Fallout4Config : public OneTweakConfig
{
public:
	Fallout4Config();
	virtual ~Fallout4Config();

	void OnReadConfig(CSimpleIniA* ini);
};


class Fallout4Host : public OneTweakHost
{
public:
    Fallout4Host(const F4SEInterface * fose)
    {	
        PrintLog(__FUNCTION__);

		config.ReadConfig();

		Run();
    }

    ~Fallout4Host(){PrintLog(__FUNCTION__);};

protected:
	const OneTweakConfig& GetConfig() const
	{
		return config;
	}

private:
	Fallout4Config config;
};