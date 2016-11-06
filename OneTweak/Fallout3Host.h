#pragma once

#include "OneTweak.h"

class Fallout3Config : public OneTweakConfig
{
public:
	Fallout3Config();
	virtual ~Fallout3Config();

	void OnReadConfig(CSimpleIniA* ini);
};


class Fallout3Host : public OneTweakHost
{
public:
    Fallout3Host(const FOSEInterface * fose)
    {	
        _MESSAGE(__FUNCTION__);

		config.ReadConfig();
    }

    ~Fallout3Host(){_MESSAGE(__FUNCTION__);};

protected:
	const OneTweakConfig& GetConfig() const override
	{
		return config;
	}

private:
	Fallout3Config config;
};