//# TableLock.cc: Class to hold table lock options
//# Copyright (C) 1997,2000
//# Associated Universities, Inc. Washington DC, USA.
//#
//# This library is free software; you can redistribute it and/or modify it
//# under the terms of the GNU Library General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or (at your
//# option) any later version.
//#
//# This library is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
//# License for more details.
//#
//# You should have received a copy of the GNU Library General Public License
//# along with this library; if not, write to the Free Software Foundation,
//# Inc., 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//#
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA
//#
//# $Id$


#include <aips/Tables/TableLock.h>
#include <aips/Tables/TableError.h>


TableLock::TableLock (LockOption option)
: itsOption            (option),
  itsReadLocking       (True),
  itsMaxWait           (0),
  itsInterval          (5),
  itsIsDefaultInterval (True)
{
  init();
}

TableLock::TableLock (LockOption option, double inspectionInterval,
		      uInt maxWait)
: itsOption            (option),
  itsReadLocking       (True),
  itsMaxWait           (maxWait),
  itsInterval          (inspectionInterval),
  itsIsDefaultInterval (False)
{
  init();
}

TableLock::TableLock (const TableLock& that)
: itsOption            (that.itsOption),
  itsReadLocking       (that.itsReadLocking),
  itsMaxWait           (that.itsMaxWait),
  itsInterval          (that.itsInterval),
  itsIsDefaultInterval (that.itsIsDefaultInterval)
{}

TableLock& TableLock::operator= (const TableLock& that)
{
  if (this != &that) {
    itsOption            = that.itsOption;
    itsReadLocking       = that.itsReadLocking;
    itsMaxWait           = that.itsMaxWait;
    itsInterval          = that.itsInterval;
    itsIsDefaultInterval = that.itsIsDefaultInterval;
  }
  return *this;
}


void TableLock::init()
{
  if (itsOption == AutoNoReadLocking) {
    itsOption      = AutoLocking;
    itsReadLocking = False;
  } else if (itsOption == UserNoReadLocking) {
    itsOption      = UserLocking;
    itsReadLocking = False;
  }
}


void TableLock::merge (const TableLock& that)
{
  if (that.itsOption < itsOption) {
    itsOption  = that.itsOption;
    itsMaxWait = that.itsMaxWait;
  }
  if (that.itsReadLocking) {
    itsReadLocking = True;
  }
  if (! that.itsIsDefaultInterval) {
    if (itsIsDefaultInterval  ||  itsInterval > that.itsInterval) {
      itsInterval = that.itsInterval;
    }
  }
}
