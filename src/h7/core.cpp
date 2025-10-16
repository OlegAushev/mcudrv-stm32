#include <stm32/device.hpp>
#include <stm32/h7/core.hpp>

namespace stm32 {
namespace h7 {
namespace core {

void enable_icache() {
  SCB_EnableICache();
}

void disable_icache() {
  SCB_DisableICache();
}

void enable_dcache() {
  SCB_EnableDCache();
}

void disable_dcache() {
  SCB_DisableDCache();
}

void init_nvic() {}

} // namespace core
} // namespace h7
} // namespace stm32
