Requires Python and pyGame to run.

http://python.org/
http://www.pygame.org
https://github.com/mrGSOF/3dWireFrame
https://github.com/mrGSOF/GSOF_Cockpit

## Running instructions
- Clone
- Install Python (tested with 3.11, 3.13.1).
- run `pip install pygame`
- run `pip install pymesh`
- Clone submodules and run `pip install .` for each one.
- run 'py Simulation.py'


//=============================================================
Linux Debian  


python3 -m venv venv 
source venv/bin/activate

pip install psutil
pip install pygame
pip install pymesh
pip install pymeshlab

# Insure the following are installed in 10-AeroSim-02-Py/0-AeroSim/

# display
cd /home/vlad/AeroSims/flightSim-main
cp -r display ../6-AeroSim-02-Py/0-AeroSim/

# GSOF_3dWireFrame
cd /home/vlad/AeroSims/GSOF_3dWireFrame-main
cp -r GSOF_3dWireFrame ../6-AeroSim-02-Py/0-AeroSim/

# GSOF_Cockpit
cd /home/vlad/AeroSims/GSOF_Cockpit-main
cp -r GSOF_Cockpit ../6-AeroSim-02-Py/0-AeroSim/

#to run 
cd /home/vlad/AeroSims/6-AeroSim-02-Py/0-AeroSim/
python3 Simulation.py


//================================================================

python3 -m venv venv 

source venv/bin/activate

python3 Simulation.py

//================================================================
UDP Data dump commands  - terminal 
# Basic capture of UDP port 5001
sudo tshark -i any -f "udp port 5001"

# Show packet details + payload
sudo tshark -i any -f "udp port 5001" -V

# Hex + ASCII dump of the payload
sudo tshark -i any -f "udp port 5001" -x

# Just the raw data (cleanest for dumping)
sudo tshark -i any -f "udp port 5001" -T fields -e data

//===================================================================
# Make sure you are in the project directory
cd ~/AeroSims/10-AeroSim-02-Py/0-AeroSim

# Deactivate the broken venv
deactivate

# Remove the old venv
rm -rf venv

# Create a fresh one (uses whatever python3 is currently on your PATH)
python3 -m venv venv

# Activate it
source venv/bin/activate

# Upgrade pip and reinstall whatever you need
pip install --upgrade pip
# then reinstall your packages, e.g.:
# pip install -r requirements.txt
# or whatever packages Simulation.py needs
//===================================================================


