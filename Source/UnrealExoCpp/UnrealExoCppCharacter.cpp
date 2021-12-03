// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealExoCppCharacter.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AUnrealExoCppCharacter

AUnrealExoCppCharacter::AUnrealExoCppCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnrealExoCppCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Crounch", IE_Pressed, this, &AUnrealExoCppCharacter::Crounch);
	PlayerInputComponent->BindAction("Crounch", IE_Released, this, &AUnrealExoCppCharacter::UnCrounch);

	PlayerInputComponent->BindAction("Drag", IE_Pressed, this, &AUnrealExoCppCharacter::PickUp);
	PlayerInputComponent->BindAction("Drag", IE_Released, this, &AUnrealExoCppCharacter::UnPickUp);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUnrealExoCppCharacter::CreateProjectile);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUnrealExoCppCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUnrealExoCppCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUnrealExoCppCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUnrealExoCppCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUnrealExoCppCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUnrealExoCppCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUnrealExoCppCharacter::OnResetVR);
}


void AUnrealExoCppCharacter::OnResetVR()
{
	// If UnrealExoCpp is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in UnrealExoCpp.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUnrealExoCppCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AUnrealExoCppCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AUnrealExoCppCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealExoCppCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealExoCppCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUnrealExoCppCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AUnrealExoCppCharacter::Crounch()
{
	Crouching = true;
}

void AUnrealExoCppCharacter::UnCrounch()
{
	Crouching = false;
}
// Called when the game starts or when spawned
void AUnrealExoCppCharacter::BeginPlay()
{
	HP = 100;
	Super::BeginPlay();
}

void AUnrealExoCppCharacter::LifeModifier(float amout)
{

	HP = HP + amout;


	if (HP <= 0)
	{
		Death();
	}

}

void AUnrealExoCppCharacter::Death()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetController()->UnPossess();
	//Respawn();
}


void AUnrealExoCppCharacter::Respawn()
{
	//GetWorld()->GetTimerManager().SetTimer(RespawnHandle, RespawnDele, 2.0f, false);
	//APawn* Pawn = GetWorld()->SpawnActor<APawn>(DefaultCharacter, FVector(-770, 370, 226), FRotator(0, 0, 0));
	//GetController()->Possess(Pawn);
}

void AUnrealExoCppCharacter::PickUp()
{
	FHitResult Hit;
	FVector Start = GetCapsuleComponent()->GetComponentLocation();
	FVector End = Start + (GetCapsuleComponent()->GetForwardVector() * 700.f);
	FCollisionQueryParams CollisionParams;


	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams))
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Emerald, false, 1);
		if (Hit.GetComponent()->Mobility == EComponentMobility::Movable)
		{

			DragObject = Hit.GetActor();
			DragObject->FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(false);
			DragObject->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepWorldTransform);
		}
	}

}
void AUnrealExoCppCharacter::UnPickUp()
{
	if (DragObject)
	{
		DragObject->FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
		DragObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}
void AUnrealExoCppCharacter::CreateProjectile()
{
	GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), GetCapsuleComponent()->GetRelativeLocation(), GetCapsuleComponent()->GetRelativeRotation());
}