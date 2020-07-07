#include "TermDispatchTracing.h"

Q_LOGGING_CATEGORY(LogTermDispatchDefault, "Log.TermDispath.Default");

TermDispatchTracing::TermDispatchTracing() {}

void TermDispatchTracing::TraceUnuseDispatch(const QString &str) const {
  qCDebug(LogTermDispatchDefault) << str;
}
