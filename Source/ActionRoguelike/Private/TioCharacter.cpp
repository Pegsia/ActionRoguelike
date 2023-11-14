// Fill out your copyright notice in the Description page of Project Settings.


# include "TioCharacter.h"
# include "GameFramework\SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TioInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "TioAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TioActionComponent.h"


// Sets default values
ATioCharacter::ATioCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UTioInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<UTioAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<UTioActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";
}

void ATioCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ATioCharacter::OnHealthChanged);
}

FVector ATioCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
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

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATioCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATioCharacter::SprintStop);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATioCharacter::Jump);
}

void ATioCharacter::Healself(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
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
	//使用controller的rotation来确定pawn的rotation
	//AddMovementInput(GetActorRightVector(), value);
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X = Forward(Red)
	// Y = Right(Green)
	// Z = Up(Blue)

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, value);
}


void ATioCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ATioCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ATioCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ATioCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByName(this, "Blackhole");
}

void ATioCharacter::Dash()
{
	ActionComp->StartActionByName(this, "Dash");
}

void ATioCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void ATioCharacter::OnHealthChanged(AActor* InstigatorActor, UTioAttributeComponent* OwninComp, float NewHealth, float Delta)
{
	//UE_LOG(LogTemp, Log, TEXT("ACTOR Health %f Take %f Damage"), NewHealth, Delta);
	if (Delta < 0.0f)
	{
		//UE_LOG(LogTemp, Log, TEXT("ACTOR Hit"));
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		// Rage added equal to damage received(Abs to turn into positive rage number)
		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRageChange(InstigatorActor, RageDelta);
	}

	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(5.0f);
	}
}

