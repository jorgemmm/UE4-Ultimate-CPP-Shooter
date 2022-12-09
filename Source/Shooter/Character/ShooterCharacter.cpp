// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

//Engine
#include "Animation/AnimInstance.h"

//components
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"

#include "GameFramework/Actor.h"

//FX
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimMontage.h"

//Utils
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AShooterCharacter::AShooterCharacter():
	BaseTurnRate{ 45.f },
	BaseLookUpRate{ 45.f },
	PawYawRate{ 540.f }
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a Camera Boom ( Pulls in towards the character if there is a collision )
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f; //The canera follow at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; //Rotate the arm base base on the controller
	CameraBoom->SocketOffset = FVector{ 0.f, 50.f , 50.f };


	//Create Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamwera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // attach camera to end of boom
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Don´t rotate when the con trroller rotates. Let the controller only affect the camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	GetCharacterMovement()->bOrientRotationToMovement=  false ;
	GetCharacterMovement()->RotationRate =  FRotator{ 0.f, PawYawRate, 0.f  } ;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = .2f;




}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*UE_LOG(LogTemp, Warning, TEXT("BeginPlay()  Call"));

	int myInt{ 42 };

	UE_LOG(LogTemp, Warning, TEXT("int myInt %d"), myInt);*/
	
}


// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	
	//Setup Move bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);

	// Set up "look" bindings.
	//Mouse
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//GamePad
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpRate);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShooterCharacter::FireWeapon);
}


void AShooterCharacter::TurnAtRate(float Rate)
{
	//Calcuñate delta for this frame from the rate information
	//Input * velgiroBase * DeltaTime       ( DeltaTime =  1 / FPS,  Tiempo entre frames , DeltaTime -> 1 tick   )
	// input[0,1] * deg/sec * sec/frames -> deg/frames
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()); 
}

void AShooterCharacter::LookUpRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::FireWeapon()
{

	FHitResult HitResult;	
	Server_FireWeapon(HitResult.ImpactPoint);

}

void AShooterCharacter::FireWeaponHandle()
{
	

	//UE_LOG(LogTemp, Warning, TEXT("Firing Wepaon"))

	//if (FireSound)
	//{
	//	//UGameplayStatics::PlaySound2D(this, FireSound);
	//	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	//}

	//if (MuzzleFlash)
	//{
	//	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GetMesh(), FName("BarrelSocket"));
	//}


	


	// Fx´s are in the Motages ( sounds and emmiters)

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HipFireMontage)
	{
		
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
		
	}
	
	//if (!HasAuthority()) return;
	
	//Line trace by channel
	
	const USkeletalMeshSocket* MuzzleFlash_BarrelSocket = GetMesh()->GetSocketByName(FName("BarrelSocket"));
	
	if (!MuzzleFlash_BarrelSocket) 
	{   
		UE_LOG(LogTemp, Error, TEXT("BarrelSocket Does NOT Exist in this Mesh"))
		return;
	}

	const FTransform BarrelSocketTransform = MuzzleFlash_BarrelSocket->GetSocketTransform(GetMesh());
	
	FHitResult FireHit;

	const FVector Start{ BarrelSocketTransform.GetLocation() };
	const FQuat  Rotation{ BarrelSocketTransform.GetRotation() };
	const FVector RotationAxis{ Rotation.GetAxisX() };
	const FVector End{ Start + RotationAxis * 50'000.f };

	FVector BeamEndPoint{ End };

	GetWorld()->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);
	
	
	if (FireHit.bBlockingHit)
	{
		/*DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 2.f,0,5.f);
		DrawDebugPoint(GetWorld(), FireHit.Location, 8.f, FColor::Red, false, 2.0);*/

		UE_LOG(LogTemp, Error, TEXT("Hit to: %s"), *FireHit.GetActor()->GetFName().ToString());
	
		if (ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FireHit.Location);
			
		}

		BeamEndPoint = FireHit.Location;
		
	}

	if (BeamParticles)
	{
		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
			GetMesh(), 
			BeamParticles, 
			BarrelSocketTransform.GetLocation());
     	//, GetMesh(), FName("BarrelSocket"));

		if (Beam) 
		{
			Beam->SetVectorParameter(FName("Target"), BeamEndPoint);
		}


	}
}

void AShooterCharacter::Server_FireWeapon_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	Multicast_FireWeapon(TraceHitTarget);
}

void AShooterCharacter::Multicast_FireWeapon_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	FireWeaponHandle();
}





void AShooterCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0.f, Rotation.Yaw , 0.f };

		const FVector Direction{ FRotationMatrix{ YawRotation }.GetUnitAxis(EAxis::X) };
		
		//FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);

	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0.f, Rotation.Yaw , 0.f };

		const FVector Direction{ FRotationMatrix{ YawRotation }.GetUnitAxis(EAxis::Y) };

		//FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

