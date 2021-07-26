/*
    File: iterateRestraints.cc
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
#define	DEBUG_LEVEL_NONE

#include <cando/chem/iterateRestraints.h>
//#include "core/archiveNode.h"
//#include "core/archive.h"
#include <cando/chem/restraint.h>
#include <clasp/core/wrappers.h>


namespace chem {


IterateRestraints_sp IterateRestraints_O::create(Matter_sp matter)
{
  auto  m  = gctools::GC<IterateRestraints_O>::allocate_with_default_constructor();
    m->initTop(matter);
    return m;
};


void	IterateRestraints_O::initialize()
{
    this->Base::initialize();
}


void	IterateRestraints_O::_accumulateRestraints(Matter_sp m)
{
  core::List_sp mine = m->allRestraints();
  this->_Restraints = core::cl__copy_seq(mine);
}






void	IterateRestraints_O::initTop(Matter_sp m)
{
    this->_Restraints = _Nil<core::T_O>();
    this->_accumulateRestraints(m);
}

void	IterateRestraints_O::first()
{
    this->_CurRestraint = this->_Restraints;
}

void	IterateRestraints_O::next()
{
    if ( this->_CurRestraint.notnilp() )
    {
      this->_CurRestraint = oCdr(this->_CurRestraint);
    }
}


    core::T_sp IterateRestraints_O::currentObject()
{
    if ( this->_CurRestraint.nilp() ) return _Nil<core::T_O>();
    return core::oCar(this->_CurRestraint);
}

bool	IterateRestraints_O::isDone()
{
    ASSERTNOTNULL(this->_CurRestraint);
    return this->_CurRestraint.nilp();
}







};
