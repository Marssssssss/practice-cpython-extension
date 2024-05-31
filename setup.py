from distutils.core import setup, Extension

module1 = Extension("cqueue", sources=["cqueuemodule.c"])

setup(
    name="cqueue",
    version="1.0",
    description="demo cqueue package",
    ext_modules=[module1]
)