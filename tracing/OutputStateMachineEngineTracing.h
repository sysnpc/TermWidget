#pragma once

#include <QLoggingCategory>
#include <QMetaEnum>
#include <QObject>

Q_DECLARE_LOGGING_CATEGORY(LogOutputStateMachineEngine);

class OutputStateMachineEngineTracing : public QObject {
  Q_OBJECT
 public:
  // Names primarily from http://inwap.com/pdp10/ansicode.txt
  enum Codes {
    CUU = 0,
    CUD,
    CUF,
    CUB,
    CNL,
    CPL,
    CHA,
    CUP,
    ED,
    EL,
    SGR,
    DECSC,
    DECRC,
    DECSET,
    DECRST,
    DECKPAM,
    DECKPNM,
    DSR,
    DA,
    VPA,
    HPR,
    VPR,
    ICH,
    DCH,
    SU,
    SD,
    ANSISYSSC,
    ANSISYSRC,
    IL,
    DL,
    DECSTBM,
    NEL,
    IND,
    RI,
    OSCWT,
    HTS,
    CHT,
    CBT,
    TBC,
    ECH,
    DesignateG0,
    DesignateG1,
    DesignateG2,
    DesignateG3,
    HVP,
    DECSTR,
    RIS,
    DECSCUSR,
    DTTERM_WM,
    OSCCT,
    OSCSCC,
    OSCRCC,
    REP,
    OSCFG,
    OSCBG,
    DECALN,
    // Only use this last enum as a count of the number of codes.
    NUMBER_OF_CODES
  };
  Q_ENUM(Codes);

 public:
  OutputStateMachineEngineTracing();

  void Log(const Codes &codes);

 private:
  QMetaEnum _metaCodes;
};

Q_DECLARE_METATYPE(OutputStateMachineEngineTracing::Codes);
