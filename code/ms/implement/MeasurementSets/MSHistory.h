//# MSHistory.h: The MeasurementSet HISTORY Table
//# Copyright (C) 1996,1999,2000
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
//#
//# $Id$

#if !defined(AIPS_MSHISTORY_H)
#define AIPS_MSHISTORY_H

#include <aips/aips.h>
#include <aips/MeasurementSets/MSTable.h>
#include <aips/MeasurementSets/MSHistoryEnums.h>

// <summary> 
// A Table intended to hold a MeasurementSet OBSERVATIONLOG table.
// </summary>

// <use visibility=export>

// <reviewed reviewer="Bob Garwood" date="1997/02/01" tests="" demos="">

// <prerequisite>
// <ul>
//   <li> <linkto class="MeasurementSet:description">MeasurementSet</linkto> 
//   <li> <linkto class="MSTable">MSTable</linkto> 
// </ul>
// </prerequisite>
//
// <etymology>
// MSHistory stands for the MeasurementSet ObservationLog table.
// </etymology>
//
// <synopsis> 
// An MSHistory is a table intended to hold the OBSERVATIONLOG table for
// the MeasurementSet. It has an identical set of member functions as
// the main MeasurementSet class, except (currently) for the default
// calibration members. For further info and examples see the 
// MeasurementSet class.
// </synopsis> 
//
// <example>
// See the MeasurementSet for an example of how to access and use this class.
// </example>
//
// <motivation>
// It was found that subtables and the main table of the MeasurementSet have
// a lot in common, therefore they derive their interface from the same
// base class. Each subtable has its own class to keep the enum definitions
// and conversion functions in separate scopes.
// </motivation>
//
// <todo asof="1999/1/14">
// see MeasurementSet.
// </todo>

class MSHistory:public MSHistoryEnums,
		public MSTable<MSHistoryEnums::PredefinedColumns,
		               MSHistoryEnums::PredefinedKeywords>
{
public:

    // This constructs an empty MSHistory
    MSHistory ();

    // These constructors mirror the Table ones with additional checking
    // on validity (verifying that the MSHistory will have the required columns
    // and keywords)
    // An exception is thrown if the constructed Table is not a valid MSHistory
    // <thrown>
    //   <li> AipsError
    // </thrown>
    // <group name=tableLikeConstructors>
    MSHistory (const String &tableName, TableOption = Table::Old);
    MSHistory (const String &tableName, const String &tableDescName,
		    TableOption = Table::Old);
    MSHistory (SetupNewTable &newTab, uInt nrrow = 0, Bool initialize = False);
    MSHistory (const Table &table);
    MSHistory (const MSHistory &other);
    // </group>

    // As with tables, the destructor writes the table if necessary.
    // Additional checking is done here to verify that all required
    // columns are still present.
    // If it is NOT valid, it will write the table and then throw an exception.
    // <thrown>
    //   <li> AipsError
    // </thrown>
    ~MSHistory();

    //  Assignment operator, reference semantics
    MSHistory& operator=(const MSHistory&);

    // Make a special copy of this Table which references all columns from
    // this Table except those mentioned; those are empty and writable.
    // Each forwarded column has the same writable status as the underlying
    // column. The mentioned columns all use the AipsIO storage manager.
    // This function is inherited from MSTable and unlikely to be of use,
    // except in the class MeasurementSet (see comment there)..
    MSHistory referenceCopy(const String& newTableName,
			   const Block<String>& writableColumns) const;

    // Initialize the statics appropriately. This does not need to be
    // called by users, it is called by the implementation class
    // MSTableImpl.
    static void init();

private:

    // required by the need to throw an exception in the destructor
    Bool hasBeenDestroyed_p;
};

#endif



