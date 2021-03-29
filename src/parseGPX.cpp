#include "parseGPX.h"

namespace GPX {
    
    // Constants to reference using 'gpxFormat.CONSTANT'
    const struct gpxStruct {
        const std::string 
        name = "name", 
        latitude = "lat", 
        longitude = "lon", 
        elevation = "ele", 
        stage = "time", 
        track = "trk", 
        trackPoint = "trkpt", 
        trackSegment = "trkseg", 
        gpxCode = "gpx", 
        route = "rte", 
        routePoint = "rtept";
    } gpxFormat;

    // Throws domain errors when elements or attributes are missing from the GPX string.
    std::domain_error domError(std::string missingElement, bool attribute = false){
        const std::string errorMessage = "Missing '";
        const std::string errorElement = "' element.";
        const std::string errorAttribute = "' attribute.";
        std::string error;
        if(attribute == true){
            error = errorMessage + missingElement + errorAttribute;
        }else {
            error = errorMessage + missingElement + errorElement;
        }
        return std::domain_error(error);
    }

    // Return GPX file as a string.
    std::string parseFile(std::string filename) {
        std::ostringstream  oss;
        std::string name;
        std::ifstream fs(filename);
        if (! fs.good()) throw std::invalid_argument("Error opening source file '" + filename + "'.");
        while (fs.good()) {
            getline(fs, name);
            oss << name << std::endl;
        }
    return oss.str();
    }

    // Check for root XML elements.
    void containsAttributes(Element candidateElement) {
        if (candidateElement.getName() != gpxFormat.gpxCode) throw domError(gpxFormat.gpxCode);
        if (! candidateElement.containsSubElement(gpxFormat.route)) throw domError(gpxFormat.route);
        if (! candidateElement.getSubElement(gpxFormat.route).containsSubElement(gpxFormat.routePoint)) throw domError(gpxFormat.routePoint);
    }

    // Checks for the sub element "name" then removes 'white space' before and after.
    std::string getName(Element element){
        std::string name;
        if (element.containsSubElement(gpxFormat.name)) {
            name = element.getSubElement(gpxFormat.name).getLeafContent();
            trim(name);
        }
        return name;
    }

    // Checks for LAT and LON elements, returns a position depending on contents of element. Throws an error if they are not present.
    Position parsePosition(Element element) {
        std::string lat, lon, el;
        if (! element.containsAttribute(gpxFormat.latitude)) throw domError(gpxFormat.latitude, true);
        if (! element.containsAttribute(gpxFormat.longitude)) throw domError(gpxFormat.longitude, true);
        lat = element.getAttribute(gpxFormat.latitude);
        lon = element.getAttribute(gpxFormat.longitude);
        // Includes elevation element if it exists.
        if (element.containsSubElement(gpxFormat.elevation)) {
            el = element.getSubElement(gpxFormat.elevation).getLeafContent();
            return Position(lat, lon, el);
        } else {
            return Position(lat, lon);
        }
    }

    std::vector<RoutePoint> parseRoute(std::string source, bool isFileName) {
        std::vector<RoutePoint> result;
        if (isFileName) source = parseFile(source);

        Element rootElement = Parser(source).parseRootElement();
        // Check if root structue exists
        containsAttributes(rootElement);

        for (int num = 0; num < (int)rootElement.getSubElement(gpxFormat.route).countSubElements(gpxFormat.routePoint); ++num) {
            Element routePoint = rootElement.getSubElement(gpxFormat.route).getSubElement(gpxFormat.routePoint, num);
            Position Pos = parsePosition(routePoint);
            std::string name = getName(routePoint);
            result.push_back({Pos, name});
        }
    return result;
    }


    TrackPoint parseTrackPoint(Element element) {
        Position resultPosition = parsePosition(element);
        std::string lat, lon, name = getName(element), time;
        tm t;
        std::istringstream iss;
        if (! element.containsSubElement(gpxFormat.stage)) throw domError(gpxFormat.stage);
        time = element.getSubElement(gpxFormat.stage).getLeafContent();
        iss.str(time);
        iss >> std::get_time(&t,"%Y-%m-%dT%H:%M:%SZ");
        if (iss.fail()) throw std::domain_error("Malformed date/time content: " + time);
        return {resultPosition, name, t};
    }

    std::vector<TrackPoint> parseTrack(std::string source, bool isFileName) {
        int num = 0, total;
        std::vector<TrackPoint> result;
        if (isFileName) source = parseFile(source);
        Element rootElement = Parser(source).parseRootElement();
        if (rootElement.getName() != gpxFormat.gpxCode) throw domError(gpxFormat.gpxCode);
        if (! rootElement.containsSubElement(gpxFormat.track)) throw domError(gpxFormat.track);
        Element trackElement = rootElement.getSubElement(gpxFormat.track);
        if (trackElement.containsSubElement(gpxFormat.trackSegment)) {
            for (int segNum = 0; segNum < (int)trackElement.countSubElements(gpxFormat.trackSegment); ++segNum) {
                Element trackSegment = trackElement.getSubElement(gpxFormat.trackSegment,segNum);
                if (! trackSegment.containsSubElement(gpxFormat.trackPoint)) throw domError(gpxFormat.trackPoint);
                    total = trackSegment.countSubElements(gpxFormat.trackPoint);
                    num = 0;
                    while (num < total) {
                        Element trackPointElement = trackSegment.getSubElement(gpxFormat.trackPoint,num);
                        TrackPoint trackPoint = parseTrackPoint(trackPointElement);
                        result.push_back(trackPoint);
                        ++num;
                    }
            }
        } else if (! trackElement.containsSubElement(gpxFormat.trackPoint)) throw domError(gpxFormat.trackPoint);
            for (int num = 0; num < (int)trackElement.countSubElements(gpxFormat.trackPoint); ++num) {
                Element trackPointElement = trackElement.getSubElement(gpxFormat.trackPoint,num);
                TrackPoint trackPoint = parseTrackPoint(trackPointElement);
                result.push_back(trackPoint);
            }

    return result;
    }
}
