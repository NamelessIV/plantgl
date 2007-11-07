/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Plant Graphic Library
 *
 *       Copyright 1995-2003 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
 *
 *       File author(s): F. Boudon
 *
 *  ----------------------------------------------------------------------------
 *
 *                      GNU General Public Licence
 *
 *       This program is free software; you can redistribute it and/or
 *       modify it under the terms of the GNU General Public License as
 *       published by the Free Software Foundation; either version 2 of
 *       the License, or (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If not,
 *       write to the Free Software Foundation, Inc., 59
 *       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  ----------------------------------------------------------------------------
 */


#include "export_vector.h"
#include "../util/tuple_converter.h"
// #include <plantgl/math/util_math.h>

#include <string>
#include <sstream>

TOOLS_USING_NAMESPACE
using namespace boost::python;
using namespace std;

std::string v2_repr( Vector2* v ) 
{ 
  stringstream ss; 
  ss << "Vector2(" << v->x() << ',' << v->y() << ')';
  return ss.str(); 
} 

std::string pol_repr(Vector2::Polar * v){
  std::stringstream ss;
  ss << "Vector2.Polar(" << v->radius << ',' << v->theta << ')';
  return ss.str();
}


object pgl_py_normSquared(object obj){
	try { 
       return obj.attr( "__normSquared__" )();
	}
	catch( error_already_set ){ PyErr_Clear(); }
	object iter_obj = boost::python::object( handle<>( PyObject_GetIter( obj.ptr() ) ) );
	real_t val = 0;
    while( 1 )
       {
		object lobj; 
		try {  lobj = iter_obj.attr( "next" )(); 		}
		catch( error_already_set ){ PyErr_Clear(); break; }
        real_t lval = boost::python::extract<real_t>( lobj );
		val += lval * lval;
   }
   return object(val);
}

object pgl_py_norm(object obj){
	try { return obj.attr( "__norm__" )();	}
	catch( error_already_set ){ PyErr_Clear(); }
	return object(sqrt(extract<real_t>(pgl_py_normSquared(obj))));
}


object pgl_py_normL1(object obj){
	try { return obj.attr( "__normL1__" )(); }
	catch( error_already_set ){ PyErr_Clear(); }
	object iter_obj = boost::python::object( handle<>( PyObject_GetIter( obj.ptr() ) ) );
	real_t val = 0;
    while( 1 )
       {
		object lobj; 
		try {  lobj = iter_obj.attr( "next" )(); 		}
		catch( error_already_set ){ PyErr_Clear(); break; }
        real_t lval = boost::python::extract<real_t>( lobj );
		val += abs(lval);
   }
   return object(val);
}

object pgl_py_normLinf(object obj){
	try { return obj.attr( "__normLinf__" )(); }
	catch( error_already_set ){ PyErr_Clear(); }
	object iter_obj = boost::python::object( handle<>( PyObject_GetIter( obj.ptr() ) ) );
	real_t val = 0;
    while( 1 )
       {
		object lobj; 
		try {  lobj = iter_obj.attr( "next" )(); 		}
		catch( error_already_set ){ PyErr_Clear(); break; }
        real_t lval = abs(boost::python::extract<real_t>( lobj ));
		if(val < lval) val = lval;
   }
   return object(val);
}

object pgl_py_dir(object obj){
	try { return obj.attr( "__dir__" )(); }
	catch( error_already_set ){ PyErr_Clear(); }
	real_t f = extract<real_t>(pgl_py_norm(obj));
	list l;
	object iter_obj = boost::python::object( handle<>( PyObject_GetIter( obj.ptr() ) ) );
	real_t val = 0;
    while( 1 )
       {
		object lobj; 
		try {  lobj = iter_obj.attr( "next" )(); 		}
		catch( error_already_set ){ PyErr_Clear(); break; }
		lobj = lobj / f;
		l.append(lobj);
   }
   return l;
}

real_t pgl_py_dot(object p1, object p2){
	extract<Vector2> pt1(p1);
    if(pt1.check()) {
        return dot(pt1(),extract<Vector2>(p2)());
    }
	extract<Vector3> pt1b(p1);
    if(pt1b.check()){
        return dot(pt1b(),extract<Vector3>(p2)());
    }
	extract<Vector4> pt1c(p1);
    if(pt1c.check()) {
        return dot(pt1c(),extract<Vector4>(p2)());
    }
    else {
        if (extract<int>(p1.attr("__len__")()) != extract<int>(p2.attr("__len__")()))
            throw PythonExc_IndexError();
	    object iter_obj1 = boost::python::object( handle<>( PyObject_GetIter( p1.ptr() ) ) );
	    object iter_obj2 = boost::python::object( handle<>( PyObject_GetIter( p2.ptr() ) ) );
	    real_t val = 0;
        while( 1 )
         {
		    object lobj1,lobj2; 
		    try {  lobj1 = iter_obj1.attr( "next" )(); lobj2 = iter_obj2.attr( "next" )(); }
		    catch( error_already_set ){ PyErr_Clear(); break; }
            real_t lval1 = extract<real_t>( lobj1 )();
            real_t lval2 = extract<real_t>( lobj2 )();
		    val += lval1*lval2;
        }
        return val;
    }
}

struct v2_pickle_suite : boost::python::pickle_suite
{
	static tuple getinitargs(Vector2 const& v)
	{
		return make_tuple(v.x(),v.y());
	}
};


void export_Vector2()
{
  {
  scope v2 = class_< Vector2 >("Vector2", init< const Vector2 & >("Vector2(Vector2 v)",args("v")))
    .def(init< optional< real_t, real_t > >("Vector2(real_t x , real_t y)",args("x","y")))
	.def(init< const Vector2::Polar & >("Vector2(Polar p)",args("p")))
    .def(self_ns::str(self))
    .def( "__str__", v2_repr )
    .def( "__repr__", v2_repr )
    .def(vector_dim2_func<Vector2>())
    .def(vector_crossdot<Vector2>())
	.def_pickle(v2_pickle_suite());
   ;

   pgltuple_from_tuple<Vector2,2>();

  class_<Vector2::Polar>("Polar", init<const Vector2&>("Polar(Vector2 v)", args("v")))
    .def(init< real_t,real_t>("Polar(real_t radius, real_t theta)",args("radius","theta")))
    .def("isValid",&Vector2::Polar::isValid, "Returns whether self is valid.")
    .def("__str__",&pol_repr)
    .def("__repr__",&pol_repr)
    .add_property("radius",make_getter(&Vector2::Polar::radius),make_setter(&Vector2::Polar::radius))
    .add_property("theta",make_getter(&Vector2::Polar::theta),make_setter(&Vector2::Polar::theta))
    ;
  }

  def("angle",(real_t (*)   ( const Vector2&, const Vector2&)) &angle , args("v1","v2"), "The angle between v1 and v2");
  def("cross",(real_t (*) ( const Vector2&, const Vector2&))   &cross , args("v1","v2"), "The cross product of v1 and v2");

  def("dot",  pgl_py_dot , args("v1","v2"), "The dot product of v1 and v2" );

  def("norm",     pgl_py_norm , args("v") , "The norm of the vector. If v.__norm__() exists, call it." );
  def("normL1",     pgl_py_normL1 , args("v") , "The L1 (Manhattan) norm of the vector. If v.__normL1__() exists, call it."  );
  def("normLinf",     pgl_py_normLinf , args("v") , "The L-infinite norm of the vector. If v.__normLinf__() exists, call it."  );
  def("normSquared",     pgl_py_normSquared , args("v") , "The square of the norm of the vector. If v.__normSquared__() exists, call it." );
  def("direction",  pgl_py_dir , args("v") , "The direction of the vector. Resulting vector is normed. If v.__dir__() exists, call it."  );

}

