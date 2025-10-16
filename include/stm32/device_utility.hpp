#pragma once

#include <concepts>
#include <cstdint>
#include <type_traits>

#include <emb/core.hpp>

#include "device.hpp"

namespace stm32 {

namespace bittype {

namespace internal {
struct rw {};
struct r {};
struct w {};
struct rc_w0 {};
struct rc_w1 {};
struct rc_w {};
struct rc_r {};
struct rs_r {};
struct rs {};
struct rwo {};
struct t {};
struct rt_w1 {};
}

inline constexpr internal::rw rw;
inline constexpr internal::r r;
inline constexpr internal::w w;
inline constexpr internal::rc_w0 rc_w0;
inline constexpr internal::rc_w1 rc_w1;
inline constexpr internal::rc_w rc_w;
inline constexpr internal::rc_r rc_r;
inline constexpr internal::rs_r rs_r;
inline constexpr internal::rs rs;
inline constexpr internal::rwo rwo;
inline constexpr internal::t t;
inline constexpr internal::rt_w1 rt_w1;

template<typename T>
concept any = emb::either<
    T,
    internal::rw,
    internal::r,
    internal::w,
    internal::rc_w0,
    internal::rc_w1,
    internal::rc_w,
    internal::rc_r,
    internal::rs_r,
    internal::rs,
    internal::rwo,
    internal::t,
    internal::rt_w1>;

template<typename T>
concept readable = any<T> && !emb::either<T, internal::w, internal::t>;

template<typename T>
concept settable = emb::either<
    T,
    internal::rw,
    internal::w,
    internal::rs_r,
    internal::rs,
    internal::rwo>;

template<typename T>
concept resettable = emb::either<
    T,
    internal::rw,
    internal::w,
    internal::rc_w0,
    internal::rc_w1,
    internal::rc_w,
    internal::rc_r,
    internal::rwo>;

} // namespace bittype

template<std::unsigned_integral Reg>
class device_register {
private:
  Reg& reg_;
public:
  using underlying_type = std::remove_cvref_t<Reg>;
  using register_type = decltype(reg_);
public:
  constexpr explicit device_register(Reg& reg) : reg_(reg) {}

  template<std::unsigned_integral B>
  [[nodiscard]] constexpr bool operator[](B bitmask) const {
    return (reg_ & static_cast<underlying_type>(bitmask)) ==
           static_cast<underlying_type>(bitmask);
  }

  template<
      std::unsigned_integral B,
      bittype::settable BType = bittype::internal::rw>
  constexpr void set(B bitmask, BType type = bittype::rw) {
    reg_ |= static_cast<underlying_type>(bitmask);
  }

  template<
      std::unsigned_integral B,
      bittype::resettable BType = bittype::internal::rw>
  constexpr void reset(B bitmask, BType type = bittype::rw) {
    if constexpr (!std::same_as<BType, bittype::internal::rc_w1>) {
      reg_ &= ~static_cast<underlying_type>(bitmask);
    } else {
      reg_ |= static_cast<underlying_type>(bitmask);
    }
  }

  constexpr underlying_type read() const {
    return reg_;
  }

  constexpr void write(underlying_type val) {
    reg_ = val;
  }

  constexpr void clear() {
    reg_ = underlying_type{0};
  }

  template<std::unsigned_integral B, bittype::any BType = bittype::internal::rw>
  constexpr void modify(B resetmask, B setmask, BType type = bittype::rw) {
    reg_ = (reg_ & ~static_cast<underlying_type>(resetmask)) |
           static_cast<underlying_type>(setmask);
  }
};

inline void foo() {
  device_register adc_cr(ADC1->CR);
  [[maybe_unused]] auto res1 = adc_cr[ADC_CR_ADEN];
  adc_cr.set(ADC_CR_ADEN);

  [[maybe_unused]] auto res2 = device_register{ADC1->CR}[ADC_CR_ADEN];
  device_register{ADC1->CR}.set(ADC_CR_ADEN);

  if (device_register(ADC1->CR)[ADC_CR_ADEN]) {

  }

  device_register(ADC1->CFGR).set(ADC_CFGR_AWD1EN, bittype::rw);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"

inline uint32_t position_val(uint32_t val) {
  return __builtin_clz(__RBIT(val));
}

#pragma GCC diagnostic pop

} // namespace stm32
