/* Copyright (c) 2013 Simon Tanaka <tanakas"at"gmx"dot"ch>

   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
   NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include <UtilLib/config.hpp>
#define private public
#define protected public
#include <UtilLib/include/geometry/QuadTree.hpp>
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


class SimpleNode {
 public:
    SimpleNode(
            double x,
            double y,
            int id
            ) : XPos_(x),
                YPos_(y),
                id_(id)
    {
    }

    void setPos(double x,
                double y)
    {
        this->XPos_ = x;
        this->YPos_ = y;
    }

    double getXPos()
    {
        return this->XPos_;
    }

    double getYPos()
    {
        return this->YPos_;
    }

private:
    double XPos_;
    double YPos_;
    const unsigned int id_;
};


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
        char* argv[]) {

    std::map<unsigned int,
            std::shared_ptr<SimpleNode> > geometryNodes;


    double x = 1.1;
    double y = 2.2;


    SimpleNode *p1 = new SimpleNode(x,y,3);
    geometryNodes[3] = std::make_shared<SimpleNode>(*p1);
//    geometryNodes[3] = std::make_shared<SimpleNode>(
//            x,
//            y,
//            3);

    SimpleNode *p2 = new SimpleNode(x,y,7);
    geometryNodes[7] = std::make_shared<SimpleNode>(*p2);
//    geometryNodes[7] = std::make_shared<SimpleNode>(
//            x,
//            y,
//            7);

    std::cout<<geometryNodes.size()<<std::endl;



    UtilLib::geometry::Rectangle *myRect = new UtilLib::geometry::Rectangle(5,-5,-5,5);
    UtilLib::geometry::QuadTree<SimpleNode>* myTree = new  UtilLib::geometry::QuadTree<SimpleNode>(*myRect,1);



    myTree->put(geometryNodes[3]);
    myTree->put(geometryNodes[7]);


    std::vector<std::shared_ptr<SimpleNode> > RE = myTree->get(*myRect);


    std::cout<<"number of points in the rectangle: "<<RE.size()<<std::endl;

    for (auto i: RE)
    {
        std::cout<<"("<<i->getXPos()<<","<<i->getYPos()<<")"<<std::endl;
    }

    delete myRect;
    delete myTree;
    myRect = nullptr;

    test_Constructor();
    test_withhin();
    test_pointWithinBounds();

    return 0;
}
