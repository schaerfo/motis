#pragma once

#include "nigiri/routing/clasz_mask.h"
#include "nigiri/types.h"

#include "motis-api/motis-api.h"

namespace motis {

api::ModeEnum to_mode(nigiri::clasz);
std::vector<api::ModeEnum> clasz_mask_to_modes(nigiri::routing::clasz_mask_t);

}  // namespace motis
