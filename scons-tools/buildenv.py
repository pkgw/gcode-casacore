import os

def CasaBuilder(env, target=None, source="*.cc", installer=None):
    if target is None:
        target = os.path.split(os.path.abspath("."))[-1]
    myenv = env.Clone()

    # get all the sourcefiles recursively
    lls = myenv.SGlob("*.ll", recursive=True )
    for f in lls:
        myenv.CXXFile(f, CXXFILESUFFIX=".lcc")
        path = os.path.split(f)[0]
        myenv.AppendUnique(CPPPATH=[Dir(path)])

    yys = myenv.SGlob("*.yy", recursive=True )
    for f in yys:
        myenv.CXXFile(f, CXXFILESUFFIX=".ycc")

    cpps = myenv.SGlob(source, excludedirs=["test"], recursive=True )

    libname = "casa_%s" % target
    lib = os.path.join(str(myenv["BUILDDIR"]), libname)

    # dependencies ...
    myenv.PrependUnique(LIBS=myenv.GetDependencies(libname))

    if not myenv.GetOption("disable_static"):
        slib =  myenv.StaticLibrary(target = lib, source = [cpps])
        myenv.Alias(target, slib)
        installer.AddLibrary(slib)

    if myenv.GetOption("enable_shared"):
        dlib =  myenv.SharedLibrary(target = lib, source = [cpps])
        myenv.Alias(target, dlib)
        installer.AddLibrary(dlib)
    myenv.Default(target)

    rootdir = myenv.Dir("#").abspath
    # install headers, only works with absolute dir.
    installer.AddHeaders( rootdir+"/%s" % target, "*.h", "casacore/%s" % target, True )
    installer.AddHeaders( rootdir+"/%s" % target , "*.tcc", "casacore/%s" % target, True )
    

def generate(env):
    def BuildEnv(buildtype):
	lenv = env.Clone()
	lenv["build"] = buildtype
	if buildtype == "dbg":
	    lenv.AppendUnique(CPPFLAGS=["-g"])
	elif buildtype == "opt":
	    lenv.AppendUnique(CPPFLAGS=["-O2"])
	    lenv.AppendUnique(FORTRANFLAGS=["-O2"])	    
	return lenv
    env.BuildEnv = BuildEnv
    env.AddMethod(CasaBuilder)


def exists(env):
    return 1
