// Fill out your copyright notice in the Description page of Project Settings.

#include "Robot.h"
#include "VRPawn.h"
#include "Parte.h"
#include "Jerarquia.h"
#include "PilaOpenGL.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "TransformacionWidget.h"


// Sets default values
ARobot::ARobot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    static ConstructorHelpers::FClassFinder<APilaOpenGL> PilaClass(TEXT("BlueprintGeneratedClass'/Game/Trasnformation/Blueprints/Jerarquia/PilaOpenGL_BP.PilaOpenGL_BP_C'"));
    if (PilaClass.Succeeded()) {
        //if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
            //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Pila encontrado."));
        TipoPila = PilaClass.Class;
    }

	DistanciaLaserMaxima = 200.0f;

	//CurrentJerarquiaTask = EVRJerarquiaTask::EArmarTask;
	CurrentJerarquiaTask = EVRJerarquiaTask::ERotationTask;
}

// Called when the game starts or when spawned
void ARobot::BeginPlay()
{
	Super::BeginPlay();
	
	Partes.SetNum(10);

	TArray<AActor *> PartesEncontradas;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AParte::StaticClass(), PartesEncontradas);
	//UE_LOG(LogClass, Log, TEXT("Numero de Partes Encontradas: %d"), PartesEncontradas.Num());
	for (int i = 0; i < PartesEncontradas.Num(); i++) {
		//if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("PARTES ENCONTRADAS"));
		//UE_LOG(LogClass, Log, TEXT("Partes Encontradas %d"), i);
		AParte * const ParteEncontrada = Cast<AParte>(PartesEncontradas[i]);
		if(ParteEncontrada)
			Partes[ParteEncontrada->Id] = ParteEncontrada;
	}

	//Instanciando jerarquia
	UWorld * const World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		FVector SpawnLocation(-210.0f, 90.0f, 130.0f);

		Jerarquia = World->SpawnActor<AJerarquia>(AJerarquia::StaticClass(), SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	}

	for (int i = 0; i < 10 && i < Partes.Num(); i++) {//asociando partes a la jerarquia
		Jerarquia->TransformacionesPartes[i].ParteAsociada = Partes[i];
		Jerarquia->TransformacionesPartes[i].ActualizarDesdeParte();
	}

	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		FVector SpawnLocation(-210.0f, -110.0f, 140.0f);

		PilaCodigo = World->SpawnActor<APilaOpenGL>(TipoPila, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		if (Jerarquia)
			Jerarquia->PilaCodigo = PilaCodigo;
	}

    AVRPawn * MyVRPawn = Cast<AVRPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (MyVRPawn) {
		Usuario = MyVRPawn;
		MyVRPawn->Jerarquia = Jerarquia;
        RightController = MyVRPawn->MotionControllerRight;
        LeftController = MyVRPawn->MotionControllerLeft;
	}
}

// Called every frame
void ARobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//esto tal vez tenga pque pasarse al blueprint
	//BuscandoComponenteRotacionConLaser();
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {//si el juego esta en Playing
            //activate the spawn volumes
			BuscandoParteConLaser();//por ahora, pero este no va aqui

        }
        break;//no se como funciona esto
        case EVRJerarquiaTask::ERotationTask: {//si  perdimos el juego
			if (ParteSeleccionada) {
				if (EjeSeleccionado != ETransformacionEje::ENone) {
					//se supone que cuando se selecciono se grabo la posicion en el presesed
					//BuscarIntereseccionEjeRotacion();
					RotarParteEnEje();
				}
				else {
					BuscandoComponenteRotacionConLaser();
					if (HitEje == ETransformacionEje::ENone) {
						BuscandoParteConLaser();//por ahora, pero este no va aqui
					}
				}
			}
			else {
				BuscandoParteConLaser();
			}
			//una vez que tenga una parte si la selecciono recien se ahbilita y busco componentes

        }
        break;//no se como funciona esto
        default:
        case EVRJerarquiaTask::ENoTask: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }
}

FVector ARobot::BuscarParte(AParte *& ParteEncontrada) {
    FCollisionQueryParams ParteTraceParams = FCollisionQueryParams(FName(TEXT("TraceParte")), true, this);
    FVector PuntoInicial = RightController->GetComponentLocation();//lo mismo que en teorioa, GetComponentTransfor().GetLocation();
    FVector Vec = RightController->GetForwardVector();
    FVector PuntoFinal = PuntoInicial + Vec*DistanciaLaserMaxima;
    //PuntoInical = PuntoInicial + Vec * 10;//para que no se choque con lo que quiero, aun que no deberia importar
    TArray<TEnumAsByte<EObjectTypeQuery> > TiposObjetos;
    TiposObjetos.Add(EObjectTypeQuery::ObjectTypeQuery7);//Nodo
    //TiposObjetos.Add(EObjectTypeQuery::ObjectTypeQuery2);//World dynamic, separado esta funcionando bien, supongo que tendre que hacer oto trace par saber si me estoy chocando con la interfaz, y no tener encuenta esta busqueda
    //podria agregar los world static y dynamic, para asi avitar siempre encontrar algun nodo que este destrar de algun menu, y que por seleccionar en el menu tambien le de click a el
    TArray<AActor*> vacio;
    FHitResult Hit;
    bool trace = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), PuntoInicial, PuntoFinal, TiposObjetos, false, vacio, EDrawDebugTrace::None, Hit, true, FLinearColor::Blue);//el none es para que no se dibuje nada
    //hit se supone que devovera al actor y el punto de impacto si encontró algo, castearlo a nodo, y listo
    if (trace) {
        //solo que al agregar el worldynamic ,tengo que castear y verificar
        ParteEncontrada = Cast<AParte>(Hit.Actor.Get());
        /*if (NodoEncontrado) {//no estaba
            //en que momento debo incluir la seccion del label? despues de todo esto, en otra funcion, o en este mismo codigo?
            return Hit.ImpactPoint;
        }*/
        //DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 1.5f, 6, FColor::Black, false, 0.0f, 0, 0.25f);
        
        return Hit.ImpactPoint;//si quito toto el if anterior debo activar esta linea, y liesto
    }
    //y si esta funcion es solo para esto, y luego ya verifico si es tal o cual cosa, en otra parte del codigo?

    //DrawDebugLine(GetWorld(), SourceNodo->GetActorLocation(), TargetNodo->GetActorLocation(), FColor::Black, false, -1.0f, 0, Radio*Escala);
    ParteEncontrada = nullptr;
    return FVector::ZeroVector;// los casos manejarlos afuera
}

void ARobot::BuscandoParteConLaser() {
    AParte * ParteEncontrada;
    FVector PuntoImpacto = BuscarParte(ParteEncontrada);
    //if (ParteEncontrada && !Interaction->IsOverHitTestVisibleWidget()) {//comprobamos la interaccion para que no se detecte lo que este detras del menu
    if (ParteEncontrada) {//comprobamos la interaccion para que no se detecte lo que este detras del menu
		//podre interactuar con los menus??
        if (HitParte && HitParte != ParteEncontrada) {//quiza se pueda hacer con el boleano, debo ocultar si es que es diferente al de ahora,
            //if (MostrarLabel) {//no sera necesario
                //HitNodo->OcultarNombre();
                //NodoEncontrado->MostrarNombre();
            //}
			//que se resalte en color blanco
            HitParte->DesactivarResaltado();//esto si, podria ser para saber que le estoy dando
            ParteEncontrada->ActivarResaltado();
        }
        else {
            //if (MostrarLabel) {
                //NodoEncontrado->MostrarNombre();
            //}
            if(!ParteEncontrada->bResaltada)
                ParteEncontrada->ActivarResaltado();
        }
        HitParte= ParteEncontrada;//podria dejar ests 3 lineas, y borrar las de adentro
        bHitParte= true;
        ImpactPoint = PuntoImpacto;
		//ParteSeleccionada = ParteEncontrada;//sacar esto , es temporal solo para prubas, pero intentar eliminarlo
    }
    else {//si no estoy golpeando algun nodo
        if (HitParte) {
            //if (MostrarLabel) {
                //HitNodo->OcultarNombre();
            //.}
            HitParte->DesactivarResaltado();
            //HitNodo = nullptr;
            //bHitNodo = false;
            //ImpactPoint = PuntoImpacto;
        }
        HitParte = nullptr;
        bHitParte = false;
        ImpactPoint = PuntoImpacto;
        //el caso contrario, seria encontrar como lo deje con el if anterior, asi que no se hace nada
    }
    //todo esto podria ser una sola funcion
    //hasta aqui he verificado si encontre algun nodo, pero no si encotnre un, menu, y tampoo he ejecutado los cambios visuales
    //hagamos algo visual, antes de incluir los menus
	if (bHitParte) {//quiza la verificacion que hago sobre si hubo cambio o no de hit nodo, ayude a evitar ciertos calculos, tal vez, por ejemplo si el laser siempre esa al maximo, no tiene mucho sentido seimpre setear con el mismo valor
		Usuario->EfectoImpacto->SetWorldLocation(ImpactPoint);
		//esto no estaba
		//if (MostrarLabel && HitNodo->Nombre->IsVisible()) {//esto podria ser util si tengo elemntos que necesito que roten en la propia parte
			//HitNodo->ActualizarRotacionNombre(Usuario->VRCamera->GetComponentLocation());
		//}
		if (Usuario->LaserActual() != 1) {
			Usuario->CambiarLaser(1);
		}
		if (!Usuario->EfectoImpacto->IsActive()) {//para el caso del puntero, no lo usao ahora
			Usuario->EfectoImpacto->ActivateSystem();
		}
		Usuario->CambiarPuntoFinal(ImpactPoint);
	}
	else {
		Usuario->EfectoImpacto->SetWorldLocation(RightController->GetComponentLocation() + RightController->GetForwardVector()*DistanciaLaserMaxima);
		//esta funcion deberia administrar le punto recbido, y verficar si acutalmente el puntero de interaccion esta sobre el menu, y tomar el adecuado para cada situacion
		if (Usuario->LaserActual() != 0) {
			Usuario->CambiarLaser(0);
		}
		if (Usuario->EfectoImpacto->IsActive()) {
			Usuario->EfectoImpacto->DeactivateSystem();
		}
		Usuario->CambiarPuntoFinal(RightController->GetComponentLocation() + RightController->GetForwardVector()*DistanciaLaserMaxima);//debieria tener un punto por defecto, pero mejor lo dejamos asi
	}

    //creo que la parte de interacion con el menu, deberia estar manajedo por el pawn, asi dentro de la funcion cambiar punto final, evaluo o verifico que no este primero algun menu
    //la pregunta es como hare con los clicks digamos para el contenido, si estoy buscando algun nodo, quiza igual deberia evitar que de algun click, si tengo algun overlap en ferente, evaluar la mejor forma de hacer todo esto
    //o usar esto en lugar de un trace solo que debo hacer esto antes de que haga cambios visuales, obtener el punto y evaluar,  antes de setear lo de hit nodo y dema
}

void ARobot::BuscandoComponenteRotacionConLaser() {
    FVector PuntoInicial = RightController->GetComponentLocation();//lo mismo que en teorioa, GetComponentTransfor().GetLocation();
    FVector Vec = RightController->GetForwardVector();
    FVector PuntoFinal = PuntoInicial + Vec*DistanciaLaserMaxima;
	ETransformacionEje EjeEncontrado;
	FVector Impacto;
	//UE_LOG(LogClass, Log, TEXT("Buscando componente"));
	if (ParteSeleccionada->TWidget->ColisionRotacion(PuntoInicial, PuntoFinal, EjeEncontrado, Impacto)) {//si he colisionado con algo
		// igual que el mecanismo de las partes
		//activar el resaltadao de la rotacion, pero tambien cmaibne rl aposicino del laser
		//UE_LOG(LogClass, Log, TEXT("Componente encontrado"));
		HitEje = EjeEncontrado;
		Usuario->CambiarPuntoFinal(Impacto);
		Usuario->EfectoImpacto->SetWorldLocation(Impacto);
		if (Usuario->LaserActual() != 2) {
			Usuario->CambiarLaser(4);
		}
		if (!Usuario->EfectoImpacto->IsActive()) {//para el caso del puntero, no lo usao ahora
			Usuario->EfectoImpacto->ActivateSystem();
		}
		Usuario->CambiarPuntoFinal(Impacto);
		Usuario->EfectoImpacto->SetWorldLocation(Impacto);
	}
	else {
		HitEje = ETransformacionEje::ENone;
		Usuario->EfectoImpacto->SetWorldLocation(RightController->GetComponentLocation() + RightController->GetForwardVector()*DistanciaLaserMaxima);
		//esta funcion deberia administrar le punto recbido, y verficar si acutalmente el puntero de interaccion esta sobre el menu, y tomar el adecuado para cada situacion
		if (Usuario->LaserActual() != 0) {
			Usuario->CambiarLaser(0);
		}
		if (Usuario->EfectoImpacto->IsActive()) {
			Usuario->EfectoImpacto->DeactivateSystem();
		}
		Usuario->CambiarPuntoFinal(RightController->GetComponentLocation() + RightController->GetForwardVector()*DistanciaLaserMaxima);//debieria tener un punto por defecto, pero mejor lo dejamos asi
	}
}

bool ARobot::ColisionRotacion(FVector Inicio, FVector Fin, ETransformacionEje & EjeColisionado, FVector & Impact) {
    FCollisionQueryParams ParteTraceParams = FCollisionQueryParams(FName(TEXT("TraceX")), true);
	FHitResult Hit;
    TArray<TEnumAsByte<EObjectTypeQuery> > TiposObjetos;
    TiposObjetos.Add(EObjectTypeQuery::ObjectTypeQuery9);//Nodo
    TArray<AActor*> vacio;
    bool trace = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Inicio, Fin, TiposObjetos, true, vacio, EDrawDebugTrace::None, Hit, true, FLinearColor::Blue);//el none es para que no se dibuje nada
	if (trace) {
		UE_LOG(LogClass, Log, TEXT("Encontrado"));
		Impact = Hit.ImpactPoint;
		return true;
	}
	Impact = FVector::ZeroVector;
	EjeColisionado = ETransformacionEje::ENone;
	return false;
}

void ARobot::SetJerarquiaTask(EVRJerarquiaTask NewJerarquiaTask) {
	CurrentJerarquiaTask = NewJerarquiaTask;
}

EVRJerarquiaTask ARobot::GetJerarquiaTask() {
	return CurrentJerarquiaTask;
}

void ARobot::BuscarIntereseccionEjeRotacion() {
    switch (EjeSeleccionado) {
        case ETransformacionEje::EEjeX: {
			PuntoInterseccion = ParteSeleccionada->TWidget->InterseccionLineaPlanoYZ(RightController->GetComponentLocation(), RightController->GetForwardVector());
			if (PuntoInterseccion.X != -1.0f) {//punto imposible
				Usuario->CambiarLaser(1);
				//este trasnfor debe ser con el trasnform del la parte el transform del objeto twidget
				//deberia encapsular eso en una funcion para trasnforar en funcion del trasnfor copia o la del objeto adecuado
				//FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->GetComponentTransform().TransformPosition(PuntoInterseccion);
				FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->TransformTemporal.TransformPosition(PuntoInterseccion);
				Usuario->CambiarPuntoFinal(PuntoInterseccionWorld);//convierte de local a global, para que sea usado por el vrpawn que lo usa en espacio global
				Usuario->EfectoImpacto->SetWorldLocation(PuntoInterseccionWorld);
				//dibujar laser apropiado
			}
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeY: {
			PuntoInterseccion = ParteSeleccionada->TWidget->InterseccionLineaPlanoXZ(RightController->GetComponentLocation(), RightController->GetForwardVector());
			if (PuntoInterseccion.Y != -1.0f) {//punto imposible
				Usuario->CambiarLaser(1);
				//este trasnfor debe ser con el trasnform del la parte el transform del objeto twidget
				//deberia encapsular eso en una funcion para trasnforar en funcion del trasnfor copia o la del objeto adecuado
				//FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->GetComponentTransform().TransformPosition(PuntoInterseccion);
				FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->TransformTemporal.TransformPosition(PuntoInterseccion);
				Usuario->CambiarPuntoFinal(PuntoInterseccionWorld);//convierte de local a global, para que sea usado por el vrpawn que lo usa en espacio global
				Usuario->EfectoImpacto->SetWorldLocation(PuntoInterseccionWorld);
				//dibujar laser apropiado
			}
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeZ: {
			PuntoInterseccion = ParteSeleccionada->TWidget->InterseccionLineaPlanoXY(RightController->GetComponentLocation(), RightController->GetForwardVector());
			if (PuntoInterseccion.Z != -1.0f) {//punto imposible
				Usuario->CambiarLaser(1);
				//este trasnfor debe ser con el trasnform del la parte el transform del objeto twidget
				//deberia encapsular eso en una funcion para trasnforar en funcion del trasnfor copia o la del objeto adecuado
				//FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->GetComponentTransform().TransformPosition(PuntoInterseccion);
				FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->TransformTemporal.TransformPosition(PuntoInterseccion);
				Usuario->CambiarPuntoFinal(PuntoInterseccionWorld);//convierte de local a global, para que sea usado por el vrpawn que lo usa en espacio global
				Usuario->EfectoImpacto->SetWorldLocation(PuntoInterseccionWorld);
				//dibujar laser apropiado
			}
        }
        break;//no se como funciona esto
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }

	UE_LOG(LogClass, Log, TEXT("Punto Intereseccino Inicial: %f, %f, %f"), PuntoInterseccion.X, PuntoInterseccion.Y, PuntoInterseccion.Z);
}

void ARobot::RotarParteEnEje() {
	BuscarIntereseccionEjeRotacion();//buscando punto, retorna el punto local en pUnto Interseccion
	//en funcon del eje selccionado acutal debo hacer calculos para saber el angulo de diferencia
    switch (EjeSeleccionado) {
        case ETransformacionEje::EEjeX: {
			FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->TransformTemporal.TransformPosition(PuntoInterseccion);

			//esta forma funciona bien, pero gira en sentido diferente
			//float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoInterseccion.GetClampedToSize(1.0f, 1.0f), PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f))));
			//float DeltaSing = FVector::CrossProduct(PuntoInterseccion.GetClampedToSize(1.0f, 1.0f), PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f)).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no

			float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoInterseccion.GetSafeNormal(), PuntoRotacionInicial.GetSafeNormal())));
			float DeltaSing = FVector::CrossProduct(PuntoInterseccion.GetSafeNormal(), PuntoRotacionInicial.GetSafeNormal()).X;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no

			//float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f), PuntoInterseccion.GetClampedToSize(1.0f, 1.0f))));
			//float DeltaSing = FVector::CrossProduct(PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f), PuntoInterseccion.GetClampedToSize(1.0f, 1.0f)).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no
			if (DeltaSing >= 0) {
				DeltaAngle = 360-DeltaAngle;
			}
			UE_LOG(LogClass, Log, TEXT("DeltaAngle: %f"), DeltaAngle);
			UE_LOG(LogClass, Log, TEXT("Rotator Inicial: %f, %f, %f"), RotacionInicial.Roll, RotacionInicial.Pitch, RotacionInicial.Yaw);
			//FRotator VariacionRotation(DeltaAngle, 0.0f, 0.0f);
			FRotator VariacionRotation(0.0f, 0.0f, -DeltaAngle);
			UE_LOG(LogClass, Log, TEXT("Variacion Rotacion: %f, %f, %f"), VariacionRotation.Roll, VariacionRotation.Pitch, VariacionRotation.Yaw);
			ParteSeleccionada->SetActorRelativeRotation(RotacionInicial);
			ParteSeleccionada->AddActorLocalRotation(VariacionRotation);
			//consigue la rotacion sin errores pero siempre en sentido contrario!!! 
			FRotator RotacionCambiada = ParteSeleccionada->GetRootComponent()->GetRelativeTransform().GetRotation().Rotator();
			UE_LOG(LogClass, Log, TEXT("Rotator Cambiado: %f, %f, %f"), RotacionCambiada.Roll, RotacionCambiada.Pitch, RotacionCambiada.Yaw);
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeY: {
			FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->TransformTemporal.TransformPosition(PuntoInterseccion);

			//esta forma funciona bien, pero gira en sentido diferente
			//float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoInterseccion.GetClampedToSize(1.0f, 1.0f), PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f))));
			//float DeltaSing = FVector::CrossProduct(PuntoInterseccion.GetClampedToSize(1.0f, 1.0f), PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f)).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no

			float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoInterseccion.GetSafeNormal(), PuntoRotacionInicial.GetSafeNormal())));
			float DeltaSing = FVector::CrossProduct(PuntoInterseccion.GetSafeNormal(), PuntoRotacionInicial.GetSafeNormal()).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no

			//float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f), PuntoInterseccion.GetClampedToSize(1.0f, 1.0f))));
			//float DeltaSing = FVector::CrossProduct(PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f), PuntoInterseccion.GetClampedToSize(1.0f, 1.0f)).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no
			if (DeltaSing >= 0) {
				DeltaAngle = 360-DeltaAngle;
			}
			//if (DeltaSing >= 0) {
				//DeltaAngle = DeltaAngle * -1;
			//}
			//por ahora aplicar la rotacion
			UE_LOG(LogClass, Log, TEXT("DeltaAngle: %f"), DeltaAngle);
			UE_LOG(LogClass, Log, TEXT("Rotator Inicial: %f, %f, %f"), RotacionInicial.Roll, RotacionInicial.Pitch, RotacionInicial.Yaw);
			//FRotator VariacionRotation(DeltaAngle, 0.0f, 0.0f);
			FRotator VariacionRotation(-DeltaAngle, 0.0f, 0.0f);
			//ParteSeleccionada->SetActorRelativeRotation(RotacionInicial + VariacionRotation);
			//ParteSeleccionada->AddActorLocalRotation(VariacionRotation);

			//FRotator VariacionRotation = RotacionInicial;
			//VariacionRotation.Add(DeltaAngle, 0.0f, 0.0f);
			UE_LOG(LogClass, Log, TEXT("Variacion Rotacion: %f, %f, %f"), VariacionRotation.Roll, VariacionRotation.Pitch, VariacionRotation.Yaw);
			//ParteSeleccionada->SetActorRelativeRotation(VariacionRotation);


			ParteSeleccionada->SetActorRelativeRotation(RotacionInicial);
			ParteSeleccionada->AddActorLocalRotation(VariacionRotation);
			//consigue la rotacion sin errores pero siempre en sentido contrario!!! 


			FRotator RotacionCambiada = ParteSeleccionada->GetRootComponent()->GetRelativeTransform().GetRotation().Rotator();
			UE_LOG(LogClass, Log, TEXT("Rotator Cambiado: %f, %f, %f"), RotacionCambiada.Roll, RotacionCambiada.Pitch, RotacionCambiada.Yaw);
        }
        break;//no se como funciona esto
        case ETransformacionEje::EEjeZ: {
			FVector PuntoInterseccionWorld = ParteSeleccionada->TWidget->TransformTemporal.TransformPosition(PuntoInterseccion);

			//esta forma funciona bien, pero gira en sentido diferente
			//float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoInterseccion.GetClampedToSize(1.0f, 1.0f), PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f))));
			//float DeltaSing = FVector::CrossProduct(PuntoInterseccion.GetClampedToSize(1.0f, 1.0f), PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f)).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no

			float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoInterseccion.GetSafeNormal(), PuntoRotacionInicial.GetSafeNormal())));
			float DeltaSing = FVector::CrossProduct(PuntoInterseccion.GetSafeNormal(), PuntoRotacionInicial.GetSafeNormal()).Z;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no

			//float DeltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f), PuntoInterseccion.GetClampedToSize(1.0f, 1.0f))));
			//float DeltaSing = FVector::CrossProduct(PuntoRotacionInicial.GetClampedToSize(1.0f, 1.0f), PuntoInterseccion.GetClampedToSize(1.0f, 1.0f)).Y;//esto es por que el signo es impotante para saber si fue un angulo mayor de 180 o no
			if (DeltaSing >= 0) {
				DeltaAngle = 360-DeltaAngle;
			}
			UE_LOG(LogClass, Log, TEXT("DeltaAngle: %f"), DeltaAngle);
			UE_LOG(LogClass, Log, TEXT("Rotator Inicial: %f, %f, %f"), RotacionInicial.Roll, RotacionInicial.Pitch, RotacionInicial.Yaw);
			//FRotator VariacionRotation(DeltaAngle, 0.0f, 0.0f);
			FRotator VariacionRotation(0.0f, DeltaAngle, 0.0f);
			UE_LOG(LogClass, Log, TEXT("Variacion Rotacion: %f, %f, %f"), VariacionRotation.Roll, VariacionRotation.Pitch, VariacionRotation.Yaw);
			ParteSeleccionada->SetActorRelativeRotation(RotacionInicial);
			ParteSeleccionada->AddActorLocalRotation(VariacionRotation);
			//consigue la rotacion sin errores pero siempre en sentido contrario!!! 
			FRotator RotacionCambiada = ParteSeleccionada->GetRootComponent()->GetRelativeTransform().GetRotation().Rotator();
			UE_LOG(LogClass, Log, TEXT("Rotator Cambiado: %f, %f, %f"), RotacionCambiada.Roll, RotacionCambiada.Pitch, RotacionCambiada.Yaw);
        }
        break;//no se como funciona esto
        default:
        case ETransformacionEje::ENone: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }

}

void ARobot::SelectPressed() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {//si el juego esta en Playing
            //activate the spawn volumes
			BuscandoParteConLaser();//por ahora, pero este no va aqui

        }
        break;//no se como funciona esto
        case EVRJerarquiaTask::ERotationTask: {//si  perdimos el juego
			if (ParteSeleccionada) {
				if (HitEje != ETransformacionEje::ENone) {
					EjeSeleccionado = HitEje;
					ParteSeleccionada->TWidget->SeleccionarEjeRotacion(EjeSeleccionado);
					//ParteSeleccionada->TWidget->CopiarTransform();
					ParteSeleccionada->CopiarTransform();
					RotacionInicial = ParteSeleccionada->GetRootComponent()->GetRelativeTransform().GetRotation().Rotator();
					BuscarIntereseccionEjeRotacion();
					PuntoRotacionInicial = PuntoInterseccion;

				}
				else {
					if (HitParte && HitParte == ParteSeleccionada) {
						ParteSeleccionada->DesactivarResaltado();
						ParteSeleccionada->TWidget->OcultarWidgetRotacion();
						ParteSeleccionada = nullptr;
					}
				}
				//rotar, es decir ver si hay componete, para capturar su plano, y empezar a hacer los calculos, aqui entra lo de la rotacion de ramas
				//si hay algun componente seleccionado, o algo, debo marcarlo por ahora
			}
			else {
				if (bHitParte && HitParte) {
					EjeSeleccionado = ETransformacionEje::ENone;
					HitEje = ETransformacionEje::ENone;
					ParteSeleccionada = HitParte;
					ParteSeleccionada->ActivarResaltado();
					ParteSeleccionada->TWidget->MostrarWidgetRotacion();
				}
			}
			//una vez que tenga una parte si la selecciono recien se ahbilita y busco componentes

        }
        break;//no se como funciona esto
        default:
        case EVRJerarquiaTask::ENoTask: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }
}

void ARobot::SelectReleased() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {//si el juego esta en Playing
            //activate the spawn volumes
			BuscandoParteConLaser();//por ahora, pero este no va aqui

        }
        break;//no se como funciona esto
        case EVRJerarquiaTask::ERotationTask: {//si  perdimos el juego
			if (ParteSeleccionada) {
				if(EjeSeleccionado != ETransformacionEje::ENone)
					ParteSeleccionada->TWidget->DeseleccionarEjeRotacion(EjeSeleccionado);
				//se edberian aplicar las rotaciones permanentemente
				EjeSeleccionado = ETransformacionEje::ENone;
				//rotar, es decir ver si hay componete, para capturar su plano, y empezar a hacer los calculos, aqui entra lo de la rotacion de ramas
				//si hay algun componente seleccionado, o algo, debo marcarlo por ahora
			}
			else {
			}
			//una vez que tenga una parte si la selecciono recien se ahbilita y busco componentes

        }
        break;//no se como funciona esto
        default:
        case EVRJerarquiaTask::ENoTask: {//unknown/ default state
            //no hacer nada
        }
        break;//no se como funciona esto
    }
}
