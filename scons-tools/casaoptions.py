import sys, os
from  SCons.Variables import ListVariable, EnumVariable
from SCons.Script import AddOption


def generate(env):

    def AddPkgOptions(libid, root=None, lib=None, libdir=None, 
                      incdir=None, help=None):
        libname = lib or libid
        add_str_option(libid+"-lib", libname,
                       help="%s library name (default: %s)" % (libname, libname))
        add_str_option(libid+"-root", root,
                       help="%s package root" % libid)
        add_str_option(libid+"-incdir", incdir,
                       help="%s package 'include' directory (overwrites '-root')" % libid)
        add_str_option(libid+"-libdir", libdir,
                       help="%s package 'lib' directory (overwrites '-root')" \
                       % libid)

    def add_str_option(optname, default=None, help=None):
        envopt = optname.replace("-", "_")
        AddOption("--%s" % optname, dest=envopt, type="string",
                  default=default, help=help)
        clopt = env.GetOption(envopt)
        env[envopt] = clopt

    def add_comp_option(optname, default=None, help=None):
        AddOption("--with-%s" % optname.lower(), dest=optname,
                  type="string", default=default, help=help)
        clopt = env.GetOption(optname)
        if clopt:
            env[optname] = clopt        

    def AddCommandLineOptions( ):
        """ Adds the build environment options to the opts.  """
        options = [("extra-cppflags", None, "Extra pre-processor flags"),
                   ("extra-cxxflags", None, "Extra c++ compiler falgs"),
                   ("extra-cflags", None, "Extra c compiler flags"),
                   ("extra-linkflags", None, "Extra linker flags"),
                   ("extra-fflags", None, "Extra fortans compiler flags"),
                   ("extra-includedir", None, "Extra 'include' dir(s)"),
                   ("extra-librarydir", None, "Extra 'lib' dir(s)"),
                   ("extra-ldlibrarypath", None, "Extra (DY)LD_LIBRARY_PATH"),
                   ("extra-path", None, "Extra PATH (bin) to search")]
        for opt in options:
            add_str_option(*opt)
        options = [("CC", None, "The c compiler"),
                   ("CXX", None, "The c++ compiler"),
                   ("FORTRAN", None, "The fortran compiler"),]
#                   ("LD", None, "The linker")]
        for opt in options:
            add_comp_option(*opt)

        opt = ("universal", None, 
               "Create universal build using any of: ppc,i386,ppc64,x86_64")
        if sys.platform == 'darwin':
            add_str_option(*opt) # ppc i386 ppc64 x86_64

    env.AddPkgOptions = AddPkgOptions
    AddCommandLineOptions()

def exists(env):
    return 1
