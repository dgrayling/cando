/*
    File: bondedJoint.cc
*/
/*
Open Source License
Copyright (c) 2016, Christian E. Schafmeister
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 
This is an open source license for the CANDO software from Temple University, but it is not the only one. Contact Temple University at mailto:techtransfer@temple.edu if you would like a different license.
*/
/* -^- */
#define DEBUG_LEVEL_FULL

#define DEBUG_UPDATEXYZCOORDS

#include <clasp/core/foundation.h>
#include <clasp/core/object.h>
#include <clasp/core/lisp.h>
#include <cando/geom/matrix.h>
#include <clasp/core/lispStream.h>
#include <clasp/core/symbolTable.h>
#include <cando/chem/atomId.h>
#include <clasp/core/numerics.h>
#include <cando/kinematics/stub.h>
#include <cando/kinematics/jumpJoint.h>
#include <cando/kinematics/complexBondedJoint.h>

namespace kinematics
{
void ComplexBondedJoint_O::fields(core::Record_sp record) {
  static_assert(ComplexBondedJoint_O::MaxInputStubJoints==2,"ComplexBondedJoint_O::MaxInputStubJoints has changed from 2 - update the code below");
  record->field_if_not_unbound(INTERN_(kw,inputStub1),this->_InputStubJoints[0]);
  record->field_if_not_unbound(INTERN_(kw,inputStub2),this->_InputStubJoints[1]);
  this->Base::fields(record);
}


void ComplexBondedJoint_O::initialize() {
  for (int i=0; i<MaxInputStubJoints; ++i ) {
    this->_InputStubJoints[i] = unbound<Joint_O>();
  }
}

CL_LAMBDA(atom-id &optional name);
CL_LISPIFY_NAME("make_ComplexBondedJoint");
CL_DEF_CLASS_METHOD
ComplexBondedJoint_sp ComplexBondedJoint_O::make(const chem::AtomId& atomId, core::T_sp name) {
  return gctools::GC<ComplexBondedJoint_O>::allocate(atomId, name);
}



Stub ComplexBondedJoint_O::getInputStub() const
{
  Stub stub;
  if (this->inputStubJoint1().unboundp()) {
    if (gc::IsA<JumpJoint_sp>(this->parent())) {
      JumpJoint_sp jumpJoint = gc::As_unsafe<JumpJoint_sp>(this->parent());
      Matrix flipped = jumpJoint->transform().flipXY();
      stub.fromMatrix(flipped);
      return stub;
    }
    SIMPLE_ERROR(("Illegal to getInputStub with only a parent and that parent is not a JumpJoint_sp - it's a %s") , _rep_(this->parent()));
  }
  if (this->inputStubJoint2().unboundp()) {
    if (gc::IsA<JumpJoint_sp>(this->parent())) {
      IMPLEMENT_MEF(("Handle inputStubJoint1() is defined but parent() is the JumpJoint"));
    }
    stub.fromCenterAndRotation( this->inputStubJoint0()->getPosition(), gc::As<JumpJoint_sp>(this->inputStubJoint1())->transform().flipXY());
//    stub.fromCenterAndRotation( this->inputStubJoint0()->getPosition(), gc::As<JumpJoint_sp>(this->inputStubJoint1())->transform());
    return stub;
  }
  stub.fromFourPoints(this->inputStubJoint0()->position(),
                      this->inputStubJoint1()->position(),
                      this->inputStubJoint0()->position(),
                      this->inputStubJoint2()->position());
  return stub;
}

CL_DEFMETHOD bool ComplexBondedJoint_O::inputStubJoint1BoundP() const {
  return !this->_InputStubJoints[0].unboundp();
}

CL_DEFMETHOD void ComplexBondedJoint_O::setInputStubJoint1(Joint_sp joint) {
  this->_InputStubJoints[0] = joint;
}

CL_DEFMETHOD void ComplexBondedJoint_O::makeUnboundInputStubJoint1() {
  this->_InputStubJoints[0] = unbound<Joint_O>();
}

CL_DEFMETHOD bool ComplexBondedJoint_O::inputStubJoint2BoundP() const {
  return !this->_InputStubJoints[1].unboundp();
}

CL_DEFMETHOD void ComplexBondedJoint_O::setInputStubJoint2(Joint_sp joint) {
  this->_InputStubJoints[1] = joint;
}

CL_DEFMETHOD void ComplexBondedJoint_O::makeUnboundInputStubJoint2() {
  this->_InputStubJoints[1] = unbound<Joint_O>();
}

void ComplexBondedJoint_O::_updateChildrenXyzCoords() {
  this->Joint_O::_updateChildrenXyzCoords();
}

void ComplexBondedJoint_O::_updateInternalCoord()
{_OF();
  KIN_LOG((" <<< %s\n") , _rep_(this->asSmartPtr()));
//	using numeric::x_rotation_matrix_radians;
//	using numerioc::z_rotation_matrix_radians;
//	using numeric::constants::d::pi;
  Joint_sp jC = this->parent();
  Vector3 C = jC->position();
  this->_Distance = geom::calculateDistance(this->_Position,C);
  KIN_LOG(("Calculated _Distance = %lf\n") , this->_Distance );
  if (!this->inputStubJoint1().unboundp()) {
    Joint_sp jB = this->inputStubJoint1();
    Vector3 B = jB->position();
//    this->_Theta = PREPARE_ANGLE(geom::calculateAngle(this->_Position,C,B)); // Must be from incoming direction
    this->_Theta = geom::calculateAngle(this->_Position,C,B); // Must be from incoming direction
    KIN_LOG(("_Theta = %lf\n") , (this->_Theta/0.0174533));
    if (!this->inputStubJoint2().unboundp()) {
      Joint_sp jA = this->inputStubJoint2();
      Vector3 A = jA->position();
      this->_Phi = geom::calculateDihedral(this->_Position,C,B,A);
      KIN_LOG(("_Phi = %lf\n") , (this->_Phi/0.0174533));
      return;
    }
  }
  
#if 0
#if 1
  internalCoordinatesFromPointAndCoordinateSystem(this->getPosition(),this->getInputStub()._Transform,
                                                  this->_Distance, this->_Theta, this->_Phi );
#else
  KIN_LOG(("gc::IsA<JumpJoint_sp>(jC)   jC = %s\n") , _rep_(jC));
  Stub stub = jC->getStub();
  KIN_LOG(("stub = \n%s\n") , stub._Transform.asString());
  Vector3 x = stub._Transform.colX();
  Vector3 y = stub._Transform.colY();
  Vector3 z = stub._Transform.colZ();
  KIN_LOG(("x = %s\n") , x.asString());
  KIN_LOG(("y = %s\n") , y.asString());
  KIN_LOG(("z = %s\n") , z.asString());
  Vector3 D = this->getPosition();
  Vector3 CD = D - C;
  double lengthCD = CD.length();
  if (lengthCD<SMALL_NUMBER) SIMPLE_ERROR(("About to divide by zero"));
  Vector3 d = CD*(1.0/lengthCD);
  KIN_LOG(("d = %s\n") , d.asString());
  double dx = d.dotProduct(x);
  double dy = d.dotProduct(y);
  double dz = d.dotProduct(z);
  KIN_LOG(("dx = %lf  dy = %lf  dz = %lf\n") , dx , dy , dz );
  this->_Phi = geom::geom__planeVectorAngle(dy,dz);
  KIN_LOG(("  dy = %lf   dz = %lf\n") , dy , dz );
  KIN_LOG(("_Phi = %lf deg\n") , (this->_Phi/0.0174533));
  Vector3 dox(1.0,0.0,0.0);
  Vector3 dop(dx,dy,dz);
  KIN_LOG(("dop = %s\n") , dop.asString());
  KIN_LOG(("dop.dotProduct(dox) = %lf\n") , dop.dotProduct(dox));
  if (dop.dotProduct(dox) > (1.0-SMALL_NUMBER)) {
    this->_Theta = 0.0;
    return;
  }
  Vector3 doz = dox.crossProduct(dop);
  doz = doz.normalized();
  KIN_LOG(("doz = %s\n") , doz.asString());
  Vector3 doy = doz.crossProduct(dox);
  KIN_LOG(("doy = %s\n") , doy.asString());
  double eox = dop.dotProduct(dox);
  double eoy = dop.dotProduct(doy);
  KIN_LOG(("eox = %lf  eoy = %lf\n") , eox , eoy );
//  double eoz = dop.dotProduct(doz); // Must be 0.0
  this->_Theta = geom::geom__planeVectorAngle(eox,eoy);
  KIN_LOG(("    this->_Theta = %lf deg\n") , (this->_Theta/0.0174533));
#endif
  #endif
}

};