import sys
import itertools

from configuration import *

FIRST_FREE_MACHINE = 0

def start_process(name):
    machine = get_free_computer()
    if machine is None:
        print "No more free machines, exiting."
        # TODO: some code that stops all running processes across all computers
        # not exactly sure how to do this
        sys.exit()
    script = "ssh -p 24 " + USER + "@" + machine + " '"
    if name is "clock":
        script += CLOCK_STARTUP_SCRIPT + "'"
    elif name is "client":
        script += CLIENT_STARTUP_SCRIPT + "'"
    elif name is "server":
        script += SERVER_STARTUP_SCRIPT + "'"
    elif name is "drawer":
        script += DRAWER_STARTUP_SCRIPT + "'"
    print "Running: " + script

    try:
        out, error = run_bash_script(script)
        print "Output: " + out.rstrip()
        print
    except BashScriptException as e:
        print "* ERROR STARTING " + name.upper() + " *"
        print e

def get_free_computer():
    """
    In the future this could check the load average on each machine
    and only return a machine based on what we deem to be free.

    For now, returns the first machine that doesn't have one of
    our processes running on it. One process per machine.
    """
    try:
        global FIRST_FREE_MACHINE
        machine = CSIL_COMPUTERS[FIRST_FREE_MACHINE]
        FIRST_FREE_MACHINE += 1
        return machine
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
        #print self.returncode
        #print self.stderr
        #return "this is the exception"


# Get the user argument
if len(sys.argv) != 2:
    print "Usage: python controller.py <sfu_username>"
    print "Make sure you've set up SSH keys for your account."
    sys.exit()

USER = sys.argv[1]

# Start clock
print "*** STARTING CLOCK ***"
print
start_process("clock")

# Start clients
print "*** STARTING CLIENTS ***"
print
for _ in itertools.repeat(None, CLIENTS):
    start_process("client")

# Start servers
print "*** STARTING SERVERS ***"
print
for _ in itertools.repeat(None, SERVERS):
    start_process("server")

# Start drawer process
print "*** STARTING DRAWER ***"
print
start_process("drawer")

