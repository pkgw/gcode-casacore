import sys, os
from  SCons.Variables import ListVariable, EnumVariable
from SCons.Script import AddOption


def generate(env):

    def AddPkgOptions(libid, root=None, lib=None, libdir=None, 
                      incdir=None):
        add_str_option(libid+"-lib", lib or libid)
        add_str_option(libid+"-root", root)
        add_str_option(libid+"-incdir", incdir)
        add_str_option(libid+"-libdir", libdir)

    def add_str_option(optname, default=None):
        envopt = optname.replace("-", "_")
        AddOption("--%s" % optname, dest=envopt, type="string",
                  default=default)
        clopt = env.GetOption(envopt)
#        print optname, clopt, type(clopt)
        env[envopt] = clopt

    def AddCommandLineOptions( ):
        """ Adds the build environment options to the opts.  """
        options = ["cppflags", "cxxflags", "cflags", "linkflags", "fflags",
                   "includedir", "librarydir", "ldlibrarypath"]
        for opt in options:
            exopt = "extra-%s" % opt
            add_str_option(exopt)
        add_str_option("buildtype", "opt")
        add_str_option("f2clib")

        if sys.platform == 'darwin':
            add_str_option("universal") # ppc i386 ppc64 x86_64

        compilers = ["CC", "CXX", "FORTRAN"]
        for comp in compilers:
            c = os.environ.get(comp)
            if c:
                env[comp] = c
    env.AddPkgOptions = AddPkgOptions
    AddCommandLineOptions()

def exists(env):
    return 1
