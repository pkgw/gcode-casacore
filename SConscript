# import root environment
Import( ["buildenv", "installer"])
print buildenv["BUILDDIR"].abspath
buildenv.SConscript(dirs=["casa","tables"], 
               exports=["buildenv", "installer"])
