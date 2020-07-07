#pragma once

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LogTermDispatchDefault);

class TermDispatchTracing final {
 public:
  TermDispatchTracing();

  void TraceUnuseDispatch(const QString &str) const;
};
