#ifndef XMLPARSER_H_211217
#define XMLPARSER_H_211217

#include <string>

#include "element.h"

namespace XML
{

class Parser
{
  public:
    Parser(RawXML);
    Element parseRootElement();

  private:
    const RawXML sourceXML;
    size_t currentPos = 0;

    struct OpeningTag;

    Element parseElement();
    OpeningTag parseOpeningTag();
    Attributes parseAttributes();
    std::string parseAttributeValue();
    SubElements parseSubElements();
    LeafContent parseLeafContent();
    void parseClosingTag(ElementName);

    std::string parseName();
    void parseWhitespace();

    void tryparseProlog(); // currently we discard the Prolog
    bool tryparseChar(char);
    bool tryparseString(std::string);

    bool nameNext() const;
    bool openingTagNext() const;
    bool leafContentNext() const;
    void require(bool condition, std::string errorMessage) const;
    bool isInRange(size_t) const;

    const std::string whitespaceChars = " \t\n\v\f\r";
    const std::string nameDelimiters = "/>=" + whitespaceChars;
};

}

#endif
