// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bloque.h"
#include "Techito.generated.h"

/**
 * 
 */
UCLASS()
class TRANSFORMATIONVR_API ATechito : public ABloque
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATechito();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation")
    FVector PosicionObjetivo;
	
	virtual void WidgetSeguir() override;

	virtual void WidgetNoSeguir() override;
	
	
};
