// Sergey Inozemcev @techies 04.08.2021.
#include "RickAndMorty_HttpProxy.h"

// Sets default values for this component's properties
URickAndMorty_HttpProxy::URickAndMorty_HttpProxy()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//When the object is constructed, Get the HTTP module
	Http = &FHttpModule::Get();

}

void URickAndMorty_HttpProxy::RequestNumCharacters()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &URickAndMorty_HttpProxy::OnNumCharactersHTTPResponse);
	Request->SetURL("https://rickandmortyapi.com/api/character");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

/*Assigned function on successfull InitializeCharacters http call*/
void URickAndMorty_HttpProxy::OnNumCharactersHTTPResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	int32 TotalCharacters = 0;

	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		//Get the value of the json object by field name "info"
		TSharedPtr<FJsonObject> Info = JsonObject->GetObjectField("info");

		if (Info)
		{
			TotalCharacters = Info->GetIntegerField("count");
			//Output it to the engine
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt(TotalCharacters));
		}
	}

	// Broadcast data for blueprint
	OnNumCharactersResponseDelegate.Broadcast(TotalCharacters);

	UE_LOG(LogTemp, Warning, TEXT("URickAndMorty_HttpProxy::OnResponseReceived status: %s"), bWasSuccessful ? TEXT("True") : TEXT("False"));
}

void URickAndMorty_HttpProxy::RequestRandomCharacter(int32 CharacterId)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &URickAndMorty_HttpProxy::OnRandomCharacterHTTPResponse);
	//This is the url on which to process the request
	FString RequestURI = FString("https://rickandmortyapi.com/api/character/") + FString::FromInt(CharacterId);
	Request->SetURL(RequestURI);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

/*Assigned function on successfull InitializeCharacters http call*/
void URickAndMorty_HttpProxy::OnRandomCharacterHTTPResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//Create CharacterProperties Struct
	FCharacterProperties CharacterProperties;

	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		CharacterProperties.Id = JsonObject->GetIntegerField("id");
		CharacterProperties.Name = JsonObject->GetStringField("name");
		CharacterProperties.Species = JsonObject->GetStringField("species");
		CharacterProperties.Status
			= URickAndMortyFunctionLibrary::LivingStringToEnum(JsonObject->GetStringField("status"));
		CharacterProperties.Gender
			= URickAndMortyFunctionLibrary::GenderStringToEnum(JsonObject->GetStringField("gender"));
		CharacterProperties.Type = JsonObject->GetStringField("type");
		CharacterProperties.Created = JsonObject->GetStringField("created");
		CharacterProperties.Url = JsonObject->GetStringField("url");
		CharacterProperties.Image = JsonObject->GetStringField("image");

		TSharedPtr<FJsonObject> OriginJson = JsonObject->GetObjectField("origin");
		if (OriginJson)
		{
			FLocationProperties OriginLocation;
			OriginLocation.Url = OriginJson->GetStringField("url");
			OriginLocation.Name = OriginJson->GetStringField("name");
			CharacterProperties.Origin = OriginLocation;
		}

		TSharedPtr<FJsonObject> LocationJson = JsonObject->GetObjectField("location");
		if (LocationJson)
		{
			FLocationProperties Location;
			Location.Url = LocationJson->GetStringField("url");
			Location.Name = LocationJson->GetStringField("name");
			CharacterProperties.Location = Location;
		}
	}

	UAsyncTaskDownloadImage* mDownloadTask = NewObject<UAsyncTaskDownloadImage>();
	mDownloadTask->OnSuccess.AddDynamic(this, &URickAndMorty_HttpProxy::OnGetTexture2DSuccess);
	mDownloadTask->OnFail.AddDynamic(this, &URickAndMorty_HttpProxy::OnGetTexture2DFail);
	mDownloadTask->Start(CharacterProperties.Image);

	// Broadcast data for blueprint
	OnRandomCharacterResponseDelegate.Broadcast(CharacterProperties);

	UE_LOG(LogTemp, Warning, TEXT("URickAndMorty_HttpProxy::OnRequestRandomCharacterResponse status: %s"), bWasSuccessful ? TEXT("True") : TEXT("False"));
}

void URickAndMorty_HttpProxy::OnGetTexture2DSuccess(UTexture2DDynamic* Texture) {
	UE_LOG(LogTemp, Warning, TEXT("Texture Loading success"));
	OnTextureLoadDelegate.Broadcast(Texture);
}

void URickAndMorty_HttpProxy::OnGetTexture2DFail (UTexture2DDynamic* Texture) {
	UE_LOG(LogTemp, Warning, TEXT("Texture Loading fail"));

}


// Called when the game starts
void URickAndMorty_HttpProxy::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//OnNumCharactersResponseDelegate.AddDynamic(this, &URickAndMorty_HttpProxy::OnNumCharactersBroadcast);

}


// Called every frame
void URickAndMorty_HttpProxy::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



FString URickAndMortyFunctionLibrary::LivingEnumToString(TEnumAsByte<ECharacterLiving::Status> StatusEnum)
{
	FString StatusName = "unknown";
	switch (StatusEnum) {
	case ECharacterLiving::Alive: StatusName = "Alive"; break;
	case ECharacterLiving::Dead: StatusName = "Dead"; break;
	}
	return StatusName;
}

TEnumAsByte<ECharacterLiving::Status> URickAndMortyFunctionLibrary::LivingStringToEnum(FString StatusName)
{
	TEnumAsByte<ECharacterLiving::Status> Status;
	if (StatusName.ToLower() == FString("alive"))
		Status = ECharacterLiving::Alive;
	else if (StatusName.ToLower() == FString("dead"))
		Status = ECharacterLiving::Dead;
	else
		Status = ECharacterLiving::unknown;
	return Status;
}

FString URickAndMortyFunctionLibrary::GenderEnumToString(TEnumAsByte<ECharacterGender::Status> StatusEnum)
{
	FString StatusName = "unknown";
	switch (StatusEnum) {
	case ECharacterGender::Male: StatusName = "Male"; break;
	case ECharacterGender::Female: StatusName = "Female"; break;
	case ECharacterGender::Genderless: StatusName = "Genderless"; break;
	}
	return StatusName;
}

TEnumAsByte<ECharacterGender::Status> URickAndMortyFunctionLibrary::GenderStringToEnum(FString StatusName)
{
	TEnumAsByte<ECharacterGender::Status> Status;
	if (StatusName.ToLower() == FString("male"))
		Status = ECharacterGender::Male;
	else if (StatusName.ToLower() == FString("female"))
		Status = ECharacterGender::Female;
	else if (StatusName.ToLower() == FString("genderless"))
		Status = ECharacterGender::Genderless;
	else
		Status = ECharacterGender::unknown;
	return Status;
}
