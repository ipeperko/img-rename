#define BOOST_TEST_MODULE test_module
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test;

#include "Formatter.h"
#include "Log.h"
#include <ImageMagick-7/Magick++.h>

BOOST_AUTO_TEST_CASE(format_test)
{
    FileItem fitem;
    fitem.image().attribute("EXIF:DateTimeOriginal", "2020:07:01 07:10:20");
    fitem.image().attribute("EXIF:Make", "Apple");
    fitem.image().attribute("EXIF:Model", "iPhone 7");
    fitem.parseAttributes();

    Formatter fm;
    fm.setFormat("prefix-%D-%T-%M_%m-suffix");

    fm.setDestDirectory("");
    BOOST_TEST(fm.rename(fitem) == "prefix-20200701-071020-Apple_iPhone_7-suffix.jpg");

    fm.setDestDirectory(".");
    BOOST_TEST(fm.rename(fitem) == "./prefix-20200701-071020-Apple_iPhone_7-suffix.jpg");

    fm.setDestDirectory("./");
    BOOST_TEST(fm.rename(fitem) == "./prefix-20200701-071020-Apple_iPhone_7-suffix.jpg");

    fm.setDestDirectory("/home/user");
    BOOST_TEST(fm.rename(fitem) == "/home/user/prefix-20200701-071020-Apple_iPhone_7-suffix.jpg");

    fm.setDestDirectory("/home/user/");
    BOOST_TEST(fm.rename(fitem) == "/home/user/prefix-20200701-071020-Apple_iPhone_7-suffix.jpg");
}

BOOST_AUTO_TEST_CASE(invalid_format_test)
{
    Formatter fm;
    BOOST_REQUIRE_THROW(fm.setFormat("%D-%T-%M_%"), std::overflow_error);
    BOOST_REQUIRE_THROW(fm.setFormat("% D-%T-%M"), std::domain_error);
}