// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherSystem.h"
#include "AlterlandTestGameMode.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AWeatherSystem::AWeatherSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeatherSystem::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AAlterlandTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->onWeatherUpdated.AddDynamic(this, &AWeatherSystem::OnWeatherChanged);
	
}

// Called every frame
void AWeatherSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeatherSystem::OnWeatherChanged()
{

	WeatherData = GameMode->CurrentWeatherData;
}

