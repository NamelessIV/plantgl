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





#include "geom_group.h"
#include "util_messages.h"
#include "Tools/util_string.h"
#include "geom_geometryarray2.h"
#include "geom_polyline.h"
GEOM_USING_NAMESPACE
TOOLS_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const GeomPolylinePtr Group::DEFAULT_SKELETON;


/* ----------------------------------------------------------------------- */


Group::Builder::Builder() :
  Geometry::Builder(),
  GeometryList(0),
  Skeleton(0) {
}


Group::Builder::~Builder( ) {
  // nothing to do
}


SceneObjectPtr Group::Builder::build( ) const {
  if (isValid())
    return SceneObjectPtr(new Group(*GeometryList,
				 Skeleton ? *Skeleton : DEFAULT_SKELETON));
  return SceneObjectPtr();
}


void Group::Builder::destroy() {
  if (GeometryList) delete GeometryList;
  if (Skeleton) delete Skeleton;
}


bool Group::Builder::isValid( ) const {

  // GeometryList field
  if (! GeometryList) {
    genMessage(WARNINGMSG(UNINITIALIZED_FIELD_ss),"Group","GeometryList");
    return false;
  };
  uint32_t _size = (*GeometryList)->getSize();
  if (_size < 1) {
    genMessage(WARNINGMSG(INVALID_FIELD_SIZE_sss),"Group","GeometryList","Must be greater than 1.");
    return false;
  };
  for (uint32_t _i = 0; _i < _size; _i++) {
    const GeometryPtr& _geometry = (*GeometryList)->getAt(_i);
    if (! (_geometry) ) {
      genMessage
	  (WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Group","GeometryList",number(_i + 1).c_str(),"Must not be a null Geometry Object.");
      return false;
    };	
    if (!_geometry->isValid() ) {
      genMessage
	  (WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Group","GeometryList",number(_i + 1).c_str(),"Must be a valid Geometry Object.");
      return false;
    };	
 };
  
  return true;
}


/* ----------------------------------------------------------------------- */


Group::Group() :
  Geometry(),
  __geometryList(0),
  __skeleton(DEFAULT_SKELETON) {
}

Group::Group( const GeometryArrayPtr& geometryList,
              const GeomPolylinePtr& skeleton ) :
  Geometry(),
  __geometryList(geometryList),
  __skeleton(skeleton) {
  GEOM_ASSERT(isValid());
}

Group::~Group( ) {
}

bool Group::apply( Action& action ) { 
  return action.process(this);
}

/* ----------------------------------------------------------------------- */

const GeometryPtr&
Group::getGeometryListAt( uint32_t i ) const {
  GEOM_ASSERT(i < __geometryList->getSize());
  return __geometryList->getAt(i);
}

GeometryPtr& 
Group::getGeometryListAt( uint32_t i ) {
  GEOM_ASSERT(i < __geometryList->getSize());
  return __geometryList->getAt(i);
}

const GeometryArrayPtr& 
Group::getGeometryList( ) const {
  return __geometryList;
}
GeometryArrayPtr& 
Group::getGeometryList( ) {
  return __geometryList;
}

uint32_t 
Group::getGeometryListSize( ) const {
  return __geometryList->getSize();
}

/* ----------------------------------------------------------------------- */

const GeomPolylinePtr& 
Group::getSkeleton( ) const {
  return __skeleton;
}

GeomPolylinePtr& 
Group::getSkeleton( ){
  return __skeleton;
}

bool 
Group::isSkeletonToDefault( ) const {
  return __skeleton == DEFAULT_SKELETON;
}

/* ----------------------------------------------------------------------- */

bool Group::isACurve( ) const {
  for (GeometryArray::const_iterator _i = __geometryList->getBegin();
       _i != __geometryList->getEnd();
       _i++)
    if (! (*_i)->isACurve()) return false;
  return true;
}


bool Group::isASurface( ) const {
  for (GeometryArray::const_iterator _i = __geometryList->getBegin();
       _i != __geometryList->getEnd();
       _i++)
    if (! (*_i)->isASurface()) return false;
  return true;
}


bool Group::isAVolume( ) const {
  for (GeometryArray::const_iterator _i = __geometryList->getBegin();
       _i != __geometryList->getEnd();
       _i++)
    if (! (*_i)->isAVolume()) return false;
  return true;
}


bool Group::isExplicit( ) const {
  for (GeometryArray::const_iterator _i = __geometryList->getBegin();
       _i != __geometryList->getEnd();
       _i++)
    if (! (*_i)->isExplicit()) return false;
  return true;
}

/* ----------------------------------------------------------------------- */

bool Group::isValid( ) const {
  Builder _builder;
  _builder.GeometryList = const_cast<GeometryArrayPtr *>(&__geometryList);
  _builder.Skeleton = const_cast<GeomPolylinePtr *>(&__skeleton);
  if(!_builder.isValid())return false;
  uint32_t _size = __geometryList->getSize();
  for (uint32_t _i = 0; _i < _size ; _i++) {
      const GeometryPtr& _geometry = __geometryList->getAt(_i);
      if (! (_geometry) && (_geometry->isValid())) {
	  genMessage
	      (WARNINGMSG(INVALID_FIELD_ITH_VALUE_ssss),"Group","GeometryList",number(_i + 1).c_str(),"Must be a valid Geometry Object.");
	  return false;
      };	
  };
  return true;
}

/* ----------------------------------------------------------------------- */

SceneObjectPtr Group::copy() const 
{
  Group * ptr = new Group(*this);
  uint32_t size = ptr->getGeometryListSize();
  GeometryArrayPtr _ga(new GeometryArray(size));
  uint32_t i = 0;
  for(GeometryArray::iterator _it = _ga->getBegin() ; _it != _ga->getEnd() ; _it ++){
    if(__geometryList->getAt(i))_it->cast(__geometryList->getAt(i)->copy());
    i++;
  }
  ptr->getGeometryList( ) = _ga;
  if(__skeleton)ptr->getSkeleton().cast(__skeleton->copy());
  return SceneObjectPtr(ptr);
}


/* ----------------------------------------------------------------------- */