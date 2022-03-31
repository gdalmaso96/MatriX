# MatriX
Matrix simulation

# Commands
Datas are saved into the build folder in the "data.root" file. You can change file name with:
<code> /Analysis/SetFileName <new_name.root> (must include ".root") </code>

It is possible to change the scintillation crystals material and size:
<code> /MatriX/det/CrysSize <newsize> <unit> </code>
<code> /MatriX/det/CrysSize3 <newsizex> <newsizey> <newsizez> <unit> </code>
<code> /MatriX/det/CrysMaterial <material_name> ("BC400" or "LYSO") </code>

It is possible to change the SiPM model:
<code> /MatriX/det/SiPMmodel <model_name> ("25PE", "50PE", "75PE", "25CS", "50CS", "75CS") </code>

It is possible to put a mask on MatriX to isolate the elements:
<code> /MatriX/det/Mask <bool> (default is "false") </code>

It is possible to activate dark counts or optical cross talk:
<code> /Element/det/OCT <bool> (default is false) </code>
<code> /Element/det/DN <bool>  (default is false) </code>

It is possible to change beam rate, particle or energy:
<code> /Primaries/Rate <value> <unit> </code>
<code> /gun/particle <particle> </code>
<code> /gun/energy <energy> <unit> (momentum as well) </code>

# Output signals

To see the signals you must use signals.C macro:
<code> bash>> root </code>
<code> root-bash>> .L signals.C </code>

<code> root-bash>> signals() </code>

or

<code> signals(path-to-file) </code>

or

<code> signals(path-to-file, path-to-output-file<no file extension>) </code>

or

<code> signals(path-to-file, threashold, path-to-output-file<no file extension>)</code> (threashold is in number of pixel activated per channel)

These commands will produce a out.txt file. Here you have the number of signals per channel, the proton current (always at max 0.22) and the run duration. To extract beam information from datas you can use readgio.cpp macro:
<code> bash>> root </code>
<code> root-bash>> .L readgio.cpp </code>
<code> root-bash>> readMatrix("out.txt") </code> (it requires a "fig" directory to exist in the build folder)
