// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "PythonScriptRunnerModule.h"
#include "UnrealEnginePython.h"

#include "CommandLine.h"


IMPLEMENT_MODULE(FPythonScriptRunnerModule, PythonScriptRunner);


void FPythonScriptRunnerModule::StartupModule()
{
	FString ScriptPath;
	if (FParse::Value(FCommandLine::Get(), TEXT("-run_script="), ScriptPath))
	{
		UE_LOG(LogTemp, Display, TEXT("FPythonScriptRunnerModule::StartupModule => Running script: %s"), *ScriptPath);

		FUnrealEnginePythonModule& PythonModule = FModuleManager::GetModuleChecked<FUnrealEnginePythonModule>("UnrealEnginePython");
		PythonModule.RunFile(TCHAR_TO_UTF8(*ScriptPath));
	}
}

void FPythonScriptRunnerModule::ShutdownModule()
{

}
