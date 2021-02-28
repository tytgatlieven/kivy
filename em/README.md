# Kivy in the browser using emscripten

This directory contains proof-of-concept build support for running Kivy in the
browser.

# Building

## Requirements

This has been built with emsdk_portable sdk-master-64bit 1.36.0 and
Cython==0.25.1

## CPython

The first step is to compile Python for the browser using emscripten. This is
done in the cpython-emscripten submodule. From the top level repo run:

```
git submodule init
git submodule update
( cd cpython-emscripten/3.5.2/ ; make )
```

## The Pong Demo

Change into the em/ directory and run ```make```. Then change into the
web/ directory and start a web server
e.g. ```python2 -m SimpleHTTPServer 8062```

By browsing to [http://localhost:8062/](http://localhost:8062/) you should
see the pong demo

# Caveats

It does not work in Chrome because the generated javascript file is too big.
This could be worked around by using dynamic linking, but this has a
significant performance overhead. It may be better to wait for wasm support
in Chrome.

Currently there is no sound support at all. SDL2 Mixer does not seem to be
available as an emscripten module.

Currently there is no multitouch support. This can be added.

The compiled files are very large (~40MB) and although they can be gzipped
well when being transported to the browser (~5MB) there is still a lot of
code for the browser to parse. This takes a long time on a desktop, mobiles
might not be able to manage at all. This is an issue that wasm is designed to
address, so it could be that this project ends up more viable over time.

# Testing more programs

The build process generates a number of files that are generally useful, such
as the core Python and Kivy engine in kivy.js, kivy.data and kivy.zip. The pong
demo is contained in app.zip. Specifically when the page is loaded the contents
of app.zip are extracted to a virtual filesystem in the browser and the app
module is executed. The supplied app module sets a few environment variables
and then executes /app/pong/main.py.

To run another pure Python Kivy app it is sufficient to create a new app.zip
with the desired code and data and serve it along side the other files
in web/.
