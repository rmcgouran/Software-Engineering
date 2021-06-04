#include <sstream>
#include <fstream>
#include <iomanip>

#include "xml/parser.h"

#include "parseGPX.h"

namespace GPX
{
  std::vector<GPS::RoutePoint> parseRoute(std::string source, bool isFileName)
  {
      using namespace std;
      using namespace GPS;
      using namespace XML;
      int num,i,j,total,skipped;
      metres deltaH,deltaV;
      string lat,lon,el,name;
      ostringstream oss,oss2;
      std::vector<RoutePoint> result;
      Element ele = SelfClosingElement("",{}), temp = ele, temp2 = ele; // Work-around because there's no public constructor in Element.
      Position startPos(0,0), prevPos = startPos, nextPos = startPos; // Same thing but for Position.
      if (isFileName) {
          ifstream fs(source);
          if (! fs.good()) throw invalid_argument("Error opening source file '" + source + "'.");
          oss << "Source file '" << source << "' opened okay." << endl;
          while (fs.good()) {
              getline(fs, name); // Using name as temporary variable as we don't need it until later
              oss2 << name << endl;
          }
          source = oss2.str();
      }
      ele = Parser(source).parseRootElement();
      if (ele.getName() != "gpx") throw domain_error("Missing 'gpx' element.");
      if (! ele.containsSubElement("rte")) throw domain_error("Missing 'rte' element.");
      ele = ele.getSubElement("rte");
      /*
        if (ele.containsSubElement("name")) {
          temp = ele.getSubElement("name");
          name = temp.getLeafContent();
          i = name.find_first_not_of(' ');
          j = name.find_last_not_of(' ');
          routeName = (i == -1) ? "" : name.substr(i,j-i+1);
          oss << "Route name is: " << routeName << endl;
      }
      */
      num = 0;
      if (! ele.containsSubElement("rtept")) throw domain_error("Missing 'rtept' element.");
      total = ele.countSubElements("rtept");
      temp = ele.getSubElement("rtept");
      if (! temp.containsAttribute("lat")) throw domain_error("Missing 'lat' attribute.");
      if (! temp.containsAttribute("lon")) throw domain_error("Missing 'lon' attribute.");
      lat = temp.getAttribute("lat");
      lon = temp.getAttribute("lon");
      if (temp.containsSubElement("ele")) {
          temp2 = temp.getSubElement("ele");
          el = temp2.getLeafContent();
          Position startPos = Position(lat,lon,el);
          result.push_back({startPos,""});
          oss << "Position added: " << endl; // << startPos.toString() << endl; // Need to update since removing toString()
          ++num;
      } else {
          Position startPos = Position(lat,lon);
          result.push_back({startPos,""});
          oss << "Position added: " << endl; // << startPos.toString() << endl; // Need to update since removing toString()
          ++num;
      }
      if (temp.containsSubElement("name")) {
          temp2 = temp.getSubElement("name");
          name = temp2.getLeafContent();
          i = name.find_first_not_of(' ');
          j = name.find_last_not_of(' ');
          name = (i == -1) ? "" : name.substr(i,j-i+1);
      } else name = ""; // Fixed bug by adding this.
      result.front().name = name;
      prevPos = result.back().position, nextPos = result.back().position;
      skipped = 0;
      while (num+skipped < total) {
          temp = ele.getSubElement("rtept",num+skipped);
          if (! temp.containsAttribute("lat")) throw domain_error("Missing 'lat' attribute.");
          if (! temp.containsAttribute("lon")) throw domain_error("Missing 'lon' attribute.");
          lat = temp.getAttribute("lat");
          lon = temp.getAttribute("lon");
          if (temp.containsSubElement("ele")) {
              temp2 = temp.getSubElement("ele");
              el = temp2.getLeafContent();
              nextPos = Position(lat,lon,el);
          } else nextPos = Position(lat,lon);
          /*
          if (areSameLocation(nextPos, prevPos))
          {
              oss << "Position ignored: " << nextPos.toString() << endl;
              ++skipped;
          }
          else {
          */
              if (temp.containsSubElement("name")) {
                  temp2 = temp.getSubElement("name");
                  name = temp2.getLeafContent();
                  i = name.find_first_not_of(' ');
                  j = name.find_last_not_of(' ');
                  // if (i == string::npos)
                  // {
                  //    name = "";
                  // }
                  // else
                  // {
                  //   name.erase(0,i);
                  //   j = name.find_last_not_of(' ');
                  //   name.erase(j+1);
                  // }
                  name = (i == -1) ? "" : name.substr(i,j-i+1); // So much shorter than Ken's version :)
              } else name = ""; // Fixed bug by adding this.
              result.push_back({nextPos,name});
              oss << "Position added: " << endl; // << nextPos.toString() << endl; // Need to update since removing toString()
              ++num;
              prevPos = nextPos;
          // }
      }
      oss << num << " positions added." << endl;
      // cout << oss.str();
      return result;
  }

  std::vector<GPS::TrackPoint> parseTrack(std::string source, bool isFileName)
  {
      using namespace std;
      using namespace GPS;
      using namespace XML;
      int num,i,j,total,skipped;
      metres deltaH,deltaV;
      string lat,lon,el,name,time;
      tm t;
      ostringstream oss,oss2;
      istringstream iss;
      std::vector<TrackPoint> result;
      Element ele = SelfClosingElement("",{}), temp = ele, temp2 = ele, ele2 = ele; // Work-around because there's no public constructor in Element.
      Position startPos(0,0), prevPos = startPos, nextPos = startPos; // Same thing but for Position.
      if (isFileName) {
          ifstream fs(source);
          if (! fs.good()) throw invalid_argument("Error opening source file '" + source + "'.");
          oss << "Source file '" << source << "' opened okay." << endl;
          while (fs.good()) {
              getline(fs, name); // Using name as temporary variable as we don't need it until later
              oss2 << name << endl;
          }
          source = oss2.str();
      }
      ele = Parser(source).parseRootElement();
      if (ele.getName() != "gpx") throw domain_error("Missing 'gpx' element.");
      if (! ele.containsSubElement("trk")) throw domain_error("Missing 'trk' element.");
      ele = ele.getSubElement("trk");
      /*
      if (ele.containsSubElement("name")) {
          temp = ele.getSubElement("name");
          name = temp.getLeafContent();
          i = name.find_first_not_of(' ');
          j = name.find_last_not_of(' ');
          routeName = (i == -1) ? "" : name.substr(i,j-i+1);
          oss << "Track name is: " << routeName << endl;
      }
      */
      num = 0;
      if (! ele.containsSubElement("trkseg")) {
          if (! ele.containsSubElement("trkpt")) throw domain_error("Missing 'trkpt' element.");
          total = ele.countSubElements("trkpt");
          temp = ele.getSubElement("trkpt");
          if (! temp.containsAttribute("lat")) throw domain_error("Missing 'lat' attribute.");
          if (! temp.containsAttribute("lon")) throw domain_error("Missing 'lon' attribute.");
          lat = temp.getAttribute("lat");
          lon = temp.getAttribute("lon");
          if (temp.containsSubElement("ele")) {
              temp2 = temp.getSubElement("ele");
              el = temp2.getLeafContent();
              startPos = Position(lat,lon,el);
              result.push_back({startPos,name,t});
              oss << "Position added: " << endl; // << startPos.toString() << endl; // Need to update since removing toString()
              ++num;
          } else {
              startPos = Position(lat,lon);
              result.push_back({startPos,name,t});
              oss << "Position added: " << endl; // << startPos.toString() << endl; // Need to update since removing toString()
              ++num;
          }
          if (temp.containsSubElement("name")) {
              temp2 = temp.getSubElement("name");
              name = temp2.getLeafContent();
              i = name.find_first_not_of(' ');
              j = name.find_last_not_of(' ');
              name = (i == -1) ? "" : name.substr(i,j-i+1);
          } else name = ""; // Fixed bug by adding this.
          result.back().name = name;
          if (! temp.containsSubElement("time")) throw domain_error("Missing 'time' element.");
          temp2 = temp.getSubElement("time");
          time = temp2.getLeafContent();
          iss.str(time);
          iss >> std::get_time(&t,"%Y-%m-%dT%H:%M:%SZ");
          if (iss.fail()) throw std::domain_error("Malformed date/time content: " + time);
          result.back().dateTime = t;
          prevPos = result.back().position, nextPos = result.back().position;
          skipped = 0;
          while (num+skipped < total) {
              temp = ele.getSubElement("trkpt",num+skipped);
              if (! temp.containsAttribute("lat")) throw domain_error("Missing 'lat' attribute.");
              if (! temp.containsAttribute("lon")) throw domain_error("Missing 'lon' attribute.");
              lat = temp.getAttribute("lat");
              lon = temp.getAttribute("lon");
              if (temp.containsSubElement("ele")) {
                  temp2 = temp.getSubElement("ele");
                  el = temp2.getLeafContent();
                  nextPos = Position(lat,lon,el);
              } else nextPos = Position(lat,lon);
              if (! temp.containsSubElement("time")) throw domain_error("Missing 'time' element.");
              temp2 = temp.getSubElement("time");
              time = temp2.getLeafContent();
              iss.str(time);
              iss >> std::get_time(&t,"%Y-%m-%dT%H:%M:%SZ");
              if (iss.fail()) throw std::domain_error("Malformed date/time content: " + time);
              /* if (areSameLocation(nextPos, prevPos)) {
                  oss << "Position ignored: " << nextPos.toString() << endl;
                  ++skipped;
              } else {
              */
                  if (temp.containsSubElement("name")) {
                      temp2 = temp.getSubElement("name");
                      name = temp2.getLeafContent();
                      i = name.find_first_not_of(' ');
                      j = name.find_last_not_of(' ');
                      name = (i == -1) ? "" : name.substr(i,j-i+1);
                  } else name = ""; // Fixed bug by adding this.
                  result.push_back({nextPos,name,t});
                  oss << "Position added: " << endl; // << nextPos.toString() << endl; // Need to update since removing toString()
                  oss << " at time: " << std::put_time(&t,"%c") << endl;
                  ++num;
                  prevPos = nextPos;
              // }
          }
      }
      else
      {
          for (unsigned int segNum = 0; segNum < ele.countSubElements("trkseg"); ++segNum) {
              ele2 = ele.getSubElement("trkseg",segNum);
              if (! ele2.containsSubElement("trkpt")) throw domain_error("Missing 'trkpt' element.");
              total = ele2.countSubElements("trkpt");
              skipped = -num; // Setting skipped to start at -num (rather than 0) cancels any points accumulated from previous segments
                              // We have to set it here, rather than just before the loop, because num may increment in the next if-statement
              if (segNum == 0) {
                  temp = ele2.getSubElement("trkpt");
                  if (! temp.containsAttribute("lat")) throw domain_error("Missing 'lat' attribute.");
                  if (! temp.containsAttribute("lon")) throw domain_error("Missing 'lon' attribute.");
                  lat = temp.getAttribute("lat");
                  lon = temp.getAttribute("lon");
                  if (temp.containsSubElement("ele")) {
                      temp2 = temp.getSubElement("ele");
                      el = temp2.getLeafContent();
                      startPos = Position(lat,lon,el);
                      result.push_back({startPos,name,t});
                      oss << "Position added: " << endl; // << startPos.toString() << endl; // Need to update since removing toString()
                      ++num;
                  }
                  else {
                      startPos = Position(lat,lon);
                      result.push_back({startPos,name,t});
                      oss << "Position added: " << endl; // << startPos.toString() << endl; // Need to update since removing toString()
                      ++num;
                  }
                  if (temp.containsSubElement("name")) {
                      temp2 = temp.getSubElement("name");
                      name = temp2.getLeafContent();
                      i = name.find_first_not_of(' ');
                      j = name.find_last_not_of(' ');
                      name = (i == -1) ? "" : name.substr(i,j-i+1);
                  } else name = ""; // Fixed bug by adding this.
                  result.back().name = name;
                  if (! temp.containsSubElement("time")) throw domain_error("Missing 'time' element.");
                  temp2 = temp.getSubElement("time");
                  time = temp2.getLeafContent();
                  iss.str(time);
                  iss >> std::get_time(&t,"%Y-%m-%dT%H:%M:%SZ");
                  if (iss.fail()) throw std::domain_error("Malformed date/time content: " + time);
                  result.back().dateTime = t;
             }
             prevPos = result.back().position, nextPos = result.back().position;
             while (num+skipped < total) {
                 temp = ele2.getSubElement("trkpt",num+skipped);
                 if (! temp.containsAttribute("lat")) throw domain_error("Missing 'lat' attribute.");
                 if (! temp.containsAttribute("lon")) throw domain_error("Missing 'lon' attribute.");
                 lat = temp.getAttribute("lat");
                 lon = temp.getAttribute("lon");
                 if (temp.containsSubElement("ele")) {
                     temp2 = temp.getSubElement("ele");
                     el = temp2.getLeafContent();
                     nextPos = Position(lat,lon,el);
                 } else nextPos = Position(lat,lon);
                 if (! temp.containsSubElement("time")) throw domain_error("Missing 'time' element.");
                 temp2 = temp.getSubElement("time");
                 time = temp2.getLeafContent();
                 iss.str(time);
                 iss >> std::get_time(&t,"%Y-%m-%dT%H:%M:%SZ");
                 if (iss.fail()) throw std::domain_error("Malformed date/time content: " + time);
                 /*
                 if (areSameLocation(nextPos, prevPos)) {
                     oss << "Position ignored: " << nextPos.toString() << endl;
                     ++skipped;
                 } else {
                 */
                     if (temp.containsSubElement("name")) {
                         temp2 = temp.getSubElement("name");
                         name = temp2.getLeafContent();
                         i = name.find_first_not_of(' ');
                         j = name.find_last_not_of(' ');
                         name = (i == -1) ? "" : name.substr(i,j-i+1);
                     } else name = ""; // Fixed bug by adding this.
                     result.push_back({nextPos,name,t});
                     oss << "Position added: " << endl; // << nextPos.toString() << endl; // Need to update since removing toString()
                     oss << " at time: " << std::put_time(&t,"%c") << endl;
                     ++num;
                     prevPos = nextPos;
                 // }
             }
          }
      }
      oss << num << " positions added." << endl;
      // cout << oss.str();
      return result;
  }
}
