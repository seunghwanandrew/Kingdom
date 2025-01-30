#pragma once
#include "Modules/ModuleInterface.h"

class FLoadingScreenModule : public IModuleInterface
{
#pragma region Variables
public:
protected:
private:
	UTexture2D* BackgroundTexture;
#pragma endregion

#pragma region Functions
public:
	virtual void StartupModule() override;
	virtual bool IsGameModule() const override;
	virtual void StartupLoadingScreen();
protected:
private:
#pragma endregion

};