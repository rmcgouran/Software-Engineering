#ifndef XMLGENERATOR_H_211217
#define XMLGENERATOR_H_211217

#include <sstream>
#include <string>
#include <stack>

#include "element.h"

namespace XML
{
  class Generator
  {
    public:
      Generator(unsigned int indentationSpaces = 4);

      void basicXMLDeclaration();
      void openBasicGPXElement();

      void element(const ElementName&, const Attributes&, const LeafContent&);
      void openElement(const ElementName&, const Attributes&);
      void closeElement();
      void closeAllElements();

      std::string closeAllElementsAndExtractString();

    private:
      std::ostringstream xml;
      std::stack<std::string> unclosedTags;
      unsigned int indentationSpaces;
      unsigned int indentationLevel = 0;

      void openingTag(const ElementName&, const Attributes&);
      void closingTag(const ElementName&);

      void attributes(const Attributes&);
      void attribute(const AttributeName&, const AttributeValue&);

      void indent();
      void newline();
  };

}

#endif
