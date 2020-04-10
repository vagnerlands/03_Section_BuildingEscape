// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
class ATriggerVolume;
class UAudioComponent;
#include "DoorTrigger.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDESCAPEFPS_API UDoorTrigger : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorTrigger();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE bool IsTriggerActive() const
	{
		return (PressurePlateState == PRESSED);
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	enum PressurePlateState
	{
		PRESSED = 0,
		PRESSED_ANIM,
		// nice trick to find if current state is pressing or releasing
		UNPRESSED
	};


	void MovePlateDown(float DeltaTime);
	void MovePlateUp(float DeltaTime);
	float TotalMassOnPressurePlate() const;
	bool FindAudioComponent();

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY()
	UAudioComponent* PressurePlateAudio = nullptr;

	// time marker for last timetag when trigger was activated, after x seconds,
	//  we want the state to be changed to "PRESSED" so the door will activate itself
	float TriggerLastClosed = 0.f;
	UPROPERTY(EditAnywhere)
	float TriggerOpenTimeout = 2.f;
	UPROPERTY(EditAnywhere)
	float MassToActivateDoor = 60.f;

	// what is the unpressed Height of the pressure plate, for animation
	float UnpressedHeight;
	// what is the pressed Height of the pressure plate, for animation
	float PressedHeight;
	// what is the current Height of the pressure plate, for animation
	float CurrentHeight;
	// if it's UNPRESSED and changes to PRESSED, sound is played
	// same otherwise
	PressurePlateState PressurePlateState = UNPRESSED;
		
};
