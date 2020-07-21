#pragma once

#include <array>
#include <string>
#include <string_view>

namespace PresudoMicrosoft::Console::VirtualTerminal {
  class TerminalOutput {
   public:
    TerminalOutput() noexcept;

    wchar_t TranslateKey(const wchar_t wch) const noexcept;
    bool Designate94Charset(const size_t gsetNumber,
                            const std::pair<wchar_t, wchar_t> charset);
    bool Designate96Charset(const size_t gsetNumber,
                            const std::pair<wchar_t, wchar_t> charset);
    bool LockingShift(const size_t gsetNumber);
    bool LockingShiftRight(const size_t gsetNumber);
    bool SingleShift(const size_t gsetNumber);
    bool NeedToTranslate() const noexcept;
    void EnableGrTranslation(bool enabled);

   private:
    bool _SetTranslationTable(const size_t gsetNumber,
                              const std::wstring_view translationTable);

    std::array<std::wstring_view, 4> _gsetTranslationTables;
    size_t _glSetNumber = 0;
    size_t _grSetNumber = 2;
    std::wstring_view _glTranslationTable;
    std::wstring_view _grTranslationTable;
    mutable std::wstring_view _ssTranslationTable;
    bool _grTranslationEnabled = false;
  };
}  // namespace PresudoMicrosoft::Console::VirtualTerminal
