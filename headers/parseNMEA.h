#ifndef PARSENMEA_H_211217
#define PARSENMEA_H_211217

#include <string>
#include <list>
#include <vector>
#include <istream>

#include "position.h"

namespace NMEA
{
  /* Determine whether the parameter is the three-character code for a sentence format
   * that is currently supported.
   * Currently the only supported sentence formats are "GLL", "GGA" and "RMC".
   */
  bool isSupportedSentenceFormat(std::string);


  /* Determine whether the parameter is a well-formed NMEA sentence.
   * A NMEA sentence contains the following contents:
   *   - the prefix "$GP";
   *   - followed by a sequence of three (English) alphabet characters identifying
   *     the sentence format;
   *   - followed by a sequence of one or more comma-prefixed data fields;
   *   - followed by a '*' character;
   *   - followed by a two-character hexadecimal checksum.
   *
   * Data fields may contain any characters except '$' or '*' (or ','), and may be empty.
   *
   * For ill-formed sentences, this function returns false (it does not throw an
   * exception or terminate the program).
   *
   * Note that this function does NOT check whether the sentence format is supported.
   */
  bool isWellFormedSentence(std::string);


  /* Verify whether a sentence has the correct checksum.
   * To be correct, the checksum value should equal the XOR reduction of the character
   * codes of all characters between the '$' and the '*' (exclusive).
   *
   * Pre-condition: the parameter is a well-formed NMEA sentence.
   */
  bool hasCorrectChecksum(std::string);


  // Stores the fields of a NMEA sentence, excluding the checksum.
  struct SentenceData
  {
      /* Stores the NMEA sentence format, excluding the 'GP' prefix.
       * E.g. "GLL".
       */
      std::string format;

      /* Stores the data fields, excluding the format and checksum.
       * E.g. the first element of the vector could be "5425.32",
       * and the second element could be "N".
       */
      std::vector<std::string> dataFields;
  };


  /* Extracts the sentence format and the field contents from a NMEA sentence string.
   * The '$GP' and the checksum are ignored.
   *
   * Pre-condition: the parameter is a well-formed NMEA sentence.
   */
  SentenceData parseSentenceData(std::string);


  /* Computes a Position from NMEA Sentence Data.
   * Currently only supports the GLL, GGA and RMC sentence formats.
   *
   * Throws a std::invalid_argument exception for unsupported sentence formats, or
   * if the neccessary data fields are missing or contain invalid data.
   */
  GPS::Position interpretSentenceData(SentenceData);


  /* Reads a stream of NMEA sentences (one sentence per line), and constructs a
   * vector of Positions, ignoring any lines that do not contain valid sentences.
   *
   * A line is a valid sentence if all of the following are true:
   *  - the line is a well-formed NMEA sentence;
   *  - the checksum is valid;
   *  - the sentence format is supported (currently GLL, GGA and RMC);
   *  - the neccessary data fields are present and contain valid data.
   */
  std::vector<GPS::Position> positionsFromLog(std::istream &);

}

#endif
