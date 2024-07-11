###################### Edit below ######################
DIRLIB = /usr/local/lib
DIRINC = /usr/local/include

DIRRANGE_ORG = ./SR\ Module/range
DIRRANGE = /usr/local/share/range_fit
RANGES = $(DIRRANGE_ORG)/*/range_fit_pol16_*.txt
DIRMACRO_ORG = ./macros

MACROS = $(DIRMACRO_ORG)/load_srim.C $(DIRMACRO_ORG)/srim_all.C \
	$(DIRMACRO_ORG)/srim_light.C $(DIRMACRO_ORG)/srim_mid.C \
	$(DIRMACRO_ORG)/srim_cribpid.C $(DIRMACRO_ORG)/srim_basic.C \
	$(DIRMACRO_ORG)/plot_dee.C
DIRMACRO = /usr/local/macros
###################### Edit above ######################

TAR1 = Srim
TAR2 = Mass

CXX           = g++
CXXFLAGS      = -std=c++11 -fPIC #-O2 -pipe -Wall -W -Woverloaded-virtual

ROOTCONFIG = $(ROOTSYS)/bin/root-config
ROOTCLING = $(ROOTSYS)/bin/rootcling

all: lib$(TAR1).so

T${TAR1}Dict.cc: T$(TAR1).h LinkDef.h
	${ROOTCLING} -f $@ -c $^

lib$(TAR1).so: T$(TAR1)Dict.cc T$(TAR1).cc $(TAR2).cc T$(TAR1).h $(TAR2).h
	$(CXX) $(CXXFLAGS) `$(ROOTCONFIG) --cflags --libs` -shared \
	T$(TAR1)Dict.cc T$(TAR1).cc $(TAR2).cc -o lib$(TAR1).so
#-L. -lmass

lib$(TAR2).so: $(TAR2).cc $(TAR2).h
	$(CXX) $(CXXFLAGS) -c $(TAR2).cc
	$(CXX) $(CXXFLAGS) -shared -o lib$(TAR2).so $(TAR2).o

clean:
	rm -f lib$(TAR1).so
	rm -f lib$(TAR2).so
	rm -f $(TAR2).o

install: lib$(TAR1).so T$(TAR1)Dict_rdict.pcm T$(TAR1).h $(RANGES) $(MACROS)
	cp -fpv lib$(TAR1).so T$(TAR1)Dict_rdict.pcm $(DIRLIB)
	cp -fpv T$(TAR1).h $(DIRINC)
	cp -fpv $(RANGES) $(DIRRANGE)
	cp -fpv $(MACROS) $(DIRMACRO)

install-$(TAR2): lib$(TAR2).so $(TAR2).h
	cp -fpv lib$(TAR2).so $(DIRLIB)
	cp -fpv $(TAR2).h $(DIRINC)
