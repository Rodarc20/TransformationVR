// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRPawn.h"
#include "Bloque.h"
#include "Casa.generated.h"

/*UENUM(BlueprintType)
enum class EVRCasaTask : uint8 {
    EArmarTask UMETA(DisplayName = "Armar"),
    EPlayTask UMETA(DisplayName = "Play"),
    ENoTask UMETA(DisplayName = "Ninguno")
};*/

UCLASS()
class TRANSFORMATIONVR_API ACasa : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACasa();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
