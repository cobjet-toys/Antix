import sys
import shutil
import itertools

from configuration import *

global FIRST_FREE_MACHINE
global current_clock_port
global current_grid_port
global current_drawer_port

def start_process(name, **kwargs):
    machine, machine_ip = get_free_computer()
    if machine is None:
        print "No more free machines, exiting."
        sys.exit()
    
    script = "ssh -f -p 24 " + USER + "@" + machine + " 'nohup " + PATH
    if name is "clock":
        global current_clock_port
        script += CLOCK_RUN_COMMAND + " -p " + str(current_clock_port)
        current_clock_port += 1
    elif name is "client":
        # setup full config file for robot client
        shutil.copy(GRIDS_IDS_TMP, "client_config.tmp")
        CLIENT_FULL_CONFIG_FILE = open("client_config.tmp", 'a', 0) # 0 means no buffer
        grid_id_append = "GRID_INIT {0}\n"
        client_num = kwargs['client_num']
        CLIENT_FULL_CONFIG_FILE.write(grid_id_append.format(GRID_BUCKETS[client_num]))
        CLIENT_FULL_CONFIG_FILE.close()
        script += ROBOT_CLIENT_RUN_COMMAND + " -f " + PATH + "Controller/client_config.tmp"
    elif name is "grid":
        global current_grid_port
        script += GRID_RUN_COMMAND + " -f " +PATH + "Controller/" + GRID_CONFIG + " -p " + str(current_grid_port)
        current_grid_port += 1
    elif name is "drawer":
        global current_drawer_port
        script += DRAWER_RUN_COMMAND + " -f " +PATH + "Controller/" + DRAWER_FULL_CONFIG + " -p " + str(current_drawer_port)
        current_drawer_port += 1
    elif name is "controller":
        script += CTRL_CLIENT_RUN_COMMAND + " -f " +PATH + "Controller/" + GRIDS_TMP

    script += " > antix." + machine + ".out &'"
    print "Running: " + script

    try:
        out, error = run_bash_script(script)
        print "Output: " + out.rstrip()
        print

        # Save the IP/port info to server.info file
        if name is "clock":
            to_append = "CLOCK {0} " + str(current_clock_port-1) + "\n"
            GRIDS_TMP_FILE.write(to_append.format(machine_ip.rstrip()))
            GRIDS_IDS_TMP_FILE.write(to_append.format(machine_ip.rstrip()))
            proc = "clock {0}\n"
            PROC_FILE.write(proc.format(machine_ip.rstrip()))
        elif name is "grid":
            to_append = "GRID {0} " + str(current_grid_port-1) + "\n"
            to_append_id = "GRID {0} {1} " + str(current_grid_port-1) + "\n"
            GRIDS_TMP_FILE.write(to_append.format(machine_ip.rstrip()))
            DRAWER_FULL_CONFIG_FILE.write(to_append.format(machine_ip.rstrip()))
            grid_num = kwargs['grid_num']
            GRIDS_IDS_TMP_FILE.write(to_append_id.format(grid_num, machine_ip.rstrip()))
            proc = "grid {0}\n"
            PROC_FILE.write(proc.format(machine_ip.rstrip()))
        elif name is "client":
            proc = "client {0}\n"
            PROC_FILE.write(proc.format(machine_ip.rstrip()))
        elif name is "controller":
            proc = "controller {0}\n"
            PROC_FILE.write(proc.format(machine_ip.rstrip()))
        elif name is "drawer":
            proc = "drawer {0}\n"
            PROC_FILE.write(proc.format(machine_ip.rstrip()))
            
    except BashScriptException as e:
        print "* ERROR STARTING " + name.upper() + " *"
        print e
        # stop everything!
        sys.exit()

def build_binary(name):
    script = "cd " + PATH
    if name is "clock":
        script += CLOCK_BUILD_DIR + "; " + CLOCK_BUILD_COMMAND
    elif name is "client":
        script += ROBOT_CLIENT_BUILD_DIR + "; " + ROBOT_CLIENT_BUILD_COMMAND
    elif name is "grid":
        script += GRID_BUILD_DIR + "; " + GRID_BUILD_COMMAND
    elif name is "drawer":
        script += DRAWER_BUILD_DIR + "; " + DRAWER_BUILD_COMMAND
    elif name is "controller":
        script += CTRL_CLIENT_BUILD_DIR + "; " + CTRL_CLIENT_BUILD_COMMAND
        
    print "Running: " + script

    try:
        out, error = run_bash_script(script)
        print "Output: " + out.rstrip()
        print
    except BashScriptException as e:
        print "* ERROR BUILDING " + name.upper() + " *"
        print e
        # stop everything!
        sys.exit()

def get_free_computer():
    """
    Checks a machine's load average, cpu idle time, and gets its IP.
    Then it decides what to use the machine for.
    """
    try:
        # check that the machine is up
        machine = None
        while(machine is None):
            global FIRST_FREE_MACHINE
            machine_to_test = CSIL_COMPUTERS[FIRST_FREE_MACHINE]
            FIRST_FREE_MACHINE += 1
            # get the machine's IP, and return this as well
            # if we can't get it, the machine's not connectable
            # in the future, we could also get the load average
            # and make a decision based on that
            get_machine_info = "ssh -p 24 " + USER + "@" + machine_to_test + \
                " '" + PATH + "Controller/machine_info.sh'"
            try:
                out, error = run_bash_script(get_machine_info)
                # machine is on, check the info and decide what to use it for
                info = out.split('\n')[:3]
                machine_ip = info[0]
                machine_lavg = info[1].split(" ")
                m_lavg_one = machine_lavg[0]
                m_lavg_five = machine_lavg[1]
                m_lavg_fifteen = machine_lavg[2]
                machine_idle = info[2].split("%")[0]

                # not using the metrics right now, just return the next machine
                machine = machine_to_test
            except BashScriptException as e:
                print "* ERROR CONNECTING TO " + machine_to_test.upper() + " *"
                print e
                print "* TRYING ANOTHER MACHINE *"
                print

        return machine, machine_ip
    except IndexError:
        return None

def run_bash_script(script, stdin=None):
    """
    Returns stderr.
    Raises an error on non-zero return code.
    """
    import subprocess
    proc = subprocess.Popen(['bash', '-c', script], stdout=subprocess.PIPE,
            stderr=subprocess.PIPE)
    stdout, stderr = proc.communicate()

    if proc.returncode:
        raise BashScriptException(proc.returncode, stderr, script)

    return stdout, stderr

class BashScriptException(Exception):
    
    def __init__(self, returncode, stderr, script):
        self.returncode = returncode
        self.stderr = stderr
        self.script = script

    def __str__(self):
        return "Returncode: " + str(self.returncode) + "\nError: " + self.stderr

# Main:

# Get the user argument
if len(sys.argv) != 5:
    print "Usage: python controller.py <sfu_username> <path/to/antix/directory/in/your/home/directory/> <grid config file> <drawer config file>"
    print "For example: python contoller.py hha13 ~/Documents/Antix/ grids.config drawer.config"
    print "Also, make sure you've set up SSH keys for your account."
    sys.exit()

USER = sys.argv[1]
PATH = sys.argv[2]
GRID_CONFIG = sys.argv[3]
DRAWER_CONFIG = sys.argv[4]

GRID_CONFIG_FILE = open(GRID_CONFIG, 'r')

# Set up full config file for drawer
DRAWER_FULL_CONFIG = "drawer_config.tmp"
shutil.copy(DRAWER_CONFIG, DRAWER_FULL_CONFIG)
DRAWER_FULL_CONFIG_FILE = open(DRAWER_FULL_CONFIG, 'a', 0) # 0 means no buffer

# Temporary config files
GRIDS_TMP = "grids.tmp"
GRIDS_IDS_TMP = "grids_ids.tmp"

GRIDS_TMP_FILE = open(GRIDS_TMP, 'w', 0) # 0 means no buffer
GRIDS_IDS_TMP_FILE = open(GRIDS_IDS_TMP, 'w', 0) # 0 means no buffer

FIRST_FREE_MACHINE = 0

# copy the clock/port setting
current_clock_port = int(CLOCK_PORT)
current_grid_port = int(GRID_PORT)
current_drawer_port = int(DRAWER_PORT)

PROC_FILE = open("proc.tmp", 'w', 0) # 0 means no buffer

# Calculate which grids each robot client will go on
bucket = 1
GRID_BUCKETS = list()
div = NUM_ROBOT_CLIENTS/NUM_GRIDS
for i in range (1, NUM_ROBOT_CLIENTS+1):
    GRID_BUCKETS.append(bucket)
    if i%div == 0:
        if bucket < NUM_GRIDS:
            bucket += 1

# Build all the code
print "*** BUILDING BINARIES ***"
print

print "** BUILDING GRID **"
print
build_binary("grid")

print "** BUILDING CLOCK **"
print
build_binary("clock")

print "** BUILDING CONTROLLER CLIENT **"
print
build_binary("controller")

print "** BUILDING ROBOT CLIENT **"
print
build_binary("client")

print "** BUILDING DRAWER **"
print
build_binary("drawer")

# Start processes
print "*** STARTING PROCESSES ***"

print "** STARTING GRIDS **"
print
for i in range(1, int(NUM_GRIDS)+1):
    start_process("grid", grid_num=i)

print "** STARTING CLOCK **"
print
start_process("clock")

GRIDS_TMP_FILE.close()
GRIDS_IDS_TMP_FILE.close()

print "** STARTING CONTROLLER CLIENT **"
print
start_process("controller")

print "** STARTING ROBOT CLIENTS **"
print
for i in range(int(NUM_ROBOT_CLIENTS)):
    start_process("client", client_num=i)

print "** STARTING DRAWER **"
print
start_process("drawer")

GRID_CONFIG_FILE.close()
DRAWER_FULL_CONFIG_FILE.close()
PROC_FILE.close()

print
print "EVERYTHING IS DONE!"
