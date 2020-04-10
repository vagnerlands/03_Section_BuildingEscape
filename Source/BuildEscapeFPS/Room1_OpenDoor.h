// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
class ATriggerVolume;
class UAudioComponent;
#include "Room1_OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDESCAPEFPS_API URoom1_OpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URoom1_OpenDoor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	enum AudioState
	{
		DOOR_CLOSE = 0,
		DOOR_OPEN
	};

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOnPressurePlate() const;
	bool FindAudioComponent() ;

	// initial Door Yaw angle
	float InitialYawAngle;
	float CurrentYawAngle;
	float DoorLastOpen = 0.f;
	float DoorLastClose = 0.f;
	AudioState DoorState = DOOR_CLOSE;

	// mass to activate the door - if total mass in the Pressure plate
	// is greater than this parameter, then it will open the door
	UPROPERTY(EditAnywhere)
	float MassToActivateDoor = 1.f;

	// Final angle for the door open
	UPROPERTY(EditAnywhere)		
	float FinalYawAngle = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY()
	UAudioComponent* AudioOpenDoor = nullptr;

	//UPROPERTY(EditAnywhere)
	//AActor* ActorThatOpens;

	UPROPERTY(EditAnywhere)
	float OpenDoorSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	float CloseDoorSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	// for how many seconds the door remains open before starting closing
	float DoorOpenTimeout = 1.f;
};
