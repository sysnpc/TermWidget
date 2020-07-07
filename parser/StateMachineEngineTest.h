#pragma once

#include "IStateMachineEngine.h"

namespace PresudoMicrosoft::Console::VirtualTerminal {

  class StateMachineEngineTest : public IStateMachineEngine {
   public:
    StateMachineEngineTest();

    // IStateMachineEngine interface
   public:
    bool ActionExecute(const wchar_t wch) override;
    bool ActionExecuteFromEscape(const wchar_t wch) override;
    bool ActionPrint(const wchar_t wch) override;
    bool ActionPrintString(const std::wstring_view string) override;
    bool ActionPassThroughString(const std::wstring_view string) override;
    bool ActionEscDispatch(
        const wchar_t wch,
        const std::basic_string_view<wchar_t> intermediates) override;
    bool ActionCsiDispatch(
        const wchar_t wch, const std::basic_string_view<wchar_t> intermediates,
        const std::basic_string_view<size_t> parameters) override;
    bool ActionClear() override;
    bool ActionIgnore() override;
    bool ActionOscDispatch(const wchar_t wch, const size_t parameter,
                           const std::wstring_view string) override;
    bool ActionSs3Dispatch(
        const wchar_t wch,
        const std::basic_string_view<size_t> parameters) override;
    bool FlushAtEndOfString() const override;
    bool DispatchControlCharsFromEscape() const override;
    bool DispatchIntermediatesFromEscape() const override;
  };

}  // namespace PresudoMicrosoft::Console::VirtualTerminal
