//# PagedImage.h: read, store and manipulate astronomical images
//# Copyright (C) 1994,1995,1996,1997,1998,1999
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

#if !defined(AIPS_PAGEDIMAGE_H)
#define AIPS_PAGEDIMAGE_H


//# Includes
#include <aips/aips.h>
#include <trial/Images/ImageInterface.h>
#include <trial/Lattices/PagedArray.h>
#include <aips/Tables/Table.h>
#include <aips/Utilities/DataType.h>
#include <aips/Tables/TableRecord.h>
#include <aips/Logging/LogIO.h>

//# Forward Declarations
class IPosition;
class LatticeNavigator;
class Slicer;
class LogTable;
class ImageRegion;
template <class T> class Array;
template <class T> class LatticeIterInterface;
class String;
#if defined(AIPS_STDLIB)
#include <iosfwd.h>
#else
class ostream;
#endif


// <summary>
// Read, store, and manipulate astronomical images.
// </summary>

// <use visibility=export>

// <reviewed reviewer="" date="" tests="tPagedmage.cc" demos="dPagedImage.cc">
// </reviewed>

// <prerequisite>
// <list>
//   <item> <linkto class=CoordinateSystem>CoordinateSystem</linkto>
//   <item> <linkto class=ImageInterface>ImageInterface</linkto>
//   <item> <linkto class=Lattice>Lattice</linkto>
//   <item> <linkto class=LatticeIterator>LatticeIterator</linkto>
//   <item> <linkto class=LatticeNavigator>LatticeNavigator</linkto>
// </list>
// </prerequisite>

// <etymology>
// The PagedImage name comes from its role as the Image class with paging 
// from persistent memory.  Users are thus invited to treat the 
// PagedImage instances like AIPS++ Lattices  
// </etymology>

// <synopsis> 
// All AIPS++ Images are Lattices.  They may be treated like any other Lattice;
// getSlice(...), putSlice(...), LatticeIterator for iterating, etc...
// ArrayImages contain a map, a mask for that map, and coordinate 
// information.  This provides a Lattice interface for images and their 
// respective coordinates.  Additional functionality is defined by the 
// ImageInterface class. 
//
// You can use the global function <src>imagePixelType</src> to determine
// what the pixel type of an image is before you open the image if your
// code can work with Images of many possible types, or for error checking.
// </synopsis> 

// <example>
// <srcblock>
// </srcblock>
// </example>

// <motivation>
// The size of astronomical data can be very large.  The ability to fit an 
// entire image into random access memory cannot be guaranteed.  Paging from 
// disk pieces of the image appeared to be the way to deal with this problem.
// </motivation>

// <todo asof="1996/09/04">
//   <li> The CoordinateSystem::store() function returns a TableRecord.  That
// TableRecord should be stored in the same row as our image.  This will 
// allow ImageStack members to have their own coordinate frames.
// </todo>


template <class T> class PagedImage: public ImageInterface<T>
{
public: 
  // Construct a new Image from shape and coordinate information.
  // Data will be stored in the argument table.
  PagedImage (const TiledShape& mapShape,
	      const CoordinateSystem& coordinateInfo,
	      Table& table,
	      uInt rowNumber = 0);
  
  // Construct a new Image from shape and coordinate information. Table
  // will be stored in the named file.
  PagedImage (const TiledShape& mapShape,
	      const CoordinateSystem& coordinateInfo,
	      const String& nameOfNewFile,
	      uInt rowNumber = 0);
  
  // Construct a new Image from shape and coordinate information. Table
  // will be stored in the named file.
  // The lock options may be specified
  // <group>
  PagedImage (const TiledShape& mapShape,
	      const CoordinateSystem& coordinateInfo,
	      const String& nameOfNewFile,
	      TableLock::LockOption,
	      uInt rowNumber = 0);
  PagedImage (const TiledShape& mapShape,
	      const CoordinateSystem& coordinateInfo,
	      const String& nameOfNewFile,
	      const TableLock& lockOptions,
	      uInt rowNumber = 0);
  // <group>
  
  // Reconstruct an image from a pre-existing file.
  // Use the default mask if the flag is True.
  PagedImage (Table& table, Bool useDefaultMask = True, uInt rowNumber = 0);
  
  // Reconstruct an image from a pre-existing file.
  // Use the default mask if the flag is True.
  PagedImage (const String& filename, Bool useDefaultMask = True,
	      uInt rowNumber = 0);
  
  // Reconstruct an image from a pre-existing file with Locking.
  // Use the default mask if the flag is True.
  // <group>
  PagedImage (const String& filename, TableLock::LockOption,
	      Bool useDefaultMask = True, uInt rowNumber = 0);
  PagedImage (const String& filename, const TableLock& lockOptions,
	      Bool useDefaultMask = True, uInt rowNumber = 0);
  // </group>
  
  // Copy constructor (reference semantics).
  PagedImage (const PagedImage<T>& other);

  ~PagedImage();
  
  // Assignment operator (reference semantics).
  PagedImage<T>& operator= (const PagedImage<T>& other);
  
  // Make a copy of the object (reference semantics).
  virtual ImageInterface<T>* cloneII() const;

  // A PagedImage is always paged to disk.
  virtual Bool isPaged() const;

  // Is the PagedImage writable?
  virtual Bool isWritable() const;

  // Get a pointer the default region/mask object used with this image.
  // It returns 0 if no default mask is used.
  virtual const LatticeRegion* getRegionPtr() const;

  // Set the default mask to the region/mask with the given name.
  // If the image table is writable, the setting is persistent by writing
  // the name as a keyword.
  // If the given regionName is the empty string, the default mask is unset.
  virtual void setDefaultMask (const String& regionName);

  // Get the name of the default mask.
  // An empty string is returned if no default mask.
  virtual String getDefaultMask() const;

  // Function to change the name of the Table file on disk.
  // PagedImage is given a file name at construction time.  You may change
  // that name here.
  void rename (const String& newName);

  // Return the current Table name. By default this includes the full path. 
  // the path preceding the file name can be stripped off on request.
  virtual String name (const Bool stripPath=False) const;

  // Return the current TableColumn row number.
  uInt rowNumber() const;

  // Return the shape of the image.
  virtual IPosition shape() const;

  // Change the shape of the image (N.B. the data is thrown away).
  virtual void resize (const TiledShape& newShape);

  // Function which extracts an array from the map.
  virtual Bool doGetSlice (Array<T>& buffer, const Slicer& theSlice);
  
  // Function to replace the values in the map with soureBuffer.
  virtual void doPutSlice (const Array<T>& sourceBuffer,
			   const IPosition& where,
			   const IPosition& stride);

  // Replace every element, x, of the lattice with the result of f(x).
  // you must pass in the address of the function -- so the function
  // must be declared and defined in the scope of your program.  
  // Both versions of apply require a function that accepts a single 
  // argument of type T (the Lattice template actual type) and returns
  // a result of the same type.  The first apply expects a function with
  // an argument passed by value; the second expects the argument to
  // be passed by const reference.  The first form ought to run faster
  // for the built-in types, which may be an issue for large Lattices
  // stored in memory, where disk access is not an issue.
  // <group>
  virtual void apply (T (*function)(T));
  virtual void apply (T (*function)(const T& ));
  virtual void apply (const Functional<T,T>& function);
  // </group>

  // Add a lattice to this image.
  PagedImage<T>& operator+= (const Lattice<T>& other);

  // Function which get and set the units associated with the image
  // pixels (i.e. the "brightness" unit). <src>setUnits()</src> returns
  // False if it cannot set the unit for some reason (e.g. the underlying
  // file is not writable).
  // <group>
  virtual Bool setUnits (const Unit& newUnits);
  virtual Unit units() const;
  // </group>

  // Return the table holding the data.
  Table table();

  // Flushes the new coordinate system to disk if the table is writable.
  virtual Bool setCoordinateInfo (const CoordinateSystem& coords);

  // Check for symmetry in data members.
  virtual Bool ok() const;

  // These are the true implementations of the paran operator.
  // <note> Not for public use </note>
  // <group>
  virtual T getAt (const IPosition& where) const;
  virtual void putAt (const T& value, const IPosition& where);
  // </group>

  // Often we have miscellaneous information we want to attach to an image.
  // This is how it is done. Eventually we will want to register that some
  // of the information is to be destroyed if the image changes so that, e.g.
  // data max/min values can be removed if the image changes.
  // <br>
  // Note that setMiscInfo REPLACES the information with the new information.
  // It can fail if, e.g., the underlying table is not writable.
  // <group>
  virtual const RecordInterface& miscInfo() const;
  virtual Bool setMiscInfo (const RecordInterface& newInfo);
  // </group>

  // Define a region belonging to the image.
  // If overwrite=False, an exception will be thrown if the region
  // already exists.
  virtual void defineRegion (const String& name, const ImageRegion& region,
			     Bool overwrite = False);

  // Get a region belonging to the image.
  // A zero pointer is returned if the region does not exist.
  // The caller has to delete the <src>ImageRegion</src> object created.
  virtual ImageRegion* getImageRegionPtr (const String& name) const;

  // Remove a region belonging to the image.
  // No exception is thrown if the region does not exist.
  virtual void removeRegion (const String& name);
  
  // This is the implementation of the letter for the envelope Iterator
  // class. <note> Not for public use </note>.
  virtual LatticeIterInterface<T>* makeIter
                                 (const LatticeNavigator& navigator) const;

  // Returns the maximum recommended number of pixels for a cursor. This is
  // the number of pixels in a tile. 
  virtual uInt maxPixels() const;

  // Help the user pick a cursor for most efficient access.
  virtual IPosition doNiceCursorShape (uInt maxPixels) const;

  // Maximum size - not necessarily all used. In pixels.
  uInt maximumCacheSize() const;

  // Set the maximum (allowed) cache size as indicated.
  void setMaximumCacheSize (uInt howManyPixels);

  // Set the cache size as to "fit" the indicated path.
  void setCacheSizeFromPath (const IPosition& sliceShape,
			     const IPosition& windowStart,
			     const IPosition& windowLength,
			     const IPosition& axisPath);
    
  // Clears and frees up the caches, but the maximum allowed cache size is 
  // unchanged from when setCacheSize was called
  void clearCache();

  // Report on cache success.
  void showCacheStatistics (ostream& os) const;

  // Handle the (un)locking.
  // <group>
  virtual Bool lock (FileLocker::LockType, uInt nattempts);
  virtual void unlock();
  virtual Bool hasLock (FileLocker::LockType) const;
  // </group>

private:  
  // the default constructor -- useless.
  PagedImage();
  // This must be called in every constructor and place where the image
  // is attached to a new image.
  void attach_logtable();
  void restore_units();
  void save_units();
  void check_conformance (const Lattice<T>& other);
  void reopenRW();
  void doReopenRW();
  void setTableType();
  void applyDefaultMask();
  void makeRegion (const String& regionName);
  void makePagedImage (const TiledShape& mapShape,
		       const CoordinateSystem& coordinateInfo,
		       const String& nameOfNewFile,
		       const TableLock& lockOptions,
		       uInt rowNumber);
  void makePagedImage (const String& filename, const TableLock& lockOptions,
		       Bool useDefaultMask, uInt rowNumber);

  Table table_p;
  PagedArray<T> map_p;
  TableLogSink* logTablePtr_p;
  LatticeRegion* regionPtr_p;
};


//# A nasty - the column name is hard-coded into this function, needs to
//# be centralized somewhere.
// <linkfrom anchor="pixeltype" classes="PagedImage">
// <here>Global functions</here> for PagedImage.
// </linkfrom>
// Determine the pixel type in the PagedImage contained in
// <src>fileName</src>.  If the file doesn't appear to be a Table or cannot
// be opened, TpOther is returned.
// <group name="pixeltype")
    DataType imagePixelType(const String& fileName);
// </group>


template<class T>
inline void PagedImage<T>::reopenRW()
{
  //# Open for write if not done yet and if writable.
  if (!table_p.isWritable()  &&  isWritable()) {
    doReopenRW();
  }
}


#endif
