Byrep is a relatively primitive tool for doing quick byte pattern replacements
inside of binary files.

It's not super efficient nor advanced; there's lots of room for improvement,
but it gets the job done. Also, don't try to replace bytes right at the end of
a file. I haven't tested it, but I'm pretty sure that will make it crash. If it
does make it crash, fixing that is left as an exercise to the reader (until I
am personally affected by this issue).

To build Byrep, you'll need a compiler with C++20 support. Configure and build
as you would with any other CMake project. An 'install' target is provided.

Byrep is licensed under the BSD 3-Clause license; see LICENSE.txt for more info.
