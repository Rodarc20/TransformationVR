// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Nodo.generated.h"

UCLASS()
class TRANSFORMATIONVR_API ANodo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANodo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jerarquia")
    UWidgetComponent * Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jerarquia")
	bool bActualizado;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jerarquia")
	FString NombreParte;//lenar al construir o depues de construir

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jerarquia")
	FString TextoTraslacion;//lenar al construir o depues de construir

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jerarquia")
	FString TextoRotacion;//lenar al construir o depues de construir

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jerarquia")
	FVector Traslacion;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jerarquia")
	FVector Rotacion;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jerarquia")
	int IdParte;

	UFUNCTION(BlueprintCallable, Category = "Jerarquia")
	void CambiarNombreParte(int NuevoIdParte);

	UFUNCTION(BlueprintCallable, Category = "Jerarquia")
	void CambiarTraslacion(FVector NuevaTraslacion);

	UFUNCTION(BlueprintCallable, Category = "Jerarquia")
	void CambiarRotacion(FVector NuevaRotacion);

	UFUNCTION(BlueprintCallable, Category = "Jerarquia")
	void ActualizarTextTraslacion();

	UFUNCTION(BlueprintCallable, Category = "Jerarquia")
	void ActualizarTextRotacion();

	
	
};
