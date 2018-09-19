Some tasks for internship.

To build project run:
    cmake -DCMAKE_BUILD_TYPE=Release && make


Box filter:

    Realization has two box filters — straight forward and optimized,
    program runs both of them to compare which one is faster.

    Go to boxFilter directory and then run boxFilter:
        cd boxFilter/
        ./boxFilter

    By default it uses image.txt as input image and prints output to res.txt.

    If you want to specify input or output use -i and -o option:
        ./boxFilter -i newInput -o newOutput



Band matrix vector multiplication:

    For running this program you need SSSE3.

    Go to bandMatrixVector directory and run bandMatrixVector:
        cd bandMatrixVector/
        ./bandMatrixVector

    By default it uses bigMatrix.txt as input band matrix,
    bigVector.txt as input vector and result will be printed to stdout.

    If you want to specify input or output use -i, -o and -v option:
        ./boxFilter -i newInput -o newOutput -v newVector