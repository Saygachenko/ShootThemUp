// Shoo Them Up Game


#include "Player/Public/STUBaseCharacter.h"
#include "Components/STUCharacterMovementComponent.h" // подключаем преобразованный компонент для нашего character
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h" // для переключения между Pawn

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer &ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{// вызвали конструктор родительского калсса. Функция SetDefaultSubobjectClass для замены компонента по умолчанию
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent"); // этот компонент чисто логический поэтому нам не нужен SetupAttachment
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent"); // этот компонент чисто логический поэтому нам не нужен SetupAttachment
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    check(HealthComponent); // проверка на nullptr через макрос check - возврашает Assertion, работает только в Developend и Debug билдах
    check(GetCharacterMovement());
    check(GetMesh());

    OnHealthChanged(HealthComponent->GetHealth(), 0.0f); // явно указали вызов функции OnHealthChanged со значением которые мы получим через GetHealth
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath); // подписались на делегат. бинд AddUObject(наш объект, указатель на функцию) только если наш делегат доступен в С++
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded); // подписываемся на делегат LandedDelegate - функция вызывается при контакте персонажа с поверхностью после падения

}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASTUBaseCharacter::IsRunning() const
{
    return false; // проверка что кнопка нажата и наш персонаж двигается вперёд, и что скорость персонажа не равна 0 + персонаж не двигается в стороны
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal(); // высчитуем нормаль нашего вектора скорости
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal)); // высчитываем скалярное произведение между Forward и вектором нормали VelocityNormal
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal); // высчитываем ортогональный вектор
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);  // перводим AngleBetween в градусы и умнажаем на знак координаты Z CrossProduct
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::OnDeath()
{
    GetCharacterMovement()->DisableMovement(); // останавливаем всё движение вызываем функцию DisableMovement

    SetLifeSpan(LifeSpanOnDeath); // уничтожение нашего персонажа

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // делаем игнорирование колизии при смерти персонажа
    WeaponComponent->StopFire(); // останавливаем стрельбу при смерти персонажа
    // симуляция смерти персонажа вместо нашей анимации DeathAnimMontage
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // функция которая включает колизию компонента SetCollisionEnabled()
    GetMesh()->SetSimulatePhysics(true); // функция SetSimulatePhysics() - активирует физическую симуляцию тела
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult &Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z; // когда наш персонаж соприкоснулся с поверхностью, так как конечная скорость прямопропорциональна высоте с которой данное тело упало

    if (FallVelocityZ < LandedDamageVelocity.X) // проверка если скорость падения меньше 900, выходим из функции
    {
        return;
    }

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ); // вычисления ущерба. Функция GetMappedRangeValueClamped(входной отрезок, урон, скорость с которой упали)
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr); // получения урона
}

void ASTUBaseCharacter::OnStartFire()
{
    if (IsRunning()) // если стрельба началась 
    {
        return; // выходим
    }
    WeaponComponent->StartFire(); // если нет, вызываем функцию StartFire
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
    const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0); // указатель на материалинстанс
    if (!MaterialInst) // если материалинстант не существует
    {
        return; // выход из функции
    }

    MaterialInst->SetVectorParameterValue(MaterialColorName, Color); // устанавливаем цвет материалу
}
