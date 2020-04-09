// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
class ATriggerVolume;
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:


	void MovePlateDown(float DeltaTime);
	void MovePlateUp(float DeltaTime);
	float TotalMassOnPressurePlate() const;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	float MassToActivateDoor = 60.f;
	float UnpressedHeight;
	float PressedHeight;
	float CurrentHeight;
		
};
