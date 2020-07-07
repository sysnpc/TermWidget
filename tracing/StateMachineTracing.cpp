#include "StateMachineTracing.h"

Q_LOGGING_CATEGORY(LogStateMachine, "Log.StateMachine");

using namespace PresudoMicrosoft::Console::VirtualTerminal;

StateMachineTracing::StateMachineTracing() noexcept { ClearSequenceTrace(); }

void StateMachineTracing::TraceStateChange(const std::wstring_view name) const
    noexcept {
  //  TraceLoggingWrite(g_hConsoleVirtTermParserEventTraceProvider,
  //                    "StateMachine_EnterState",
  //                    TraceLoggingCountedWideString(
  //                        name.data(), gsl::narrow_cast<ULONG>(name.size())),
  //                    TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE));
}

void StateMachineTracing::TraceOnAction(const std::wstring_view name) const
    noexcept {
  //  TraceLoggingWrite(g_hConsoleVirtTermParserEventTraceProvider,
  //                    "StateMachine_Action",
  //                    TraceLoggingCountedWideString(
  //                        name.data(), gsl::narrow_cast<ULONG>(name.size())),
  //                    TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE));
}

void StateMachineTracing::TraceOnExecute(const wchar_t wch) const {
  //  const auto sch = gsl::narrow_cast<INT16>(wch);
  //  TraceLoggingWrite(g_hConsoleVirtTermParserEventTraceProvider,
  //                    "StateMachine_Execute", TraceLoggingWChar(wch),
  //                    TraceLoggingHexInt16(sch),
  //                    TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE));
}

void StateMachineTracing::TraceOnExecuteFromEscape(const wchar_t wch) const {
  //  const auto sch = gsl::narrow_cast<INT16>(wch);
  //  TraceLoggingWrite(g_hConsoleVirtTermParserEventTraceProvider,
  //                    "StateMachine_ExecuteFromEscape",
  //                    TraceLoggingWChar(wch), TraceLoggingHexInt16(sch),
  //                    TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE));
}

void StateMachineTracing::TraceOnEvent(const std::wstring_view name) const
    noexcept {
  //  TraceLoggingWrite(g_hConsoleVirtTermParserEventTraceProvider,
  //                    "StateMachine_Event",
  //                    TraceLoggingCountedWideString(
  //                        name.data(), gsl::narrow_cast<ULONG>(name.size())),
  //                    TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE));
}

void StateMachineTracing::TraceCharInput(const wchar_t wch) {
  //  AddSequenceTrace(wch);
  //  const auto sch = gsl::narrow_cast<INT16>(wch);

  //  TraceLoggingWrite(g_hConsoleVirtTermParserEventTraceProvider,
  //                    "StateMachine_NewChar", TraceLoggingWChar(wch),
  //                    TraceLoggingHexInt16(sch),
  //                    TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE));
}

void StateMachineTracing::AddSequenceTrace(const wchar_t wch) {
  //  _sequenceTrace.push_back(wch);
}

void StateMachineTracing::DispatchSequenceTrace(const bool fSuccess) noexcept {
  //  if (fSuccess) {
  //    TraceLoggingWrite(g_hConsoleVirtTermParserEventTraceProvider,
  //                      "StateMachine_Sequence_OK",
  //                      TraceLoggingWideString(_sequenceTrace.c_str()),
  //                      TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE));
  //  } else {
  //    TraceLoggingWrite(g_hConsoleVirtTermParserEventTraceProvider,
  //                      "StateMachine_Sequence_FAIL",
  //                      TraceLoggingWideString(_sequenceTrace.c_str()),
  //                      TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE));
  //  }

  //  ClearSequenceTrace();
}

void StateMachineTracing::ClearSequenceTrace() noexcept {
  _sequenceTrace.clear();
}

// NOTE: I'm expecting this to not be null terminated
void StateMachineTracing::DispatchPrintRunTrace(
    const std::wstring_view string) const {
  //  if (string.size() == 1) {
  //    const auto wch = til::at(string, 0);
  //    const auto sch = gsl::narrow_cast<INT16>(wch);
  //    TraceLoggingWrite(g_hConsoleVirtTermParserEventTraceProvider,
  //                      "StateMachine_PrintRun", TraceLoggingWChar(wch),
  //                      TraceLoggingHexInt16(sch),
  //                      TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE));
  //  } else {
  //    const auto length = gsl::narrow_cast<ULONG>(string.size());

  //    TraceLoggingWrite(
  //        g_hConsoleVirtTermParserEventTraceProvider, "StateMachine_PrintRun",
  //        TraceLoggingCountedWideString(string.data(), length),
  //        TraceLoggingValue(length),
  //        TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE));
  //  }
}
