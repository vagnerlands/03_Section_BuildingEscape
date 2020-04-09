// Fill out your copyright notice in the Description page of Project Settings.


#include "Room1_OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TriggerVolume.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
URoom1_OpenDoor::URoom1_OpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URoom1_OpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...

	FRotator doorRotation = GetOwner()->GetActorRotation();
	// since yaw can be from -180.f up to 180.f, we must adjust the yaw in case it's negative
	float AdjustedYaw = doorRotation.Yaw;
	//if (doorRotation.Yaw < 0.f) AdjustedYaw += 360.f;
	InitialYawAngle = AdjustedYaw;
	// get the current yaw angle
	CurrentYawAngle = AdjustedYaw;
	// door maximum angle is 100 from start yaw
	FinalYawAngle += AdjustedYaw;
	//APawn* pActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	//ActorThatOpens = pActor;	
}


// Called every frame 
void URoom1_OpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (PressurePlate)
	{
		if (TotalMassOnPressurePlate() > MassToActivateDoor)
		{
			OpenDoor(DeltaTime);
			DoorLastOpen = GetWorld()->GetTimeSeconds();
		}
		else // keep door shut if actor isn't there
		{
			const float TimeNow = GetWorld()->GetTimeSeconds();
			if (TimeNow > (DoorLastOpen + DoorOpenTimeout))
			{
				CloseDoor(DeltaTime);
			}
		}
	}
	else
	{
		// bad error!
		UE_LOG(LogTemp, Error, TEXT(" *** Actor '%s' Has no 'Pressure Plate' Actor defined!"), *GetOwner()->GetFName().ToString());
	}
	
}

void URoom1_OpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYawAngle = FMath::FInterpTo(CurrentYawAngle, FinalYawAngle, DeltaTime, OpenDoorSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYawAngle;
	GetOwner()->SetActorRotation(DoorRotation);

	//FName objName = GetOwner()->GetFName();
	//FString outlog = objName.ToString();
	//UE_LOG(LogTemp, Warning, TEXT(" *** Actor '%s' Yaw: %f [%f / %f] Ajusted %f"), *outlog, doorRotation.Yaw, InitialYawAngle, FinalYawAngle, CurrentAdjustedYaw);

}

void URoom1_OpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYawAngle = FMath::FInterpTo(CurrentYawAngle,InitialYawAngle, DeltaTime, CloseDoorSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYawAngle;
	GetOwner()->SetActorRotation(DoorRotation);

	//FName objName = GetOwner()->GetFName();
	//FString outlog = objName.ToString();
	//UE_LOG(LogTemp, Warning, TEXT(" *** Actor '%s' Yaw: %f [%f / %f] Ajusted %f"), *outlog, doorRotation.Yaw, InitialYawAngle, FinalYawAngle, CurrentAdjustedYaw);

}

float URoom1_OpenDoor::TotalMassOnPressurePlate() const
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;

	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (AActor* i : OverlappingActors)
	{
		TotalMass += i->FindComponentByClass< UPrimitiveComponent >()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("  Found actor (%s) mass %f"), *i->GetName(), TotalMass);
	}

	return TotalMass;
}

