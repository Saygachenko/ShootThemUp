#pragma once

class AnimUtils
{
 public:
    template <typename T> T 
    static *FindNotifyByClass(UAnimSequenceBase *Animation)
    {
        if (!Animation)
        {
            return nullptr;
        }

        const auto NotifyEvents = Animation->Notifies; // �������� ������ � �������� Notify
        for (auto NotifyEvent : NotifyEvents) // ���������� �� ����� ������� ������������� ������ NotifyEvents
        {
            auto AnimNotify = Cast<T>(NotifyEvent.Notify); // ����� ������ �������� ������ Notify ����� OnEquipFinished �� ����������
                                             // ������������� ������ Notify � ������ ���� USTUEquipFinishedAnimNotify
            if (AnimNotify) // ���� ��� ��� Notify �� ���������� AnimNotify
            {
                return AnimNotify;
            }
        }
        return nullptr; // ���� �� �����, ���������� 0
    }
};