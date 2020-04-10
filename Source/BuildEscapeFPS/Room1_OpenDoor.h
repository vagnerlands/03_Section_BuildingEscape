// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
class UAudioComponent;
class UDoorTrigger;
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
	bool FindAudioComponent() ;

	// initial Door Yaw angle
	float InitialYawAngle;
	float CurrentYawAngle;
	AudioState DoorState = DOOR_CLOSE;

	// Final angle for the door open
	UPROPERTY(EditAnywhere)		
	float FinalYawAngle = 90.f;

	UPROPERTY(EditAnywhere)
	AActor* PressurePlate = nullptr;
	UDoorTrigger* PressurePlateDoorTrigger = nullptr;

	UPROPERTY()
	UAudioComponent* OpenDoorAudio = nullptr;

	UPROPERTY(EditAnywhere)
	float OpenDoorSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	float CloseDoorSpeed = 2.f;
};
