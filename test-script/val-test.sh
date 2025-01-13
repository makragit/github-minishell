#!/bin/bash

# TODO unfreed return from readline ends up in suppressed!
#   input = readline(...); free(input) input must be freed!

#  TODO	char *commands[] = {"/", "/usr/bin/cat", "cat", "/nix", "/usr/nix", 
#		"usr/bin/cat", "/usr/bin/cat/", NULL};
#       test relative path using "./test-script/val-test.h"





# TODO " '1 2 3' " not parsed correctly

PROGNAME='minishell'
DOT='.'

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'  # Yellow color
NC='\033[0m'  # No color

# Readline Suppression
SUPPRESSION_FILE="./test-script/minishell.supp"
VALGRINDFLAGS="--errors-for-leak-kinds=all --leak-check=full --read-var-info=yes --show-error-list=yes --show-leak-kinds=all --suppressions=$SUPPRESSION_FILE --trace-children=yes --track-origins=yes"
VALGRINDFDFLAGS="--track-fds=all"
IGNORED_PATHS="/bin/* /usr/bin/* /usr/sbin/* $(which -a norminette)"
VALGRINDFLAGS+=" --trace-children-skip=$(echo $IGNORED_PATHS | sed 's/ /,/g')"

# TODO echo 1 "2 3" 4
# TODO echo $HOME
test_cases=(
    "exit"
    "asdf\nexit\n"
    "asdf\nexit\n"
    "/\nexit\n"

    "cd\nexit\n"
    "cd 1\nexit\n"
    "cd 1 2\nexit\n"
    "cd ./test-script/\nexit\n"

    "pwd\nexit\n"
    "pwd 1\nexit\n"
    "pwd 1 2\nexit\n"

    # "/usr/bin/cat\nexit\n"
    # "cat\nexit\n"
    "/nix\nexit\n"
    "/usr/nix\nexit\n"
    # "usr/bin/cat\nexit\n"
    "./notexisting\nexit\n"
    #"./test.sh (echos something)
    "ls\nexit\n"
    "ls -la\nexit\n"
    # "/usr/bin/cat/\nexit\n"
    # "./minishell\nexit\n"
    # "./test-script/val-test.h\nexit\n"
    "NULL\nexit\n"
    "env\nexit\n"
    "env\nenv\nexit\n"

    "export\nexit"
    "unset\nexit"
    "export test\nexit"
    "export test\nunset test\nexit"

    "echo\nexit"
    "echo \nexit"
    "echo hello world\nexit"
    "echo 1 2 3 4\nexit"
)

# Flag handling
if [[ -z "$1" || "$1" == "-v" ]]; then
    echo -e "${GREEN}Running Valgrind Tests...${NC}"
    for args in "${test_cases[@]}"; do
    echo "Running test case: $args"
    valgrind_output=$(printf "$args" | valgrind $VALGRINDFLAGS $VALGRINDFDFLAGS ./minishell 2>&1)

    # Extract and check the LEAK SUMMARY
    leak_summary=$(echo "$valgrind_output" | sed -n '/Command:/,/ERROR SUMMARY:/p')
    if [ -n "$leak_summary" ]; then
        # if echo "$leak_summary" | grep -q "ERROR SUMMARY: 0 errors from 0 contexts" && \
        #    echo "$leak_summary" | grep -q "definitely lost: 0 bytes in 0 blocks" && \
        #    echo "$leak_summary" | grep -q "indirectly lost: 0 bytes in 0 blocks" && \
        #    echo "$leak_summary" | grep -q "possibly lost: 0 bytes in 0 blocks" && \
        #    echo "$leak_summary" | grep -q "still reachable: 0 bytes in 0 blocks" && \
        #    ! echo "$leak_summary" | grep -q "Open file descriptor [3-9]"; then


        if echo "$leak_summary" | grep -q "ERROR SUMMARY: 0 errors from 0 contexts" && \
            (echo "$leak_summary" | grep -q "All heap blocks were freed -- no leaks are possible" || \
            (echo "$leak_summary" | grep -q "definitely lost: 0 bytes in 0 blocks" && \
            echo "$leak_summary" | grep -q "indirectly lost: 0 bytes in 0 blocks" && \
            echo "$leak_summary" | grep -q "possibly lost: 0 bytes in 0 blocks" && \
            echo "$leak_summary" | grep -q "still reachable: 0 bytes in 0 blocks")) && \
            ! echo "$leak_summary" | grep -q "Open file descriptor [3-9]"; then


            echo -e "${GREEN}$leak_summary${NC}\n"
            echo -e "${GREEN}NO Memory leaks detected${NC}\n"
        else
            echo -e "${RED}$leak_summary${NC}\n"
            echo -e "${RED}Memory leaks found${NC}\n"
        fi
    else
        echo "$valgrind_output" | awk -v yellow="$YELLOW" -v reset="$NC" '{print yellow $0 reset}'
        echo -e "${YELLOW}No LEAK SUMMARY found. Full Valgrind output:${NC}"
    fi
    echo
    done

elif [[ "$1" == "-f" ]]; then
    echo -e "${GREEN}Running Funcheck Tests...${NC}"
    for args in "${test_cases[@]}"; do
        echo "Running: funcheck -a $args"
        funcheck -a $args

        echo -e "\nPress any key to proceed to the next test, or Ctrl+C to exit."
        read -n 1 -s
        echo
    done

elif [[ "$1" == "-h" ]]; then
    echo -e "${YELLOW}Usage:${NC} ./test.sh [-v] (Valgrind) | -f (Funcheck)"

else
    echo -e "${RED}Error:${NC} Unrecognized flag '$1'"
    echo -e "${YELLOW}Usage:${NC} ./test.sh [-v] (Valgrind) | -f (Funcheck)"
fi
