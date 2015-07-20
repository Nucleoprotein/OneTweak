#pragma once

#include "OneTweak.h"
#include "OneTweakConfig.h"

class SkyrimConfig : public OneTweakConfig
{
public:
	SkyrimConfig();
	virtual ~SkyrimConfig();

	bool precachekiller;
	bool nointro;

	void OnReadConfig(CSimpleIniA* ini);
	void OnSaveConfig(CSimpleIniA* ini);
};


class SkyrimHost : public OneTweakHost
{
public:
	SkyrimHost(const SKSEInterface * skse)
	{
		PrintLog(__FUNCTION__);

		config.ReadConfig();

		if (config.precachekiller) PrecacheKiller();
		if (config.nointro) NoIntro();

		Run();
	}

	~SkyrimHost()
	{
		PrintLog(__FUNCTION__);
	};

protected:
	const OneTweakConfig& GetConfig() const
	{
		return config;
	}

private:
	SkyrimConfig config;

	void PrecacheKiller();
	void NoIntro();
};