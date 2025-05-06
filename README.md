Two ways to build a basic C extension to run "system", liberally copy and pasted from:

https://docs.python.org/3/extending/extending.html#a-simple-example

and 

https://setuptools.pypa.io/en/latest/userguide/ext_modules.html




Build the version in ./setuptools_ext with pyproject-build in a matter of seconds.  

Or build CPython from source in a matter of minutes, copy the contents of ./C_ext into the CPython repo's ./Modules, and then build in a matter of seconds (run "Make" in the CPython repo's root).

Tested on Ubuntu 24.04 (on ARM) with gcc.

I am not claiming Copyright over the other files, as it is not mine to claim.
