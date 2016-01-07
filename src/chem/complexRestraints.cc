#define	DEBUG_LEVEL_FULL

#include <cando/chem/complexRestraints.h>
//#include "core/archiveNode.h"
//#include "core/archive.h"
#include <clasp/core/str.h>
#include <cando/chem/restraint.h>
#include <clasp/core/symbolTable.h>
#include <cando/chem/residue.h>
#include <cando/chem/atom.h>
#include <cando/chem/chemInfo.h>
#
// last include is wrappers.h
#include <clasp/core/wrappers.h>


namespace chem {








void ComplexRestraint_O::exposeCando(core::Lisp_sp env)
{
    core::class_<ComplexRestraint_O>()
	;
}

    void ComplexRestraint_O::exposePython(core::Lisp_sp lisp)
    {_G();
#ifdef	USEBOOSTPYTHON //[
	PYTHON_CLASS(ChemPkg,ComplexRestraint,"","",_lisp)
    ;
#endif //]
}


#define ARGS_ComplexRestraint_O_make "()"
#define DECL_ComplexRestraint_O_make ""
#define DOCS_ComplexRestraint_O_make "make ComplexRestraint"
  ComplexRestraint_sp ComplexRestraint_O::make()
  {_G();
    IMPLEMENT_ME();
  };


void	ComplexRestraint_O::initialize()
{
    this->Base::initialize();
}

#ifdef XML_ARCHIVE
    void	ComplexRestraint_O::archiveBase(core::ArchiveP node)
{
    this->Base::archiveBase(node);
}
#endif


void RestrainedPiBond_O::exposeCando(core::Lisp_sp env)
{
    core::class_<RestrainedPiBond_O>()
    .def("getConfiguration",&RestrainedPiBond_O::getConfiguration)
    .def("setConfiguration",&RestrainedPiBond_O::setConfiguration)
//	.def("getPriority1AtomOnAtomX",&RestrainedPiBond_O::getPriority1AtomOnAtomX)
//	.def("setPriority1AtomOnAtomX",&RestrainedPiBond_O::setPriority1AtomOnAtomX)
//	.def("getPriority2AtomOnAtomX",&RestrainedPiBond_O::getPriority2AtomOnAtomX)
//	.def("setPriority2AtomOnAtomX",&RestrainedPiBond_O::setPriority2AtomOnAtomX)
    .def("getPiAtomX",&RestrainedPiBond_O::getPiAtomX)
    .def("setPiAtomX",&RestrainedPiBond_O::setPiAtomX)
//	.def("getPriority1AtomOnAtomY",&RestrainedPiBond_O::getPriority1AtomOnAtomY)
//	.def("setPriority1AtomOnAtomY",&RestrainedPiBond_O::setPriority1AtomOnAtomY)
//	.def("getPriority2AtomOnAtomY",&RestrainedPiBond_O::getPriority2AtomOnAtomY)
//	.def("setPriority2AtomOnAtomY",&RestrainedPiBond_O::setPriority2AtomOnAtomY)
    .def("getPiAtomY",&RestrainedPiBond_O::getPiAtomY)
    .def("setPiAtomY",&RestrainedPiBond_O::setPiAtomY)
    ;
};
    void RestrainedPiBond_O::exposePython(core::Lisp_sp lisp)
    {_G();
#ifdef	USEBOOSTPYTHON
	PYTHON_CLASS(ChemPkg,RestrainedPiBond,"","",_lisp)
	    .def("getConfiguration",&RestrainedPiBond_O::getConfiguration)
	    .def("setConfiguration",&RestrainedPiBond_O::setConfiguration)
//	.def("getPriority1AtomOnAtomX",&RestrainedPiBond_O::getPriority1AtomOnAtomX)
//	.def("setPriority1AtomOnAtomX",&RestrainedPiBond_O::setPriority1AtomOnAtomX)
//	.def("getPriority2AtomOnAtomX",&RestrainedPiBond_O::getPriority2AtomOnAtomX)
//	.def("setPriority2AtomOnAtomX",&RestrainedPiBond_O::setPriority2AtomOnAtomX)
	    .def("getPiAtomX",&RestrainedPiBond_O::getPiAtomX)
	    .def("setPiAtomX",&RestrainedPiBond_O::setPiAtomX)
//	.def("getPriority1AtomOnAtomY",&RestrainedPiBond_O::getPriority1AtomOnAtomY)
//	.def("setPriority1AtomOnAtomY",&RestrainedPiBond_O::setPriority1AtomOnAtomY)
//	.def("getPriority2AtomOnAtomY",&RestrainedPiBond_O::getPriority2AtomOnAtomY)
//	.def("setPriority2AtomOnAtomY",&RestrainedPiBond_O::setPriority2AtomOnAtomY)
	    .def("getPiAtomY",&RestrainedPiBond_O::getPiAtomY)
	    .def("setPiAtomY",&RestrainedPiBond_O::setPiAtomY)
	    ;
#endif
    };
#ifdef XML_ARCHIVE
    void	RestrainedPiBond_O::archive(core::ArchiveP node)
{
    node->attribute( "Configuration",this->_Configuration);
//    node->attribute( "Priority1AtomOnAtomX",this->_Priority1AtomOnAtomX);
//    node->attribute( "Priority2AtomOnAtomX",this->_Priority2AtomOnAtomX);
    node->attribute( "PiAtomX",this->_PiAtomX);
//    node->attribute( "Priority1AtomOnAtomY",this->_Priority1AtomOnAtomY);
//    node->attribute( "Priority2AtomOnAtomY",this->_Priority2AtomOnAtomY);
    node->attribute( "PiAtomY",this->_PiAtomY);
}
#endif

#if INIT_TO_FACTORIES

SYMBOL_EXPORT_SC_(ChemKwPkg,E);
SYMBOL_EXPORT_SC_(ChemKwPkg,Z);

#define ARGS_RestrainedPiBond_O_make "(config pi_atoms)"
#define DECL_RestrainedPiBond_O_make ""
#define DOCS_RestrainedPiBond_O_make "make RestrainedPiBond"
RestrainedPiBond_sp RestrainedPiBond_O::make(core::Symbol_sp config, core::List_sp piAtoms)
  {_G();
      GC_ALLOCATE(RestrainedPiBond_O, me );
    me->_Configuration = config;
    if ( me->_Configuration != chemkw::_sym_E && me->_Configuration != chemkw::_sym_Z )
    {
	SIMPLE_ERROR(BF("config:  must be ""E"" or ""Z""" ));
    }
    if ( core::cl__length(piAtoms) != 2 )
    {
	SIMPLE_ERROR(BF("You must supply two atom names in piAtoms: argument"));
    }
    me->_PiAtomX = oCar(piAtoms).as<MatterName::Type>();
    me->_PiAtomY = oCadr(piAtoms).as<MatterName::Type>();
    return me;
  };

#else

    core::T_sp 	RestrainedPiBond_O::__init__(core::Function_sp exec, core::Cons_sp args, core::Environment_sp env, core::Lisp_sp lisp)
{_OF();
    this->_Configuration = env->lookup(Pkg(),"config").as<core::Str_O>()->get();
    core::Cons_sp piAtoms = env->lookup(Pkg(),"piAtoms").as<core::Cons_O>();
    if ( this->_Configuration != "E" && this->_Configuration != "Z" )
    {
	SIMPLE_ERROR(BF("config:  must be ""E"" or ""Z""" ));
    }
    if ( core::cl__length(piAtoms) != 2 )
    {
	SIMPLE_ERROR(BF("You must supply two atom names in piAtoms: argument"));
    }
    this->_PiAtomX = piAtoms->car<core::Str_O>()->get();
    this->_PiAtomY = piAtoms->cadr<core::Str_O>()->get();
    return _Nil<core::T_O>();
}

#endif

void	RestrainedPiBond_O::fillOneDihedralRestraint(Residue_sp residue,
				Atom_sp p1X, Atom_sp pX, Atom_sp pY, Atom_sp p1Y,
				double min, double max, double weight)
{
    RestraintDihedral_sp dih = RestraintDihedral_O::create();
    dih->setAtomA(p1X);
    dih->setAtomB(pX);
    dih->setAtomC(pY);
    dih->setAtomD(p1Y);
    dih->setParameters(min,max,weight);
    residue->addRestraint(dih);
}



/*! Create four dihedral restraints for each RestrainedPiBond entry
 * Only one trans bond was defined, the other three interactions are inferred
 *
 *	x1   y1
 *	  X=Y
 *	x2   y2
 *
 * Depending on if _Configuration is "E" or "Z"
 * four dihedral restraints are set up with:
 * for "E"  x1&y1 trans
 * for "Z"  x1&y1 cis
 */
void	RestrainedPiBond_O::fillRestraints(Residue_sp residue)
{_OF();
    core::Symbol_sp config = this->_Configuration;
    Atom_sp pX  = residue->atomWithName(this->getPiAtomX());
    Atom_sp pY  = residue->atomWithName(this->getPiAtomY());

    Atom_sp p1X = pX->highestPriorityNeighborThatIsnt(pY);
    Atom_sp p2X = pX->lowestPriorityNeighborThatIsnt(pY);
    if ( p2X == p1X )
    {
	p2X = _Nil<Atom_O>();
    }
    Atom_sp p1Y = pY->highestPriorityNeighborThatIsnt(pX);
    Atom_sp p2Y = pY->lowestPriorityNeighborThatIsnt(pX);
    if ( p2Y == p1Y )
    {
	p2Y = _Nil<Atom_O>();
    }
    if ( pX->numberOfBonds() < 2 || pX->numberOfBonds() > 3  )
    {
	stringstream ss;
	ss << "Dihedral restraints X atom: " << this->_PiAtomX;
	ss << " must have between 2 and 3 bonds - it has " << pX->numberOfBonds();
	SIMPLE_ERROR(BF("Dihedral restraints Y atom: %s") % this->_PiAtomY );
	ss << " must have between 2 and 3 bonds - it has " << pY->numberOfBonds();
	SIMPLE_ERROR(BF("%s")%ss.str() );
    }
    double transMin = -170.0;
    double transMax = 170.0;
    double cisMin = 10.0;
    double cisMax = -10.0;
    double weight = 1.0;
    if ( config == chemkw::_sym_Z )
    {
	this->fillOneDihedralRestraint(residue,p1X,pX,pY,p1Y,cisMin,cisMax,weight);
	if ( p2Y.notnilp() ) 
	{
	    this->fillOneDihedralRestraint(residue,p1X,pX,pY,p2Y,transMin,transMax,weight);
	}
	if ( p2X.notnilp() ) 
	{
	    this->fillOneDihedralRestraint(residue,p2X,pX,pY,p1Y,transMin,transMax,weight);
	}
	if ( p2X.notnilp() && p2Y.notnilp() )
	{
	    this->fillOneDihedralRestraint(residue,p2X,pX,pY,p2Y,cisMin,cisMax,weight);
	}
    } else if ( config == chemkw::_sym_E )
    {
	this->fillOneDihedralRestraint(residue,p1X,pX,pY,p1Y,transMin,transMax,weight);
	if ( p2Y.notnilp() )
	{
	    this->fillOneDihedralRestraint(residue,p1X,pX,pY,p2Y,cisMin,cisMax,weight);
	}
	if ( p2X.notnilp() )
	{
	    this->fillOneDihedralRestraint(residue,p2X,pX,pY,p1Y,cisMin,cisMax,weight);
	}
	if ( p2X.notnilp() && p2Y.notnilp() )
	{
	    this->fillOneDihedralRestraint(residue,p2X,pX,pY,p2Y,transMin,transMax,weight);
	}
    } else
    {
	SIMPLE_ERROR(BF("Illegal Pi bond configuration: %s must be E or Z") % config);
    }
}


void RestrainedExoCyclicAtom_O::exposeCando(core::Lisp_sp env)
{
    core::class_<RestrainedExoCyclicAtom_O>()
	;
}

    void RestrainedExoCyclicAtom_O::exposePython(core::Lisp_sp lisp)
    {_G();
#ifdef	USEBOOSTPYTHON //[
	PYTHON_CLASS(ChemPkg,RestrainedExoCyclicAtom,"","",_lisp)
    ;
#endif //]
}

bool RestrainedExoCyclicAtom_O::_LazyInitializedSmarts = false;
ChemInfo_sp RestrainedExoCyclicAtom_O::_AtomExoToSixMemberedRing;

void	RestrainedExoCyclicAtom_O::lazyInitializeSmarts()
{_OF();
    if ( !RestrainedExoCyclicAtom_O::_LazyInitializedSmarts )
    {
	RestrainedExoCyclicAtom_O::_LazyInitializedSmarts = true;
	RestrainedExoCyclicAtom_O::_AtomExoToSixMemberedRing = ChemInfo_O::create();
	bool success = RestrainedExoCyclicAtom_O::_AtomExoToSixMemberedRing->compileSmarts("[*]9[*]1[*]2[*]3[*]4[*]5[*]6[?9]");
	if ( !success )
	{
	    SIMPLE_ERROR(BF("Error compiling SMARTS code for _AtomExoToSixMemberedRing"));
	}
    }
}

void	RestrainedExoCyclicAtom_O::initialize()
{
    this->_ExoCyclicAtomName = _Nil<MatterName::Type>();
    this->_Axial = true;
}

#if INIT_TO_FACTORIES

#define ARGS_RestrainedExoCyclicAtom_O_make "()"
#define DECL_RestrainedExoCyclicAtom_O_make ""
#define DOCS_RestrainedExoCyclicAtom_O_make "make RestrainedExoCyclicAtom"
  RestrainedExoCyclicAtom_sp RestrainedExoCyclicAtom_O::make()
  {_G();
    IMPLEMENT_ME();
  };

#else

    core::T_sp 	RestrainedExoCyclicAtom_O::__init__(core::Function_sp exec, core::Cons_sp args, core::Environment_sp env, core::Lisp_sp lisp)
{
    IMPLEMENT_ME();
//    this->Base::oldLispInitialize(kargs,env);
}

#endif

#ifdef XML_ARCHIVE
void	RestrainedExoCyclicAtom_O::archiveBase(core::ArchiveP node)
{
    this->Base::archiveBase(node);
    node->attribute("exoCyclicAtomName",this->_ExoCyclicAtomName);
    node->attribute("axial",this->_Axial);
}
#endif



void RestrainedExoCyclicAtom_O::fillRestraints(Residue_sp residue )
{_OF();
    this->lazyInitializeSmarts();
    if ( !residue->hasAtomWithName(this->_ExoCyclicAtomName) )
    {
	SIMPLE_ERROR(BF("Residue(%s) doesn't have atom with name(%s)") % residue->description() % this->_ExoCyclicAtomName );
    }
    Atom_sp exoCyclicAtom = residue->atomWithName(this->_ExoCyclicAtomName);
    RestrainedExoCyclicAtom_O::_AtomExoToSixMemberedRing->matches(exoCyclicAtom);
    ChemInfoMatch_sp match = RestrainedExoCyclicAtom_O::_AtomExoToSixMemberedRing->getMatch();
    if ( !match->matches() )
    {
	SIMPLE_ERROR(BF("In residue(%s) the atom with name(%s) is not exo-cyclic to a six-membered ring") % residue->description() % _rep_(this->_ExoCyclicAtomName) );
    }
    SYMBOL_EXPORT_SC_(ChemKwPkg,1);
    SYMBOL_EXPORT_SC_(ChemKwPkg,2);
    SYMBOL_EXPORT_SC_(ChemKwPkg,3);
    SYMBOL_EXPORT_SC_(ChemKwPkg,4);
    SYMBOL_EXPORT_SC_(ChemKwPkg,5);
    SYMBOL_EXPORT_SC_(ChemKwPkg,6);
    Atom_sp a1 = match->tag(chemkw::_sym_1);
    Atom_sp a2 = match->tag(chemkw::_sym_2);
    Atom_sp a3 = match->tag(chemkw::_sym_3);
    Atom_sp a4 = match->tag(chemkw::_sym_4);
    Atom_sp a5 = match->tag(chemkw::_sym_5);
    Atom_sp a6 = match->tag(chemkw::_sym_6);
    FIX_ME(); // is the above correct?
}

    EXPOSE_CLASS(chem, ComplexRestraint_O );
    EXPOSE_CLASS(chem, RestrainedPiBond_O );
    EXPOSE_CLASS(chem, RestrainedExoCyclicAtom_O );

};


