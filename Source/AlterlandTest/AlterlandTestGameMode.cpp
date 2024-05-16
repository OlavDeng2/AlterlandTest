// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlterlandTestGameMode.h"
#include "AlterlandTestCharacter.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

AAlterlandTestGameMode::AAlterlandTestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}


// Called when the game starts or when spawned
void AAlterlandTestGameMode::BeginPlay()
{
    Super::BeginPlay();
}



// Called every frame
void AAlterlandTestGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


void AAlterlandTestGameMode::GetWeatherData(float lat,float lon, bool bIsMetric)
{
    CurrentWeatherData = FWeatherData();
    CurrentWeatherData.bIsMetric = bIsMetric;
    auto request = FHttpModule::Get().CreateRequest();
    FString unit = bIsMetric ? "metric" : "imperial";
    FString requestURL = FString("https://api.openweathermap.org/data/2.5/weather?lat=") + FString::SanitizeFloat(lat) + FString("&lon=") + FString::SanitizeFloat(lon) + FString("&units=") + unit + FString("&appid=") + OpenWeatherAPIKey;
    request->SetURL(requestURL);
    request->OnProcessRequestComplete().BindLambda([& ](FHttpRequestPtr request, FHttpResponsePtr response, bool success) {
        if (success)
        {
            UE_LOG(LogTemp, Log, TEXT("OpenWeather response: %s"), *(response->GetContentAsString()));

            ProcessWeatherData(response->GetContentAsString());
        }
        else
        {
            switch (request->GetStatus()) {
            case EHttpRequestStatus::Failed_ConnectionError:
                UE_LOG(LogTemp, Error, TEXT("Connection failed."));
            default:
                UE_LOG(LogTemp, Error, TEXT("Request failed."));
            }
        }
        });
    request->ProcessRequest();
}

void AAlterlandTestGameMode::ProcessWeatherData(const FString& ResponseContent)
{

    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseContent);
    TSharedPtr<FJsonValue> WeatherJSonValue;
    if (FJsonSerializer::Deserialize(JsonReader, WeatherJSonValue))
    {
        UE_LOG(LogTemp, Log, TEXT("Deserializing OpenWeather"));
        TSharedPtr<FJsonObject> JsonResponseObject = WeatherJSonValue->AsObject();
        ProcessWeatherDataObject(JsonResponseObject);

    }
    else
    {
    }
}




void AAlterlandTestGameMode::ProcessWeatherDataObject(const TSharedPtr<FJsonObject>& JsonResponseObject)
{

    if (JsonResponseObject)
    {
        //Get Coords
        const TSharedPtr<FJsonObject>* Coordinates;
        if (JsonResponseObject->TryGetObjectField(TEXT("coord"), Coordinates))
        {
            Coordinates->Get()->TryGetNumberField(TEXT("lat"), CurrentWeatherData.Lattitude);
            Coordinates->Get()->TryGetNumberField(TEXT("lon"), CurrentWeatherData.Longtitude);
        }


        //Get weather data
        const TArray<TSharedPtr<FJsonValue>>* Weather;
        if (JsonResponseObject->TryGetArrayField(TEXT("weather"), Weather))
        {
            //From what I could see, this will only ever return 1 array element, so we just assume there is one if it is succesful.
            Weather->GetData()->Get()->AsObject()->TryGetStringField(TEXT("main"), CurrentWeatherData.WeatherType);
            Weather->GetData()->Get()->AsObject()->TryGetStringField(TEXT("description"), CurrentWeatherData.WeatherDescription);
            Weather->GetData()->Get()->AsObject()->TryGetStringField(TEXT("icon"), CurrentWeatherData.WeatherIcon);

        }

        //get main
        const TSharedPtr<FJsonObject>* Main;
        if (JsonResponseObject->TryGetObjectField(TEXT("main"), Main))
        {
            Main->Get()->TryGetNumberField(TEXT("temp"), CurrentWeatherData.Temperature);
            Main->Get()->TryGetNumberField(TEXT("feels_like"), CurrentWeatherData.FeltTemperature);
            Main->Get()->TryGetNumberField(TEXT("humidity"), CurrentWeatherData.Humidity);

        }

        //get visibility
        JsonResponseObject->TryGetNumberField(TEXT("visibility"), CurrentWeatherData.Visibility);


        //get wind
        const TSharedPtr<FJsonObject>* wind;
        if (JsonResponseObject->TryGetObjectField(TEXT("wind"), wind))
        {
            wind->Get()->TryGetNumberField(TEXT("speed"), CurrentWeatherData.WindSpeed);
            wind->Get()->TryGetNumberField(TEXT("deg"), CurrentWeatherData.WindDirection);
            wind->Get()->TryGetNumberField(TEXT("gust"), CurrentWeatherData.GustSpeed);
        }

        //get rain
        const TSharedPtr<FJsonObject>* rain;
        if (JsonResponseObject->TryGetObjectField(TEXT("rain"), rain))
        {
            rain->Get()->TryGetNumberField(TEXT("1h"), CurrentWeatherData.RainAmount);
        }

        //get snow
        //get rain
        const TSharedPtr<FJsonObject>* snow;
        if (JsonResponseObject->TryGetObjectField(TEXT("snow"), snow))
        {
            snow->Get()->TryGetNumberField(TEXT("1h"), CurrentWeatherData.SnowAmount);
        }


        //get clouds
        const TSharedPtr<FJsonObject>* clouds;
        if (JsonResponseObject->TryGetObjectField(TEXT("clouds"), clouds))
        {
            clouds->Get()->TryGetNumberField(TEXT("all"), CurrentWeatherData.CloudCoverage);

        }

        //get time
        JsonResponseObject->TryGetNumberField(TEXT("dt"), CurrentWeatherData.CurrentTime);

        //get sys info
        const TSharedPtr<FJsonObject>* sys;
        if (JsonResponseObject->TryGetObjectField(TEXT("sys"), sys))
        {
            sys->Get()->TryGetStringField(TEXT("country"), CurrentWeatherData.Country);
            sys->Get()->TryGetNumberField(TEXT("sunrise"), CurrentWeatherData.SunRise);
            sys->Get()->TryGetNumberField(TEXT("sunset"), CurrentWeatherData.SunSet);
        }

        //get timezone
        JsonResponseObject->TryGetNumberField(TEXT("timezone"), CurrentWeatherData.TimeZone);

        //get place name
        JsonResponseObject->TryGetStringField(TEXT("name"), CurrentWeatherData.City);

        onWeatherUpdated.Broadcast();
    }
}

void AAlterlandTestGameMode::GetCityData(FString CityName, int CityCount)
{
    //Make sure the list is empty to avoid duplicates/old data
    CityList.Empty();

    auto request = FHttpModule::Get().CreateRequest();

    //Teplace spaces in Cityname as URLs do not support space
    CityName = CityName.Replace(TEXT(" "), TEXT("_"));
    FString requestURL = FString("http://api.openweathermap.org/geo/1.0/direct?q=") + CityName + FString("&limit=") + FString::FromInt(CityCount) + FString("&appid=") + OpenWeatherAPIKey;
    request->SetURL(requestURL);

    request->OnProcessRequestComplete().BindLambda([&](FHttpRequestPtr request, FHttpResponsePtr response, bool success) {
        if (success)
        {
            UE_LOG(LogTemp, Log, TEXT("Geo response: %s"), *(response->GetContentAsString()));

            ProcessCityData(response->GetContentAsString());
        }
        else
        {
            switch (request->GetStatus()) {
            case EHttpRequestStatus::Failed_ConnectionError:
                UE_LOG(LogTemp, Error, TEXT("Connection failed."));
            default:
                UE_LOG(LogTemp, Error, TEXT("Request failed."));
            }
        }
        });
    request->ProcessRequest();
}

void AAlterlandTestGameMode::ProcessCityData(const FString& ResponseContent)
{

    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseContent);

    TSharedPtr<FJsonValue> CityJsonValue;
    if (FJsonSerializer::Deserialize(JsonReader, CityJsonValue))
    {
        TArray< TSharedPtr<FJsonValue>> CityArray = CityJsonValue->AsArray();
        for (TSharedPtr<FJsonValue> value : CityArray)
        {
            FCityData CityDataObject = FCityData();


            value->AsObject()->TryGetStringField(FString("name"), CityDataObject.CityName);
            value->AsObject()->TryGetStringField(FString("country"), CityDataObject.CountryName);
            value->AsObject()->TryGetNumberField(FString("lat"), CityDataObject.Lat);
            value->AsObject()->TryGetNumberField(FString("lon"), CityDataObject.Lon);


            CityList.Add(CityDataObject);
        }
        onCityListUpdated.Broadcast();
    }
}

void AAlterlandTestGameMode::ProcessCityDataObject(const TSharedPtr<FJsonObject>& JsonResponseObject)
{
}

