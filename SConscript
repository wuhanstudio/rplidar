from building import *
import rtconfig

cwd     = GetCurrentDir()

src     = Glob('sdk/*.c')
src    += Glob('drv/*.c')
src    += Glob('src/*.c')

path    = [cwd]
path   += [cwd + '/sdk']
path   += [cwd + '/src']
path   += [cwd + '/driver']

LOCAL_CCFLAGS = ''

group = DefineGroup('rplidar', src, depend = [''], CPPPATH = path, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')
