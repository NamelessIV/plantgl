/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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


/*! \file geom_pointarray.h
    \brief Definition of the container class Point2Array, Point3Array and Point4Array.
*/


#ifndef __geom_pointarray_h__
#define __geom_pointarray_h__

/* ----------------------------------------------------------------------- */

#include "../sg_config.h"
#include <plantgl/tool/util_array.h>
#include <plantgl/math/util_vector.h>

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

/**
   \class Point2Array
   \brief An array of 2D points.
*/

/* ----------------------------------------------------------------------- */

class SG_API Point2Array : public TOOLS(Array1)<TOOLS(Vector2)>
{

public:

  /** Constructs a Point2Array of size of \e size.
      \post
      - \e self is valid. */
  Point2Array( uint32_t size = 0 );

  /** Constructs a Point2Array of size of \e size.
      \post
      - \e self is valid. */
  Point2Array( uint32_t size, const TOOLS(Vector2)& t);

  /** Constructs a Point2Array of size 2 with \e a and \e b.
      \post
      - \e self is valid. */
  Point2Array( TOOLS(Vector2) a, TOOLS(Vector2) b );

  /** Constructs a PointArray with the range of points [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  Point2Array( InIterator first, InIterator last ) :
    TOOLS(Array1)<TOOLS(Vector2)>(first,last) {
    GEOM_ASSERT(isValid());
  }

  /// Destructor
  virtual ~Point2Array( );

  /// Returns the mimimum and maximum points bounding \e self.
  std::pair<TOOLS(Vector2),TOOLS(Vector2)> getBounds( ) const;

  /// Returns the center of \e self.
  TOOLS(Vector2) getCenter( ) const;

  /// Returns the extent along the \c axis and \c y axis of \e self.
  TOOLS(Vector2) getExtent( ) const;

  /** Returns the total length of the segments formed by adjacent points.
      It takes sense in the case that \e self denotes a polyline. */
  real_t getLength( ) const;

  /// Returns an iterator at the point with the maximum \c x coordinate.
  const_iterator getXMax( ) const;

  /// Returns an iterator at the point with the minimum \c x coordinate.
  const_iterator getXMin( ) const;

  /** Returns a pair of iterator at first the point with the minimum and
      second the point with the maximum \c x coordinate. */
  std::pair<const_iterator,const_iterator> getXMinAndMax( ) const;

  /// Returns an iterator at the point with the maximum \c y coordinate.
  const_iterator getYMax( ) const;

  /// Returns an iterator at the point with the minimum \c y coordinate.
  const_iterator getYMin( ) const;

  /** Returns first an iterator at the point with the minimum \c y
      coordinate and second an iterator at the point with the maximum
      \c y coordinate. */
  std::pair<const_iterator,const_iterator> getYMinAndMax( ) const;

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  /// Normalize all the points.
  void normalize();

  real_t * data() const;

};

/// Point2Array Pointer
typedef RCPtr<Point2Array> Point2ArrayPtr;


/* ----------------------------------------------------------------------- */


/**
   \class Point3Array
   \brief An of 3D points.
*/


class SG_API Point3Array : public TOOLS(Array1)<TOOLS(Vector3)>
{

public:

  /** Constructs a Point3Array of size of \e size.
      \post
      - \e self is valid. */
  Point3Array( uint32_t size = 0 );

  /** Constructs a Point3Array of size of \e size.
      \post
      - \e self is valid. */
  Point3Array( uint32_t size, const TOOLS(Vector3)& t);

  /** Constructs a Point3Array of size 2 with \e a and \e b.
      \post
      - \e self is valid. */
  Point3Array( const TOOLS(Vector3)& a, const TOOLS(Vector3)& b );

  /** Constructs a Point3Array from the Point2Array \e a and with z value \e z.
      \post
      - \e self is valid. */
  Point3Array( const Point2Array& a, real_t z );

  /** Constructs a Point3Array with the range of points [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  Point3Array( InIterator first, InIterator last ) :
    TOOLS(Array1)<TOOLS(Vector3)>(first,last) {
    GEOM_ASSERT(isValid());
  }

  /// Destructor
  virtual ~Point3Array( );

  /// Returns the mimimum and maximum points bounding \e self.
  std::pair<TOOLS(Vector3),TOOLS(Vector3)> getBounds( ) const;

  /// Returns the center of \e self.
  TOOLS(Vector3) getCenter( ) const;

  /// Returns the extent along the \c axis and \c y axis of \e self.
  TOOLS(Vector3) getExtent( ) const;

 /** Returns the total length of the segments formed by adjacent points.
      It takes sense in the case that \e self denotes a polyline. */
  real_t getLength( ) const;

  /// Returns an iterator at the point with the maximum \c x coordinate.
  const_iterator getXMax( ) const;

  /// Returns an iterator at the point with the minimum \c x coordinate.
  const_iterator getXMin( ) const;

  /** Returns a pair of iterator at first the point with the minimum and
      second the point with the maximum \c x coordinate. */
  std::pair<const_iterator,const_iterator> getXMinAndMax( ) const;

  /// Returns an iterator at the point with the maximum \c y coordinate.
  const_iterator getYMax( ) const;

  /// Returns an iterator at the point with the minimum \c y coordinate.
  const_iterator getYMin( ) const;

  /** Returns first an iterator at the point with the minimum \c y
      coordinate and second an iterator at the point with the maximum
      \c y coordinate. */
  std::pair<const_iterator,const_iterator> getYMinAndMax( ) const;

  /// Returns an iterator at the point with the maximum \c z coordinate.
  const_iterator getZMax( ) const;

  /// Returns an iterator at the point with the minimum \c z coordinate.
  const_iterator getZMin( ) const;

 /** Returns first an iterator at the point with the minimum \c z
      coordinate and second an iterator at the point with the maximum
      \c z coordinate. */
  std::pair<const_iterator,const_iterator> getZMinAndMax( ) const;

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  /** Projects \e self in 2D space.
      For each points, the \c x and \c y  coordinates are divided by the
      \c z coordinate. */
  Point2ArrayPtr project( ) const;

  /// Normalize all the points.
  void normalize();

  real_t * data() const;

};

/// Point3Array Pointer
typedef RCPtr<Point3Array> Point3ArrayPtr;


/* ----------------------------------------------------------------------- */


/**
   \class Point4Array
   \brief An of 4D points.
*/


class SG_API Point4Array : public TOOLS(Array1)<TOOLS(Vector4)>
{

public:

  /** Constructs a Point4Array of size of \e size.
      \post
      - \e self is valid. */
  Point4Array( uint32_t size = 0 );

  /** Constructs a Point4Array with the range of points [\e first, \e last).
      \post
      - \e self is valid. */
  template <class InIterator>
  Point4Array( InIterator first, InIterator last ) :
    TOOLS(Array1)<TOOLS(Vector4)>(first,last) {
    GEOM_ASSERT(isValid());
  }

  /** Constructs a Point4Array with the Point2Array \e points2, \c z and \c w
      coordinates for each points is set to \e z and \e w.
      \pre
      - \e points2 must valid.
      \post
      - \e self is valid. */
  Point4Array( const Point2Array& points2, real_t z , real_t w );

  /** Constructs a Point4Array with the Point3Array \e points3, \c w
      coordinate for each points is set to \e w.
      \pre
      - \e points3 must valid.
      \post
      - \e self is valid. */
  Point4Array( const Point3Array& points3, real_t w );

  /// Destructor
  virtual ~Point4Array( );

  /// Returns the mimimum and maximum points bounding \e self.
  std::pair<TOOLS(Vector4),TOOLS(Vector4)> getBounds( ) const;

  /// Returns the center of \e self.
  TOOLS(Vector4) getCenter( ) const;

  /// Returns the extent along the \c axis and \c y axis of \e self.
  TOOLS(Vector4) getExtent( ) const;

  /** Returns the total length of the segments formed adjacent points.
      It takes sense in the case that \e self denotes a polyline. */
  real_t getLength( ) const;

  /// Returns an iterator at the point with the maximum \c x coordinate.
  const_iterator getXMax( ) const;

  /// Returns an iterator at the point with the minimum \c x coordinate.
  const_iterator getXMin( ) const;

  /** Returns a pair of iterator at first the point with the minimum and
      second the point with the maximum \c x coordinate. */
  std::pair<const_iterator,const_iterator> getXMinAndMax( ) const;

  /// Returns an iterator at the point with the maximum \c y coordinate.
  const_iterator getYMax( ) const;

  /// Returns an iterator at the point with the minimum \c y coordinate.
  const_iterator getYMin( ) const;

  /** Returns first an iterator at the point with the minimum \c y
      coordinate and second an iterator at the point with the maximum
      \c y coordinate. */
  std::pair<const_iterator,const_iterator> getYMinAndMax( ) const;

  /// Returns an iterator at the point with the maximum \c z coordinate.
  const_iterator getZMax( ) const;

  /// Returns an iterator at the point with the minimum \c z coordinate.
  const_iterator getZMin( ) const;

 /** Returns first an iterator at the point with the minimum \c z
      coordinate and second an iterator at the point with the maximum
      \c z coordinate. */
  std::pair<const_iterator,const_iterator> getZMinAndMax( ) const;

  /// Returns an iterator at the point with the maximum \c w coordinate.
  const_iterator getWMax( ) const;

  /// Returns an iterator at the point with the minimum \c w coordinate.
  const_iterator getWMin( ) const;

 /** Returns first an iterator at the point with the minimum \c w
      coordinate and second an iterator at the point with the maximum
      \c w coordinate. */
  std::pair<const_iterator,const_iterator> getWMinAndMax( ) const;

  /// Returns whether \e self is valid.
  virtual bool isValid( ) const;

  /** Projects \e self in 2D space.
      For each points, the \c x, \c y and \c z coordinates are divided by
      the \c w coordinate. */
  Point3ArrayPtr project( ) const;

  /// Normalize all the points.
  void normalize();

  real_t * data() const;

};

/// Point4Array Pointer
typedef RCPtr<Point4Array> Point4ArrayPtr;


/* ----------------------------------------------------------------------- */

template <class T>
real_t hausdorff_distance(const RCPtr<T> pts1,
						  const RCPtr<T> pts2)
{
	if(pts1.isNull() || pts2.isNull() || pts1->isEmpty() ||pts2->isEmpty())
        return 0;
	real_t dist1 = 0;
	for(typename T::const_iterator it1 = pts1->getBegin(); it1 != pts1->getEnd();++it1){
		real_t dist1a = REAL_MAX;
		for(typename T::const_iterator it2 = pts2->getBegin(); it2 != pts2->getEnd();++it2)
            dist1a = std::min(dist1a,norm((*it1)-(*it2)));
		dist1 += dist1a;
	}
	real_t dist2 = 0;
	for(typename T::const_iterator it2 = pts2->getBegin(); it2 != pts2->getEnd();++it2){
		real_t dist2a = REAL_MAX;
		for(typename T::const_iterator it1 = pts1->getBegin(); it1 != pts1->getEnd();++it1)
			dist2a = std::min(dist2a,norm((*it1)-(*it2)));
		dist2 += dist2a;
	}
	return (dist1+dist2)/(pts1->getSize()+pts2->getSize());
}

// __geom_pointarray.h__
/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
#endif

