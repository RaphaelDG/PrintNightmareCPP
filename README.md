# PrintNightmare LPE PoC

This is a juste a C++ exploit code for Print Nightmare LPE vulnerability (CVE 2021-1675, CVE-2021-34527).

Add your custom DLL line `94` & `95` and use the following to build on Linux with Mingw32 :
```bash
x86_64-w64-mingw32-g++-win32 printNightmare.cpp -lwinspool -static
```
