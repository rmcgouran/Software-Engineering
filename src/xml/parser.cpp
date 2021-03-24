#include <cassert>
#include <map>
#include <vector>
#include <cctype>

#include "element.h"
#include "parser.h"

namespace XML
{

Parser::Parser(RawXML xml)
    : sourceXML{xml}
{}

struct Parser::OpeningTag
{
    ElementName name;
    std::map<AttributeName, AttributeValue> attributes;
    bool isSelfClosing;
};

Element Parser::parseRootElement()
{
    parseWhitespace();
    tryparseProlog();
    parseWhitespace();
    return parseElement();
}

void Parser::tryparseProlog()
{
    if (tryparseString("<?xml"))
    {
        parseWhitespace();
        parseAttributes();
        parseWhitespace();
        require(tryparseString("?>"), "prolog not terminated correctly.");
    }
}

Element Parser::parseElement()
{
    OpeningTag openingTag = parseOpeningTag();

    if (openingTag.isSelfClosing)
    {
        return SelfClosingElement(openingTag.name,openingTag.attributes);
    }

    if (leafContentNext())
    {
        LeafContent leafContent = parseLeafContent();
        parseClosingTag(openingTag.name);
        return LeafElement(openingTag.name,openingTag.attributes,leafContent);
    }
    else
    {
        SubElements subElements = parseSubElements();
        parseClosingTag(openingTag.name);
        return InternalNodeElement(openingTag.name,openingTag.attributes,subElements);
    }
}

Parser::OpeningTag Parser::parseOpeningTag()
{
    OpeningTag tag;
    require(tryparseChar('<'), "opening tag not started correctly.");
    tag.name = parseName();
    parseWhitespace();
    tag.attributes = parseAttributes();

    if (tryparseChar('>'))
    {
        tag.isSelfClosing = false;
    }
    else
    {
        require(tryparseString("/>"), "opening tag not terminated correctly.");
        tag.isSelfClosing = true;
    }

    return tag;
}

void Parser::parseClosingTag(ElementName tagName)
{
    std::string closingTag = "</" + tagName + ">";
    require(tryparseString(closingTag), "missing closing tag: " + closingTag);
}

Attributes Parser::parseAttributes()
{
    std::map<AttributeName, AttributeValue> attributes;
    while (nameNext())
    {
        AttributeName name = parseName();
        parseWhitespace();
        require(tryparseChar('='),"attribute missing '='");
        parseWhitespace();
        AttributeValue value = parseAttributeValue();
        parseWhitespace();
        attributes[name] = value;
    }
    return attributes;
}

SubElements Parser::parseSubElements()
{
    std::map<ElementName, std::vector<Element>> subElements;

    parseWhitespace();
    while (openingTagNext())
    {
        Element element = parseElement();
        subElements[element.getName()].push_back(element);

        parseWhitespace();
    }

    return subElements;
}

AttributeValue Parser::parseAttributeValue()
{
    require(tryparseChar('\"'), "attribute value not delimited correctly.");
    size_t delimiterPos = sourceXML.find('\"',currentPos);
    require(isInRange(delimiterPos), "attribute value not delimited correctly.");
    AttributeValue val = sourceXML.substr(currentPos,delimiterPos-currentPos);
    currentPos = delimiterPos+1;
    return val;
}

LeafContent Parser::parseLeafContent()
{
    assert (leafContentNext());

    size_t contentEnd = sourceXML.find('<',currentPos);
    require (isInRange(contentEnd), "no closing tag.");

    LeafContent leafContent = sourceXML.substr(currentPos,contentEnd-currentPos);
    currentPos = contentEnd;
    return leafContent;
}

std::string Parser::parseName()
{
    assert (nameNext());
    size_t textEnd = sourceXML.find_first_of(nameDelimiters, currentPos);

    require(isInRange(textEnd), "unterminated name.");

    std::string text = sourceXML.substr(currentPos,textEnd-currentPos);
    currentPos = textEnd;
    return text;
}

void Parser::parseWhitespace()
{
    size_t whitespaceEnd = sourceXML.find_first_not_of(whitespaceChars,currentPos);
    currentPos = isInRange(whitespaceEnd) ? whitespaceEnd : sourceXML.length();
}

bool Parser::tryparseChar(char charToMatch)
{
    if (isInRange(currentPos) && (sourceXML[currentPos] == charToMatch))
    {
        ++currentPos;
        return true;
    }
    return false;
}

bool Parser::tryparseString(std::string stringToMatch)
{
    if (isInRange(currentPos))
    {
        std::string parsedString = sourceXML.substr(currentPos,stringToMatch.length());
        if (parsedString == stringToMatch)
        {
            currentPos += stringToMatch.length();
            return true;
        }
    }
    return false;
}

bool Parser::nameNext() const
{
    return (isInRange(currentPos) && isalpha(sourceXML[currentPos]) );
}

bool Parser::openingTagNext() const
{
    return (isInRange(currentPos+1) && (sourceXML[currentPos] == '<') && (sourceXML[currentPos+1] != '/') );
}

bool Parser::leafContentNext() const
{
     size_t nextTagStart = sourceXML.find('<',currentPos);

     require(isInRange(nextTagStart+1), "unterminated element content.");

     return sourceXML[nextTagStart+1] == '/'; // the next tag after leaf content must be a closing tag
}

void Parser::require(bool condition, std::string errorMessage) const
{
  if (! condition) throw std::domain_error("Malformed XML: " + errorMessage);
}

bool Parser::isInRange(size_t index) const
{
    return index < sourceXML.length();
}

}
