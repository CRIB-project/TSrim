# Overview
TSrim is a ROOT class inherited from TF1, which enables us to calculate range or energy loss of an ion in a material based on the SRIM range table.

Instead of directly reading a SRIM's range output file and performing interpolation every time it is called, TSrim treats a polynomial function which fits to the log(Energy) vs log(Range) table of a specified ion-target pair provided by SRIM.

We recommend the polynomial degree of 16 for the moment, which guarantees the deviation from the original range values by SRIM mostly less than 1%, and still fast enough to repeat calculations. Fit parameter files for a number of materials are already available (in SR\ Module/range/*/range_fit_pol16_*.txt).

### Units and material information
- Thickness and range in mm
- Energy in MeV
- Energy per nucleon in MeV/u
- Temperature in K
- Pressure in Torr
- Available material list (as of 04-July-2024)
  - "3he", "air", "al", "anthracene", "ar", "au",
  "be", "butane", "c", "cd2", "ch2", "cu",
  "h", "havar", "he", "he_90_co2_10", "kapton", "mo",
  "mylar", "ni", "si", "ti"
  - Minimum energy = 1 keV
  - Maximum energy per nucleon = 400 MeV/u
  - 1H to 243U (SRIM does not treat beyond Uranium)


# Configuration and Install

## Building with Makefile

Users need to specify some relevant directory and file names in the Makefile which are used by _make install_.
- **DIRLIB**: Library directory which ROOT can access, where
to copy the dynamic library files (libSrim.so, TSrimDict_rdict.pcm and libMass.so).
- **DIRINC**: Include directory which ROOT can access. where to copy the header files (TSrim.h and Mass.h).
- **DIRRANGE_ORG**: Range data directory
- **DIRRANGE**: The directory where fit parameter files are stored. The macro file load_srim.C loads the range fit parameter files from there.
- **RANGES**: Specifies fit parameter files to be copied to DIRRANGE/.
- **DIRMACRO_ORG**: Directory where macro files are stored.
- **MACROS**: macro file names to be copied to DIRMACRO/.
- **DIRMACRO**: Macro directory where ROOT can access.

DIRLIB and DIRMACRO should be defined in your `.rootrc`
as Unix.*.Root.DynamicPath and Unix.*.Root.MacroPath so that ROOT can access them.

DIRINC should be defined in your
`.rootlogon.C`
by
> gInterpreter->AddIncludePath("/usr/local/include");

(Maybe DIRLIB, too?)
> gSystem->AddDynamicPath("/usr/local/lib");


Then
> $ make

will compile the source and create libSrim.so and libMass.so library files, and
> $ (sudo) make install

will copy the related files to the specified directories so that ROOT can load the libraries, headers and macros.

If Makefile fails, please edit it for your environments.

## Building with CMake
You can build **TSrim** as any other project with a CMake-based build system.

```shell
mkdir build
cd build
cmake (option) ..
make
(sudo) make install
```

In the CMake option, you can specify install directory.

```shell
cmake -DCMAKE_INSTALL_PREFIX=../install ..
```

Then the library and header files will be installed "install" directory.

```
.
├── build
├── install
│   ├── bin
│   ├── include
│   └── lib
...
├── TSrim.cc
└── TSrim.h
```

If no options are specified, it will be installed in the **/usr/local** directory.

If you want to uninstall, you can use "make uninstall" command in build directory.

```shell
cd build
(sudo) make uninstall
```

# Usage
### Most basic example
On the ROOT interpreter,
> root [0] .L (DIRLIB/)libSrim.so
(Load the library)

> root [1] TSrim *srim=new TSrim("si",16,"(DIRRANGE)/range_fit_pol16_si.txt")
(Define a pointer)

> root [2] srim->Range(2,4,50,"si")
(The pointer can return values by the member functions)

> (double) 1.0492494
(Returned value in mm)

### Useful usage
A macro `load_srim.C` defines some useful functions to treat multiple range fit files. For example,
> root [0] .L load_srim.C

Then you can use, for example,
> root [1] TSrim *srim=load_srim({"havar","mylar"});

to load Havar and Mylar parameters at once. Users can also specify a range of nuclides, for example
> root [1] TSrim *srim=load_srim({"havar","mylar"},1,1,6,12);

from 1H to 12C (rectangular range in chart of nuclides) to save memory and faster execution.

Some macros like `srim_basic.C` calls specified material list and nuclide ranges. I.g.,
> root [0] .L srim_basic.C

loads material list of {
  "al", "ar", "au",
  "c", "ch2",
  "h", "havar", "he", "kapton",
  "mylar", "si"
}, and nuclide range from 1H to 56Ni. And then you can immediately call, e.g.
> root [1] srim_basic->EnergyLoss(14,26,20,"he",100,300,293)

> (double) 16.272412

i.g., "energy loss of 26Si in He gas of 100 mm, 300 Torr at 293 K is 16.272412 MeV".

Please also check `srim_all.C`, `srim_light.C`, `srim_mid.C`, `srim_cribpid.C`, etc.

### Macro example
TSrim can easily be used in ROOT macros.
Inncluding `#include <Mass.h>` and `#include <TSrim.h>` and load `gSystem->Load("libSrim.so");`, then TSrim can be used.
An example macro `plot_dee.C` plots dE-E PID curves by
> root [0] .x plot_dee.C(0.05,1.5,10,30)

i.e. ΔE thickness = 50 um, E thickness = 1.5 mm, ΔE range = 10 MeV, E_total range = 30 MeV.
![PID_p-a](https://www.cns.s.u-tokyo.ac.jp/gitlab/hayakawa/tsrim/-/raw/main/macros/PID_p-a.png?ref_type=heads "PID_p-a")

### Use this library in other CMake projects

It supports `find_package` of CMake if you build it with CMake.

In your own CMake project, you can make link to this library like;

```cmake
# CMakeLists.txt

find_package(TSrim)
if(TSrim_FOUND)
  message(STATUS "Found TSrim library")
else()
  message(STATUS "Not Found TSrim library")
endif()

# ...

# NOTE: library name is TSrim::Srim
target_link_libraries(${TARGET_NAME}  TSrim::Srim)
```

# Range database
The parameters of 16th polynomial for all the nuclides up to Uranium listed in the mass table (mass2020.dat) are already provided for several commonly-used materials (as shown in Overview).

Users can also add range data and fit parameter files by themselves. In SR\ Module/ directory, there are perl script get_sr_fit_prm.pl and a ROOT macro fit_srim_table.C
- Prepare target material information (in SR.IN format)
  - Start Ion Stopping and Range Tables (SR.exe)
  - Specify target material
  - Click “Calculate Table”
    - Then ./SR.IN file will be created
    - Extract only the “target” part from SR.IN
    - Store it in SR\ Module/range/(material name)/(material name).IN
  - Then you can close SR.exe
- Set up get_sr_fit_prm.pl parameters
  - A and Element ranges (abeg--aend, elbeg--elend)
  - Material list (.IN files should be prepared in advance)
  - Degree of the polynomial function to be fitted to the range table (16 by default)
  - etc.
- Execute the script
  - $ perl get_sr_fit_prm.pl
  - It may take even hours for 3132 nuclides
- One can select to run or not to run the SR.exe part, and fitting part in get_sr_fit_prm.pl, respectively.

Original SRIM range table files (named like 4He_in_havar.txt, etc.) are also available here [range_link](https://www.dropbox.com/scl/fo/3nqo5lhjq3vgqpbyoymmv/AB7X6ewIv6IBYjoZaQ5-7SU?rlkey=bzidq0fdmj2k8zfeh3j3rdsqu&st=6leer867&dl=0)
which are created by the get_sr_fit_prm.pl script.
They are set out of git management since there are as many files as number of nuclides in the mass table (mass2020.dat) up to 243U (= 3132!) for each material,
and not directly used by TSrim.

# See also
[SRIM on ROOT](https://docs.google.com/presentation/d/1v2fcSzfREJnktkHS7z6tXroHQbBpR1BSlsRj4ryszQc/edit?usp=sharing) for details.
