#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

class LogUtils {
public:
	static void Log(const FString& Message = "LOG!", const FColor& Color = FColor::White, float Duration = 5.f)
	{
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message);
		}
	};

	// Log("hello", FColor::Green)
	static void Log(const char* Message, FColor Color = FColor::White, float Duration = 5.f)
	{
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, Duration, Color, FString(Message));
		}
	}

	// // LogValue("Speed : ", speed, FColor::Green);
	// template<typename T>
	// static void LogValue(const char* Name, const T& Value, FColor Color = FColor::White, float Duration = 5.f)
	// {
	// 	if (GEngine) {
	// 		FString Message = FString::Printf(TEXT("%s: %s"), UTF8_TO_TCHAR(Name), *FString::SanitizeFloat(Value));
	// 		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message);
	// 	}
	// }
	//
	// static void LogFormat(const TCHAR* Format, ...) {}
};
