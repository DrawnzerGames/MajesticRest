//  Copyright 2022 - Infinity DrawnzerGames, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Config/MajesticConfig.h"
#include "JsonObjectConverter.h"
#include "MajesticRestManager.generated.h"

USTRUCT()
struct FMajesticRestResponse
{
	GENERATED_BODY()
	FMajesticRestResponse() : RequestId(-1), JsonResponse("")
	{
	}

	explicit FMajesticRestResponse(int32 Id, FString Response) : RequestId(Id), JsonResponse(Response)
	{
	}

	int32 RequestId;

	template <typename OutStructType>
	OutStructType GetResponse()
	{
		OutStructType Out;
		FJsonObjectConverter::JsonObjectStringToUStruct(JsonResponse, &Out);
		return Out;
	}

	template <typename OutStructType>
	TArray<OutStructType> GetResponseAsArray()
	{
		TArray<OutStructType> Out;
		FJsonObjectConverter::JsonArrayStringToUStruct(JsonResponse, &Out);
		return Out;
	}

private:
	FString JsonResponse;
};

USTRUCT()
struct FMajesticRestError
{
	GENERATED_BODY()

	FMajesticRestError() : RequestId(-1), JsonResponse("")
	{
	}

	explicit FMajesticRestError(int32 Id, FString Response) : RequestId(Id), JsonResponse(Response)
	{
	}

	int32 RequestId;
private:
	FString JsonResponse;
};

DECLARE_DELEGATE_TwoParams(FMajesticRestCallback, const FMajesticRestResponse* /*Response*/,
                           const FMajesticRestError* /*Error*/);

/**
 * Rest API manager.
 */
UCLASS()
class MAJESTICREST_API UMajesticRestManager : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UMajesticRestManager* Get();
	int32 MakeRestCall(FString Name, const UStruct* BodyDefinition, const void* Body, FMajesticRestCallback& Callback);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	static UMajesticRestManager* Manager;

	UPROPERTY()
	UMajesticConfig* Config;

	int32 RequestCount;
};
