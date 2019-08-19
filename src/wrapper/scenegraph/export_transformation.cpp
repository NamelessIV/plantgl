/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
 *
 *  ----------------------------------------------------------------------------
 */



#include <plantgl/scenegraph/transformation/transformed.h>
#include <plantgl/scenegraph/transformation/mattransformed.h>
#include <plantgl/scenegraph/transformation/tapered.h>
#include <plantgl/scenegraph/transformation/axisrotated.h>
#include <plantgl/scenegraph/transformation/eulerrotated.h>
#include <plantgl/scenegraph/transformation/oriented.h>
#include <plantgl/scenegraph/transformation/translated.h>
#include <plantgl/scenegraph/transformation/scaled.h>
#include <plantgl/scenegraph/container/pointarray.h>
#include <plantgl/scenegraph/container/pointmatrix.h>

#include <plantgl/python/export_refcountptr.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>

PGL_USING_NAMESPACE
using namespace boost::python;
using namespace std;


DEF_POINTEE(Transformation)
DEF_POINTEE(Transformation2D)
DEF_POINTEE(Transformation3D)
DEF_POINTEE(Matrix3Transformation)
DEF_POINTEE(Matrix4Transformation)
DEF_POINTEE(OrthonormalBasis3D)
DEF_POINTEE(Deformation)

DEF_POINTEE(Taper)
DEF_POINTEE(Scaling)
DEF_POINTEE(Translation)
DEF_POINTEE(AxisRotation)
DEF_POINTEE(EulerRotation)
DEF_POINTEE(BaseOrientation)
DEF_POINTEE(Transform4)


void export_Transformation()
{
  class_< Transformation, TransformationPtr, boost::noncopyable >("Transformation", no_init);

  class_< Transformation2D, Transformation2DPtr, bases< Transformation >, boost::noncopyable  >
    ("Transformation2D", no_init)
    .def("transform",(Point2ArrayPtr (Transformation2D::*)( const Point2ArrayPtr&) const)&Transformation2D::transform)
    // Cannot add transform on other type. boost.python will try to cast the object into a Point2Array anyway.
    .def("transformP3List",(Point3ArrayPtr (Transformation2D::*)( const Point3ArrayPtr&) const)&Transformation2D::transform)
    .def("transformP2Matrix",(Point2MatrixPtr (Transformation2D::*)( const Point2MatrixPtr&) const)&Transformation2D::transform)
    .def("transformP3Matrix",(Point3MatrixPtr (Transformation2D::*)( const Point3MatrixPtr&) const)&Transformation2D::transform)
    // .def("transform",&tr2D )
    ;

  class_< Transformation3D, Transformation3DPtr, bases< Transformation >, boost::noncopyable  >
    ("Transformation3D", no_init)
    .def("transform",(Point3ArrayPtr (Transformation3D::*)( const Point3ArrayPtr&) const)&Transformation3D::transform)
    // Cannot add transform on other type. boost.python will try to cast the object into a Point2Array anyway.
    // .def("transform",&tr3D )
    .def("transformP4List",(Point4ArrayPtr (Transformation3D::*)( const Point4ArrayPtr&) const)&Transformation3D::transform)
    .def("transformP3Matrix",(Point3MatrixPtr (Transformation3D::*)( const Point3MatrixPtr&) const)&Transformation3D::transform)
    .def("transformP4Matrix",(Point4MatrixPtr (Transformation3D::*)( const Point4MatrixPtr&) const)&Transformation3D::transform)
    ;

  class_< Matrix3Transformation, Matrix3TransformationPtr, bases< Transformation2D >, boost::noncopyable  >
    ("Matrix3Transformation", no_init);
  class_< Matrix4Transformation, Matrix4TransformationPtr, bases< Transformation3D >, boost::noncopyable  >
    ("Matrix4Transformation", no_init)
    .def("getMatrix",&Matrix4Transformation::getMatrix);
    ;
  class_< Deformation, DeformationPtr, bases< Transformation3D >, boost::noncopyable  >
    ("Deformation", no_init);

  implicitly_convertible<Transformation2DPtr, TransformationPtr >();
  implicitly_convertible<Transformation3DPtr, TransformationPtr >();
  implicitly_convertible<Matrix3TransformationPtr, Transformation2DPtr >();
  implicitly_convertible<Matrix4TransformationPtr, Transformation3DPtr >();
  implicitly_convertible<DeformationPtr, Transformation3DPtr >();

}

void export_Taper()
{
  class_< Taper, bases< Deformation > , TaperPtr, boost::noncopyable >
    ("Taper", init< const real_t&, const real_t&>() )
    .def("getBaseRadius",&Taper::getBaseRadius, return_value_policy<copy_const_reference>())
    .def("getTopRadius",&Taper::getTopRadius);

  implicitly_convertible<TaperPtr, DeformationPtr>();
}


void export_Scaling()
{
  class_< Scaling, bases< Matrix4Transformation > , ScalingPtr, boost::noncopyable >
    ("Scaling", init< const Vector3& >() );

  implicitly_convertible<ScalingPtr, Matrix4TransformationPtr>();
}

void export_Translation()
{
  class_< Translation, bases< Matrix4Transformation > , TranslationPtr, boost::noncopyable >
    ("Translation", init< const Vector3& >() );

  implicitly_convertible<TranslationPtr, Matrix4TransformationPtr>();
}


void export_OrthonormalBasis3D()
{
  class_< OrthonormalBasis3D, bases< Matrix4Transformation > , OrthonormalBasis3DPtr, boost::noncopyable >
    ("OrthonormalBasis3D", init< const Matrix3& >() )
    .def("getMatrix3",&OrthonormalBasis3D::getMatrix3)
    .def("getMatrix",&OrthonormalBasis3D::getMatrix);

  implicitly_convertible<OrthonormalBasis3DPtr, Matrix4TransformationPtr>();
}

void export_AxisRotation()
{
  class_< AxisRotation, bases< OrthonormalBasis3D > , AxisRotationPtr, boost::noncopyable >
    ("AxisRotation", init< const Vector3&, const real_t&>() );

  implicitly_convertible<AxisRotationPtr, OrthonormalBasis3DPtr>();
}

void export_EulerRotation()
{
  class_< EulerRotation, bases< OrthonormalBasis3D > , EulerRotationPtr, boost::noncopyable >
    ("EulerRotation", init< const real_t&, const real_t&, const real_t&>() );

  implicitly_convertible<EulerRotationPtr, OrthonormalBasis3DPtr>();
}

void export_BaseOrientation()
{
  class_< BaseOrientation, bases< OrthonormalBasis3D > , BaseOrientationPtr, boost::noncopyable >
    ("BaseOrientation", init< const Vector3&, const Vector3&>() );

  implicitly_convertible<EulerRotationPtr, OrthonormalBasis3DPtr>();
}


void translate( Transform4& t, const Vector3& v ) { t.translate(v); }
void scale( Transform4& t, const Vector3& v ) { t.scale(v); }
void rotate_t4( Transform4& t, const Matrix3& v ) { t.rotate(v); }

Transform4Ptr t4_default()
{ return Transform4Ptr(new Transform4());}

std::string t4_str( Transform4* t )
{
  std::stringstream ss;
  ss << t->getMatrix();
  return ss.str();
}

std::string t4_repr( Transform4 * a )
{
  std::stringstream ss;
  Matrix4 m=a->getMatrix();
  real_t* data= m.getData();
  ss << "Transform4(Matrix4( ";
  std::copy( data, data+15, std::ostream_iterator< real_t >( ss, ", " ) );
  ss << *(data+15) << "])";
  return ss.str();
}

Point3ArrayPtr t4_transform_p3( Transform4 * t, const Point3ArrayPtr& points )
{
  return t->transform(points);
}

boost::python::tuple t4_getTransformation(Transform4 * t)
{
    Vector3 scale,rotate,translate;
    t->getTransformation(scale,rotate,translate);
    return boost::python::make_tuple(scale,rotate,translate);
}

void export_Transform4()
{
  class_< Transform4, Transform4Ptr, bases< Matrix4Transformation > , boost::noncopyable >
    m2("Transform4", init<const Matrix4&>() );
  m2.def("__init__", make_constructor(t4_default));
  m2.def("isValid", &Transform4::isValid);
  m2.def("translate", translate,"Add a translation at the transformation");
  m2.def("scale", scale,"Add a scaling at the transformation");
  m2.def("rotate", rotate_t4,"Add a rotation at the transformation");
  m2.def("getTransformation", &t4_getTransformation,"Return scale, rotate and translate corresponding values.");
  m2.def("getVolume", &Transform4::getVolume,"compute the Matrix3 determinant of the transformation");
  m2.def("transform", t4_transform_p3, "transform a set of points");
  m2.def("__str__",t4_str);
  m2.def("__repr__",t4_repr);

  implicitly_convertible<Transform4Ptr, Matrix4TransformationPtr>();

}

