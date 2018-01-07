// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parte.h"
#include "Components/SphereComponent.h"
#include "PieIzquierdo.generated.h"

/**
 * 
 */
UCLASS()
class TRANSFORMATIONVR_API APieIzquierdo : public AParte
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	APieIzquierdo();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * PieI;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * ArticulacionTobilloI;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	USphereComponent * ColisionTobilloI;
	
	
	
};
