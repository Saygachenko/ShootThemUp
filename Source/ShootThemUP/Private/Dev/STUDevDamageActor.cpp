// Shoo Them Up Game


#include "Dev/STUDevDamageActor.h"
#include "DrawDebugHelpers.h" // библиотека которая помогает рисовать графические примитивы в том числе и сферу
#include "Kismet/GameplayStatics.h" // для получения всех Actor со сцены

// Sets default values
ASTUDevDamageActor::ASTUDevDamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent"); // для трансформации, чтобы могли перемещать Actor по сцене
    SetRootComponent(SceneComponent); // главным компонентом делаем

}

// Called when the game starts or when spawned
void ASTUDevDamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTUDevDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor); // рисуем нашу сферу каждый тик DrawDebugSphere(указатель на мир, центр сферы, радиус сферы, количество сегментов сферы, цвет сферы)

	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage); // UGameplayStatics функция ApplyRadialDamage(указатель на мир, количество урона, центр урона, радиус, тип урона, можем указать массив акторов которым не будем наносить урон, актор который наносит ущерб, контроллер, полный урон (true) или по приближению к центру урона (false))
}

