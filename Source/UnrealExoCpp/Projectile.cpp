// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/DecalActor.h"
#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UMaterial> SplashMaterial(TEXT("Material'/Game/Material/SplashMaterial.SplashMaterial'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	Splash = SplashMaterial.Object;
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NomMesh"));
	MovementSphere = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("NomMovement"));

	if (StaticMesh.Succeeded())
	{
		SphereMesh->SetStaticMesh(StaticMesh.Object);
	}

	SphereMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	SphereMesh->SetCollisionProfileName("Custom");
	SphereMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SphereMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	MovementSphere->InitialSpeed = 2000;
	SphereMesh->SetWorldScale3D(FVector(0.25, 0.25, 0.25));
	RootComponent = SphereMesh;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Call On Hit
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ADecalActor* Bullet = GetWorld()->SpawnActor<ADecalActor>(Hit.Location, FRotator(0, 0, 0));
	Bullet->SetDecalMaterial(Splash);
	Bullet->SetLifeSpan(2);
	Destroy();
}