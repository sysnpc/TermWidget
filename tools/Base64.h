#pragma once

#include <string>
#include <string_view>

namespace PresudoMicrosoft::Console::VirtualTerminal {
  class Base64 {
   public:
    static std::wstring s_Encode(const std::wstring_view src) noexcept;
    static bool s_Decode(const std::wstring_view src,
                         std::wstring& dst) noexcept;

   private:
    static constexpr bool s_IsSpace(const wchar_t ch) noexcept;
  };
}  // namespace PresudoMicrosoft::Console::VirtualTerminal
