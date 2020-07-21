HEADERS += \
    $$PWD/TermRender.h \
    $$PWD/TermViewport.h \
    $$PWD/TermWidget.h \
#    $$PWD/buffer/CharRow.h \
#    $$PWD/buffer/CharRowCell.h \
#    $$PWD/buffer/Cursor.h \
#    $$PWD/buffer/Row.h \
    $$PWD/buffer/TextAttribute.h \
    $$PWD/buffer/TextBuffer.h \
    $$PWD/buffer/TextCell.h \
    $$PWD/buffer/TextColor.h \
    $$PWD/buffer/TextCursor.h \
    $$PWD/buffer/TextRow.h \
    $$PWD/buffer/TextStyle.h \
#    $$PWD/buffer/Types.h \
#    $$PWD/buffer/UAttrCharCell.h \
    $$PWD/buffer/UChar.h \
#    $$PWD/buffer/Viewport.h \
    $$PWD/common/Ascii.h \
    $$PWD/common/DispatchTypes.h \
    $$PWD/common/precomp.h \
    $$PWD/config/TermConfig.h \
    $$PWD/parser/IStateMachineEngine.h \
    $$PWD/parser/ITermDispatch.h \
    $$PWD/parser/ITerminalOutputConnection.h \
    $$PWD/parser/OutputStateMachineEngine.h \
    $$PWD/parser/StateMachine.h \
    $$PWD/parser/TermDispatch.h \
#    $$PWD/render/IRenderData.h \
    $$PWD/terminal/Charsets.h \
    $$PWD/terminal/ITerminal.h \
    $$PWD/terminal/ITerminalInput.h \
    $$PWD/terminal/Terminal.h \
    $$PWD/terminal/TerminalDispatch.h \
    $$PWD/terminal/TerminalOutput.h \
    $$PWD/tools/Argb.h \
    $$PWD/tools/Base64.h \
    $$PWD/tools/til/at.h \
#    $$PWD/tracing/OutputStateMachineEngineTracing.h \
    $$PWD/tracing/ParserTracing.h \
    $$PWD/tracing/StateMachineTracing.h \
    $$PWD/tracing/Telemetry.h \
    $$PWD/tracing/TermDispatchTracing.h

SOURCES += \
    $$PWD/TermRender.cpp \
    $$PWD/TermViewport.cpp \
    $$PWD/TermWidget.cpp \
#    $$PWD/buffer/CharRow.cpp \
#    $$PWD/buffer/CharRowCell.cpp \
#    $$PWD/buffer/Cursor.cpp \
#    $$PWD/buffer/Row.cpp \
    $$PWD/buffer/TextAttribute.cpp \
    $$PWD/buffer/TextBuffer.cpp \
    $$PWD/buffer/TextCell.cpp \
    $$PWD/buffer/TextColor.cpp \
    $$PWD/buffer/TextCursor.cpp \
    $$PWD/buffer/TextRow.cpp \
#    $$PWD/buffer/UAttrCharCell.cpp \
    $$PWD/buffer/UChar.cpp \
#    $$PWD/buffer/Viewport.cpp \
    $$PWD/config/TermConfig.cpp \
    $$PWD/parser/OutputStateMachineEngine.cpp \
    $$PWD/parser/StateMachine.cpp \
    $$PWD/terminal/ITerminalInput.cpp \
    $$PWD/terminal/Terminal.cpp \
    $$PWD/terminal/TerminalDispatch.cpp \
    $$PWD/terminal/TerminalOutput.cpp \
    $$PWD/tools/Base64.cpp \
#    $$PWD/tracing/OutputStateMachineEngineTracing.cpp \
    $$PWD/tracing/ParserTracing.cpp \
    $$PWD/tracing/StateMachineTracing.cpp \
    $$PWD/tracing/Telemetry.cpp \
    $$PWD/tracing/TermDispatchTracing.cpp
