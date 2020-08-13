// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "PythonScriptRunnerModule.h"

#include "CommandLine.h"
#include "ConfigCacheIni.h"
#include "Engine/Engine.h"


IMPLEMENT_MODULE(FPythonScriptRunnerModule, PythonScriptRunner);


FString FPythonScriptRunnerModule::CachedPythonHome = {};


void FPythonScriptRunnerModule::StartupModule()
{
	if (!CanRunPython())
		return;

	RunPython();
}

void FPythonScriptRunnerModule::ShutdownModule() { }

bool FPythonScriptRunnerModule::CanRunPython() const
{
	FString PythonHome;
	if (GConfig->GetString(UTF8_TO_TCHAR("Python"), UTF8_TO_TCHAR("Home"), PythonHome, GEngineIni))
	{
		if (CheckPathIsPythonHome(PythonHome))
			return true;
	}

	if (GConfig->GetString(UTF8_TO_TCHAR("Python"), UTF8_TO_TCHAR("RelativeHome"), PythonHome, GEngineIni))
	{
		PythonHome = FPaths::Combine(FPaths::ProjectContentDir(), PythonHome);
		FPaths::NormalizeFilename(PythonHome);

		PythonHome = FPaths::ConvertRelativePathToFull(PythonHome);

		if (CheckPathIsPythonHome(PythonHome))
		{
			CachePythonHomePath(PythonHome);
			return true;
		}
	}

	const int32 MaxPathVarLen = MAX_int16;
	FString OrigPathVar = FString::ChrN(MaxPathVarLen, TEXT('\0'));
#if ENGINE_MINOR_VERSION >= 21
	OrigPathVar = FPlatformMisc::GetEnvironmentVariable(TEXT("PATH"));
#else
	FPlatformMisc::GetEnvironmentVariable(TEXT("PATH"), OrigPathVar.GetCharArray().GetData(), MaxPathVarLen);
#endif
	// Get the current path and remove elements with python in them, we don't want any conflicts
	const TCHAR* PathDelimiter = FPlatformMisc::GetPathVarDelimiter();
	TArray<FString> PathVars;
	OrigPathVar.ParseIntoArray(PathVars, PathDelimiter, true);

	auto FindPython38DLL = [](
		const FString& Path) -> bool
	{
		if (!Path.Contains("Python", ESearchCase::IgnoreCase))
			return false;

		return CheckPathIsPythonHome(Path);
	};

	if (auto NewPythonHome = PathVars.FindByPredicate(FindPython38DLL))
	{
		PythonHome = *NewPythonHome;
		CachePythonHomePath(PythonHome);
		return true;
	}

	return false;
}

bool FPythonScriptRunnerModule::CheckPathIsPythonHome(const FString& InPath)
{
	return FPaths::FileExists(InPath / "Python38.dll");
}

void FPythonScriptRunnerModule::CachePythonHomePath(const FString& InPath)
{
	CachedPythonHome = InPath;
	FPaths::NormalizeFilename(CachedPythonHome);
	GConfig->SetString(UTF8_TO_TCHAR("Python"), UTF8_TO_TCHAR("Home"), *CachedPythonHome, GEngineIni);
}

void FPythonScriptRunnerModule::RunPython()
{
	FModuleManager::Get().LoadModule("UnrealEnginePython");
#if WITH_EDITOR
	// We won't run Engine parts if we are in game! (E.g.: "-game" command line arg)
	if (!IsRunningGame())
	{
		FModuleManager::Get().LoadModule("PythonEditor");
		FModuleManager::Get().LoadModule("PythonConsole");
		FModuleManager::Get().LoadModule("PythonAutomation");
	}
#endif
}
