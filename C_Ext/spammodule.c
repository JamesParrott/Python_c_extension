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


// https://docs.python.org/3/extending/extending.html#the-module-s-method-table-and-initialization-function
// "how spam_system() is called from Python programs. 
// First, we need to list its name and address in a "method table":"

static PyMethodDef SpamMethods[] = {
    // ...
    {"system",  spam_system, METH_VARARGS,
     "Execute a shell command."},
    // ...
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

// "Note the third entry (METH_VARARGS). This is a flag telling the 
// interpreter the calling convention to be used for the C function. 
// It should normally always be 
// METH_VARARGS or METH_VARARGS | METH_KEYWORDS; 
// a value of 0 means that an obsolete variant of 
// PyArg_ParseTuple() is used. "
//
// " The METH_KEYWORDS bit may be set in the third field if keyword 
// arguments should be passed to the function. In this case, 
// the C function should accept a third PyObject * parameter which 
// will be a dictionary of keywords. Use PyArg_ParseTupleAndKeywords() 
// to parse the arguments to such a function. "

// "The method table must be referenced in the module definition structure:"



// spammodule is referred to in PyInit_spam, where it is passed to the interpreter.
static struct PyModuleDef spammodule = {
    PyModuleDef_HEAD_INIT,
    "spam",   /* name of module */
    NULL, //spam_doc, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    SpamMethods
};





// "your module's initialization function".
//  "initializes" SpamError "with an exception object".
//
// "When the Python program imports module spam for 
// the first time, PyInit_spam() is called. "
// "It calls PyModule_Create(), which returns a module object, 
// and inserts built-in function objects into the newly created 
// module based upon the table (an array of PyMethodDef structures) 
// found in the module definition. PyModule_Create() returns a 
// pointer to the module object that it creates. It may abort 
// with a fatal error for certain errors, or return NULL if the 
// module could not be initialized satisfactorily. The init function 
// must return the module object to its caller, so that it then gets 
// inserted into sys.modules."
PyMODINIT_FUNC PyInit_spam(void)
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
// "When embedding Python, the PyInit_spam() function is not called 
// automatically unless there's an entry in the PyImport_Inittab table. 
// To add the module to the initialization table, use 
// PyImport_AppendInittab(), optionally followed by an import of 
// the module...  "



