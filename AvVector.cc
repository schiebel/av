//# Copyright (C) 1995-97 Board of Trustees of the University of Illinois
//#
//# This software, both binary and source, is copyrighted by The
//# Board of Trustees of the University of Illinois. Ownership
//# remains with the University. You should have received a copy 
//# of a licensing agreement with this software. See the file 
//# "AIPSVIEW_COPYRIGHT", or contact the University at this address:
//#
//#     The NCSA AipsView Visualization System
//#     National Center for Supercomputing Applications
//#     University of Illinois
//#     405 North Mathews Ave.
//#     Urbana, IL 61801
//#     aipsview@ncsa.uiuc.edu
//# --------------------------------------------------
//
//
// $Header: /home/cvs/aips++/code/trial/apps/aipsview/Attic/AvVector.cc,v 19.0 2003/07/16 05:46:50 aips2adm Exp $
//
// $Log: AvVector.cc,v $
// Revision 19.0  2003/07/16 05:46:50  aips2adm
// exhale: Base release 19.000.00
//
// Revision 18.0  2002/06/07 21:28:35  aips2adm
// exhale: Base release 18.000.00
//
// Revision 17.0  2001/11/12 19:42:04  aips2adm
// exhale: Base release 17.000.00
//
// Revision 16.0  2001/05/03 01:42:13  aips2adm
// exhale: Base release 16.000.00
//
// Revision 15.0  2000/10/26 17:08:58  aips2adm
// exhale: Base release 15.000.00
//
// Revision 14.0  2000/03/23 16:07:52  aips2adm
// exhale: Base release 14.000.00
//
// Revision 13.1  2000/03/09 17:03:35  hravlin
// Removed a couple of compiler warnings.
//
// Revision 13.0  1999/08/10 18:39:36  aips2adm
// exhale: Base release 13.000.00
//
// Revision 12.0  1999/07/15 00:21:56  aips2adm
// exhale: Base release 12.000.00
//
// Revision 11.0  1998/10/03 06:58:46  aips2adm
// exhale: Base release 11.000.00
//
// Revision 10.0  1998/07/20 17:52:25  aips2adm
// exhale: Base release 10.000.00
//
// Revision 9.0  1997/08/25 21:25:24  aips2adm
// exhale: Base release 09.000.00
//
// Revision 8.0  1997/02/20 03:15:30  aips2adm
// exhale: Base release 08.000.00
//
// Revision 7.3  1996/12/12 09:51:53  droberts
// Final update from monet archive.
//
// Revision 1.4  1996/12/11 20:40:12  hr
// Added RCS header and copyright.
//
//
//---------------------------------------------------------------------------

#include <iostream.h>

#include "AvVector.h"

template <class T> 
AvVector<T>::AvVector(unsigned int length, int /* origin */) : length_(length)
{
  vecrep_ = new T[length_];
}

template <class T>
const T & AvVector<T>::operator()(int index) const
{
  if (index < 0 || (unsigned int)index >= length_)
    {
      cerr << "AvVector<T>operator() const: bad index : " << index << endl;
      return *vecrep_;
    }
  else
    return vecrep_[index];
}

template <class T>
T & AvVector<T>::operator()(int index)
{
  if (index < 0 || (unsigned int)index >= length_)
    {
      cerr << "AvVector<T>operator(): bad index : " << index << endl;
      return *vecrep_;
    }
  else
    return vecrep_[index];
}

template <class T>
AvVector<T>::~AvVector()
{
  delete [] vecrep_;
}
