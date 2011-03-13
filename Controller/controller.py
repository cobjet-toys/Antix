import sys
import itertools

from configuration import *

FIRST_FREE_MACHINE = 0

def start_process(name, **kwargs):
    machine, machine_ip = get_free_computer()
    if machine is None:
        print "No more free machines, exiting."
        sys.exit()
    
    script = "ssh -f -p 24 " + USER + "@" + machine + " 'nohup " + PATH
    if name is "clock":
        script += CLOCK_RUN_COMMAND + " " + str(current_clock_port) + " " + NUM_CLIENTS
        global current_clock_port
        current_clock_port += 1
    elif name is "client":
        client_num = kwargs['client_num']
        script += CLIENT_RUN_COMMAND + " " + PATH + "Controller/" + SERVER_INFO + " " + PATH + "Controller/" + SYSTEM_CONFIG + " " + str(client_num)
    elif name is "grid":
        #script += GRID_RUN_COMMAND + " " + SERVER_INFO + " " + str(current_grid_port)
        script += GRID_RUN_COMMAND + " " + str(current_grid_port)
        global current_grid_port
        current_grid_port += 1
    elif name is "drawer":
        if FOV == "1":
            script += DRAWER_RUN_COMMAND + " " + WI_SIZE + " " + WO_SIZE + " " + H_RADIUS + " 1 " + FOV_ANGLE + " " + FOV_RANGE
        else:
            script += DRAWER_RUN_COMMAND + " " + WI_SIZE + " " + WO_SIZE + " " + H_RADIUS + " 0"
    script += " > antix." + machine + ".out &'"
    print "Running: " + script

    try:
        out, error = run_bash_script(script)
        print "Output: " + out.rstrip()
        print

        # Save the IP/port info to server.info file
        if name is "clock":
            to_append = "clock,{0}," + str(current_clock_port-1)
        if name is "drawer":
            to_append = "drawer,{0}"
        if name is "grid":
            to_append = "grid,{0}," + str(current_grid_port-1)
        if name is "client":
            to_append = "client,{0}"

        to_append += "\n"
        SERVER_INFO_FILE.write(to_append.format(machine_ip.rstrip()))
            
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
        script += CLIENT_BUILD_DIR + "; " + CLIENT_BUILD_COMMAND
    elif name is "grid":
        script += GRID_BUILD_DIR + "; " + GRID_BUILD_COMMAND
    elif name is "drawer":
        script += DRAWER_BUILD_DIR + "; " + DRAWER_BUILD_COMMAND
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
if len(sys.argv) != 4:
    print "Usage: python controller.py <sfu_username> <path/to/antix/directory/in/your/home/directory/> <system.config>"
    print "For example: python contoller.py hha13 ~/Documents/Antix/ system.config"
    print "Also, make sure you've set up SSH keys for your account."
    sys.exit()

USER = sys.argv[1]
PATH = sys.argv[2]
SYSTEM_CONFIG = sys.argv[3]

# Parse system config file
config_file = open(SYSTEM_CONFIG, 'r')
def strip_newlines(s): return s.rstrip()
configs = map(strip_newlines, config_file.readlines())

NUM_TEAMS = configs[0]
NUM_ROBOTS_PER_TEAM = configs[1]
NUM_CLIENTS = configs[2]
NUM_GRIDS = configs[3]
WI_SIZE = configs[4]
WO_SIZE = configs[5]
H_RADIUS = configs[6]
FOV = configs[7]
FOV_ANGLE = configs[8]
FOV_RANGE = configs[9]

SERVER_INFO = "server.info"
SERVER_INFO_FILE = open(SERVER_INFO, 'w', 0) # 0 means no buffer

# copy the clock/port setting
current_clock_port = int(CLOCK_PORT)
current_grid_port = int(GRID_PORT)

# Build all the code
print "*** BUILDING BINARIES ***"
print

print "** BUILDING CLOCK **"
print
build_binary("clock")

print "** BUILDING GRID **"
print
build_binary("grid")

print "** BUILDING CLIENT **"
print
build_binary("client")

print "** BUILDING DRAWER **"
print
build_binary("drawer")

# Start processes
print "*** STARTING PROCESSES ***"

print "** STARTING CLOCK **"
print
start_process("clock")

print "** STARTING DRAWER **"
print
start_process("drawer")

print "** STARTING GRIDS **"
print
for _ in itertools.repeat(None, int(NUM_GRIDS)):
    start_process("grid")

print "** STARTING CLIENTS **"
print
for i in range(1, int(NUM_CLIENTS)+1):
    start_process("client", client_num=i)

SERVER_INFO_FILE.close()

print
print "EVERYTHING IS DONE!"
