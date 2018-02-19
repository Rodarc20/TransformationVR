// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "PilaOpenGL.generated.h"

UCLASS()
class TRANSFORMATIONVR_API APilaOpenGL : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APilaOpenGL();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jerarquia")
	bool bActualizado;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jerarquia")
    UWidgetComponent * Widget;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jerarquia")
	FString Codigo;//lenar al construir o depues de construir
	
	UFUNCTION(BlueprintCallable, Category = "Jerarquia")
	void CambiarCodigo(FString NuevoCodigo);

    UFUNCTION(BlueprintCallable, Category = "Jerarquia")
    void Mostrar();

    UFUNCTION(BlueprintCallable, Category = "Jerarquia")
    void Ocultar();
};
