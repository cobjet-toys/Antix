import sys

from configuration import CLIENT_RUN_COMMAND, GRID_RUN_COMMAND, CLOCK_RUN_COMMAND, DRAWER_RUN_COMMAND

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

if len(sys.argv) != 2:
    print "Usage: python killthemall.py <sfu_username>"
    print "Example: python killthemall.py hha13"
    print "Make sure you've set up SSH keys for your account."
    sys.exit()

USER = sys.argv[1]

# Parse server.info file, kill processes one by one
server_file = open("server.info", "r")
def strip_newlines(s): return s.rstrip()
processes = map(strip_newlines, server_file.readlines())

guillotine = """
.___________________.
|`._______________,'|
| |               | |
| |_______________| |
| ||             || |
| ||_   _        || |
| || ""--._._    || |
| ||       `-.   || |
| ||          `. || |
| ||            \|| |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||             || |
| ||_____________|| |
| |/     ,--.    \| |
| |-(_)-(    )-(_)| |
| |      `--'     | |
|===================|
|                   |
"""

print "I'm ready to kill them all, march 'em up all up the guillotine."
print guillotine

for process in processes:
    info = process.split(',')
    p_type = info[0]
    p_ip = info[1]

    script = "ssh -p 24 " + USER + "@" + p_ip + " 'killall "
    if p_type == "client":
        thing = CLIENT_RUN_COMMAND.split('/')
        script += thing[-1]
    elif p_type == "grid":
        script += GRID_RUN_COMMAND.split('/')[-1]
    elif p_type == "clock":
        script += CLOCK_RUN_COMMAND.split('/')[-1]
    elif p_type == "drawer":
        script += DRAWER_RUN_COMMAND.split('/')[-1]
    script += "'"

    print "Running: " + script

    try:
        out, error = run_bash_script(script)
        print "Output: " + out.rstrip()
        print
            
    except BashScriptException as e:
        print "* ERROR! I just can't kill them all, I don't have it in me anymore :( *"
        print e

print
print "Heads have rolled."
