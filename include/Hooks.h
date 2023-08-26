#pragma once

constexpr auto body_slot{ RE::BGSBipedObjectForm::BipedObjectSlot::kBody };
constexpr auto head_slot{ RE::BGSBipedObjectForm::BipedObjectSlot::kHead };
constexpr auto hands_slot{ RE::BGSBipedObjectForm::BipedObjectSlot::kHands };
constexpr auto feet_slot{ RE::BGSBipedObjectForm::BipedObjectSlot::kFeet };
constexpr auto remove_reason{ RE::ITEM_REMOVE_REASON::kRemove };

namespace Hooks {
    void Install();

    class Load3D : public Singleton<Load3D> {
    public:
        static RE::NiAVObject* Thunk(RE::Character* a_this, bool a_arg1);

        inline static REL::Relocation<decltype(&Thunk)> func;

        static constexpr std::size_t idx{ 106 };
    };
}
