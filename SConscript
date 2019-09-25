from building import *
import rtconfig

cwd     = GetCurrentDir()

src     = Glob('sdk/*.c')
src    += Glob('drv/*.c')
src    += Glob('src/*.c')
src    += Glob('examples/*.c')

path    = [cwd]
path   += [cwd + '/sdk']
path   += [cwd + '/src']
path   += [cwd + '/driver']
path   += [cwd + 'examples/driver']

LOCAL_CCFLAGS = ''

group = DefineGroup('rplidar', src, depend = ['PKG_USING_RPLIDAR'], CPPPATH = path, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')
