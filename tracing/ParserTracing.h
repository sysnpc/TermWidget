#pragma once

#include <string>
#include <string_view>

namespace PresudoMicrosoft::Console::VirtualTerminal {

  class ParserTracing {
   public:
    ParserTracing() noexcept;

    void TraceStateChange(const std::wstring_view name) const noexcept;
    void TraceOnAction(const std::wstring_view name) const noexcept;
    void TraceOnExecute(const wchar_t wch) const;
    void TraceOnExecuteFromEscape(const wchar_t wch) const;
    void TraceOnEvent(const std::wstring_view name) const noexcept;
    void TraceCharInput(const wchar_t wch);

    void AddSequenceTrace(const wchar_t wch);
    void DispatchSequenceTrace(const bool fSuccess) noexcept;
    void ClearSequenceTrace() noexcept;
    void DispatchPrintRunTrace(const std::wstring_view string) const;

   private:
    std::wstring _sequenceTrace;
  };

}  // namespace PresudoMicrosoft::Console::VirtualTerminal
