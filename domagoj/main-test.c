/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main-test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbogovic <dbogovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:50:08 by dbogovic          #+#    #+#             */
/*   Updated: 2025/01/16 20:18:32 by dbogovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


char	*fetch_test(int counter)
{

	const char *test[] =
{
    // Redirection errors
	 "ls | grep 'test'",  // Lists files and filters those containing "test".
    "echo $HOME | wc -c",  // Prints the home directory and counts characters.
    "ls > out.txt && echo Output written",  // Redirects `ls` output to a file, prints a message.
    "echo Hello > file.txt",  // Writes "Hello" to file.txt.
    "cat file.txt | wc -w",  // Counts words in file.txt.
    "ls | cat > result.txt",  // Redirects `ls` output to result.txt using `cat`.
    "echo $USER | cat >> user.txt",  // Appends the username to user.txt.
    "ls | wc -l > count.txt",  // Counts lines in `ls` output and writes to count.txt.
    "echo $? > exit_status.txt",  // Writes the exit status of the previous command to a file.
    "echo This is a test | wc -c",  // Counts characters in the string.

    // Commands with errors or specific behaviors
    "ls -l | ls -l-a",  // Executes `ls -l` successfully; error on `ls -l-a` due to invalid option.
    "nonexistent_command | cat",  // `nonexistent_command` fails; `cat` runs but has no input.
    "ls | nonexistent_command",  // `ls` runs; `nonexistent_command` fails.
    "echo 'test' > /root/test.txt",  // Fails due to permission error writing to /root.
    "echo $? | wc -l | nonexistent_command",  // `echo $?` and `wc -l` run; `nonexistent_command` fails.
    "< nonexistent_file cat",  // Fails because nonexistent_file does not exist.
    "ls | cat | nonexistent_command > output.txt",  // Runs `ls` and `cat`; `nonexistent_command` fails.
    "cat | wc -l",  // Waits for input; does nothing without additional input.
    "ls >>",  // Syntax error due to incomplete redirection.
    "ls < nonexistent_file | wc -l",  // Fails due to nonexistent_file.

    // Using environment variables
    "echo $PATH | wc -l",  // Counts lines in the PATH environment variable.
    "echo $HOME | grep '/'",  // Prints the home directory and checks for slashes.
    "ls $PWD",  // Lists files in the current working directory.
    "echo $SHELL | wc -c",  // Counts characters in the shell's path.
    "cat < $HOME/.bashrc | wc -l",  // Counts lines in .bashrc if it exists.

    // Using exit status substitution
    "true | echo $?",  // `true` exits with 0; `echo $?` prints 0.
    "false | echo $?",  // `false` exits with 1; `echo $?` prints 1.
    "ls > out.txt && echo $?",  // Writes `ls` output to a file and prints 0.
    "ls nonexistent_file && echo $?",  // Prints nothing; $? is not 0.
    "echo $? > status.txt",  // Writes the exit status of the last command to a file.

    // Pipes and redirections
    "ls | grep 'a' > result.txt",  // Filters `ls` output and writes matches to result.txt.
    "echo 'line1\nline2' | wc -l",  // Counts the number of lines in the string.
    "ls > out.txt | cat out.txt",  // Redirects `ls` output to a file, then prints it with `cat`.
    "cat < input.txt | wc -c",  // Counts characters in input.txt.
    "ls | wc -l | cat >> count.txt",  // Appends line count from `ls` output to count.txt.

    // Invalid combinations or syntax
    "echo 'test > test.txt",  // Syntax error: missing closing quote.
    "ls | cat > > file.txt",  // Syntax error: invalid redirection.
    "ls | | cat",  // Syntax error: double pipe is not allowed.
    "echo $?",  // Prints the exit status of the last command.
    "ls > | wc -l",  // Syntax error: incomplete redirection.

    // Various combinations of allowed features
    "ls -la | wc -l",  // Lists files in long format and counts lines.
    "echo 'Test' | grep 'T' > out.txt",  // Filters `echo` output for 'T' and writes to out.txt.
    "cat < /etc/passwd | grep 'root'",  // Filters lines containing 'root' in /etc/passwd.
    "echo $? | wc -c >> status.txt",  // Appends the length of the exit status to status.txt.
    "ls >> appended.txt | wc -c",  // Appends `ls` output to appended.txt; counts characters.

    // Edge cases
    "echo $NONEXISTENT_VAR",  // Prints an empty line because the variable is unset.
    "echo $? > file.txt | ls nonexistent",  // $? is written to file.txt; `ls nonexistent` fails.
    "ls | echo $PWD",  // Prints the current working directory; ignores `ls` output.
    "echo $HOME | wc -l | nonexistent_command",  // Runs `echo $HOME` and `wc -l`; fails on `nonexistent_command`.
	"echo | > out.txt",  // Syntax error: invalid token sequence.
	    "ls | | grep 'a'",  // Syntax error: consecutive `|`.
    ">",  // Syntax error: missing file after `>`.
    ">>",  // Syntax error: missing file after `>>`.
    "<",  // Syntax error: missing file after `<`.
    "echo > > file.txt",  // Syntax error: duplicate `>` token.
    "ls > out.txt >",  // Syntax error: missing file after `>`.

    // Pipe-related errors
    "| ls",  // Syntax error: `|` at the beginning of the command.
    "ls |",  // Syntax error: `|` at the end of the command.

    // Quotes and escape sequence errors
    "echo 'Hello",  // Syntax error: missing closing single quote.
    "echo \"Hello",  // Syntax error: missing closing double quote.
    "echo \\\"",  // Syntax error: invalid escape sequence.

    // Environment variable and substitution errors
    "echo $",  // Syntax error: incomplete variable substitution.
    "echo $? $? >",  // Syntax error: missing file after `>`.

    // Combination errors
    "ls > out.txt | >",  // Syntax error: missing command/file after `>`.

    // Invalid command structure
    "&& echo 'Test'",  // Syntax error: `&&` without preceding command.
    "echo 'Test' &&",  // Syntax error: `&&` without following command.
    "|",  // Syntax error: lone `|` token.
    ">",  // Syntax error: lone `>` token.
    ">>",  // Syntax error: lone `>>` token.

    // Misuse of redirections
    "ls < > out.txt",  // Syntax error: `<` followed by `>` without file.
    "ls < < input.txt",  // Syntax error: duplicate `<`.

    // Invalid syntax with pipes and redirections
    "cat < input.txt | >",  // Syntax error: missing command/file after `>`.
    "| cat",  // Syntax error: pipe at the beginning.
    "echo |",  // Syntax error: pipe at the end.

    // Edge cases with invalid redirections
    "echo hi < > >",  // Syntax error: consecutive invalid redirections.
    "ls | >",  // Syntax error: pipe followed by invalid redirection.

    // Redirection without commands
   // "< file.txt",  // Syntax error: `<` without a command. //SEG FAULT!
  //  "> file.txt",  // Syntax error: `>` without a command. // SEG FAULT!!!!!!!!!!!!!!!!!!!!!!
    //">> file.txt",  // Syntax error: `>>` without a command// SEG FAULT!!!!!!!!!!!!!!!!!!!!!!!!

    // Other invalid command structures
    "echo hi | && ls",  // Syntax error: `&&` cannot follow `|`.
    "echo 'hi | grep 'h'",  // Syntax error: missing closing quote.
		"ls | grep -v '^d'",
		"echo $HOME | cat \"hi\"",
		"ls > out.txt && cat $?out.txt",
		"ls | wc -l > a.txt > b.txt | cat",
		"ls | cat",
		"hi",
		"ls | wc -l | cat",
		"< b.txt cat /dev/null > test.txt",
		" echo 'stderr' >&2",
		"grep 'pattern'",
		"cat < input.txt",
		"uniq > output.txt",
		"cat < input.txt | grep 'pattern' | ls | uniq > output.txt",
		"ls | wc -l | cat",
		"echo $$",
		"echo $)",
		"echo $HOME",
		"echo $HOME$",
		"echo $$HOME",
		"echo $$$HOME",
		"echo $$$l",
		//"cat << EOF | grep 'pattern'",
		//"< 'input.txy' echo 'hi' > 1.out > 2.out", SEG FAULT
		"touch file.txt && rm -i file.txt",
		"ls &&",
		//	"cat << EOF\nNested << redirection\nEOF",
		//		"cat << EOF\nHello, EOF\n",
		"echo$HOME$$",
		"echo hello$HOME$",
		"echo hello$HOME duh",
		"echo$HOME$",
		//	"cat << END\nHello\nWorld\nEND",
		//	"cat << EOF | wc -l$?",
		//		"cat << EOF | grep pattern\npattern",
		//	"cat << EOF | grep 'data' | wc -l",
		"echo 'new line' >> output1.txt >> output2.txt",
		"ls -l > output.txt 2> error.txt",
		"ls nonexistent > /dev/null 2>&1",
		"cat | grep '$HOMEpattern'",
		"cat < input.txt",
		"echo $HOME \"Double $HOMEquotes\"",
		"ls -l",
		"echo 'hello world$HOME' | grep 'world'",
		"cat < input.txt | grep 'pattern'",
		"ls | grep 'pattern' > output.txt",
		"ls | grep 'pattern' >> output.txt",
		"cat < input.txt | grep 'pattern' > output.txt",
		"cat < non_existent_file.txt | grep 'pattern'",
		"| grep 'pattern'",
		"echo 'data' | tee -a file1.txt file2.txt",
		"echo 'hello' | grep 'hello' > overwrite.txt",
		"grep 'nonexistent' file.txt | wc -l",
		"cat < input.txt | grep 'word' > temp.txt | wc -l >> result.txt",
		"ls nonexistent_dir | grep 'txt' > empty.txt",
		"ls -l | grep minishell | awk",
		"echo Hello, World!",
		"cat file.txt | wc -l",
		"pwd",
		"rm -rf test_dir",
		"echo \"Quotes test\"",
		"echo 'Single quotes test'",
		"echo Mix \"Double\" and 'Single' quotes",
		"cat < input.txt > output.txt",
		"mkdir test_dir && cd test_dir && ls",
		//"export PATH=$PATH:/new/path && echo $PATH", SEG FAULT
		"echo \"Multiline\\nOutput\"",
		"cd .. && pwd",
		"echo $(echo Nested Command)",
		"$(echo ls) -l",
		"cd .. && cd -",
		"sleep 1 && echo Done",
		"sleep 2 || echo Skipped",
		"ls | wc | cat -e",
		"env | grep HOME",
		"echo $? > result.txt",
		"ls -la | sort",
		"ls non_existent_dir",
		"grep 'pattern' file.txt",
		"touch file1 file2 file3",
		"rm file1 file2 file3",
		"echo $USER",
		"echo $HOME | wc -c",
		"cd ~",
		"echo \"\\$HOME\"",
		"ls | grep -v file",
		"echo \"\\n\"",
		"ls | tee file.txt",
		"cat file.txt | grep ls",
		"echo \"end\" > end.txt",
		"cat end.txt | wc -c",
		"rm end.txt",
		"ls | head -n 5",
		"seq 1 10 | tail -n 2",
		"echo \"nested $(echo subcommand)\"",
		"cd .. && ls | sort | head -n 3",
		//"export TEST=\"multiline value\" && echo $TEST", SEG FAULT
		"unset TEST && echo $TEST",
		"grep -E 'line[0-9]+' file.txt",
		"rm -f file.txt",
		"touch file{1..10}",
		"ls file*",
		"rm file{1..10}",
		"echo \"Subshell test\" $(echo works!)",
		"echo \"Line\\nbreak\" | wc -l",
		"echo \"Spacing test\" | tr -s ' '",
		"ls | grep .c",
		"echo \"Simple test\"",
		"echo 'Single quotes with $VAR'",
		//"export TEST=test_var && echo \"Double quotes $TEST\"", SEG FAULT
		"unset TEST && echo \"Unset $TEST\"",
		"echo $(pwd)",
		"seq 1 5 | xargs -I {} echo Number: {}",
		"echo \"Tabs\\tTest\" | expand",
		"echo \"This\\tis\\ta\\ttest\" | tr -d '\\t'",
		"ls | grep -i test",
	//	"export EMPTY_VAR='' && echo $EMPTY_VAR", SEG FAULT
		"echo $?",
		"echo \"Escaped dollar \\$VAR\"",
		"echo \"Ending with space \"",
		"cd -",
		"echo $PATH",
		"echo 'Single quotes'",
		"echo \"Combination 'of' quotes\"",
		"ls | head -n 3 | tail -n 1",
		"echo 'Quotes' \"inside\" mixed",
		"echo -n \"No new line\"",
		"touch test_file && rm test_file",
		"echo $(ls -l)",
		"ls | grep pattern | sort",
		"echo 'Hello' 'World'!",
		"echo 'stderr' >&2",
		"env | grep PATH",
		///"export TEST=hello && echo $TEST && unset TEST",
	//	"export PATH=$PATH:/bin && echo $PATH",
		//"unset PATH && echo $PATH",
		"echo Nested $(echo subcommand $(echo deeper))",
		"echo \"Test \\\\$VAR\"",
		"echo 'Preserve single quotes $VAR'",
		"echo $(seq 1 5 | head -n 2)",
		"echo 'Quotes inside $(subcommand)'",
		"echo Multiline\\\ncommand",
	//	"export VAR=test && echo $VAR && unset VAR",
		"echo Nested dollar \\$",
		"seq 1 100 | grep 50",
		"echo Multiple $UNKNOWN_VAR and $HOME",
		"cd /bin && ls | grep bash",
		"ls | grep -v '^d'",
		"echo $((5+5))",
		"echo 'math $((10/2))'",
		"echo \"Escaped space \\ \"",
		"echo \"New\\tline\"",
		//"export TEST=test && echo \"Mixed $TEST string\"",
		"echo Empty $EMPTY",
		"echo Done.",
		"| echo",
		"echo |",
	//	"",
		"|",
		"test <<",
		"test >>",
		"test <",
		"test >",
		"test >< test",
		"test <> test",
		"echo\ntest",
		"echo $(HOME)",
    NULL
};


	return ((char *)test[counter]);
}
