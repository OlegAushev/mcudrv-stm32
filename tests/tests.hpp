#pragma once

#define STM32_CONSTEXPR_ASSERT(expression) \
  if (!(expression)) {                   \
    return false;                        \
  }
