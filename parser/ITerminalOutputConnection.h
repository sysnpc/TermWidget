#pragma once

#include <string_view>

namespace PresudoMicrosoft::Console {
  class ITerminalOutputConnection {
   public:
    virtual ~ITerminalOutputConnection() = 0;

    virtual bool WriteTerminalUtf8(const std::string_view str) = 0;
    virtual bool WriteTerminalW(const std::wstring_view wstr) = 0;
  };

  inline ITerminalOutputConnection::~ITerminalOutputConnection() {}

}  // namespace PresudoMicrosoft::Console
