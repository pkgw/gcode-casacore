//# tDirectory.cc: Test program for class Directory
//# Copyright (C) 1996
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


#include <aips/OS/Directory.h>
#include <aips/OS/RegularFile.h>
#include <aips/OS/SymLink.h>
#include <aips/Utilities/String.h>
#include <aips/Utilities/Assert.h>
#include <aips/Exceptions.h>
#include <aips/Arrays/Vector.h>
#include <aips/Utilities/GenSort.h>
#include <iostream.h>


// <summary>
// Test program for class Directory.
// </summary>

// This program tests the class Directory.
// The results are written to stdout. The script executing this program,
// compares the results with the reference output file.
// <p>
// When an argument is given, no exceptions will be thrown.
// This can be used to check if no memory leaks occur in normal operation.


void doIt (Bool doExcp)
{
    // Test the constructors.
    Directory tmp(Path("tDirectory_tmp/"));
    Directory test1("tDirectory_tmp/test1");
    Directory test2("tDirectory_tmp/test2");
    Directory linkDir(File("tDirectory_tmp/linkDir"));
    Directory linkDir2 (linkDir);
    Directory linkDir3 (linkDir);
    AlwaysAssertExit (linkDir.isDirectory());
    AlwaysAssertExit (linkDir2.isDirectory());
    AlwaysAssertExit (linkDir3.isDirectory());
    AlwaysAssertExit (linkDir3.path().expandedName() ==
		                                "tDirectory_tmp/linkDir");

    {
        // Directory::find
        Vector<String> found = tmp.find (Regex::fromString("test1"));
	genSort (found.ac());
	cout << found.ac() << endl;
        Vector<String> found1 = tmp.find (Regex("test[12]"), True);
	genSort (found1.ac());
	cout << found1.ac() << endl;
        Vector<String> found2 = tmp.find (Regex(".*"), True);
	genSort (found2.ac());
	cout << found2.ac() << endl;
    }

    AlwaysAssertExit (! tmp.isEmpty());
    AlwaysAssertExit (tmp.nEntries() == 6);

    // Construct and create a new directory.
    Directory newDir ("tDirectory_tmp/newDir");
    tmp.setPermissions (0555);
    if (doExcp) {
	try {
	    newDir.create();
	} catch (AipsError x) {
	    cout << x.getMesg() << endl;               // not writable
	} end_try;
    }
    tmp.setPermissions (0755);
    AlwaysAssertExit (! newDir.isDirectory());
    AlwaysAssertExit (! newDir.exists());
    newDir.create();
    AlwaysAssertExit (newDir.isDirectory());
    AlwaysAssertExit (newDir.exists());
    AlwaysAssertExit (newDir.isEmpty());
    AlwaysAssertExit (newDir.nEntries() == 0);

    // Some erronous constructs.
    if (doExcp) {
	try {
	    Directory file1("tDirectory_tmp/test1/testLink2");
	} catch (AipsError x) {
	    cout << x.getMesg() << endl;               // symlink, no directory
	} end_try;
	try {
	    Directory file1("tDirectory_tmp/test1/testFile2");
	} catch (AipsError x) {
	    cout << x.getMesg() << endl;               // symlink, no directory
	} end_try;
    }
    tmp.setPermissions (0555);
    if (doExcp) {
	try {
	    Directory file1("tDirectory_tmp/something");
	} catch (AipsError x) {
	    cout << x.getMesg() << endl;               // not writable
	} end_try;
    }
    tmp.setPermissions (0755);

    // Copy a directory tree to a new directory.
    test1.copy ("tDirectory_tmp/test3");
    Directory test3 ("tDirectory_tmp/test3");
    Directory test3dir ("tDirectory_tmp/test3/isDir1");
    AlwaysAssertExit (test3.isDirectory());
    AlwaysAssertExit (test3.nEntries() == 6);
    AlwaysAssertExit (test3dir.isDirectory());
    AlwaysAssertExit (test3dir.nEntries() == 3);

    // Copy a directory tree to a existing directory.
    AlwaysAssertExit (test2.isDirectory());
    AlwaysAssertExit (test2.nEntries() == 1);
    test1.copy ("tDirectory_tmp/test2");
    AlwaysAssertExit (test2.isDirectory());
    AlwaysAssertExit (test2.nEntries() == 6);
    AlwaysAssertExit (test1.isDirectory());
    AlwaysAssertExit (test1.nEntries() == 6);

    // Remove files and directory.
    test3dir.removeFiles();
    AlwaysAssertExit (test3dir.isDirectory());
    AlwaysAssertExit (test3dir.nEntries() == 0);
    test3dir.remove();
    AlwaysAssertExit (!test3dir.exists());
    AlwaysAssertExit (test3.nEntries() == 5);
    // Do an erronous remove.
    if (doExcp) {
	try {
	    test3.remove();
	} catch (AipsError x) {
	    cout << x.getMesg() << endl;               // not empty
	} end_try;
    }
    test3.removeRecursive();
    AlwaysAssertExit (!test3.exists());

    // Move a directory tree to a new directory.
    test1.move ("tDirectory_tmp/test3");
    AlwaysAssertExit (test3.isDirectory());
    AlwaysAssertExit (test3.nEntries() == 6);
    AlwaysAssertExit (test3dir.isDirectory());
    AlwaysAssertExit (test3dir.nEntries() == 3);
    AlwaysAssertExit (!test1.exists());

    // Move a directory tree to an existing directory.
    Directory test5 ("tDirectory_tmp/test5");
    AlwaysAssertExit (test5.isDirectory());
    AlwaysAssertExit (test5.nEntries() == 1);
    test2.move ("tDirectory_tmp/test5");
    AlwaysAssertExit (test5.isDirectory());
    AlwaysAssertExit (test5.nEntries() == 6);
    AlwaysAssertExit (!test2.exists());

    // Move a directory tree across a file system and back.
    Directory testtmp ("/tmp/test5");
    test5.move ("/tmp/test5");
    AlwaysAssertExit (testtmp.isDirectory());
    AlwaysAssertExit (testtmp.nEntries() == 6);
    AlwaysAssertExit (!test5.exists());
    testtmp.move ("tDirectory_tmp/test5");
    AlwaysAssertExit (test5.isDirectory());
    AlwaysAssertExit (test5.nEntries() == 6);
    AlwaysAssertExit (!testtmp.exists());

    // Move to the same name should succeed.
    test5.move ("tDirectory_tmp/test5");
    AlwaysAssertExit (test5.isDirectory());
    AlwaysAssertExit (test5.nEntries() == 6);

    // Test directory creation.
    Directory test6;
    test6 = test5;
    AlwaysAssertExit (test6.nEntries() == 6);
    if (doExcp) {
	try {
	    test6.create (False);
	} catch (AipsError x) {
	    cout << x.getMesg() << endl;               // already existing
	} end_try;
    }
    test6.create (True);
    AlwaysAssertExit (test6.isEmpty());

    Directory test7("tDirectory_tmp/newDir2");
    RegularFile rfile("tDirectory_tmp/newDir2");
    rfile.create();
    if (doExcp) {
	try {
	    test7.create (False);
	} catch (AipsError x) {
	    cout << x.getMesg() << endl;               // already existing
	} end_try;
    }
    rfile.remove();
    test7.create (False);
    AlwaysAssertExit (test7.isEmpty());

    // Remove a directory via a symlink (which will be removed too).
    SymLink slink("tDirectory_tmp/newDir2Link");
    slink.create ("newDir2");
    Directory test8("tDirectory_tmp/newDir2Link");
    AlwaysAssertExit (test8.isEmpty());
    AlwaysAssertExit (test7.exists());
    AlwaysAssertExit (slink.exists());

    // Test the freeSpace function.
    cout << ">>>" << endl;
    cout << "Free Space: " << test7.freeSpace() << ' '
	 << test8.freeSpace() << endl;
    cout << "<<<" << endl;

    test8.remove();
    AlwaysAssertExit (!test7.exists());
    AlwaysAssertExit (!slink.exists());
}


main (int argc)
{
    try {
	doIt (ToBool (argc<2));
    } catch (AipsError x) {
	cout << "Caught an exception: " << x.getMesg() << endl;
	cout << "at line " << x.thrownLine() 
	     << " in " << x.thrownFile() << endl;
	return 1;
    } end_try;
    cout << "OK" << endl;
    return 0;                           // exit with success status
}
