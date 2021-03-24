#include <boost/test/unit_test.hpp>

#include "xml/element.h"
#include "xml/parser.h"

using namespace XML;

BOOST_AUTO_TEST_SUITE( XML_Parser )

BOOST_AUTO_TEST_CASE( ParseRootElement )
{
    std::string xmlData = "<a></a>";

    XML::Parser parser(xmlData);

    BOOST_CHECK_NO_THROW(parser.parseRootElement());
}

BOOST_AUTO_TEST_CASE( ElementWithClosingTag )
{
    std::string xmlData = "<a></a>";

    XML::Parser parser(xmlData);

    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.getName(), "a");
    BOOST_CHECK_EQUAL(element.getLeafContent() , "");
}

BOOST_AUTO_TEST_CASE( SelfClosingElement )
{
    std::string xmlData = "<a/>";

    XML::Parser parser(xmlData);

    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.getName(), "a");
    BOOST_CHECK_EQUAL(element.getLeafContent() , "");
}

BOOST_AUTO_TEST_CASE( SingleAttribute )
{
    std::string xmlData = "<a at=\"data\"/>";

    XML::Parser parser(xmlData);

    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.getAttribute("at"), "data");
}

BOOST_AUTO_TEST_CASE( MultipleAttributes )
{
    std::string xmlData = "<a at1=\"data1\" at2=\"data2\" />";

    XML::Parser parser(xmlData);

    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.getAttribute("at1"), "data1");
    BOOST_CHECK_EQUAL(element.getAttribute("at2"), "data2");
}

BOOST_AUTO_TEST_CASE( LeafContent )
{
    std::string xmlData = "<a>Hello</a>";

    XML::Parser parser(xmlData);

    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.getLeafContent() , "Hello");
}

BOOST_AUTO_TEST_CASE( LeafContentNotTrimming )
{
    std::string leafContent = "  Hello World   ";

    std::string xmlData = "<a>" + leafContent + "</a>";

    XML::Parser parser(xmlData);

    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.getLeafContent() , leafContent);
}

BOOST_AUTO_TEST_CASE( ContainsSubElements )
{
    std::string xmlData = "<root><a></a><b/></root>";

    XML::Parser parser(xmlData);

    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.countSubElements("a"), 1);
    BOOST_CHECK_EQUAL(element.countSubElements("b"), 1);
}

BOOST_AUTO_TEST_CASE( CountSubElements )
{
    std::string xmlData = "<root><a></a><b/><a></a><b/><b></b></root>";

    XML::Parser parser(xmlData);

    Element element = parser.parseRootElement();

    BOOST_CHECK_EQUAL(element.countSubElements("a"), 2);
    BOOST_CHECK_EQUAL(element.countSubElements("b"), 3);
}

BOOST_AUTO_TEST_CASE( SubElementNameIsName )
{
    std::string subElementName = "a";
    std::string xmlData = "<root><" + subElementName + "></" + subElementName + "></root>";

    XML::Parser parser(xmlData);

    Element element = parser.parseRootElement();

    BOOST_REQUIRE_EQUAL(element.countSubElements(subElementName),1);

    Element subElement = element.getSubElement(subElementName);

    BOOST_CHECK_EQUAL(subElement.getName(), subElementName);
}

BOOST_AUTO_TEST_CASE( SubElementLeafContent )
{
    std::string subElementName = "a";
    std::string xmlData = "<root><" + subElementName + ">Hello</" + subElementName + "></root>";

    XML::Parser parser(xmlData);

    Element parsedElement = parser.parseRootElement();

    BOOST_REQUIRE_EQUAL(parsedElement.countSubElements(subElementName),1);

    Element subElement = parsedElement.getSubElement(subElementName);

    BOOST_CHECK_EQUAL(subElement.getLeafContent() , "Hello");
}


BOOST_AUTO_TEST_SUITE_END()
