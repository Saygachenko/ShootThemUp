// Shoo Them Up Game


#include "Player/Public/STUBaseCharacter.h"
#include "Camera/CameraComponent.h" // библиотека камеры
#include "Components/InputComponent.h" // библиотека ввода
#include "GameFramework/SpringArmComponent.h" // библиотека вращения по орбите (для камеры)
#include "Components/STUCharacterMovementComponent.h" // подключаем преобразованный компонент для нашего character
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h" // для переключения между Pawn

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer &ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{// вызвали конструктор родительского калсса. Функция SetDefaultSubobjectClass для замены компонента по умолчанию
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true; // чтобы можно было вращать
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f); // изменили расположение камеры

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent"); // этот компонент чисто логический поэтому нам не нужен SetupAttachment

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true); // выключили видимость текста ХП

    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent"); // этот компонент чисто логический поэтому нам не нужен SetupAttachment
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    check(HealthComponent); // проверка на nullptr через макрос check - возврашает Assertion, работает только в Developend и Debug билдах
    check(HealthTextComponent);
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

    //const auto Health = HealthComponent->GetHealth(); // каждый тик мы будет передавать компоненту HealthComponent значение переменной Health

    //TakeDamage(0.1f, FDamageEvent{}, Controller, this); // персонаж наносит урон сам себе TakeDamage (сколько урона мы хотим нанести, DamageEvent конструктор по умолчанию (там 2 вида DamageType), указатель на контроллер персонажа которому наносят ущерб, указатель на актора который наносит ущерб)
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

	if (PlayerInputComponent)
    {
        PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
        PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);

        PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput); // для поворота камеры по вертикали мы будем использовать функцию AddControllerPitchInput(значение угла Pit
    // ch) которая есть у каждого Pawn. Можно их подключать напрямую
        PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput); // для поворота камеры вокруг Character используем функцию AddControllerYawInput

        PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump); // функция прыжка Jump. IE_Pressed - состояние Controlle (когда клавиша нажата)

        PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning); // IE_Pressed кнопка нажата
        PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning); // IE_Released кнопка отжата
        
        PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASTUBaseCharacter::OnStartFire); // начало стрельбы из оружия
        PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire); // конец стрельбы из оружия

        PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent,&USTUWeaponComponent::NextWeapon); // бинд на смену оружия

         PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload); // бинд перезарядки
    }
}

bool ASTUBaseCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero() && !IsMovingRight; // проверка что кнопка нажата и наш персонаж двигается вперёд, и что скорость персонажа не равна 0 + персонаж не двигается в стороны
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

void ASTUBaseCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f; // true если Amount > 0 - нажата клавиша за движение вперёд
    if (Amount == 0.0f)
    {
         return;
    }
    AddMovementInput(GetActorForwardVector(), Amount); // для перемещения нашего персонажа воспользуемся функцией AddMovementInput(направление в
                        // которое будет перемещать нашего персонажа, scale) - данная функция существует у каждого Pawn
    if (IsRunning() && WeaponComponent->IsFiring()) // если мы бежим и стрельба началась
    {
         WeaponComponent->StopFire(); // останавливаем стрельбу
    }
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f) return;
    IsMovingRight = true; // проверка нажата клавиша за движение в бок
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::OnStartRunning() // когда клавиша нажата
{
    WantsToRun = true;
    IsMovingRight = false; // если не нажата то выполняем функцию
    if (IsRunning()) // если стрельба началась
    {
         WeaponComponent->StopFire(); // останавливаем стрельбу
    }
}

void ASTUBaseCharacter::OnStopRunning() // когда клавиша отжата
{
    WantsToRun = false;
}

void ASTUBaseCharacter::OnDeath()
{
    // UE_LOG(LogBaseCharacter, Display, TEXT("Player %s is dead!"), *GetName()); // сообщение что наш персонаж погиб

    // PlayAnimMontage(DeathAnimMontage); // для проигрывания анимационного монтажа функция PlayAnimMontage(указатель на анимацию)

    GetCharacterMovement()->DisableMovement(); // останавливаем всё движение вызываем функцию DisableMovement

    SetLifeSpan(LifeSpanOnDeath); // уничтожение нашего персонажа

    if (Controller) // проверка на контроллер персонажа
    {
        Controller->ChangeState(NAME_Spectating); // если контроля нет, то мы меняем на NAME_Spectating (свободная камера)
    }
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // делаем игнорирование колизии при смерти персонажа
    WeaponComponent->StopFire(); // останавливаем стрельбу при смерти персонажа
    // симуляция смерти персонажа вместо нашей анимации DeathAnimMontage
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // функция которая включает колизию компонента SetCollisionEnabled()
    GetMesh()->SetSimulatePhysics(true); // функция SetSimulatePhysics() - активирует физическую симуляцию тела
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health))); // SetText принимает переменную типа FText и воспользуемся статичной функцией FromString и преобразуем в FString
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult &Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z; // когда наш персонаж соприкоснулся с поверхностью, так как конечная скорость прямопропорциональна высоте с которой данное тело упало
    // UE_LOG(LogBaseCharacter, Display, TEXT("On landed: %f"), FallVelocityZ); // вывод скорости падения

    if (FallVelocityZ < LandedDamageVelocity.X) // проверка если скорость падения меньше 900, выходим из функции
    {
        return;
    }

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ); // вычисления ущерба. Функция GetMappedRangeValueClamped(входной отрезок, урон, скорость с которой упали)
    // UE_LOG(LogBaseCharacter, Display, TEXT("FinalDamage: %f"), FinalDamage);
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
