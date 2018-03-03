// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PruebaInput.generated.h"

UCLASS()
class TRANSFORMATIONVR_API APruebaInput : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APruebaInput();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

    void PadIzquierdoPressed();

    void PadIzquierdoReleased();
	
	
};
