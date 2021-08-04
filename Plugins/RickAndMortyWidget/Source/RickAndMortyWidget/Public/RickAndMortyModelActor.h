// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RickAndMorty_HttpProxy.h"
#include "RickAndMortyModelActor.generated.h"

UCLASS()
class RICKANDMORTYWIDGET_API ARickAndMortyModelActor : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Server")
	URickAndMorty_HttpProxy* Proxy;

public:
	// Sets default values for this actor's properties
	ARickAndMortyModelActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
