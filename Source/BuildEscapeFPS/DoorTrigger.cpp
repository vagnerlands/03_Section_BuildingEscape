// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorTrigger.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UDoorTrigger::UDoorTrigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorTrigger::BeginPlay()
{
	Super::BeginPlay();

	FVector plateLocation = GetOwner()->GetActorLocation();

	FindAudioComponent();

	// saves the initial height of the pressure plate
	UnpressedHeight = plateLocation.Z;
	// when pressed, it should resize Z axis by 10cm  (5cm actually)
	PressedHeight = plateLocation.Z - 10.f;
	
}

bool UDoorTrigger::FindAudioComponent()
{
	PressurePlateAudio = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (PressurePlateAudio)
	{
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("  *** Failed to find audio component for (%s)"), *GetOwner()->GetName());

	return false;
}

void UDoorTrigger::MovePlateDown(float DeltaTime)
{
	if (PressurePlateState > PRESSED_ANIM)
	{
		PressurePlateState = PRESSED_ANIM;
		if (PressurePlateAudio)
		{
			PressurePlateAudio->Play();
		}
	}
	CurrentHeight = FMath::FInterpTo(CurrentHeight, PressedHeight, DeltaTime, 2.f);
	FVector actLocation = GetOwner()->GetActorLocation();
	actLocation.Z = CurrentHeight;
	GetOwner()->SetActorLocation(actLocation);

	
}

void UDoorTrigger::MovePlateUp(float DeltaTime)
{
	if (PressurePlateState != UNPRESSED)
	{
		PressurePlateState = UNPRESSED;
		if (PressurePlateAudio)
		{
			PressurePlateAudio->Play();
		}
	}
	CurrentHeight = FMath::FInterpTo(CurrentHeight, UnpressedHeight, DeltaTime, 2.f);
	FVector actLocation = GetOwner()->GetActorLocation();
	actLocation.Z = CurrentHeight;
	GetOwner()->SetActorLocation(actLocation);
}

float UDoorTrigger::TotalMassOnPressurePlate() const
{
	
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;

	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (AActor* i : OverlappingActors)
	{
		TotalMass += i->FindComponentByClass< UPrimitiveComponent >()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("  Found actor (%s) mass %f"), *i->GetName(), TotalMass);
	}

	return TotalMass;
	
}


// Called every frame
void UDoorTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate)
	{
		if (TotalMassOnPressurePlate() > MassToActivateDoor)
		{
			MovePlateDown(DeltaTime);
			const float TimeNow = GetWorld()->GetTimeSeconds();
			if (TimeNow > (TriggerLastClosed + TriggerOpenTimeout))
			{
				PressurePlateState = PRESSED;
				UE_LOG(LogTemp, Warning, TEXT("   PressurePlate is PRESSED"));
			}
		}
		else // keep door shut if actor isn't there
		{
			MovePlateUp(DeltaTime);
			TriggerLastClosed = GetWorld()->GetTimeSeconds();
			PressurePlateState = UNPRESSED;
		}
	}
	else
	{
		// bad error!
		UE_LOG(LogTemp, Error, TEXT(" *** Actor '%s' Has no 'Pressure Plate' Actor defined!"), *GetOwner()->GetFName().ToString());
	}
}

