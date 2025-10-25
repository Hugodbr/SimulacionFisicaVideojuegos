cd c:\Users\hugod\GitHub\SimulacionFisicaVideojuegos
rmdir /s /q build
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug -j 3