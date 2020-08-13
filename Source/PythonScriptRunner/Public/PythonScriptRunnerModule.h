// Copyright 1998-2018 20Tab S.r.l. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#if ENGINE_MAJOR_VERSION==4 && ENGINE_MINOR_VERSION>=22
#include "Modules/ModuleInterface.h"
#else
#include "ModuleInterface.h"
#endif

class FPythonScriptRunnerModule : public IModuleInterface
{
public:
	virtual void StartupModule();
	virtual void ShutdownModule();

private:
	// Ensure we have python on this machine
	bool CanRunPython() const;

	static bool CheckPathIsPythonHome(const FString& InPath);
	
	static void CachePythonHomePath(const FString& InPath);

	static void RunPython();

	static FString CachedPythonHome;
};
