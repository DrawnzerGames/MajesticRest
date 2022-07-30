//  Copyright 2022 - Infinity DrawnzerGames, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MajesticConfig.generated.h"

USTRUCT()
struct FBaseUrls
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Config)
	FString Name;

	UPROPERTY(Config)
	FString Url;
};

USTRUCT()
struct FRestCalls
{
	GENERATED_BODY()

	UPROPERTY(Config)
	FString Method;

	UPROPERTY(Config)
	FString Name;

	UPROPERTY(Config)
	FString Uri;

	UPROPERTY(Config)
	FString ContentType = "application/json";
};

/**
 * The config class for the rest calls.
 */
UCLASS(Config=Game)
class MAJESTICREST_API UMajesticConfig : public UObject
{
	GENERATED_BODY()

public:
	// Example usage +BaseUrls=(Name="base0", Url="https://my.api.com")
	UPROPERTY(Config)
	TArray<FBaseUrls> BaseUrls;

	// Example usage +RestCalls=(Method="POST", Name="Login", Uri="/login")
	UPROPERTY(Config)
	TArray<FRestCalls> RestCalls;
};
