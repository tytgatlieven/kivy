"""Stub thread module for Python without thread support."""


class Lock:
    def acquire(self):
        pass

    def release(self):
        pass

    def __enter__(self, *_):
        pass

    def __exit__(self, *_):
        pass


class RLock(Lock):
    pass


class Event:
    pass


def get_ident():
    return 0


class Thread:
    name = 'main'


MainThread = Thread()


def current_thread():
    return MainThread


class _Local:
    pass


_tls = _Local()


def local():
    return _tls
