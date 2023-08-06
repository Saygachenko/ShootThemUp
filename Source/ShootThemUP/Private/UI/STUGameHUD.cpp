// Shoo Them Up Game


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h" // ����� �������� �������
#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();

    //DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHudWidgetClass); // CreateWidget<��� �������>(��������� �� ������ ���������, ����� ������� ������� ����� �������) ������ ������
    if (PlayerHudWidgetClass) // ���� ������ ��������
    {
        PlayerHUDWidget->AddToViewport(); // �������� ������� ������� AddToViewport(������� ��������� �������)
    }

    if (GetWorld()) // ���� ��������� �� ��� ����������
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()); // �������� ��������� �� �������
        if (GameMode) // ���� ��������� ����������
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged); // ������������� �� ��� ������� ��������� ����
        }
    }
}

void ASTUGameHUD::DrawCrossHair()
{
    // TInterval(����� ������ ������) - ��� ���������� ������ ������
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f); // ��� ��������� ������� ������ ������������ �������������� ������ Canvas

    const float HalfLineSize = 10.0f; // ������ �������� �����
    const float LineThickness = 2.0f; // ������� �����
    const FLinearColor LineColor = FLinearColor::Green; // ���� ������� (�����)

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness); // DrawLine(��������� ����� �� ������ X, Y, �������� ����� �� ������ X, Y, ���� �����, ������� �����) ������� ����, ��������� �������� ����� �� ������
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}

void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State)
{
    UE_LOG(LogSTUGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State)); // ������� � ���� ���������� �� ���������� ��������� ���� GetValueAsString - ����������� �������� enum � ������
}
