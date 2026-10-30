//
// Created by emi on 10/29/2026.
//

#include "sdk_base.hpp"
#include "system/systems_manager.hpp"

systems_manager* sdk_base::sysman() { return systems_manager::i(); }
signature_system* sdk_base::sigsys() { return sysman()->get<signature_system>(); }
