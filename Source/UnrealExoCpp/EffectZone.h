// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectZone.generated.h"

UCLASS()
class UNREALEXOCPP_API AEffectZone : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AEffectZone();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float amoutLifeModifier;

	FTimerHandle TimerHandle;

	FTimerDelegate TimerDele;

private:

	void checkModifierLife(class AActor* OtherActor);

	void takeModifierLife(class AActor* OtherActor);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* TrigerBox;
};
