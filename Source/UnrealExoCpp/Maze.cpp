// Fill out your copyright notice in the Description page of Project Settings.


#include "Maze.h"

// Sets default values
AMaze::AMaze()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TrigerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("NomTriger"));
	TrigerBox->SetCollisionProfileName(TEXT("Trigger"), true);

	TrigerBox->OnComponentBeginOverlap.AddDynamic(this, &AMaze::OnOverlapBegin);
	TrigerBox->OnComponentEndOverlap.AddDynamic(this, &AMaze::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AMaze::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMaze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMaze::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FLatentActionInfo LatentInfo;

	UGameplayStatics::LoadStreamLevel(this, AMaze::Level, true, false,
		LatentInfo);
	

}

void AMaze::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FLatentActionInfo LatentInfo;

	UGameplayStatics::UnloadStreamLevel(this, AMaze::Level, LatentInfo, true);
}