// Fill out your copyright notice in the Description page of Project Settings.


#include "StreetLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "../AlterlandTestGameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AStreetLight::AStreetLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightFixture = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light Fixture"));
	LightFixture->SetupAttachment(RootComponent);
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(LightFixture);
}

// Called when the game starts or when spawned
void AStreetLight::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AAlterlandTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->onWeatherUpdated.AddDynamic(this, &AStreetLight::OnWeatherChanged);
	
}

// Called every frame
void AStreetLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStreetLight::TurnOnLight()
{
	Light->SetVisibility(true);
}

void AStreetLight::TurnOffLight()
{
	Light->SetVisibility(false);
}

void AStreetLight::OnWeatherChanged()
{

	//Its day time
	if (GameMode->CurrentWeatherData.SunRise < GameMode->CurrentWeatherData.CurrentTime && GameMode->CurrentWeatherData.SunSet > GameMode->CurrentWeatherData.CurrentTime)
	{
		TurnOffLight();

	}
	else
	{
		TurnOnLight();
	}
}

