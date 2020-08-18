// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformationVRGameModeBase.h"
#include "VRPawn.h"
#include "Parte.h"
#include "Jerarquia.h"
#include "PilaOpenGL.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"


ATransformationVRGameModeBase::ATransformationVRGameModeBase() {
    PrimaryActorTick.bCanEverTick = true;//esto hacia falta en el tutprial, sin esto unreal no llama a la funcion tick en cada cuadro

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(TEXT("Class'/Script/TransformationVR.VRPawn'"));
	if (PlayerPawnClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnClass.Class;
	}

    /*static ConstructorHelpers::FClassFinder<APilaOpenGL> PilaClass(TEXT("BlueprintGeneratedClass'/Game/Trasnformation/Blueprints/Jerarquia/PilaOpenGL_BP.PilaOpenGL_BP_C'"));
    if (PilaClass.Succeeded()) {
        //if(GEngine)//no hacer esta verificación provocaba error al iniciar el editor
            //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Pila encontrado."));
        TipoPila = PilaClass.Class;
    }

	DistanciaLaserMaxima = 200.0f;

	//CurrentJerarquiaTask = EVRJerarquiaTask::EArmarTask;
	CurrentJerarquiaTask = EVRJerarquiaTask::ERotationTask;*/
}

void ATransformationVRGameModeBase::BeginPlay() {

	/*Partes.SetNum(10);

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
	}*/
}

void ATransformationVRGameModeBase::Tick(float DeltaTime) {

	//BuscandoComponenteRotacionConLaser();
    /*switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {//si el juego esta en Playing
            //activate the spawn volumes
			BuscandoParteConLaser();//por ahora, pero este no va aqui

        }
        break;//no se como funciona esto
        case EVRJerarquiaTask::ERotationTask: {//si  perdimos el juego
			if (ParteSeleccionada) {
				BuscandoComponenteRotacionConLaser();
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
    }*/
}

/*FVector ATransformationVRGameModeBase::BuscarParte(AParte *& ParteEncontrada) {
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
        }* /
        //DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 1.5f, 6, FColor::Black, false, 0.0f, 0, 0.25f);
        
        return Hit.ImpactPoint;//si quito toto el if anterior debo activar esta linea, y liesto
    }
    //y si esta funcion es solo para esto, y luego ya verifico si es tal o cual cosa, en otra parte del codigo?

    //DrawDebugLine(GetWorld(), SourceNodo->GetActorLocation(), TargetNodo->GetActorLocation(), FColor::Black, false, -1.0f, 0, Radio*Escala);
    ParteEncontrada = nullptr;
    return FVector::ZeroVector;// los casos manejarlos afuera
}

void ATransformationVRGameModeBase::BuscandoParteConLaser() {
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
		ParteSeleccionada = ParteEncontrada;//sacar esto , es temporal solo para prubas, pero intentar eliminarlo
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
		Usuario->CambiarPuntoFinal(ImpactPoint);
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
	}
	else {
		Usuario->CambiarPuntoFinal(RightController->GetComponentLocation() + RightController->GetForwardVector()*DistanciaLaserMaxima);//debieria tener un punto por defecto, pero mejor lo dejamos asi
		Usuario->EfectoImpacto->SetWorldLocation(RightController->GetComponentLocation() + RightController->GetForwardVector()*DistanciaLaserMaxima);
		//esta funcion deberia administrar le punto recbido, y verficar si acutalmente el puntero de interaccion esta sobre el menu, y tomar el adecuado para cada situacion
		if (Usuario->LaserActual() != 0) {
			Usuario->CambiarLaser(0);
		}
		if (Usuario->EfectoImpacto->IsActive()) {
			Usuario->EfectoImpacto->DeactivateSystem();
		}
	}

    //creo que la parte de interacion con el menu, deberia estar manajedo por el pawn, asi dentro de la funcion cambiar punto final, evaluo o verifico que no este primero algun menu
    //la pregunta es como hare con los clicks digamos para el contenido, si estoy buscando algun nodo, quiza igual deberia evitar que de algun click, si tengo algun overlap en ferente, evaluar la mejor forma de hacer todo esto
    //o usar esto en lugar de un trace solo que debo hacer esto antes de que haga cambios visuales, obtener el punto y evaluar,  antes de setear lo de hit nodo y dema
}

void ATransformationVRGameModeBase::BuscandoComponenteRotacionConLaser() {
    FVector PuntoInicial = RightController->GetComponentLocation();//lo mismo que en teorioa, GetComponentTransfor().GetLocation();
    FVector Vec = RightController->GetForwardVector();
    FVector PuntoFinal = PuntoInicial + Vec*DistanciaLaserMaxima;
	ETransformacionEje EjeColisionado;
	FVector Impacto;
	UE_LOG(LogClass, Log, TEXT("Buscando componente"));
	if (ParteSeleccionada->TWidget->ColisionRotacion(PuntoInicial, PuntoFinal, EjeColisionado, Impacto)) {//si he colisionado con algo
	//if (ColisionRotacion(PuntoInicial, PuntoFinal, EjeColisionado, Impacto)) {//si he colisionado con algo
		//activar el resaltadao de la rotacion, pero tambien cmaibne rl aposicino del laser
		UE_LOG(LogClass, Log, TEXT("Componente encontrado"));
		Usuario->CambiarPuntoFinal(Impacto);
		Usuario->EfectoImpacto->SetWorldLocation(Impacto);
		if (Usuario->LaserActual() != 2) {
			Usuario->CambiarLaser(4);
		}
		if (!Usuario->EfectoImpacto->IsActive()) {//para el caso del puntero, no lo usao ahora
			Usuario->EfectoImpacto->ActivateSystem();
		}
	}
}

bool ATransformationVRGameModeBase::ColisionRotacion(FVector Inicio, FVector Fin, ETransformacionEje & EjeColisionado, FVector & Impact) {
    FCollisionQueryParams ParteTraceParams = FCollisionQueryParams(FName(TEXT("TraceX")), true);
	FHitResult Hit;
	//UE_LOG(LogClass, Log, TEXT("Buscando componente"));
	/*if (ArcoX->LineTraceComponent(Hit, Inicio, Fin, ParteTraceParams)) {
		UE_LOG(LogClass, Log, TEXT("Encontrado X"));
		Impact = Hit.ImpactPoint;
		EjeColisionado = ETransformacionEje::EEjeX;
		return true;
	}
	//deberia quedarme con el mas cercano al inico de los 3, por ahora esta con el orden de los if
    ParteTraceParams = FCollisionQueryParams(FName(TEXT("TraceComponenteY")), true);
	if (ArcoY->LineTraceComponent(Hit, Inicio, Fin, ParteTraceParams)) {
		UE_LOG(LogClass, Log, TEXT("Encontrado Y"));
		Impact = Hit.ImpactPoint;
		EjeColisionado = ETransformacionEje::EEjeY;
		return true;
	}
    ParteTraceParams = FCollisionQueryParams(FName(TEXT("TraceComponenteZ")), true);
	if (ArcoZ->LineTraceComponent(Hit, Inicio, Fin, ParteTraceParams)) {
		UE_LOG(LogClass, Log, TEXT("Encontrado Z"));
		Impact = Hit.ImpactPoint;
		EjeColisionado = ETransformacionEje::EEjeZ;
		return true;
	}* /

    TArray<TEnumAsByte<EObjectTypeQuery> > TiposObjetos;
    TiposObjetos.Add(EObjectTypeQuery::ObjectTypeQuery9);//Nodo
    //TiposObjetos.Add(EObjectTypeQuery::ObjectTypeQuery2);//World dynamic, separado esta funcionando bien, supongo que tendre que hacer oto trace par saber si me estoy chocando con la interfaz, y no tener encuenta esta busqueda
    //podria agregar los world static y dynamic, para asi avitar siempre encontrar algun nodo que este destrar de algun menu, y que por seleccionar en el menu tambien le de click a el
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

void ATransformationVRGameModeBase::SetJerarquiaTask(EVRJerarquiaTask NewJerarquiaTask) {
	CurrentJerarquiaTask = NewJerarquiaTask;
}

EVRJerarquiaTask ATransformationVRGameModeBase::GetJerarquiaTask() {
	return CurrentJerarquiaTask;
}

void ATransformationVRGameModeBase::SelectPressed() {
    switch (CurrentJerarquiaTask) {
        case EVRJerarquiaTask::EArmarTask: {//si el juego esta en Playing
            //activate the spawn volumes
			BuscandoParteConLaser();//por ahora, pero este no va aqui

        }
        break;//no se como funciona esto
        case EVRJerarquiaTask::ERotationTask: {//si  perdimos el juego
			if (ParteSeleccionada) {
				//rotar, es decir ver si hay componete, para capturar su plano, y empezar a hacer los calculos, aqui entra lo de la rotacion de ramas
			}
			else {
				if (HitParte) {
					ParteSeleccionada = HitParte;
					ParteSeleccionada->ActivarResaltado();
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
*/