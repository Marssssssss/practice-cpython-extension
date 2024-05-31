参考文档：[文档](https://docs.python.org/zh-cn/3.11/extending/extending.html)

基于 `cpython` 进行的生成。

## 静态编入 Python 解释器
1. 将 `cqueuemodule.c` 文件放到 `cpython` 的 `Modules` 目录下
2. 到 `PC` 目录下，配置 `config.c` 文件，指明需要预先初始化的模块（内置），代码如下：

``` c
extern PyObject* PyInit__xxsubinterpreters(void);
extern PyObject* PyInit__xxinterpchannels(void);
// STEP1：类似其他 extern，加上 cqueue 的初始化函数定义
extern PyObject* PyInit_cqueue(void);

// STEP2：向 _PyImport_Inittab 表里加上对应的初始化函数，解释器初始化时会初始化对应模块，此后可以直接当作内置库 import 和使用
struct _inittab _PyImport_Inittab[] = {
    // ...
    {"cqueue", PyInit_cqueue},
    // ...
}
```
3. `Visual Studio` 打开 `PCBuild` 解决方案，生成即可
    1. 也可以选择用 `PCbuild` 下面的 `build.bat` 生成 

## 动态链接库导出
看了下最方便的方法是借助 `setuptools` 来生成，两个步骤：

- 编写 `setup.py` 脚本，如目录里提供的
- 安装 `setuptools` 并生成

``` bash
pip install setuptools
python setup.py build
```

## Linux 下的生成
改法同上，动态链接库导出同上，静态编译方式略有改动：

#### 静态编译解释器
1. 在 `Setup.local` 下把 `cqueue` 加进去
    1. 一开始编译不成功看了下 `linux` 下的 `config.c` 是由 `makesetup` 脚本生成的，生成方式看 `makesetup` 的实现应该是直接复制的 `config.c.in`，所以上面说的要加到 `config.c` 里的东西应该是要先加到 `config.c.in` 里，按照这个思路搞了一版结果生成失败
    2. 全局搜了下其他模块的名字，发现一个叫 `Setup` 文件的，看了下文件注释，发现 `makesetup` 会根据这个文件来生成 `Makefile` 和 `config.c`，遂直接改之，编译还是出错
    3. 然后再细看了下 `Setup` 文件的注释，说是本地修改应该放在 `Setup.local` 下，而且刚才傻逼了，`Setup` 里面写的全是注释写法，我仿照这个写法本身也屁用没有...改到 `Setup.local` 之后编译成功
    4. 改后的文件类似下面：

``` bash
# Edit this file for local setup changes
cqueue cqueuemodule.c
```

2. 使用 `gcc` + `make` 编译：

``` bash
apt-get install gcc make -y
./configure  # 这步会把 config.c.in 整合到 config.c 里
make

# 这里编译可能会出现 ssl 版本问题，会导致 ssl 库编不出来，但不影响解释器正常使用，如果需要 ssl 的话可以自行编一个对应版本的 openssl
```

## 版本相关的适配
编译碰到的另一个问题是版本的问题，本来用 `3.12` 版本就不用关心这个适配，心血来潮想想要不适配个看看。
具体问题就是：

- 在模块定义里定义了 `Py_mod_multiple_interpreters`
- 这个槽文档里写的是 `3.12` 版本后才有

所以在 `3.11` 下这行会直接 `error`，解决方法是用 `PythonAPI` 自带的宏（自己翻出来的...），代码写成：

``` c
#if PY_MAJOR_VERSION > 3 || (PY_MAJOR_VERSION == 3 && PY_MINOR_VERSION >= 12)
    {Py_mod_multiple_interpreters, Py_MOD_PER_INTERPRETER_GIL_SUPPORTED},
#endif
```

## 简单 benchmark 做个对比
-- TODO
