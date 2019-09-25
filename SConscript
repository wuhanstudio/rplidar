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

if GetDepend('RPLIDAR_USING_HEALTH_EXAMPLE'):
	src    += Glob('examples/rplidar_health_example.c')

LOCAL_CCFLAGS = ''

group = DefineGroup('rplidar', src, depend = ['PKG_USING_RPLIDAR'], CPPPATH = path, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')
