// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"

// Sets default values
AMySaveGame::AMySaveGame()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMySaveGame::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMySaveGame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMySaveGame::Save() 
{

}

void AMySaveGame::Load()
{

}

void AMySaveGame::Restart()
{

}