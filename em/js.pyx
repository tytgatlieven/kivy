cdef extern from "emscripten.h":
    void emscripten_run_script(const char *)
    void emscripten_set_main_loop(void (*thunk)(), int, int)


def run(code):
    emscripten_run_script(code)


py_callback = None


cdef void once():
    py_callback()


def run_forever(callback):
    global py_callback
    py_callback = callback
    emscripten_set_main_loop(once, 0, 1)
