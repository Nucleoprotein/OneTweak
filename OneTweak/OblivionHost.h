#pragma once

#include "OneTweak.h"

class OblivionConfig : public OneTweakConfig
{
public:
	OblivionConfig();
	virtual ~OblivionConfig();

    void OnReadConfig(IniFile* ini);
};


class OblivionHost : public OneTweakHost
{
public:
    OblivionHost(const OBSEInterface * obse)
    {	
		PrintLog(__FUNCTION__);

		config.ReadConfig();

        Run();
    }

	~OblivionHost(){ PrintLog(__FUNCTION__); };

protected:
	const OneTweakConfig& GetConfig() const
	{
		return config;
	}

private:
	OblivionConfig config;
};