#include "earth.h"
#include "parseNMEA.h"

namespace NMEA
{
  bool isSupportedSentenceFormat(std::string format)
  {
      // Stub definition, needs implementing
      return false;
  }

  bool isWellFormedSentence(std::string candidateSentence)
  {
      // Stub definition, needs implementing
      return true;
  }

  bool hasCorrectChecksum(std::string sentence)
  {
      // Stub definition, needs implementing
      return false;
  }

  SentenceData parseSentenceData(std::string sentence)
  {
      // Stub definition, needs implementing
      return {"",{}};
  }

  GPS::Position interpretSentenceData(SentenceData data)
  {
      // Stub definition, needs implementing
      return GPS::Earth::NorthPole;
  }

  std::vector<GPS::Position> positionsFromLog(std::istream & log)
  {
      // Stub definition, needs implementing
      return {};
  }

}
