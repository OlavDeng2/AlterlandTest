// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeatherUI.generated.h"

/**
 * 
 */
UCLASS()
class ALTERLANDTEST_API UWeatherUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void SetCorrectTimeString(int Hours, int Minutes, FString& TimeString);

	UFUNCTION(BlueprintCallable)
	void SetCorrectTimezoneString(int TimeZone/*Timezone in unix time*/, FString& TimeZoneString);

	
};
