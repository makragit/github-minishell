import pexpect
import re

# HOW TO run: in /minishell/ -> 'python3 ./test-script/py-test.py'

""" TODO

Get Output of bash and minishell somehow?
Get Exit Codes and cross compare them too?

child = pexpect.spawn('/bin/bash')
child = pexpect.spawn('..../minishell')
child.sendline('pwd')
child.sendline('echo $?')
child.expect(pexpect.EOF)  # Wait until the process ends
output = child.before
print(output)

"""

error_count = 0

# Valgrind flags
SUPPRESSION_FILE = "./test-script/minishell.supp"
VALGRINDFLAGS = (
    f"--errors-for-leak-kinds=all --leak-check=full --read-var-info=yes "
    f"--show-error-list=yes --show-leak-kinds=all --suppressions={SUPPRESSION_FILE} "
    f"--trace-children=yes --track-origins=yes --track-fds=all"
)

IGNORED_PATHS = "/bin/* /usr/bin/* /usr/sbin/*"
VALGRINDFLAGS += f" --trace-children-skip={IGNORED_PATHS.replace(' ', ',')}"

def run_test_case(command):
    """
    Run a single test case using pexpect to interact with the shell process.
    """
    print(f"Running command: {command}")

    # Start Valgrind with the target program
    valgrind_cmd = f"valgrind {VALGRINDFLAGS} ./minishell"
    child = pexpect.spawn(valgrind_cmd, encoding='utf-8', timeout=10)

    try:
        # Send the command to the child process
        child.expect_exact("$ ", timeout=4)  # Wait for the shell prompt
        child.sendline(command)  # Send the command

        # Send the exit command after each test case
        if command != "exit":
            child.expect_exact("$ ", timeout=10)
            child.sendline("exit")

        # Wait for the process to exit
        child.expect(pexpect.EOF)

        # Collect and process Valgrind output
        output = child.before

        analyze_valgrind_output(output)

    except pexpect.exceptions.TIMEOUT as e:
        print(f"ERROR: Timeout occurred for command: {command}")
        print(f"Current output:\n{child.before}")
    finally:
        child.close()


def analyze_valgrind_output(output):
    """
    Analyze the Valgrind output for memory leaks and open file descriptors.
    """
    print("Full Valgrind output:")

    # Check File Descriptors
    no_fd_issues = bool(re.search(r"FILE DESCRIPTORS:\s*3 open\s*\(3 std\)\s*at exit", output))

    # Check for LEAK SUMMARY
    no_leaks = bool(re.search(r"definitely lost:\s*0 bytes in 0 blocks.*?", output))
    no_leaks = no_leaks and bool(re.search(r"indirectly lost:\s*0 bytes in 0 blocks.*?", output))
    no_leaks = no_leaks and bool(re.search(r"possibly lost:\s*0 bytes in 0 blocks.*?", output))
    no_leaks = no_leaks and bool(re.search(r"still reachable:\s*0 bytes in 0 blocks.*?", output))

    # Check for ERROR SUMMARY
    no_errors = bool(re.search(r"ERROR SUMMARY:\s*0 errors from 0 contexts", output))

    # Final decision
    if no_leaks and no_errors and no_fd_issues:
        print("\033[0;32m" + "=======================" + "\033[0m")
        print(output)
        print("\033[0;32mNO Memory leaks detected\033[0m")
    else:
        print("\033[0;31m" + "=======================" + "\033[0m")
        print(output)
        print("\033[0;31mMemory leaks or errors found\033[0m")

        global error_count
        error_count += 1

    if not no_leaks:
        # print("Leak summary indicates potential memory leaks.")
        print("\033[0;31mLeak summary indicates potential memory leaks.\033[0m")
    if not no_errors:
        # print("Error summary indicates errors.")
        print("\033[0;31mError summary indicates errors.\033[0m")
    if not no_fd_issues:
        # print("File descriptor issues detected.")
        print("\033[0;31mFile descriptor issues detected.\033[0m")
    print()

def main():
    # Read commands from the file
    with open('./test-script/test_cases.txt', 'r') as file:
        commands = file.readlines()

    # Run each command in the file
    for command in commands:
        command = command.strip()  # Remove any leading/trailing whitespace
        if command and not command.startswith('#'):  # Skip empty lines and comments
            run_test_case(command)
    print()
    print(f"Final error count: {error_count}")

if __name__ == "__main__":
    main()
