// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "HeadMountedDisplay.h"
#include "WidgetComponent.h"
#include "WidgetInteractionComponent.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "VRPawn.generated.h"

UCLASS()
class TRANSFORMATIONVR_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    //Root component
    UPROPERTY(VisibleAnywhere, Category = "VRPawn")
    USceneComponent * DefaultSceneRoot;
    //Camara del HMD
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VRPawn")
    UCameraComponent * VRCamera;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VRPawn")
    class UMotionControllerComponent * MotionControllerLeft;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VRPawn")
    class UMotionControllerComponent * MotionControllerRight;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VRPawn")
    UStaticMeshComponent * ViveControllerLeft;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VRPawn")
    UStaticMeshComponent * ViveControllerRight;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VRPawn")
    UWidgetInteractionComponent * Interaction;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VRPawn")
    UParticleSystemComponent * Laser;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VRPawn")
    UParticleSystemComponent * EfectoImpacto;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VRPawn")
    UFloatingPawnMovement * Movimiento;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRPawn")
    TArray<UParticleSystem *> Lasers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRPawn")
    bool bPadDerecho;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRPawn")
    float Velocidad;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRPawn")
    float LaserIndice;

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void CambiarLaser(int Indice);

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    int LaserActual();

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void CambiarPuntoFinal(FVector PuntFinal);

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void PadDerechoPressed();

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void PadDerechoReleased();

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void SelectPressed();

    UFUNCTION(BlueprintCallable, Category = "VRPawn")
    void SelectReleased();

	
	
};
