#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

class LogUtils {
public:
	// LogUtils::Log()
	// LogUtils::Log("Hello")
	static void Log(const FString& Message = TEXT("LOG!"))
	{
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, Message);
		}
	}

	// LogUtils::Log("Speed", SpeedA, SpeedB, SpeedC)  --> Speed : 10.0, 20.0, 30.0
	template<typename... Args>
	static void Log(const FString& Prefix, Args... args) {
		if (GEngine){
			FString Message = FString::Format(TEXT("{0} : {1}"), { Prefix, FString::Join(TArray<FString>{FString::Printf(TEXT("%s"), *FString::SanitizeFloat(args))...}, TEXT(", ")) });
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, Message);
		}
	}
};