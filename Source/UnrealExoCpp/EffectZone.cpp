// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Character.h"
#include "UnrealExoCppCharacter.h"
#include <iostream>
#include "EffectZone.h"

// Sets default values
AEffectZone::AEffectZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TrigerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("NomTriger"));
	TrigerBox->SetCollisionProfileName(TEXT("Trigger"), true);

	TrigerBox->OnComponentBeginOverlap.AddDynamic(this, &AEffectZone::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AEffectZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEffectZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Call in triger
void AEffectZone::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AUnrealExoCppCharacter* Player = Cast<AUnrealExoCppCharacter>(OtherActor);


	if (Player == nullptr)
		return;

	Player->HP = Player->HP + amoutLifeModifier;
	if (Player->HP <= 0)
	{
		Player->Death();
	}
	//Player->LifeModifier(amoutLifeModifier);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, FString::Printf(TEXT("Player health : %f"), Player->HP));
}

void AEffectZone::takeModifierLife(class AActor* OtherActor)
{
	/*AUnrealExoCppCharacter* Player = Cast<AUnrealExoCppCharacter>(OtherActor);
	Player->LifeModifier(amoutLifeModifier);
	delay 0,5 second = 500 ms GetWorldTimerManager().SetTimer(HarvestTimerHandle, this, &ACubeFarmBlock::Harvest, HarvestTime,false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDele, 0.5f, false);
	checkModifierLife(Player);*/
}

void AEffectZone::checkModifierLife(class AActor* OtherActor)
{
	/*AUnrealExoCppCharacter* Player = Cast<AUnrealExoCppCharacter>(OtherActor);

	if (Player->HP <= 0 || Player->HP == 100)
	{
		std::cout << "Player life is " << Player->HP << std::endl;
	}
	else
	{
		if (IsOverlappingActor(Player))
		{
			takeModifierLife(Player);
		}
	}*/
}