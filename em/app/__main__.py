import os
os.environ['KIVY_NO_ARGS'] = '1'
os.environ['KIVY_NO_CONFIG'] = '1'
os.environ['KIVY_BUILD'] = 'emscripten'


import runpy
runpy.run_path('/app/pong/main.py', run_name='__main__')
