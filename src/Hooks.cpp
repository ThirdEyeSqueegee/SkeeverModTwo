#include "Hooks.h"

namespace Hooks {
    void Install() {
        stl::write_vfunc<RE::Character, Load3D>();
        logger::info("Installed Character::Load3D hook");
    }

    RE::NiAVObject* Load3D::Thunk(RE::Character* a_this, bool a_arg1) {
        const auto result = func(a_this, a_arg1);

        std::jthread([=] {
            std::this_thread::sleep_for(1s);
            SKSE::GetTaskInterface()->AddTask([=] {
                if (!a_this->IsPlayerRef() && a_this->HasKeywordString("ActorTypeNPC"sv) && !a_this->IsPlayerTeammate()) {
                    const auto body_worn  = a_this->GetWornArmor(body_slot);
                    const auto head_worn  = a_this->GetWornArmor(head_slot);
                    const auto hands_worn = a_this->GetWornArmor(hands_slot);
                    const auto feet_worn  = a_this->GetWornArmor(feet_slot);
                    if (!body_worn || !head_worn || !hands_worn || !feet_worn) {
                        const auto manager    = RE::ActorEquipManager::GetSingleton();
                        const auto inv        = a_this->GetInventory();
                        auto       body_found = false, head_found = false, hands_found = false, feet_found = false;
                        for (const auto& item : inv | std::views::keys) {
                            if (const auto armo = item->As<RE::TESObjectARMO>()) {
                                if (armo->GetSlotMask() == hands_slot && !hands_worn && !hands_found) {
                                    hands_found = true;
                                    manager->EquipObject(a_this, item, nullptr, 1, armo->GetEquipSlot(), true, false, false, false);
                                    logger::debug("{} spawned naked, equipped hands slot item {} found in inventory", a_this->GetName(),
                                                  item->GetName());
                                }
                                if (armo->GetSlotMask() == feet_slot && !feet_worn && !feet_found) {
                                    feet_found = true;
                                    manager->EquipObject(a_this, item, nullptr, 1, armo->GetEquipSlot(), true, false, false, false);
                                    logger::debug("{} spawned naked, equipped feet slot item {} found in inventory", a_this->GetName(),
                                                  item->GetName());
                                }
                                if (armo->GetSlotMask() == head_slot && !head_worn && !head_found) {
                                    head_found = true;
                                    manager->EquipObject(a_this, item, nullptr, 1, armo->GetEquipSlot(), true, false, false, false);
                                    logger::debug("{} spawned naked, equipped head slot item {} found in inventory", a_this->GetName(),
                                                  item->GetName());
                                }
                                if (armo->GetSlotMask() == body_slot && !body_worn && !body_found) {
                                    body_found = true;
                                    manager->EquipObject(a_this, item, nullptr, 1, armo->GetEquipSlot(), true, false, false, false);
                                    logger::debug("{} spawned naked, equipped body slot item {} found in inventory", a_this->GetName(),
                                                  item->GetName());
                                }
                            }
                        }
                    }
                }
            });
        }).detach();

        return result;
    }
}
