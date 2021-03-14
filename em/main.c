#include <stdlib.h>
#include <stdio.h>

#include <emscripten.h>
#include <Python.h>

#include "gen0/builtins.h"


static int load_counter;


static void onload(const char *filename) {
    printf("Loaded %s.\n", filename);
    char extract[128];
    snprintf(
        extract, sizeof(extract),
        "import zipfile;"
        "zipfile.ZipFile('%s').extractall('/');",
        filename
    );
    PyRun_SimpleString(extract);

    --load_counter;
    if (!load_counter) {
        PyRun_SimpleString(
            "import sys;"
            "sys.path.insert(0, '/');"
            "sys.argv = ['python'];"
            "import runpy;"
            "runpy.run_module('app');"
        );
    }
}


static void onloaderror(const char *filename) {
    printf("Failed to load %s, aborting.\n", filename);
}


typedef void *PyThread_type_lock;


PyThread_type_lock PyThread_allocate_lock(void) {
    return (PyThread_type_lock)1;
}


void PyThread_free_lock(PyThread_type_lock lock) {
}


int main(int argc, char** argv) {
    setenv("PYTHONHOME", "/", 0);

    PyImport_ExtendInittab(builtins);

    Py_InitializeEx(0);

    PyRun_SimpleString(
        "import importlib.abc\n" \
        "import importlib.machinery\n" \
        "import sys\n" \
        "\n" \
        "\n" \
        "class Finder(importlib.abc.MetaPathFinder):\n" \
        "    def find_spec(self, fullname, path, target=None):\n" \
        "        if fullname in sys.builtin_module_names:\n" \
        "            return importlib.machinery.ModuleSpec(\n" \
        "                fullname,\n" \
        "                importlib.machinery.BuiltinImporter,\n" \
        "            )\n" \
        "\n" \
        "\n" \
        "sys.meta_path.append(Finder())\n" \
    );

    // Fetch kivy.zip and app.zip from the server.
    load_counter = 2;
    emscripten_async_wget("kivy.zip", "/kivy.zip", onload, onloaderror);
    emscripten_async_wget("app.zip", "/app.zip", onload, onloaderror);

    emscripten_exit_with_live_runtime();
    return 0;
}
