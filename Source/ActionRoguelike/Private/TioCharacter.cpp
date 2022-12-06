// Fill out your copyright notice in the Description page of Project Settings.


# include "TioCharacter.h"
# include "GameFramework\SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TioInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "TioAttributeComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATioCharacter::ATioCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UTioInteractionComponent>(TEXT("InteractionComp"));

	AttributeComp = CreateDefaultSubobject<UTioAttributeComponent>(TEXT("AttributeComp"));

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	AttackAnimeDelay = 0.16f;

	TimeToHitParamName = "TimeToHit";
	HandSocketName = "Muzzle_01";
}

void ATioCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATioCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void ATioCharacter::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ATioCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//const float DrawScale = 100.0f;
	//const float Thickness = 5.0f;

	//FVector LineStart = GetActorLocation();
	//// offset to the right of pawn
	//LineStart += GetActorRightVector() * 100.0f;
	//// Set line end in direction of the actor's forward
	//FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	//// Draw Actor's Direction
	//DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	//FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	//DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called to bind functionality to input
void ATioCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATioCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATioCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ATioCharacter::PrimaryAttack);
	// use generic name "SecnodaryAttack" for binding
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ATioCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ATioCharacter::Dash);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ATioCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATioCharacter::Jump);
}

void ATioCharacter::MoveForward(float value)
{
	//AddMovementInput(GetActorForwardVector(), value);

	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	
	AddMovementInput(ControlRot.Vector(), value);
}

void ATioCharacter::MoveRight(float value)
{
	//AddMovementInput(GetActorRightVector(), value);
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//// X = Forward(Red)
	//// Y = Right(Green)
	//// Z = Up(Blue)

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, value);
}


void ATioCharacter::PrimaryAttack()
{
	StartAttackEffects();
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ATioCharacter::PrimaryAttack_TimeElapsed, AttackAnimeDelay);
}

void ATioCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void ATioCharacter::BlackHoleAttack()
{
	StartAttackEffects();
	GetWorldTimerManager().SetTimer(TimerHandle_BlackholeAttack, this, &ATioCharacter::BlackHoleAttack_TimeElapsed, AttackAnimeDelay);
}

void ATioCharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void ATioCharacter::Dash()
{
	StartAttackEffects();
	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ATioCharacter::Dash_TimeElapsed, AttackAnimeDelay);
}

void ATioCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void ATioCharacter::StartAttackEffects()
{
	PlayAnimMontage(AttackAnim);

	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
}

void ATioCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = CameraComp->GetComponentLocation();

		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd,FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		//Find new direction/rotation from Hand pointing to impact point in world
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

void ATioCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void ATioCharacter::OnHealthChanged(AActor* InstigatorActor, UTioAttributeComponent* OwninComp, float NewHealth, float Delta)
{
	UE_LOG(LogTemp, Log, TEXT("ACTOR Health %f Take %f Damage"), NewHealth, Delta);
	if (Delta < 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("ACTOR Hit"));
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}

	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

