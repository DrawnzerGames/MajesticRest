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
	void GetResponse(OutStructType& Out)
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(JsonResponse, &Out);
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

	template <typename OutStructType>
	void GetResponse(OutStructType& Out)
	{
		FJsonObjectConverter::JsonObjectStringToUStruct(JsonResponse, &Out);
	}

	int32 RequestId;
private:
	FString JsonResponse;
};

USTRUCT()
struct FMajesticFormData
{
	GENERATED_BODY()

	void AddKeyValue(FString InKey, FString InValue)
	{
		Key.Add(InKey);
		Value.Add(InValue);
	}

	FString GetFormData()
	{
		FString Content = "";
		for (int i = 0; i < Key.Num(); ++i)
		{
			if (i > 0)
			{
				Content = Content + "&";
			}
			Content = Content + Key[i] + "=" + Value[i];
		}
		return Content;
	}

private:
	TArray<FString> Key;
	TArray<FString> Value;
};

DECLARE_DELEGATE_TwoParams(FMajesticRestCallback, FMajesticRestResponse* /*Response*/, FMajesticRestError* /*Error*/);

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
