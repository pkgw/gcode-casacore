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
        env[envopt] = clopt

    def add_comp_option(optname):
        AddOption("--with-%s" % optname.lower(), dest=optname,
                  type="string", default=None)
        clopt = env.GetOption(optname)
        if clopt:
            env[optname] = clopt        

    def AddCommandLineOptions( ):
        """ Adds the build environment options to the opts.  """
        options = ["cppflags", "cxxflags", "cflags", "linkflags", "fflags",
                   "includedir", "librarydir", "ldlibrarypath", "path" ]
        for opt in options:
            exopt = "extra-%s" % opt
            add_str_option(exopt)
        options = ["CC", "CXX", "FORTRAN"]
        for opt in options:
            add_comp_option(opt)

        if sys.platform == 'darwin':
            add_str_option("universal") # ppc i386 ppc64 x86_64

    env.AddPkgOptions = AddPkgOptions
    AddCommandLineOptions()

def exists(env):
    return 1
