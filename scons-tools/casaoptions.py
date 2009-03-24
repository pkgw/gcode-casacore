import sys, os
from  SCons.Variables import ListVariable, EnumVariable
from SCons.Script import AddOption


def generate(env):
    def add_str_option(optname, default=None):
        AddOption("--%s" % optname, dest=optname, type="string",
                  default=default)
        clopt = env.GetOption(optname)
        if clopt:
            envopt = optname.replace("-", "")
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

    AddCommandLineOptions()

def exists(env):
    return 1
