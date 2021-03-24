#include "earth.h"
#include "parseNMEA.h"
namespace NMEA
{

  bool isSupportedSentenceFormat(std::string format)
  {

      //Use find to see if the supported sentance format code is at the start of the program
      std::vector<std::string> SupportedCodes{"GLL","GGA","RMC"};
      return std::find(std::begin(SupportedCodes), std::end(SupportedCodes), format) != std::end(SupportedCodes);
  }
  bool isWellFormedSentence(std::string candidateSentence)
    {
        // Uses regex to check for a valid NMEA sentence
        std::regex regexSentence("\\$GP[A-Z]{3},[\-A-Za-z0-9,.]*\\*[0-9A-Fa-f]{2}");
        return regex_match(candidateSentence, regexSentence);
    }
  bool hasCorrectChecksum(std::string sentence)
    {
        assert (isWellFormedSentence(sentence));
        //Get original checksum and convert to int
        std::string checkSum = sentence.substr(sentence.find('*') + 1);
        int iCheckSum = std::stoi(checkSum, 0, 16);

        //Start and endpoint of characters to XOR

        size_t first = sentence.find('$');
        size_t last = sentence.find('*', first);
        std::string sub = sentence.substr(first + 1, last - first -1);


        //XOR value
        unsigned int cSum = 0;

        //Loop over each character and XOR it

        for (unsigned int i = 0; i < sub.length(); i++ ){
            cSum ^= sub[i];
        }
        return((unsigned)iCheckSum == cSum);
    }

   SentenceData parseSentenceData(std::string sentence)
  {
       assert (isWellFormedSentence(sentence));
        //Create substring to hold sentence format
        std::string sentenceFormat = sentence.substr(3,3);
    
        //Vector to hold field data
        std::vector<std::string> fields;
        std::string dataContainer = "";

        //Loop pushes seperate fields into the vector value
        for (unsigned int i = 7 ; i <sentence.length() - 2; i++){
            char Container = sentence[i];
            if ((Container == ',')|| (Container == '*')){
                fields.push_back(dataContainer);
                dataContainer = "";
              }
              else {
                  dataContainer = dataContainer + Container;
              }
          }

          return {sentenceFormat, fields};
  }
  GPS::Position sentanceInterpreter(SentenceData data, unsigned expectedSize, int latPos, int longPos, int northingPos, int eastingPos, int elevatonPos = 0){

      if(data.dataFields.size() != expectedSize){
          throw std::invalid_argument("Unsupported sentence format.");
      }
      //Gets data from the array
       std::string lat = data.dataFields[latPos];
       std::string lon = data.dataFields[longPos];
       char northing = data.dataFields[northingPos].front();
       char easting = data.dataFields[eastingPos].front();
       std::string elevation = data.dataFields[elevatonPos];
       //If the elevation position is passed through use constructor with elevation
       if(!(elevatonPos == 0)){
           return GPS::Position(lat,northing, lon,easting,elevation);
       }
       return GPS::Position(lat,northing,lon,easting);
  }
  GPS::Position interpretSentenceData(SentenceData data)
  {
      
      if(!(isSupportedSentenceFormat(data.format) == true)){
          throw std::invalid_argument("Unsupported sentence format.");
      }
      if(data.format == "GLL"){
           return sentanceInterpreter(data,5,0,2,1,3);
      }
      if(data.format == "GGA"){
        return sentanceInterpreter(data,14,1,3,2,4,8);
        }
      if(data.format == "RMC"){
        return sentanceInterpreter(data,11,2,4,3,5);
        }
      throw std::invalid_argument("Unsupported sentance format");
  }

  std::vector<GPS::Position> positionsFromLog(std::istream & log)
    {


      //Declare vector positions 
      std::vector<GPS::Position> vec;

      //Loop the file until no sentences remain
      while (true){
          //Create variable to represent single log file line
          std::string data;
          log >> data;
          if (log.eof()){
              break;
          }
          if (!data.length()){
              continue;
          }

       //Check if input values are valid
       try {
              if(!isWellFormedSentence(data)){
                  throw std::invalid_argument("Unsupported sentance format");
              }
          SentenceData sentence (parseSentenceData(data));

          if ((isSupportedSentenceFormat(sentence.format))&&(isWellFormedSentence(data))&&(hasCorrectChecksum(data))){
              vec.push_back(interpretSentenceData(sentence));
          }
       }

       //Catching invalid inputs
       catch (const std::invalid_argument &) {
            //Catch the error but do not handle it
       }

       }
          return vec;
      }

}
