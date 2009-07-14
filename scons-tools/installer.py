""" installer

This module defines a minimal installer for scons build scripts.  It is aimed
at *nix like systems, but I guess it could easily be adapted to other ones as
well.
"""

import fnmatch, os, os.path
import SCons.Defaults
from SCons.Script import AddOption

PREFIX = "prefix"
EPREFIX = "eprefix"
BINDIR = "bindir"
LIBDIR = "libdir"
INCLUDEDIR = "includedir"
SHAREDIR = "sharedir"

def AddOptions():
    """ Adds the installer command-line options"""
    defdir = "/usr/local"
    
    AddOption("--"+PREFIX, dest=PREFIX,
              type="string", default=defdir, 
              help="The installation prefix (default: %s)" % defdir)
    AddOption("--"+EPREFIX, dest=EPREFIX,
              type="string", default=defdir)
    AddOption("--"+BINDIR, dest=BINDIR,
              type="string", default=None, 
              help="The installation bin directory (default: %s/bin)" % defdir)
    AddOption("--"+LIBDIR, dest=LIBDIR,
              type="string", default=None, 
              help="The installation lib directory (default: %s/lib)" % defdir)
    AddOption("--"+INCLUDEDIR, dest=INCLUDEDIR,
              type="string", default=None, 
              help="The installation include directory (default: %s/include)" % defdir)
    AddOption("--"+SHAREDIR, dest=SHAREDIR,
              type="string", default=None, 
              help="The installation share directory (default: %s/share)" % defdir)

def generate(env):
    class Installer:
        """ A basic installer. """
        def __init__( self):
            """ Initialize the installer.
            
            @param configuration A dictionary containing the configuration.
            @param env The installation environment.
            """
            self._prefix = env.GetOption( PREFIX )
            self._eprefix = env.GetOption( EPREFIX )
            self._bindir = env.GetOption( BINDIR ) \
                           or os.path.join(self._prefix, "bin")
            self._libdir = env.GetOption( LIBDIR ) \
                           or os.path.join(self._prefix, "lib")
            self._includedir = env.GetOption( INCLUDEDIR ) \
                           or os.path.join(self._prefix, "include")
            self._sharedir = env.GetOption( SHAREDIR ) \
                           or os.path.join(self._prefix, "share")
            env.Alias( "install", env.Dir(self._bindir) )
            env.Alias( "install", env.Dir(self._libdir ) )
            env.Alias( "install", env.Dir(self._includedir ) )
            env.Alias( "install", env.Dir(self._sharedir ) )
            self._env = env
	
        def Add( self, destdir, name, basedir="", perm=None ):
            destination = os.path.join( destdir, basedir )
            obj = self._env.Install( destination, name )
            for i in obj:
                if perm:
                    self._env.AddPostAction(i,
                                            SCons.Defaults.Chmod(str(i),
                                                                 perm))
	
        def AddProgram( self, program ):
	        """ Install a program.
                
	        @param program The program to install.
	        """
	        self.Add( self._bindir, program, perm=0755 )
                
        def AddLibrary( self, library ):
	        """ Install a library.
	
	        @param library the library to install.
	        """
	        self.Add( self._libdir, library )

        def AddShare( self, header, basedir="" ):
	        self.Add( self._sharedir, header, basedir )

        def AddShares( self, parent , pattern, basedir="", recursive=False):
	        for entry in os.listdir( parent ):
	            entrypath = os.path.join( parent, entry )
	            if os.path.isfile( entrypath ) and  \
			   fnmatch.fnmatch( entry, pattern ):
	                self.AddShare( entrypath, basedir )
	            elif os.path.isdir( entrypath ) and recursive:
	                self.AddShares( entrypath, pattern,
					 os.path.join( basedir, entry ),
					 recursive )


        def AddHeader( self, header, basedir="" ):
	        self.Add( self._includedir, header, basedir )
	
        def AddHeaders( self, parent, pattern, basedir="", recursive=False, 
                        exclude_tests=True ):
	        """ Installs a set of headers.
	
	        @param parent The parent directory of the headers.
	        @param pattern A pattern to identify the files that
                       are headers.
	        @param basedir The subdirectory in which to install the
                       headers.
	        @param recursive Search recursively for headers.
	        """
                if parent.find("test") > -1:
                    return
	        for entry in os.listdir( parent ):
	            entrypath = os.path.join( parent, entry )
	            if os.path.isfile( entrypath ) and  \
			   fnmatch.fnmatch( entry, pattern ):
	                self.AddHeader( entrypath, basedir )
	            elif os.path.isdir( entrypath ) and recursive:
	                self.AddHeaders( entrypath, pattern,
					 os.path.join( basedir, entry ),
					 recursive )
    env.Installer = Installer
    AddOptions()

def exists(env):
    return True
