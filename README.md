# MatriX
Matrix simulation

# Commands
Datas are saved into the build folder in the "data.root" file. You can change file name with:
	/Analysis/SetFileName <new_name.root> (must include ".root")

It is possible to change the scintillation crystals material and size:
	/MatriX/det/CrysSize <newsize> <unit>
	/MatriX/det/CrysSize3 <newsizex> <newsizey> <newsizez> <unit>
	/MatriX/det/CrysMaterial <material_name> ("BC400" or "LYSO")

It is possible to change the SiPM model:
	/MatriX/det/SiPMmodel <model_name> ("25PE", "50PE", "75PE", "25CS", "50CS", "75CS")

It is possible to put a mask on MatriX to isolate the elements:
	/MatriX/det/Mask <bool> (default is "false")

It is possible to activate dark counts or optical cross talk:
	/Element/det/OCT <bool> (default is false)
	/Element/det/DN <bool>  (default is false)

It is possible to change beam rate, particle or energy:
	/Primaries/Rate <value> <unit>
	/gun/particle <particle>
	/gun/energy <energy> <unit> (momentum as well)

# Output signals
To see the signals you must use signals.C macro:
	bash>> root
	root-bash>> .L signals.C
	root-bash>> signals() signals(path-to-file) or signals(path-to-file, threashold) (threashold is in number of pixel activated per channel)

These commands will produce a out.txt file. Here you have the number of signals per channel, the proton current (always at max 0.22) and the run duration. To extract beam information from datas you can use readgio.cpp macro:
	bash>> root
	root-bash>> .L readgio.cpp
	root-bash>> readMatrix("out.txt")


