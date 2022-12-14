//  Copyright 2022 - Infinity DrawnzerGames, Inc. All Rights Reserved.

#include "Manager/MajesticRestManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

UMajesticRestManager* UMajesticRestManager::Manager;

UMajesticRestManager* UMajesticRestManager::Get()
{
	return Manager;
}

int32 UMajesticRestManager::MakeRestCall(FString Name, const UStruct* BodyDefinition, const void* Body, FMajesticRestCallback& Callback)
{
	for (const auto& Calls : Config->RestCalls)
	{
		if (Calls.Name.Equals(Name))
		{
			const auto HttpModule = &FHttpModule::Get();
			const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule->CreateRequest();
			Request->SetVerb(Calls.Method);
			Request->SetHeader("Content-Type", Calls.ContentType);
			Request->SetURL(Config->BaseUrls[0].Url + Calls.Uri);
			if (Calls.Method.Equals("POST") && Body != nullptr && BodyDefinition != nullptr)
			{
				if (Calls.ContentType.Equals("application/x-www-form-urlencoded"))
				{
					// Must go as key/value.
					const auto FormData = (struct FMajesticFormData*)Body;
					Request->SetContentAsString(FormData->GetFormData());
				}
				else
				{
					FString StringBody = "";
					FJsonObjectConverter::UStructToJsonObjectString(BodyDefinition, Body, StringBody);
					Request->SetContentAsString(StringBody);
				}
			}

			const auto CurrentRequest = RequestCount++;
			Request->OnProcessRequestComplete()
			       .BindLambda([CurrentRequest, Callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessfully) mutable
			       {
				       if (bSuccessfully)
				       {
					       if (Response->GetResponseCode() >= 200 && Response->GetResponseCode() < 300)
					       {
						       FMajesticRestResponse* RestResponse = new FMajesticRestResponse(CurrentRequest, Response->GetContentAsString());
						       Callback.Execute(RestResponse, nullptr);
					       }
					       else
					       {
						       FMajesticRestError* Error = new FMajesticRestError(CurrentRequest, Response->GetContentAsString());
						       Callback.Execute(nullptr, Error);
					       }
				       }
				       else
				       {
					       FMajesticRestError* Error = new FMajesticRestError(CurrentRequest, "");
					       Callback.Execute(nullptr, Error);
				       }
			       });
			Request->ProcessRequest();
			return CurrentRequest;
		}
	}
	return -1;
}

void UMajesticRestManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Manager = GEngine->GetEngineSubsystem<UMajesticRestManager>();
	Config = NewObject<UMajesticConfig>(this, TEXT("MajesticConfig"));
	Config->AddToRoot();
}
