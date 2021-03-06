// Generic Geometry Library
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef GGL_UTIL_FOR_EACH_COORDINATE_HPP
#define GGL_UTIL_FOR_EACH_COORDINATE_HPP

#include <boost/concept/requires.hpp>
#include <ggl/core/concepts/point_concept.hpp>

namespace ggl
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail
{

template <typename Point, int Dimension, int DimensionCount>
struct coordinates_scanner
{
    template <typename Op>
    static inline void apply(Point& point, Op operation)
    {
        operation.template apply<Point, Dimension>(point);
        coordinates_scanner
            <
                Point,
                Dimension+1,
                DimensionCount
            >::apply(point, operation);
    }
};

template <typename Point, int DimensionCount>
struct coordinates_scanner<Point, DimensionCount, DimensionCount>
{
    template <typename Op>
    static inline void apply(Point&, Op)
    {}
};

} // namespace detail
#endif // DOXYGEN_NO_DETAIL

template <typename Point, typename Op>
inline void for_each_coordinate(Point& point, Op operation)
{
    BOOST_CONCEPT_ASSERT( (concept::Point<Point>) );

    typedef typename detail::coordinates_scanner
        <
            Point, 0, dimension<Point>::type::value
        > scanner;

    scanner::apply(point, operation);
}

} // namespace ggl

#endif // GGL_UTIL_FOR_EACH_COORDINATE_HPP
