#include "tests.hpp"
#include <cstdint>
#include <stm32/device_utility.hpp>

namespace stm32 {
namespace internal {
namespace tests {

constexpr bool test_bit_operations() {
  uint32_t raw_reg = 0;
  device_register reg(raw_reg);

  uint16_t b0 = 0x0F;
  reg.set(b0);
  STM32_CONSTEXPR_ASSERT(reg[b0]);
  reg.reset(b0);
  STM32_CONSTEXPR_ASSERT(!reg[b0]);

  reg.write(0xFFF0);
  reg.reset(b0, bittype::rc_w1);
  STM32_CONSTEXPR_ASSERT(reg[b0]);


  return true;
}

static_assert(test_bit_operations());

} // namespace tests
} // namespace internal
} // namespace stm32
