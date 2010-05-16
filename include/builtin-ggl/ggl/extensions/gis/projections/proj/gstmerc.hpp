#ifndef GGL_PROJECTIONS_GSTMERC_HPP
#define GGL_PROJECTIONS_GSTMERC_HPP

// Generic Geometry Library - projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright Barend Gehrels (1995-2009), Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande (2008-2009)
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Geometry Library by Barend Gehrels (Geodan, Amsterdam)

// Original copyright notice:
 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/math/special_functions/hypot.hpp>

#include <ggl/extensions/gis/projections/impl/base_static.hpp>
#include <ggl/extensions/gis/projections/impl/base_dynamic.hpp>
#include <ggl/extensions/gis/projections/impl/projects.hpp>
#include <ggl/extensions/gis/projections/impl/factory_entry.hpp>
#include <ggl/extensions/gis/projections/impl/pj_tsfn.hpp>
#include <ggl/extensions/gis/projections/impl/pj_phi2.hpp>

namespace ggl { namespace projection
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace gstmerc{ 

            struct par_gstmerc
            {
                double lamc;
                double phic;
                double c;
                double n1;
                double n2;
                double XS;
                double YS;
            };
            

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_gstmerc_spheroid : public base_t_fi<base_gstmerc_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_gstmerc m_proj_parm;

                inline base_gstmerc_spheroid(const Parameters& par)
                    : base_t_fi<base_gstmerc_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double L, Ls, sinLs1, Ls1;
                    L= this->m_proj_parm.n1*lp_lon;
                    Ls= this->m_proj_parm.c+this->m_proj_parm.n1*log(pj_tsfn(-1.0*lp_lat,-1.0*sin(lp_lat),this->m_par.e));
                    sinLs1= sin(L)/cosh(Ls);
                    Ls1= log(pj_tsfn(-1.0*asin(sinLs1),0.0,0.0));
                    xy_x= (this->m_proj_parm.XS + this->m_proj_parm.n2*Ls1)*this->m_par.ra;
                    xy_y= (this->m_proj_parm.YS + this->m_proj_parm.n2*atan(sinh(Ls)/cos(L)))*this->m_par.ra;
                    /*fprintf(stderr,"fwd:\nL      =%16.13f\nLs     =%16.13f\nLs1    =%16.13f\nLP(%16.13f,%16.13f)=XY(%16.4f,%16.4f)\n",L,Ls,Ls1,lp_lon+this->m_par.lam0,lp_lat,(xy_x*this->m_par.a + this->m_par.x0)*this->m_par.to_meter,(xy_y*this->m_par.a + this->m_par.y0)*this->m_par.to_meter);*/
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double L, LC, sinC;
                    L= atan(sinh((xy_x*this->m_par.a - this->m_proj_parm.XS)/this->m_proj_parm.n2)/cos((xy_y*this->m_par.a - this->m_proj_parm.YS)/this->m_proj_parm.n2));
                    sinC= sin((xy_y*this->m_par.a - this->m_proj_parm.YS)/this->m_proj_parm.n2)/cosh((xy_x*this->m_par.a - this->m_proj_parm.XS)/this->m_proj_parm.n2);
                    LC= log(pj_tsfn(-1.0*asin(sinC),0.0,0.0));
                    lp_lon= L/this->m_proj_parm.n1;
                    lp_lat= -1.0*pj_phi2(exp((LC-this->m_proj_parm.c)/this->m_proj_parm.n1),this->m_par.e);
                    /*fprintf(stderr,"inv:\nL      =%16.13f\nsinC   =%16.13f\nLC     =%16.13f\nXY(%16.4f,%16.4f)=LP(%16.13f,%16.13f)\n",L,sinC,LC,((xy_x/this->m_par.ra)+this->m_par.x0)/this->m_par.to_meter,((xy_y/this->m_par.ra)+this->m_par.y0)/this->m_par.to_meter,lp_lon+this->m_par.lam0,lp_lat);*/
                }
            };

            // Gauss-Schreiber Transverse Mercator (aka Gauss-Laborde Reunion)
            template <typename Parameters>
            void setup_gstmerc(Parameters& par, par_gstmerc& proj_parm)
            {
                proj_parm.lamc= par.lam0;
                proj_parm.n1= sqrt(1.0+par.es*pow(cos(par.phi0),4.0)/(1.0-par.es));
                proj_parm.phic= asin(sin(par.phi0)/proj_parm.n1);
                proj_parm.c=       log(pj_tsfn(-1.0*proj_parm.phic,0.0,0.0))
                     -proj_parm.n1*log(pj_tsfn(-1.0*par.phi0,-1.0*sin(par.phi0),par.e));
                proj_parm.n2= par.k0*par.a*sqrt(1.0-par.es)/(1.0-par.es*sin(par.phi0)*sin(par.phi0));
                proj_parm.XS= 0;
            /* -par.x0 */
                proj_parm.YS= -1.0*proj_parm.n2*proj_parm.phic;
            /* -par.y0 */
                // par.inv= s_inverse;
                // par.fwd= s_forward;
                /*fprintf(stderr,"a  (m) =%16.4f\ne      =%16.13f\nl0(rad)=%16.13f\np0(rad)=%16.13f\nk0     =%16.4f\nX0  (m)=%16.4f\nY0  (m)=%16.4f\n\nlC(rad)=%16.13f\npC(rad)=%16.13f\nc      =%16.13f\nn1     =%16.13f\nn2 (m) =%16.4f\nXS (m) =%16.4f\nYS (m) =%16.4f\n", par.a, par.e, par.lam0, par.phi0, par.k0, par.x0, par.y0, proj_parm.lamc, proj_parm.phic, proj_parm.c, proj_parm.n1, proj_parm.n2, proj_parm.XS +par.x0, proj_parm.YS + par.y0);
            */
            }

        }} // namespace detail::gstmerc
    #endif // doxygen 

    /*!
        \brief Gauss-Schreiber Transverse Mercator (aka Gauss-Laborde Reunion) projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Cylindrical
         - Spheroid
         - Ellipsoid
         - lat_0= lon_0= k_0=
        \par Example
        \image html ex_gstmerc.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct gstmerc_spheroid : public detail::gstmerc::base_gstmerc_spheroid<Geographic, Cartesian, Parameters>
    {
        inline gstmerc_spheroid(const Parameters& par) : detail::gstmerc::base_gstmerc_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::gstmerc::setup_gstmerc(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class gstmerc_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<gstmerc_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void gstmerc_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("gstmerc", new gstmerc_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail 
    #endif // doxygen

}} // namespace ggl::projection

#endif // GGL_PROJECTIONS_GSTMERC_HPP

