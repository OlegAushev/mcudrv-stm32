#pragma once

namespace stm32 {
namespace h7 {
namespace core {

void enable_icache();
void disable_icache();
void enable_dcache();
void disable_dcache();
void init_nvic();

} // namespace core
} // namespace h7
} // namespace stm32
