// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StreetLight.generated.h"

UCLASS()
class ALTERLANDTEST_API AStreetLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStreetLight();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LightFixture = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPointLightComponent* Light = nullptr;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void TurnOnLight();
	UFUNCTION()
	void TurnOffLight();

private:
	UFUNCTION()
	void OnWeatherChanged();

	class AAlterlandTestGameMode* GameMode = nullptr;


};
