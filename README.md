To build the application make a new folder in root dir of the utlilib

    mkdir build

change into this folder

    cd build

and now execute cmake

    cmake ../

now do normal make

    make

To build the documentation run (Doxygen must be installed):

    make doc

To run the tests run:

    make test

For running coverage analysis run:

    make coverage

For more details see the generated html documentation.



DEVELOPERS INFO:

This project sticks the the google style of code. Please therefore activate the following git hoock and adapt your code to the given error messages:
$ln -s ./doc/pre-commit-google.py ./.git/hooks/pre-commit
