#include "ParserTracing.h"

using namespace PresudoMicrosoft::Console::VirtualTerminal;

ParserTracing::ParserTracing() noexcept {}

void ParserTracing::TraceStateChange(const std::wstring_view name) const
    noexcept {}

void ParserTracing::TraceOnAction(const std::wstring_view name) const noexcept {
  
}

void ParserTracing::TraceOnExecute(const wchar_t wch) const {}

void ParserTracing::TraceOnExecuteFromEscape(const wchar_t wch) const {}

void ParserTracing::TraceOnEvent(const std::wstring_view name) const noexcept {}

void ParserTracing::TraceCharInput(const wchar_t wch) {}

void ParserTracing::AddSequenceTrace(const wchar_t wch) {}

void ParserTracing::DispatchSequenceTrace(const bool fSuccess) noexcept {}

void ParserTracing::ClearSequenceTrace() noexcept {}

void ParserTracing::DispatchPrintRunTrace(
    const std::wstring_view string) const {}
