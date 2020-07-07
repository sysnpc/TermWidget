#include "OutputStateMachineEngineTracing.h"

Q_LOGGING_CATEGORY(LogOutputStateMachineEngine, "Log.OutputStateMachineEngine");

OutputStateMachineEngineTracing::OutputStateMachineEngineTracing()
    : _metaCodes{QMetaEnum::fromType<Codes>()} {}

void OutputStateMachineEngineTracing::Log(const Codes &codes) {
  qCDebug(LogOutputStateMachineEngine) << _metaCodes.valueToKey(codes);
}
