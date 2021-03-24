#include <map>
#include <vector>

#include "element.h"
#include "parser.h"

namespace XML
{

Element::Element(ElementName name, Attributes attributes)
    : name{name}, attributes{attributes}
{}

InternalNodeElement::InternalNodeElement(ElementName name, Attributes attributes, SubElements subElements)
    : Element(name, attributes)
{
    this->subElements = subElements;
}

LeafElement::LeafElement(ElementName name, Attributes attributes, LeafContent leafContent)
    : Element(name,attributes)
{
    this->leafContent = leafContent;
}

SelfClosingElement::SelfClosingElement(ElementName name, Attributes attributes)
    : LeafElement(name,attributes,"")
{}

ElementName Element::getName() const
{
    return name;
}

bool Element::containsAttribute(AttributeName attributeName) const
{
    return attributes.count(attributeName);
}

bool Element::containsSubElement(ElementName subElementName) const
{
    return subElements.count(subElementName); // replace with .contains() from C++20
}

unsigned int Element::countSubElements(ElementName subElementName) const
{
    if (containsSubElement(subElementName))
    {
        return subElements.at(subElementName).size();
    }
    else
    {
        return 0;
    }
}

bool Element::isLeaf() const
{
    return subElements.empty();
}

AttributeValue Element::getAttribute(AttributeName attributeName) const
{
    return attributes.at(attributeName);
}

Element Element::getSubElement(ElementName subElementName, size_t index) const
{
    return subElements.at(subElementName).at(index);
}

LeafContent Element::getLeafContent() const
{
    return leafContent;
}

}
