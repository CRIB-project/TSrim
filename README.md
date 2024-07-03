# Overview
TSrim is a ROOT class inherited from TF1, which enables us to calculate range or energy loss of a specified isotope in a material based on the SRIM range calculation.

Instead of directly reading a SRIM's range output file and performing interpolation everytime it is called, TSrim treats a polynomial function which fits to the log(Energy) vs log(Range) calculated by SRIM.

We recommend the polynomial dgree of 16, and fitted parameter files for many materials are already available (in SR\ Module/range/*/range_fit_pol16_*.txt).

# Configuration and Install
Users need to specify some relevant directory and file names in the Makefile which are used by _make install_ and some macros.
- **DIRLIB**: Library directory which ROOT can access.
The dynamic library files (libSrim.so, TSrimDict_rdict.pcm and libMass.so) will be copied there.
- **DIRINC**: Include directory which ROOT can access. The header files (TSrim.h and Mass.h) will be copied there.
- **DIRRANGE_ORG**: Range data directory
- **DIRRANGE**: The directory where fit parameter files are stored. The macro file load_srim.C loads the range fit parameter files from there.
- **RANGES**: Specifies fit parameter files to be copied to DIRRANGE/.
- **DIRMACRO_ORG**: Directory where macro files are stored.
- **MACROS**: macro file names to be copied to DIRMACRO/.
- **DIRMACRO**: Macro directory where ROOT can access.

DIRLIB and DIRMACRO should be defined in `.rootrc`
as Unix.*.Root.DynamicPath and Unix.*.Root.MacroPath so that ROOT can access them.

DIRINC should be defined in
`.rootlogon.C`
by
> gInterpreter->AddIncludePath("/usr/local/include");

(Maybe DIRLIB, too?)
> gSystem->AddDynamicPath("/usr/local/lib");


Then 
> $ make

will conpile the source and create libSrim.so and libMass.so library files, and
> $ (sudo) make install

will copy the related files to the specified directories so that ROOT can load the libraries and macros.

If Makefile fails, please edit it for your environments.

# Usage
## Most basic example
On the ROOT interpreter, 
> root [0] .L (DIRLIB/)libSrim.so   // Load the library 

> root [1] TSrim *srim=new TSrim("si",16,"(DIRRANGE)/range_fit_pol16_si.txt")   // Define a pointer

> root [2] srim->Range(2,4,50,"si")   // The pointer can return values by the member functions

> (double) 1.0492494   // Returned value

## Advanced usage
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
}, and nuclide range from 1H to 56Ni. And then you can imediatly call, e.g.
> root [1] srim_basic->EnergyLoss(14,26,20,"he",100,300,293)

> (double) 16.272412

i.g., "energy loss of 26Si in He gas of 100 mm, 300 Torr at 293 K is 16.272412 MeV".

## Macro example
TSrim can easily be used in ROOT macros.
Please include `#include <Mass.h>` and `#include <TSrim.h>` and load `gSystem->Load("libSrim.so");`, then TSrim can be used.
An example macro `plot_dee.C` plots dE-E PID curves by
> root [0] .x plot_dee.C(0.05,1.5,10,30)

![PID_p-a](https://www.cns.s.u-tokyo.ac.jp/gitlab/hayakawa/tsrim/-/raw/main/macros/PID_p-a.png?ref_type=heads "PID_p-a")


# Range database
The parameters of 16th polynomial for all the isotopes up to Uranium listed in the mass table (mass2020.dat) are already provided for several commonly-used materials.

Users can also add range data and fit parameter files by themselves. In SR\ Module/ directory, there are perl script get_sr_fit_prm.pl and a ROOT macro fit_srim_table.C

Original SRIM range files (named like 4He_in_havar.txt, etc.) are also available here [range_link](https://www.dropbox.com/scl/fo/3nqo5lhjq3vgqpbyoymmv/AB7X6ewIv6IBYjoZaQ5-7SU?rlkey=bzidq0fdmj2k8zfeh3j3rdsqu&st=6leer867&dl=0)
which are created by the get_sr_fit_prm.pl macro.
They are out of git management since they are as many files as number of isotopes in the mass table (mass2020.dat) for each material,
and not directly used by TSrim (the fit parameter files range_fit_pol16_*.txt). The range list may be added sometimes.

# See also
[SRIM on ROOT](https://docs.google.com/presentation/d/1v2fcSzfREJnktkHS7z6tXroHQbBpR1BSlsRj4ryszQc/edit?usp=sharing) for details.