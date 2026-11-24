# skia - overlay port

A copy of the `skia` port of microsoft/vcpkg (version 148, vcpkg commit
`6cf5a3612342c3cf0d7dee5cf6687d0882ec6663`, the baseline of this repository; MIT licence) with
one change in `portfile.cmake`, marked `OpenS100`: on Windows x64 Skia is compiled by clang-cl
instead of MSVC.

Why: Skia compiles its raster pipeline without SIMD for any compiler but Clang and GCC
(`SKRP_CPU_SCALAR` in `src/opts/SkRasterPipeline_opts.h`). Drawing the 808 cells of the KOR test
data set on the CPU took 134.0 s with the MSVC build and 22.5 s with the clang-cl build; the
pictures are the same (`Documents/TechDocs/2026-11-19-r6-skia-backend-design.md`, 8.6 and 8.8).

vcpkg fetches LLVM for this by itself (`vcpkg_find_acquire_program(CLANG)`), a download of a few
hundred megabytes the first time. Nothing has to be installed. Code that links Skia is compiled
by MSVC as before.

When the baseline of the repository moves, copy the port again from that vcpkg commit and put
the marked lines back. The port can go once the vcpkg port offers the same choice.
