// Fill out your copyright notice in the Description page of Project Settings.


#include "TioTargetDummy.h"
#include "TioAttributeComponent.h"

// Sets default values
ATioTargetDummy::ATioTargetDummy()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
    RootComponent = MeshComp;

    AttributeComp = CreateDefaultSubobject<UTioAttributeComponent>("AttributeComp");
    AttributeComp->OnHealthChanged.AddDynamic(this, &ATioTargetDummy::OnHealthChanged);

}

void ATioTargetDummy::OnHealthChanged(AActor* InstigatorActor, UTioAttributeComponent* OwninComp, float NewHealth, float Delta)
{
    if (Delta < 0.0f)
    {
        MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
    }
    
}


