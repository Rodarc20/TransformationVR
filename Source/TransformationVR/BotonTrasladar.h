// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boton.h"
#include "BotonTrasladar.generated.h"

/**
 * 
 */
UCLASS()
class TRANSFORMATIONVR_API ABotonTrasladar : public ABoton
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABotonTrasladar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UStaticMeshComponent * Borde;
	
	
};
