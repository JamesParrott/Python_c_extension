// Boilerplate (essential define & headers for Python C extensions).    
// https://docs.python.org/3/extending/extending.html#a-simple-example
#define PY_SSIZE_T_CLEAN
#include <Python.h>
// "All user-visible symbols defined by Python.h have a prefix of 
// Py or PY, except those defined in standard header files. For 
// convenience, and since they are used extensively by the Python 
// interpreter, "Python.h" includes a few standard header files: 
// <stdio.h>, <string.h>, <errno.h>, and <stdlib.h>. 
// If the latter header file does not exist on your system, 
// it declares the functions malloc(), free() and realloc() directly. "

// "You can also define a new exception that is unique 
// to your module. For this, you usually declare a 
// static object variable at the beginning of your file:"
// https://docs.python.org/3/extending/extending.html#intermezzo-errors-and-exceptions
static PyObject *SpamError;

// "your module's initialization function".
//  "initializes" SpamError "with an exception object".
PyMODINIT_FUNC
PyInit_spam(void)
{
    PyObject *m;

    m = PyModule_Create(&spammodule);
    if (m == NULL)
        return NULL;


    // "Note that the Python name for the exception object is spam.error. 
    // The PyErr_NewException() function may create a class with the 
    // base class being Exception (unless another class is passed in 
    // instead of NULL), described in Built-in Exceptions."

    // "Note also that the SpamError variable retains a reference to 
    // the newly created exception class; this is intentional! Since 
    // the exception could be removed from the module by external code, 
    // an owned reference to the class is needed to ensure that it 
    // will not be discarded, causing SpamError to become a dangling 
    // pointer. Should it become a dangling pointer, C code which 
    // raises the exception could cause a core dump or other 
    // unintended side effects."

    SpamError = PyErr_NewException("spam.error", NULL, NULL);
    if (PyModule_AddObjectRef(m, "error", SpamError) < 0) {
        Py_CLEAR(SpamError);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}


// the C function that will be called when the 
// Python expression spam.system(string) is evaluated
static PyObject *
spam_system(PyObject *self, PyObject *args)
{
    // This is a pointer assignment and you are not 
    // supposed to modify the string to which it points 
    // (so in Standard C, the variable command should 
    // properly be declared as const char *command).
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);


    // "The spam.error exception can be raised in your 
    // extension module using a call to PyErr_SetString() 
    // as shown below: "
    if (sts < 0) {
        PyErr_SetString(SpamError, "System command failed");
        return NULL;
    }

    // "Our spam.system() function must return the value of 
    // sts as a Python object. This is done using the 
    // function PyLong_FromLong().  
    // In this case, it will return an integer object. 
    // (Yes, even integers are objects on the heap in Python!) "
    return PyLong_FromLong(sts);


    // "If you have a C function that returns no useful argument 
    // (a function returning void), the corresponding Python 
    // function must return None. You need this idiom to do 
    // so (which is implemented by the Py_RETURN_NONE macro): "

    // Py_INCREF(Py_None);
    // return Py_None;

    // "Py_None is the C name for the special Python object None. 
    // It is a genuine Python object rather than a NULL pointer, 
    // which means "error" in most contexts, as we have seen."

}

