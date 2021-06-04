#include <stdexcept>
#include <iostream>

#include "generator.h"

namespace XML
{
  Generator::Generator(unsigned int indentationSpaces) : indentationSpaces{indentationSpaces} {}

  void Generator::basicXMLDeclaration()
  {
      xml << "<?xml";

      Attributes xmlAttributes =
       { {"version","1.0"},
         {"encoding","ISO-8859-1"},
         {"standalone","yes"}
        };

      attributes(xmlAttributes);

      xml << "?>";
      newline();
  }

  void Generator::openBasicGPXElement()
  {
      const Attributes gpxAttributes =
        { {"version","1.1"},
          {"creator","NTU"},
          {"xmlns","http://www.topografix.com/GPX/1/1"},
          {"xmlns:xsd","http://www.w3.org/2001/XMLSchema"},
          {"xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance"},
          {"xsi:schemaLocation","http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd"}
        };
      openElement("gpx",gpxAttributes);
  }

  void Generator::element(const ElementName& name, const Attributes& attribs, const LeafContent& content)
  {
      indent();
      openingTag(name,attribs);
      xml << content;
      closingTag(name);
      newline();
  }

  void Generator::openElement(const ElementName& name, const Attributes& attribs)
  {
      indent();
      openingTag(name,attribs);
      unclosedTags.push(name);
      ++indentationLevel;
      newline();
  }

  void Generator::closeElement()
  {
      if (unclosedTags.empty()) throw std::domain_error("No XML element to close.");
      --indentationLevel;
      indent();
      closingTag(unclosedTags.top());
      unclosedTags.pop();
      newline();
  }

  void Generator::closeAllElements()
  {
      while (! unclosedTags.empty())
      {
          --indentationLevel;
          indent();
          closingTag(unclosedTags.top());
          unclosedTags.pop();
          newline();
      }
  }

  std::string Generator::closeAllElementsAndExtractString()
  {
      closeAllElements();
      return xml.str();
  }

  void Generator::openingTag(const ElementName& name, const Attributes& attribs)
  {
      xml << '<' << name;
      attributes(attribs);
      xml << '>';
  }

  void Generator::closingTag(const ElementName& name)
  {
      xml << "</" << name << '>';
  }

  void Generator::attributes(const Attributes& attribsMap)
  {
      for (const std::pair<AttributeName,AttributeValue>& nameValPair : attribsMap)
      {
          attribute(nameValPair.first,nameValPair.second);
      }
  }

  void Generator::attribute(const AttributeName& name, const AttributeValue& value)
  {
      xml << " " << name << "=\"" << value << "\"";
  }

  void Generator::indent()
  {
      xml << std::string(indentationLevel*indentationSpaces,' ');
  }

  void Generator::newline()
  {
      xml << std::endl;
  }
}
