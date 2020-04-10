// Fill out your copyright notice in the Description page of Project Settings.


#include "Room1_OpenDoor.h"
// project includes
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DoorTrigger.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

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
	// Find audio
	FindAudioComponent();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT(" *** Pressure Plate wasn't defined for (%s)"), *GetOwner()->GetName());
	}
	else
	{
		PressurePlateDoorTrigger = PressurePlate->FindComponentByClass<UDoorTrigger>();
		if (!PressurePlateDoorTrigger)
		{
			UE_LOG(LogTemp, Error, TEXT(" *** Pressure Plate Door Trigger wasn't defined for (%s)"), *PressurePlate->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(" Found DoorTrigger in (%s)"), *PressurePlate->GetName());
		}
	}

}


// Called every frame 
void URoom1_OpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if ((PressurePlate) && (PressurePlateDoorTrigger))
	{
		if (PressurePlateDoorTrigger->IsTriggerActive())
		{
			OpenDoor(DeltaTime);
		}
		else // keep door shut if actor isn't there
		{
			CloseDoor(DeltaTime);
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
	if (DoorState != DOOR_OPEN)
	{
		DoorState = DOOR_OPEN;
		if (OpenDoorAudio)
		{
			OpenDoorAudio->Play();
		}
	}
	CurrentYawAngle = FMath::FInterpTo(CurrentYawAngle, FinalYawAngle, DeltaTime, OpenDoorSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYawAngle;
	GetOwner()->SetActorRotation(DoorRotation);
}

void URoom1_OpenDoor::CloseDoor(float DeltaTime)
{
	if (DoorState != DOOR_CLOSE)
	{
		DoorState = DOOR_CLOSE;
		if (OpenDoorAudio)
		{
			OpenDoorAudio->Play();
		}
	}
	CurrentYawAngle = FMath::FInterpTo(CurrentYawAngle,InitialYawAngle, DeltaTime, CloseDoorSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYawAngle;
	GetOwner()->SetActorRotation(DoorRotation);
}

bool URoom1_OpenDoor::FindAudioComponent() 
{
	OpenDoorAudio = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (OpenDoorAudio)
	{
		return true;
	}
	
	UE_LOG(LogTemp, Error, TEXT("  *** Failed to find an Audio Component on (%s)"), *GetOwner()->GetName());

	return false;
}

