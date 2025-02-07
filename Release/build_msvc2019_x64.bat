mkdir build_msvc2019_x64
cd build_msvc2019_x64
cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_INSTALL_PREFIX=../install_msvc2019_x64 -DUSE_LLGI=ON -DBUILD_DX12=ON .. -DCMAKE_DEBUG_POSTFIX=d
cmake --build . --config Debug --target INSTALL
cmake --build . --config Release --target INSTALL
