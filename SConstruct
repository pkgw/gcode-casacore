import os
import sys

EnsureSConsVersion(1,1)

# general options

def add_pkg_options(libid, root="/usr", lib=None, libdir=None, incdir=None):
    AddOption("--%s-root" % libid, dest="%s_root" % libid,
              type="string", default=root)
    AddOption("--%s-incdir" % libid, dest="%s_incdir" % libid,
              type="string", default=incdir or os.path.join(root, "include"))
    AddOption("--%s-libdir" % libid, dest="%s_libdir" % libid,
              type="string", default=libdir or os.path.join(root, "lib"))
#    AddOption("--%s-lib" % libid, dest="%s_lib"  % libid,
#              type="string", default=lib or libid, action="store")

    

env = Environment(ENV = { 'PATH' : os.environ[ 'PATH' ],
			  'HOME' : os.environ[ 'HOME' ] 
			  },
                  tools = ["default", "casaoptions", "buildenv", "casa", "utils",
                           "assaytest", "installer"], toolpath = ["scons-tools"],
                  casashrdir=["scons-tools"],
		  )
# keep a local sconsign database, rather than in very directory
env.SConsignFile()

AddOption("--enable-hdf5", dest="enable_hdf5",
          action="store_true", default=False)
add_pkg_options("hdf5")
AddOption("--hdf5-lib" , dest="hdf5_lib",
          type="string", action="store")

AddOption("--disable-dl", dest="disable_dl",
          action="store_true", default=False)

print "DDDD",GetOption("hdf5_lib")

env["casashrdir"] = ["./scons-tools"]
env["casalibdir"] = None
env["casaincdir"] = None

env["build"]=["opt"]


if not (env.Detect(["flex","lex"]) and env.Detect(["bison", "yacc"])):
    print "lex/yacc needs to be installed"
    env.Exit(1)

# Auto configure
if not env.GetOption('clean'):
    conf = Configure(env)
    pkgname = "hdf5"
    libname = env.GetOption(pkgname+"_lib")
    print pkgname, libname
    conf.env.AddCustomPackage(pkgname)
    if conf.env.GetOption("enable_hdf5") and \
           conf.CheckLib(libname, autoadd=0):
        conf.env.PrependUnique(LIBS=[libname])
        conf.env.Append(CPPFLAGS=['-DHAVE_LIBHDF5'])
    else:
        print "Building without HDF5 support"
    if not conf.env.GetOption("disable_dl") and conf.CheckLib('dl', autoadd=0):    
        conf.env.AppendUnique(LIBS=['dl'])
        conf.env.Append(CPPFLAGS=['-DHAVE_DLOPEN'])
    else:
        print "Building without dlopen support"
    env = conf.Finish()
else:
    env.Execute(Delete("options.cfg"))
# create the installer which handles installing the final build
installer = env.Installer()

# to find package based includes
env.Append(CPPPATH='#')

# Replace some builder commands (lex,yacc) with custom versions
env.CustomCasaCom()

for bopt in env["build"]:
    # create an environment copy with the dbg/opt compiler flags
    buildenv = env.BuildEnv(bopt)
    # buildir name
    buildenv["BUILDDIR"] = Dir("#/build_%s/%s" % (env.PlatformIdent(), bopt))
    env.SConscript("SConscript" , 
		   build_dir= buildenv["BUILDDIR"],
		   duplicate=0, exports=["buildenv", "installer"])

# add the Tools to the casacore/share directory. This way they can be imported
# by the other casacore packages without having to duplicate them.
installer.AddShares("scons-tools", "*.py", "casacore/", True)
installer.AddShares("scons-tools", "casacore_assay", "casacore/")
installer.AddShares("scons-tools", "floatcheck.sh", "casacore/")
