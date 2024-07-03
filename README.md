# Overview
TSrim is a ROOT class inherited from TF1, which enables us to calculate range or energy loss of an isotope in material.

# Install

# Usage



See https://docs.google.com/presentation/d/1v2fcSzfREJnktkHS7z6tXroHQbBpR1BSlsRj4ryszQc/edit?usp=sharing for details.

# Range database
Original SRIM range files (named like 4He_in_havar.txt, etc.) are also available in the link below:
https://www.dropbox.com/scl/fo/3nqo5lhjq3vgqpbyoymmv/AB7X6ewIv6IBYjoZaQ5-7SU?rlkey=bzidq0fdmj2k8zfeh3j3rdsqu&st=6leer867&dl=0
which are created by the get_sr_fit_prm.pl macro.
They are out of git management since they are as many files as number of isotopes in the mass table (mass2020.dat) for each material,
and not directly used by TSrim (the fit parameter files range_fit_pol16_*.txt). The range list may be added sometimes.