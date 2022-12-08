// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Hunter.h"
#include "EcoActorCrocodile.h"
#include "EcoActorElephant.h"
#include "EcoActorZebra.h"
#include "Gun.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	auto capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = capsule;
	capsule->InitCapsuleSize(42.f, 96.0f);
	capsule->SetCollisionProfileName(TEXT("NoCollision"));

	HunterTime = 1.0f;
	AnimalTime = 1.0f;
	GunTime = 3.0f;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnHunter();
	SpawnAnimal();
	SpawnGun();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::SpawnHunter()
{
	GetWorld()->GetTimerManager().SetTimer(HunterHandle, FTimerDelegate::CreateLambda([&]()->void {
		FVector2D RandomPoint = FMath::RandPointInCircle(8000.0f);
		GetWorld()->SpawnActor<AHunter>(GetActorLocation() + FVector(RandomPoint, 400.0f), FRotator::ZeroRotator);
		}), HunterTime, true);
}

void ASpawner::SpawnAnimal()
{
	GetWorld()->GetTimerManager().SetTimer(AnimalHandle, FTimerDelegate::CreateLambda([&]()->void {
		FVector2D RandomPoint = FMath::RandPointInCircle(8000.0f);
		int8 RandomAnimal = FMath::RandRange(1, 3);
		switch (RandomAnimal)
		{
		case 1:
		{
			GetWorld()->SpawnActor<AEcoActorZebra>(GetActorLocation() + FVector(RandomPoint, 400.0f), FRotator::ZeroRotator); break;
		}
		case 2:
		{
			GetWorld()->SpawnActor<AEcoActorElephant>(GetActorLocation() + FVector(RandomPoint, 400.0f), FRotator::ZeroRotator); break;
		}
		case 3:
		{
			GetWorld()->SpawnActor<AEcoActorCrocodile>(GetActorLocation() + FVector(RandomPoint, 400.0f), FRotator::ZeroRotator); break;
		}
		}
		}), AnimalTime, true);
}

void ASpawner::SpawnGun()
{
	GetWorld()->GetTimerManager().SetTimer(GunHandle, FTimerDelegate::CreateLambda([&]()->void {
		FVector2D RandomPoint = FMath::RandPointInCircle(8000.0f);
		GetWorld()->SpawnActor<AGun>(GetActorLocation() + FVector(RandomPoint, 400.0f), FRotator::ZeroRotator);
		}), GunTime, true);
}