#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/filesystem.hpp>

#include <boost/geometry/index/rtree.hpp>
#include <boost/foreach.hpp>

#include <boost/bimap.hpp>
#include <boost/circular_buffer.hpp>

#include <cmath>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <list>
#include <memory>

namespace boost_test {

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<int, 2, bg::cs::cartesian> POINT_TYPE;
// typedef bg::model::point<double, 2, bg::cs::cartesian> point;
// typedef bg::model::box<point> box;
typedef bg::model::polygon<POINT_TYPE, false, false> POLYGON_TYPE; // ccw, open polygon

static void make_polygon_point_list(std::vector<std::list<std::pair<int, int>>> &pplist) {
    // Polygon 1
    std::vector<int> p1_ptx{2, 6, 6, 10, 10, 15, 15, 2};
    std::vector<int> p1_pty{5, 5, 4,  4,  5,  5,  6, 6};
    // Polygon 2
    std::vector<int> p2_ptx{2, 3, 3, 5,  5,  3, 3, 2};
    std::vector<int> p2_pty{8, 8, 7, 7, 10, 10, 9, 9};
    // Polygon 3
    std::vector<int> p3_ptx{7, 9, 9, 7};
    std::vector<int> p3_pty{8, 8, 9, 9};
    // Polygon 4
    std::vector<int> p4_ptx{8, 10, 10, 8};
    std::vector<int> p4_pty{7,  7,  9, 9};
    // Polygon 5
    std::vector<int> p5_ptx{11, 13, 13, 11};
    std::vector<int> p5_pty{ 7,  7,  9,  9};
    // Polygon 6
    std::vector<int> p6_ptx{ 2,  5,  5,  4,  4,  2};
    std::vector<int> p6_pty{11, 11, 13, 13, 15, 15};
    // Polygon 7
    std::vector<int> p7_ptx{ 6,  7,  7,  6};
    std::vector<int> p7_pty{10, 10, 11, 11};
    // Polygon 8
    std::vector<int> p8_ptx{ 7,  9,  9,  7};
    std::vector<int> p8_pty{12, 12, 14, 14};
    // Polygon 9
    std::vector<int> p9_ptx{10, 13, 13, 11, 11, 10};
    std::vector<int> p9_pty{10, 10, 11, 11, 14, 14};
    // Polygon 10
    std::vector<int> p10_ptx{10, 14, 14, 15, 15, 10};
    std::vector<int> p10_pty{15, 15, 10, 10, 16, 16};
    // Polygon 11
    std::vector<int> p11_ptx{4, 6,  6,  4};
    std::vector<int> p11_pty{9, 9, 12, 12};
    // Polygon 12
    std::vector<int> p12_ptx{12, 14, 14, 12};
    std::vector<int> p12_pty{ 3,  3,  4,  4};
    // Polygon 13
    std::vector<int> p13_ptx{0, 0, 0, 5, 5, 3, 3, 5, 5, 3, 3, 5, 5};
    std::vector<int> p13_pty{6, 4, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6};

    /*
    std::vector<std::vector<int>> vtest{{1, 2}, {1, 2}, {1, 2, 3}};
    std::vector<std::vector<std::pair<int, int>>> vptest{{{1, 2}, {1, 2}, {2, 3}}, {{1, 2}, {4, 5}, {6, 7}, {8, 9}}};
    */
    std::vector<std::vector<int>*> ptrXlist{ &p1_ptx, &p2_ptx, &p3_ptx, &p4_ptx, &p5_ptx, &p6_ptx,
                                             &p7_ptx, &p8_ptx, &p9_ptx, &p10_ptx, &p11_ptx, &p12_ptx, &p13_ptx };
    std::vector<std::vector<int>*> ptrYlist{ &p1_pty, &p2_pty, &p3_pty, &p4_pty, &p5_pty, &p6_pty,
                                             &p7_pty, &p8_pty, &p9_pty, &p10_pty, &p11_pty, &p12_pty, &p13_pty };

    std::list<std::pair<int, int>> vertlist;

    const size_t POLYNUM = ptrXlist.size();
    for (size_t i = 0; i < POLYNUM; i++) {
        const size_t VERTEXNUM = ptrXlist[i]->size();
        for (size_t j = 0; j < VERTEXNUM; j++) {
            vertlist.push_back(std::make_pair(ptrXlist[i]->at(j), ptrYlist[i]->at(j)));
        }
        pplist.push_back(std::move(vertlist));
        vertlist.clear();
    }
}

void rtree_query_test() {
    // typedef bg::model::point<float, 2, bg::cs::cartesian> point;
    typedef bg::model::point<int, 2, bg::cs::cartesian> point;
    typedef bg::model::box<point> box;
    typedef bg::model::polygon<point, false, false> polygon; // ccw, open polygon
    typedef std::pair<box, unsigned> value;

    // polygons
    std::vector<polygon> polygons;

    // typedef std::unordered_map<int, std::list<std::pair<int, int>>> UMAP;
    // Polygon point list
    typedef std::vector<std::list<std::pair<int, int>>> PPLIST;
    PPLIST pplist;
    make_polygon_point_list(pplist);

    for (PPLIST::iterator itr = pplist.begin(); itr != pplist.end(); itr++) {
        polygon p;
        for (auto j = itr->begin(); j != itr->end(); j++) {
            int x = j->first;
            int y = j->second;
            p.outer().push_back(point(x, y));
        }
        polygons.push_back(p);
    }

    std::cout << "Generated polygons:" << std::endl;
    BOOST_FOREACH(polygon const &p, polygons) {
        std::cout << bg::wkt<polygon>(p) << std::endl;
    }
    std::cout << "------------" << std::endl;

    bgi::rtree<value, bgi::quadratic<16> > rtree;
    for (size_t i = 0; i < polygons.size(); i++) {
        box b = bg::return_envelope<box>(polygons[i]);
        rtree.insert(std::make_pair(b, i));
    }

    // box query_box(point(4,4), point(12,12));
    box query_box(point(1,2), point(2,3));

    // std::vector<value> result_covered_by;
    std::list<value> result_covered_by;
    rtree.query(bgi::covered_by(query_box), std::back_inserter(result_covered_by));

    std::vector<value> result_overlaps;
    rtree.query(bgi::overlaps(query_box), std::back_inserter(result_overlaps));

    std::vector<value> result_within;
    rtree.query(bgi::within(query_box), std::back_inserter(result_within));

    std::vector<value> result_intersects;
    rtree.query(bgi::intersects(query_box), std::back_inserter(result_intersects));

    std::cout << "covered_by polygons:" << std::endl;
    BOOST_FOREACH(value const &v, result_covered_by) {
        std::cout << v.second + 1 << "  " << bg::wkt<polygon>(polygons[v.second]) << std::endl;
    }
    std::cout << "------------" << std::endl;

    std::cout << "overlaps polygons:" << std::endl;
    BOOST_FOREACH(value const &v, result_overlaps) {
        std::cout << v.second + 1 << "  " << bg::wkt<polygon>(polygons[v.second]) << std::endl;
    }
    std::cout << "------------" << std::endl;

    std::cout << "within polygons:" << std::endl;
    BOOST_FOREACH(value const &v, result_within) {
        std::cout << v.second + 1 << "  " << bg::wkt<polygon>(polygons[v.second]) << std::endl;
    }
    std::cout << "------------" << std::endl;

    std::cout << "intersects polygons:" << std::endl;
    BOOST_FOREACH(value const &v, result_intersects) {
        std::cout << v.second + 1 << "  " << bg::wkt<polygon>(polygons[v.second]) << std::endl;
    }
    std::cout << "------------" << std::endl;

}

void boost_polygon_test() {

    typedef bg::model::point<int, 2, bg::cs::cartesian> point;
    // typedef bg::model::point<double, 2, bg::cs::cartesian> point;
    // typedef bg::model::box<point> box;
    typedef bg::model::polygon<point, false, false> polygon; // ccw, open polygon

    // typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<int> > polygon;
    polygon blue, green;
    // boost::geometry::read_wkt("POLYGON((1 5, 1 4, 1 3, 1 2, 1 1, 2 1, 3 1, 4 1, 5 1, 5 2, 5 3, 5 4, 5 5, 4 5, 3 5, 2 5, 1 5))", blue);
    // boost::geometry::read_wkt("POLYGON((4 8, 4 7, 4 6, 4 5, 4 4, 5 4, 6 4, 7 4, 8 4, 8 5, 8 6, 8 7, 8 8, 7 8, 6 8, 5 8, 4 8))", green);

    // boost::geometry::read_wkt("POLYGON((1 5, 1 4, 1 3, 1 2, 1 1, 2 1, 3 1, 4 1, 5 1, 5 2, 5 3, 5 4, 5 4, 5 5, 4 5, 4 5, 3 5, 2 5, 1 5))", blue);
    // boost::geometry::read_wkt("POLYGON((4 8, 4 7, 4 6, 4 5, 4 4, 5 4, 6 4, 7 4, 8 4, 8 5, 8 6, 8 7, 8 8, 7 8, 6 8, 5 8, 4 8))", green);

    boost::geometry::read_wkt("POLYGON((3 5, 2 4, 1 4, 1 3, 1 2, 1 1, 2 1, 3 1, 4 1, 5 1, 5 2, 5 3, 4 4, 3 5))", blue);
    boost::geometry::read_wkt("POLYGON((5 5, 4 4, 3 3, 3 2, 4 1, 5 2, 6 3, 6 4, 6 5, 5 5))", green);
    // boost::geometry::read_wkt("POLYGON((5 5, 5 4, 6 4, 6 5))", green);

    // boost::geometry::read_wkt("POLYGON((3 5, 2 4, 1 4, 1 3, 1 2, 1 1, 2 1, 3 1, 4 1, 5 1, 5 2, 5 3, 4 4, 3 5))", blue);
    // boost::geometry::read_wkt("POLYGON((5 5, 4 5, 3 4, 3 3, 3 2, 4 1, 5 2, 6 3, 6 4, 6 5, 5 5))", green);

    std::list<polygon> out;
    boost::geometry::intersection(blue, green, out);
    int i = 0;
    std::cout << "blue && green: " << std::endl;
    BOOST_FOREACH(polygon const &p, out) {
        std::cout << i++ << ": " << bg::wkt<polygon>(p) << std::endl;
    }

    // std::list<polygon> blue_green_diff;
    std::list<polygon> blue_green_diff(out.begin(), out.end());
    boost::geometry::difference(blue, green, blue_green_diff);
    i = 0;
    std::cout << "blue - green: " << std::endl;
    BOOST_FOREACH(polygon const &p, blue_green_diff) {
        std::cout << i++ << ": " << bg::wkt<polygon>(p) << std::endl;
    }


    polygon yellow, red;
    boost::geometry::read_wkt("POLYGON((1 1, 2 1, 2 2, 1 2))", yellow);
    boost::geometry::read_wkt("POLYGON((0 0, 5 0, 5 5, 0 5))", red);

    std::list<polygon> yellow_red_intersection;
    bool yr_intersect = boost::geometry::intersects(yellow, red);
    printf("yellow intersects with red: %s\n", yr_intersect ? "YES" : "NO");
    boost::geometry::intersection(yellow, red, yellow_red_intersection);

    i = 0;
    std::cout << "yellow && red: " << std::endl;
    BOOST_FOREACH(polygon const &p, yellow_red_intersection) {
        std::cout << i++ << ": " << bg::wkt<polygon>(p) << std::endl;
    }

    std::list<polygon> yellow_red_diff;
    boost::geometry::difference(yellow, red, yellow_red_diff);

    i = 0;
    std::cout << "yellow - red: " << yellow_red_diff.size() << std::endl;
    BOOST_FOREACH(polygon const &p, yellow_red_diff) {
        std::cout << i++ << ": " << bg::wkt<polygon>(p) << std::endl;
    }

    i = 0;
    std::cout << "yellow - red (outer): " << std::endl;
    for (const auto &poly : yellow_red_diff) {
        printf("outer %d size: %llu\n", i, poly.outer().size());
    }

    printf("yellow - red inner size: %llu\n", yellow_red_diff.size());
    i = 0;
    for (const auto &poly: yellow_red_diff) {
        ++i;
        int j = 0;
        for (const auto &p : poly.inners()) {
            printf("%d inner %d size: %llu\n", i, ++j, p.size());
        }
    }

    std::list<polygon> red_yellow_diff;
    boost::geometry::difference(red, yellow, red_yellow_diff);

    i = 0;
    std::cout << "red - yellow: " << red_yellow_diff.size() << std::endl;
    BOOST_FOREACH(polygon const &p, red_yellow_diff) {
        std::cout << i++ << ": " << bg::wkt<polygon>(p) << std::endl;
    }

    i = 0;
    std::cout << "red - yellow (outer): " << std::endl;
    for (const auto &poly : red_yellow_diff) {
        printf("outer %d size: %llu\n", i, poly.outer().size());
    }

    printf("red - yellow inner size: %llu\n", red_yellow_diff.size());
    i = 0;
    for (const auto &poly: red_yellow_diff) {
        ++i;
        int j = 0;
        for (const auto &p : poly.inners()) {
            printf("%d inner %d size: %llu\n", i, ++j, p.size());
        }
    }



}

void boost_polygon_access_test() {
    std::vector<int> x1{1, 1, 1, 1, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1};
    std::vector<int> y1{5, 4, 3, 2, 1, 1, 1, 1, 1, 2, 3, 4, 5, 5, 5, 5, 5};

    std::vector<int> x2{4, 4, 4, 4, 4, 5, 6, 7, 8, 8, 8, 8, 8, 7, 6, 5, 4};
    std::vector<int> y2{8, 7, 6, 5, 4, 4, 4, 4, 4, 5, 6, 7, 8, 8, 8, 8, 8};

    typedef bg::model::point<int, 2, bg::cs::cartesian> point_type;
    typedef bg::model::polygon<point_type> polygon_type;
    typedef bg::model::polygon<point_type, false, true> polygon_type2;

    polygon_type polygon_blue;
    polygon_type polygon_green;

    for (size_t i = 0; i < x1.size(); ++i) {
        bg::append(polygon_blue.outer(), point_type(x1[i], y1[i]));
        bg::append(polygon_green.outer(), point_type(x2[i], y2[i]));
    }

    printf("Polygon Blue:\n");
    for (const auto &pnt : polygon_blue.outer()) {
        printf("%d, %d\n", bg::get<0>(pnt), bg::get<1>(pnt));
    }

    printf("Polygon Green:\n");
    for (const auto &pnt : polygon_green.outer()) {
        printf("%d, %d\n", bg::get<0>(pnt), bg::get<1>(pnt));
    }

    polygon_type2 polygon_red;
    polygon_type2 polygon_yellow;

    for (size_t i = 0; i < x1.size(); ++i) {
        bg::append(polygon_red.outer(), point_type(x1[i], y1[i]));
        bg::append(polygon_yellow.outer(), point_type(x2[i], y2[i]));
    }

    printf("Polygon Red:\n");
    for (const auto &pnt : polygon_red.outer()) {
        printf("%d, %d\n", bg::get<0>(pnt), bg::get<1>(pnt));
    }

    printf("Polygon Yellow:\n");
    for (const auto &pnt : polygon_yellow.outer()) {
        printf("%d, %d\n", bg::get<0>(pnt), bg::get<1>(pnt));
    }
}

std::shared_ptr<POLYGON_TYPE> boost_polygon_creation() {

    // std::vector<int> x1{1, 1, 1, 1, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1};
    // std::vector<int> y1{5, 4, 3, 2, 1, 1, 1, 1, 1, 2, 3, 4, 5, 5, 5, 5, 5};

    return NULL;
}

void boost_polygon_creation_test() {
    std::shared_ptr<POLYGON_TYPE> p = boost_polygon_creation();
    if (p) {
        printf("p is NOT null after boost_polygon_creation\n");
    } else {
        printf("p is null after boost_polygon_creation\n");
    }
}

void test_boost_all() {
    std::cout << "Start of function: " << __FUNCTION__ << "\n";

    // boost::bimap
    {
        printf("\n");
        typedef boost::bimap<std::string, int> bimap;
        bimap animals;

        animals.insert({"cat", 4});
        animals.insert({"shark", 0});
        animals.insert({"spider", 8});

        std::cout << animals.left.count("cat") << '\n';
        std::cout << animals.right.count(8) << '\n';
    }

    // boost::circular_buffer
    {
        printf("\n");
        typedef boost::circular_buffer<int> circular_buffer;
        circular_buffer cb{3};

        cb.push_back(0);
        cb.push_back(1);
        cb.push_back(2);
        cb.push_back(3);

        std::cout << std::boolalpha << cb.is_linearized() << '\n';

        circular_buffer::array_range ar1, ar2;

        ar1 = cb.array_one();
        ar2 = cb.array_two();
        std::cout << ar1.second << ";" << ar2.second << '\n';

        for (int i : cb)
            std::cout << i << '\n';

        cb.linearize();

        ar1 = cb.array_one();
        ar2 = cb.array_two();
        std::cout << ar1.second << ";" << ar2.second << '\n';
    }

    std::cout << "End of function: " << __FUNCTION__ << "\n";

} /* func tester_boost_all */


void boost_normal_test_all() {
    std::cout << "---------- START OF FUNCTION: " << __FUNCTION__ << std::endl;

    // R-Tree test
    {
#if 1
        rtree_query_test();
#endif
    }

    // File system test
    // [Note]
    // Add "-llibboost_filesystem-mgw63-1_65_1 -llibboost_system-mgw63-1_65_1" to the compile options
    // At the same time, add BOOST_LIB_PATH to system path: path = %BOOST_LIB_PATH%;path
    // Here BOOST_LIB_PATH is C:\synopsys_longc\boost_1_65_1\stage\lib
    {
#if 0
        namespace BF = boost::filesystem;
        std::string pathstr{"LONGC_TEST\\template14"};
        BF::path tdir{pathstr};
        if (BF::is_directory(tdir)) {
            printf("%s already exists\n", pathstr.c_str());
        } else {
            if (BF::create_directories(tdir)) {
                printf("Dir %s is created successfully\n", pathstr.c_str());
            } else {
                printf("Dir %s is NOT created\n", pathstr.c_str());
            }
        }
#endif
    }

    boost_polygon_test();

    boost_polygon_access_test();

    boost_polygon_creation_test();

    std::cout << "---------- END OF FUNCTION: " << __FUNCTION__ << std::endl;
}

} // namespace boost_test
