#include "Python.h"
#include "structmember.h"         // PyMemberDef

/* Queue Type
*/
typedef struct {
    PyListObject list;
} Queue;

static PyObject*
pop(Queue* self, PyObject* args)
{
    PyObject* list = (PyObject*)(&(self->list));
    Py_ssize_t size = PyList_Size(list);
    if (size < 0) {
        return NULL;
    }
    if (size == 0) {
        Py_RETURN_NONE;
    }
    PyObject* head = PyList_GetItem(list, 0);
    if (!head) {
        return NULL;
    }
    Py_INCREF(head);
    if (PySequence_DelItem(list, 0) < 0) {
        return NULL;
    }
    return head;
}

static PyObject*
push(Queue* self, PyObject* args)
{
    PyObject* obj;
    if (!PyArg_ParseTuple(args, "O", &obj)) {
        return NULL;
    }
    PyObject* list = (PyObject*)(&(self->list));
    if (PyList_Append(list, obj) < 0) {
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject*
size(Queue* self, PyObject* args)
{
    Py_ssize_t size = PyList_Size((PyObject*)(&(self->list)));
    if (size < 0) {
        return NULL;
    }
    PyObject* size_obj = PyLong_FromLong(size);
    if (!size_obj) {
        return NULL;
    }
    return size_obj;
}

static PyMethodDef queue_methods[] = {
    {"pop", (PyCFunction)pop, METH_VARARGS, PyDoc_STR("pop object from head")},
    {"push", (PyCFunction)push, METH_VARARGS, PyDoc_STR("push object to tail")},
    {"size", (PyCFunction)size, METH_VARARGS, PyDoc_STR("get size of queue")},
    {NULL, NULL},
};

static int
queue_init(Queue* self, PyObject* args, PyObject* kwds)
{
    if (PyList_Type.tp_init((PyObject*)self, args, kwds) < 0) {
        return -1;
    }
    return 0;
}

static PyTypeObject queue_type = {
    PyVarObject_HEAD_INIT(0, 0)
    .tp_name = "cqueue.queue",
    .tp_basicsize = sizeof(Queue),
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_methods = queue_methods,
    .tp_init = (initproc)queue_init,                    /* tp_init */
};

/* cqueue Module
*/
static int
cqueue_exec(PyObject* m)
{
    queue_type.tp_base = &PyList_Type;
    if (PyType_Ready(&queue_type) < 0)
        return -1;
    if (PyModule_AddObjectRef(m, "queue", (PyObject*)&queue_type) < 0)
        return -1;
    return 0;
}

static PyMethodDef cqueue_functions[] = {
    {NULL,              NULL}           /* sentinel */
};

static struct PyModuleDef_Slot cqueue_slots[] = {
    {Py_mod_exec, cqueue_exec},
#if PY_MAJOR_VERSION > 3 || (PY_MAJOR_VERSION == 3 && PY_MINOR_VERSION >= 12)
    {Py_mod_multiple_interpreters, Py_MOD_PER_INTERPRETER_GIL_SUPPORTED},
#endif
    {0, NULL},
};

static struct PyModuleDef cqueuemodule = {
    PyModuleDef_HEAD_INIT,
    "cqueue",
    PyDoc_STR("queue module implementation by c"),
    0,
    cqueue_functions,
    cqueue_slots,
    NULL,
    NULL,
    NULL
};


PyMODINIT_FUNC
PyInit_cqueue(void)
{
    return PyModuleDef_Init(&cqueuemodule);
}