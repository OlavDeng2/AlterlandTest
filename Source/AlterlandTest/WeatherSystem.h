// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AlterlandTestGameMode.h"
#include "WeatherSystem.generated.h"

UCLASS()
class ALTERLANDTEST_API AWeatherSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadonly)
	FWeatherData WeatherData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:



	UFUNCTION()
	void OnWeatherChanged();

	class AAlterlandTestGameMode *GameMode = nullptr;


};
