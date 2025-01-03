#include "../minishell.h"


int main_test(void)
{

	int		counter = 0;
	const char *test_cases[] =
	{
		"ls > out.txt && cat $?out.txt",
		"cat << EOF\nHello, EOF\nEOF",
		"cat << END\nHello\nWorld\nEND",
		"cat << EOF | grep 'pattern'",
		"cat << EOF | wc -l$?",
		"cat << EOF | grep pattern\npattern\nEOF",
		"cat << EOF | grep 'data' | wc -l",
		"cat << EOF\nNested << redirection\nEOF",
		"echo 'new line' >> output1.txt >> output2.txt",
		"ls -l > output.txt 2> error.txt",
		"ls nonexistent > /dev/null 2>&1",
		"cat < input.txt | grep 'pattern' | ls | uniq > output.txt",
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
		"export PATH=$PATH:/new/path && echo $PATH",
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
		"ls non_existent_dir",//efe
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
		"export TEST=\"multiline value\" && echo $TEST",
		"unset TEST && echo $TEST",//60
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
		"export TEST=test_var && echo \"Double quotes $TEST\"",
		"unset TEST && echo \"Unset $TEST\"",
		"echo $(pwd)",
		"seq 1 5 | xargs -I {} echo Number: {}",
		"echo \"Tabs\\tTest\" | expand",
		"echo \"This\\tis\\ta\\ttest\" | tr -d '\\t'",
		"cat /dev/null > test.txt",
		"ls | grep -i test",
		"export EMPTY_VAR='' && echo $EMPTY_VAR",
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
		"export TEST=hello && echo $TEST && unset TEST",
		"export PATH=$PATH:/bin && echo $PATH",
		"unset PATH && echo $PATH",
		"echo Nested $(echo subcommand $(echo deeper))",
		"echo \"Test \\\\$VAR\"",
		"echo 'Preserve single quotes $VAR'",
		"echo $(seq 1 5 | head -n 2)",
		"echo 'Quotes inside $(subcommand)'",
		"echo Multiline\\\ncommand",
		"export VAR=test && echo $VAR && unset VAR",
		"echo Nested dollar \\$",
		"touch file.txt && rm -i file.txt",
		"seq 1 100 | grep 50",
		"echo Multiple $UNKNOWN_VAR and $HOME",
		"cd /bin && ls | grep bash",
		"ls | grep -v '^d'",
		"echo $((5+5))",
		"echo 'math $((10/2))'",
		"echo \"Escaped space \\ \"",
		"echo \"New\\tline\"",
		"export TEST=test && echo \"Mixed $TEST string\"",
		"echo Empty $EMPTY",
		"echo Done.",
		"echo Hello world",
		"echo 1 \"2 3$HOME	\" 4",
		"echo 1 '2 3$HOME' 5",
		NULL
	};

	while (test_cases[counter])
	{
		const char *current_test = (test_cases[counter]);
		printf("\n\n");
		printf("---Test n.%i---\n %s\n", counter, current_test);
		t_cmd_table *table = parse(current_test);

		if (table)
		{
			cmd_print(table);
			free_table(table);

			table = NULL;
		}
		else if (!table)
			printf("-test_is_error-\n");
		counter++;
	}
	return (0);
}
