import os
import sys

EnsureSConsVersion(1,1)

# general options

AddOption("--enable-hdf5", dest="enable_hdf5",
          action="store_true", default=False)
AddOption("--hdf5-root", dest="hdf5root",
          type="string", default="/usr")
#opts.Add(("--enable_hdf5", "Enable HDF5 support", False))
#opts.Add(("--hdf5-root", 
#          "The root directory where hdf5 is installed", None))
#opts.Add(("--hdf5-incdir", "The hdf5 header location", None))
#opts.Add(("--hdf5-libdir", "The hdf5 library location", None))

env = Environment(ENV = { 'PATH' : os.environ[ 'PATH' ],
			  'HOME' : os.environ[ 'HOME' ] 
			  },
                  tools = ["default", "casaoptions", "buildenv", "casa", "utils",
                           "assaytest", "installer"], toolpath = ["scons-tools"],
                  casashrdir=["scons-tools"],
		  )
# keep a local sconsign database, rather than in very directory
env.SConsignFile()

env["casashrdir"] = ["./scons-tools"]
env["casalibdir"] = None
env["casaincdir"] = None

env["build"]=["opt"]


if not (env.Detect(["flex","lex"]) and env.Detect(["bison", "yacc"])):
    print "lex/yacc needs to be installed"
    env.Exit(1)

# Auto configure
hdf5c = env.File("casa/HDF5Config.h")
if not env.GetOption('clean'):
    conf = Configure(env, config_h=hdf5c)
    conf.env.AddCustomPackage('hdf5')
    if conf.env.get("enable-hdf5") and conf.CheckLib('hdf5', autoadd=0):
        conf.env.PrependUnique(LIBS=['hdf5'])
    else:
        print "Building without HDF5 support"
    env = conf.Finish()
else:
    env.Execute(Delete("options.cfg"))
    env.Execute(Delete(hdf5c))
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
    env.SConscript(["%s/SConscript" % "casa"], 
		   build_dir= buildenv["BUILDDIR"],
		   duplicate=0, exports=["buildenv", "installer"])

# add the Tools to the casacore/share directory. This way they can be imported
# by the other casacore packages without having to duplicate them.
installer.AddShares("scons-tools", "*.py", "casacore/", True)
installer.AddShares("scons-tools", "casacore_assay", "casacore/")
installer.AddShares("scons-tools", "floatcheck.sh", "casacore/")
installer.AddHeader( str(hdf5c), "casacore/casa")

print env.get("disablestatic")
