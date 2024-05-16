// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeatherIcon.generated.h"

/**
 * 
 */
UCLASS()
class ALTERLANDTEST_API UWeatherIcon : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ApplyWeatherIcon(FString &IconID);
	UFUNCTION(BlueprintCallable)
	void GetWeatherIcon(FString IconID);

};
