#include <UtilLib/config.hpp>
#define private public
#define protected public
#include <UtilLib/include/geometry/Rectangle.hpp>
#undef protected
#undef private
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <boost/test/minimal.hpp>
using namespace boost::unit_test;
using namespace UtilLib;


void test_Constructor() {
    geometry::Rectangle rect(1.0, 2.0, 3.0, 4.0);
    BOOST_CHECK(rect.north == 1.0);
    BOOST_CHECK(rect.south == 2.0);
    BOOST_CHECK(rect.west == 3.0);
    BOOST_CHECK(rect.east == 4.0);

    geometry::Rectangle rect2(4.0, 4.0, 4.0);
    BOOST_CHECK(rect2.north == 6.0);
    BOOST_CHECK(rect2.south == 2.0);
    BOOST_CHECK(rect2.west == 2.0);
    BOOST_CHECK(rect2.east == 6.0);

    geometry::Rectangle rect3(rect);
    BOOST_CHECK(rect3.north == 1.0);
    BOOST_CHECK(rect3.south == 2.0);
    BOOST_CHECK(rect3.west == 3.0);
    BOOST_CHECK(rect3.east == 4.0);
}

void test_withhin() {
    geometry::Rectangle rect2(4.0, 4.0, 4.0);
    geometry::Rectangle rect3(4.0, 4.0, 2.0);
    geometry::Rectangle rect4(4.0, 4.0, 6.0);

    BOOST_CHECK(rect2.within(rect3) == true);
    BOOST_CHECK(rect2.within(rect4) == false);

    geometry::Rectangle rect(4.0, 2.0, 2.0, 4.0);
    BOOST_CHECK(rect.within(3.5, 2.5, 2.5, 5.0) == false);
    BOOST_CHECK(rect.within(4.5, 2.5, 2.5, 3.5) == false);
    BOOST_CHECK(rect.within(3.5, 1.5, 2.5, 3.0) == false);
    BOOST_CHECK(rect.within(3.5, 2.5, 1.5, 3.0) == false);
    BOOST_CHECK(rect.within(3.5, 2.5, 2.5, 3.0) == true);
}

void test_pointWithinBounds() {
    geometry::Rectangle rect(4.0, 2.0, 2.0, 4.0);
    BOOST_CHECK(rect.pointWithinBounds(3.0, 3.0) == true);
    BOOST_CHECK(rect.pointWithinBounds(5.0, 3.0) == false);
}

int test_main(
        int argc,
        char* argv[]) { // note the name!
    test_Constructor();
    test_withhin();
    test_pointWithinBounds();

    return 0;
}
