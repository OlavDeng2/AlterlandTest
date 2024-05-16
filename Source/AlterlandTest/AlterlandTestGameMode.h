// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HttpModule.h"
#include "AlterlandTestGameMode.generated.h"

#define DEFAULT_OPENWEATHER_API_KEY TEXT("NO API KEY SET")


USTRUCT(BlueprintType)
struct FWeatherData
{
    GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	bool bIsMetric = true;
	UPROPERTY(BlueprintReadOnly)
	float Longtitude = 0;
	UPROPERTY(BlueprintReadOnly)
	float Lattitude = 0;
	UPROPERTY(BlueprintReadOnly)
	FString WeatherType = "None";
	UPROPERTY(BlueprintReadOnly)
	FString WeatherDescription = "None";
	UPROPERTY(BlueprintReadOnly)
	FString WeatherIcon = "None";
	UPROPERTY(BlueprintReadOnly)
	float Temperature = 0;
	UPROPERTY(BlueprintReadOnly)
	float FeltTemperature = 0;
	UPROPERTY(BlueprintReadOnly)
	int Humidity = 0; // in %
	UPROPERTY(BlueprintReadOnly)
	float Visibility = 0;
	UPROPERTY(BlueprintReadOnly)
	float WindSpeed = 0;
	UPROPERTY(BlueprintReadOnly)
	float WindDirection = 0; // in degrees
	UPROPERTY(BlueprintReadOnly)
	float GustSpeed = 0;
	UPROPERTY(BlueprintReadOnly)
	float RainAmount = 0; //in mm
	UPROPERTY(BlueprintReadOnly)
	float SnowAmount = 0; //in mm
	UPROPERTY(BlueprintReadOnly)
	int CloudCoverage = 0; // in %
	UPROPERTY(BlueprintReadOnly)
	int CurrentTime = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Country = "None";
	UPROPERTY(BlueprintReadOnly)
	FString City = "None";
	UPROPERTY(BlueprintReadOnly)
	int SunRise = 0;
	UPROPERTY(BlueprintReadOnly)
	int SunSet = 0;
	UPROPERTY(BlueprintReadOnly)
	int TimeZone = 0;
};

USTRUCT(BlueprintType)
struct FCityData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	FString CityName = "None";
	UPROPERTY(BlueprintReadOnly)
	FString CountryName = "None";
	UPROPERTY(BlueprintReadOnly)
	float Lat = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float Lon = 0.0f;

};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeatherUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCityListUpdated);



UCLASS(minimalapi)
class AAlterlandTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAlterlandTestGameMode();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString OpenWeatherAPIKey = DEFAULT_OPENWEATHER_API_KEY;
	UPROPERTY(BlueprintReadOnly)
	FWeatherData CurrentWeatherData;
	UPROPERTY(BlueprintReadOnly)
	TArray<FCityData> CityList;


	UPROPERTY(BlueprintAssignable)
	FOnWeatherUpdated onWeatherUpdated;
	UPROPERTY(BlueprintAssignable)
	FOnCityListUpdated onCityListUpdated;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void GetWeatherData(float lat, float lon, bool bIsMetric);

	UFUNCTION(BlueprintCallable)
	void GetCityData(FString CityName, int CityCount);


private:
	void ProcessWeatherData(const FString& ResponseContent);
	void ProcessWeatherDataObject(const TSharedPtr<FJsonObject>& JsonResponseObject);

	void ProcessCityData(const FString& ResponseContent);
	void ProcessCityDataObject(const TSharedPtr<FJsonObject>& JsonResponseObject);
};



