// Sergey Inozemcev @techies 04.08.2021.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "HttpModule.h"
#include "RunTime/UMG/Public/Blueprint/AsyncTaskDownloadImage.h"
#include "RickAndMorty_HttpProxy.generated.h"

UENUM(BlueprintType)
namespace ECharacterLiving
{
	enum Status
	{
		Alive, Dead, unknown
	};
}

UENUM(BlueprintType)
namespace ECharacterGender
{
	enum Status
	{
		Male, Female, Genderless, unknown
	};
}

USTRUCT(BlueprintType)
struct RICKANDMORTYWIDGET_API FLocationProperties {

	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Location Data")
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Location Data")
	FString Url;
};


USTRUCT(BlueprintType)
struct RICKANDMORTYWIDGET_API FCharacterProperties {

	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Data")
	int32 Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Data")
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Data")
	TEnumAsByte<ECharacterLiving::Status> Status;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Data")
	FString Species;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Data")
	FString Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Data")
	TEnumAsByte<ECharacterGender::Status> Gender;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Data")
	FString Url;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Data")
	FString Created; // TODO: convert to DataTime

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Data")
	FLocationProperties Origin;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Data")
	FLocationProperties Location;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Data")
	FString Image;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNumCharactersResponseDelegate, int32, NumCharacters);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRandomCharacterResponseDelegate, FCharacterProperties, CharactersProperties);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FImageResponseDelegate, UTexture2DDynamic*, CharacterImage);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RICKANDMORTYWIDGET_API URickAndMorty_HttpProxy : public UActorComponent
{
	GENERATED_BODY()

	FHttpModule* Http;

public:

	UPROPERTY(BlueprintAssignable, Category = "ServerResponse")
	FNumCharactersResponseDelegate OnNumCharactersResponseDelegate;

	UPROPERTY(BlueprintAssignable, Category = "ServerResponse")
	FRandomCharacterResponseDelegate OnRandomCharacterResponseDelegate;

	UPROPERTY(BlueprintAssignable, Category = "ServerResponse")
	FImageResponseDelegate OnTextureLoadDelegate;

public:
	// Sets default values for this component's properties
	URickAndMorty_HttpProxy();

	// Request all Rick and Morty Characters ids
	UFUNCTION(BlueprintCallable, Category = "ServerRequest")
	void RequestNumCharacters();

	// Response all Rick and Morty Characters ids parser
	void OnNumCharactersHTTPResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// Request Rick and Morty Character by id
	UFUNCTION(BlueprintCallable, Category = "ServerRequest")
	void RequestRandomCharacter(int32 CharacterId);

	// Response Rick and Morty Character parser
	void OnRandomCharacterHTTPResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// Async listeners
	UFUNCTION()
	void OnGetTexture2DSuccess(UTexture2DDynamic* _texture);

	UFUNCTION()
	void OnGetTexture2DFail(UTexture2DDynamic* Texture);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};

UCLASS()
class RICKANDMORTYWIDGET_API URickAndMortyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	*/
	UFUNCTION(BlueprintPure, Category = "RickAndMorty Enums")
	static FString LivingEnumToString(TEnumAsByte<ECharacterLiving::Status> StatusEnum);

	UFUNCTION(BlueprintPure, Category = "RickAndMorty Enums")
	static TEnumAsByte<ECharacterLiving::Status> LivingStringToEnum(FString StatusName);

	UFUNCTION(BlueprintPure, Category = "RickAndMorty Enums")
	static FString GenderEnumToString(TEnumAsByte<ECharacterGender::Status> StatusEnum);

	UFUNCTION(BlueprintPure, Category = "RickAndMorty Enums")
	static TEnumAsByte<ECharacterGender::Status> GenderStringToEnum(FString StatusName);

};
