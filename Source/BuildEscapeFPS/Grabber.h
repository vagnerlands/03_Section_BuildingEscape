// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDESCAPEFPS_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	// implementation of start grab
	void Grab();

	// implementation of release (stop) grab
	void Release();
	
	void SetupPhysicsHandle();
	void SetupInputComponent();

	FHitResult GetFirstPhysicsBodyInReach();

	void Update();


	// Eye location 
	FVector EyeLocation;
	// Eye Direction
	FRotator EyeDirection;

	// Input Component (defined in BeginPlay)
	UInputComponent* InputComponent = nullptr;


	// --------------------------------
	//       GRAB RELATED MEMBERS
	// --------------------------------
	// Grabbed object (if there is any)
	UPrimitiveComponent* ObjectInHands = nullptr;
	// Physics Handle (defined in Blueprint)
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	FVector StartLocation;
	FVector EndLocation;

};
