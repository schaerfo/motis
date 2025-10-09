#include "motis/timetable/clasz_to_mode.h"

namespace n = nigiri;

namespace motis {

api::ModeEnum to_mode(n::clasz const c) {
  switch (c) {
    case n::clasz::kAir: return api::ModeEnum::AIRPLANE;
    case n::clasz::kHighSpeed: return api::ModeEnum::HIGHSPEED_RAIL;
    case n::clasz::kLongDistance: return api::ModeEnum::LONG_DISTANCE;
    case n::clasz::kCoach: return api::ModeEnum::COACH;
    case n::clasz::kNight: return api::ModeEnum::NIGHT_RAIL;
    case n::clasz::kRegionalFast: return api::ModeEnum::REGIONAL_FAST_RAIL;
    case n::clasz::kRegional: return api::ModeEnum::REGIONAL_RAIL;
    case n::clasz::kMetro: return api::ModeEnum::METRO;
    case n::clasz::kSubway: return api::ModeEnum::SUBWAY;
    case n::clasz::kTram: return api::ModeEnum::TRAM;
    case n::clasz::kBus: return api::ModeEnum::BUS;
    case n::clasz::kShip: return api::ModeEnum::FERRY;
    case n::clasz::kCableCar: return api::ModeEnum::CABLE_CAR;
    case n::clasz::kFunicular: return api::ModeEnum::FUNICULAR;
    case n::clasz::kAreaLift: return api::ModeEnum::AREAL_LIFT;
    case n::clasz::kOther: return api::ModeEnum::OTHER;
    case n::clasz::kNumClasses:;
  }
  std::unreachable();
}

std::vector<api::ModeEnum> clasz_mask_to_modes(n::routing::clasz_mask_t mask) {
  auto result = std::vector<api::ModeEnum>{};
  if (mask &
      (1U << static_cast<std::underlying_type_t<n::clasz>>(n::clasz::kAir))) {
    result.emplace_back(api::ModeEnum::AIRPLANE);
  }
  if (mask & (1U << static_cast<std::underlying_type_t<n::clasz>>(
                  n::clasz::kHighSpeed))) {
    result.emplace_back(api::ModeEnum::HIGHSPEED_RAIL);
  }
  if (mask & (1U << static_cast<std::underlying_type_t<n::clasz>>(
                  n::clasz::kLongDistance))) {
    result.emplace_back(api::ModeEnum::LONG_DISTANCE);
  }
  if (mask &
      (1U << static_cast<std::underlying_type_t<n::clasz>>(n::clasz::kCoach))) {
    result.emplace_back(api::ModeEnum::COACH);
  }
  if (mask &
      (1U << static_cast<std::underlying_type_t<n::clasz>>(n::clasz::kNight))) {
    result.emplace_back(api::ModeEnum::NIGHT_RAIL);
  }
  if (mask & (1U << static_cast<std::underlying_type_t<n::clasz>>(
                  n::clasz::kRegionalFast))) {
    result.emplace_back(api::ModeEnum::REGIONAL_FAST_RAIL);
  }
  if (mask & (1U << static_cast<std::underlying_type_t<n::clasz>>(
                  n::clasz::kRegional))) {
    result.emplace_back(api::ModeEnum::REGIONAL_RAIL);
  }
  if (mask &
      (1U << static_cast<std::underlying_type_t<n::clasz>>(n::clasz::kMetro))) {
    result.emplace_back(api::ModeEnum::METRO);
  }
  if (mask & (1U << static_cast<std::underlying_type_t<n::clasz>>(
                  n::clasz::kSubway))) {
    result.emplace_back(api::ModeEnum::SUBWAY);
  }
  if (mask &
      (1U << static_cast<std::underlying_type_t<n::clasz>>(n::clasz::kTram))) {
    result.emplace_back(api::ModeEnum::TRAM);
  }
  if (mask &
      (1U << static_cast<std::underlying_type_t<n::clasz>>(n::clasz::kBus))) {
    result.emplace_back(api::ModeEnum::BUS);
  }
  if (mask &
      (1U << static_cast<std::underlying_type_t<n::clasz>>(n::clasz::kShip))) {
    result.emplace_back(api::ModeEnum::FERRY);
  }
  if (mask & (1U << static_cast<std::underlying_type_t<n::clasz>>(
                  n::clasz::kCableCar))) {
    result.emplace_back(api::ModeEnum::CABLE_CAR);
  }
  if (mask & (1U << static_cast<std::underlying_type_t<n::clasz>>(
                  n::clasz::kFunicular))) {
    result.emplace_back(api::ModeEnum::FUNICULAR);
  }
  if (mask & (1U << static_cast<std::underlying_type_t<n::clasz>>(
                  n::clasz::kAreaLift))) {
    result.emplace_back(api::ModeEnum::AREAL_LIFT);
  }
  if (mask &
      (1U << static_cast<std::underlying_type_t<n::clasz>>(n::clasz::kOther))) {
    result.emplace_back(api::ModeEnum::OTHER);
  }
  return result;
}

}  // namespace motis
