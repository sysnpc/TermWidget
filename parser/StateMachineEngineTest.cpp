#include "StateMachineEngineTest.h"

#include <QDebug>

using namespace PresudoMicrosoft::Console::VirtualTerminal;

StateMachineEngineTest::StateMachineEngineTest() {}

bool StateMachineEngineTest::ActionExecute(const wchar_t wch) {
  Q_UNUSED(wch)
  qDebug() << "execute";
  return true;
}

bool StateMachineEngineTest::ActionExecuteFromEscape(const wchar_t wch) {
  Q_UNUSED(wch)
  qDebug() << "execute escape";
  return true;
}

bool StateMachineEngineTest::ActionPrint(const wchar_t wch) {
  Q_UNUSED(wch)
  qDebug() << "print";
  return true;
}

bool StateMachineEngineTest::ActionPrintString(const std::wstring_view string) {
  Q_UNUSED(string)
  qDebug() << "printstring";
  return true;
}

bool StateMachineEngineTest::ActionPassThroughString(
    const std::wstring_view string) {
  Q_UNUSED(string)
  qDebug() << "pass through";
  return true;
}

bool StateMachineEngineTest::ActionEscDispatch(
    const wchar_t wch, const std::basic_string_view<wchar_t> intermediates) {
  Q_UNUSED(wch)
  Q_UNUSED(intermediates)
  qDebug() << "esc dispatch";
  return true;
}

bool StateMachineEngineTest::ActionCsiDispatch(
    const wchar_t wch, const std::basic_string_view<wchar_t> intermediates,
    const std::basic_string_view<size_t> parameters) {
  Q_UNUSED(wch)
  Q_UNUSED(intermediates)
  Q_UNUSED(parameters)
  qDebug() << "csi dispatch";
  return true;
}

bool StateMachineEngineTest::ActionClear() {
  qDebug() << "clear";
  return true;
}

bool StateMachineEngineTest::ActionIgnore() {
  qDebug() << "ignore";
  return true;
}

bool StateMachineEngineTest::ActionOscDispatch(const wchar_t wch,
                                               const size_t parameter,
                                               const std::wstring_view string) {
  Q_UNUSED(wch)
  Q_UNUSED(parameter)
  Q_UNUSED(string)
  qDebug() << "osc dispatch";
  return true;
}

bool StateMachineEngineTest::ActionSs3Dispatch(
    const wchar_t wch, const std::basic_string_view<size_t> parameters) {
  Q_UNUSED(wch)
  Q_UNUSED(parameters)
  qDebug() << "ss3 dispatch";
  return true;
}

bool StateMachineEngineTest::FlushAtEndOfString() const {
  qDebug() << "flush at end of string";
  return true;
}

bool StateMachineEngineTest::DispatchControlCharsFromEscape() const {
  qDebug() << "dispatch control";
  return true;
}

bool StateMachineEngineTest::DispatchIntermediatesFromEscape() const {
  qDebug() << "dispatch intermediates";
  return true;
}
