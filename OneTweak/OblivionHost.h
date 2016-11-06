#pragma once

#include "OneTweak.h"

class OblivionConfig : public OneTweakConfig
{
public:
	OblivionConfig();
	virtual ~OblivionConfig();

	void OnReadConfig(CSimpleIniA* ini);
};


class OblivionHost : public OneTweakHost
{
public:
    OblivionHost(const OBSEInterface * obse)
    {	
		_MESSAGE(__FUNCTION__);

		config.ReadConfig();
    }

	~OblivionHost(){ _MESSAGE(__FUNCTION__); };

protected:
	const OneTweakConfig& GetConfig() const override
	{
		return config;
	}

private:
	OblivionConfig config;
};