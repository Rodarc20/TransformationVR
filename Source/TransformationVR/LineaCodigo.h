// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "LineaCodigo.generated.h"

UCLASS()
class TRANSFORMATIONVR_API ALineaCodigo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALineaCodigo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
    UWidgetComponent * Widget;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Transformation")
	FString Codigo;//lenar al construir o depues de construir
	
    UFUNCTION(BlueprintCallable, Category = "Trasngormation")
    void Mostrar();

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    void Ocultar();
	
	
};
