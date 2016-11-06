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
		_MESSAGE(__FUNCTION__);

		config.ReadConfig();

		if (config.precachekiller) PrecacheKiller();
		if (config.nointro) NoIntro();
	}

	~SkyrimHost()
	{
		_MESSAGE(__FUNCTION__);
	};

protected:
	const OneTweakConfig& GetConfig() const override
	{
		return config;
	}

private:
	SkyrimConfig config;

	void PrecacheKiller();
	void NoIntro();
};