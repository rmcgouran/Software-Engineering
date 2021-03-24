#ifndef ELEMENT_H_03062019
#define ELEMENT_H_03062019

#include <string>
#include <map>
#include <vector>

namespace XML
{

class Element;

using RawXML = std::string;
using ElementName = std::string;
using AttributeName = std::string;
using AttributeValue = std::string;
using Attributes = std::map<AttributeName, AttributeValue>;
using SubElements = std::map<ElementName, std::vector<Element>>;
using LeafContent = std::string;

class Element
{
  public:
    ElementName getName() const;
    AttributeValue getAttribute(AttributeName) const;
    Element getSubElement(ElementName, std::size_t subElementNum = 0) const;
    LeafContent getLeafContent() const;

    bool containsAttribute(AttributeName) const;
    bool containsSubElement(ElementName) const;
    unsigned int countSubElements(ElementName) const;
    bool isLeaf() const;

  protected:
    Element(ElementName, Attributes);
    ElementName name;
    Attributes attributes;
    SubElements subElements;
    LeafContent leafContent;
};

class InternalNodeElement : public Element
{
  public: InternalNodeElement(ElementName, Attributes, SubElements);
};

class LeafElement : public Element
{
  public: LeafElement(ElementName, Attributes, LeafContent);
};

class SelfClosingElement : public LeafElement
{
  public: SelfClosingElement(ElementName, Attributes);
};

}

#endif
