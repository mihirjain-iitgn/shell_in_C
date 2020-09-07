all : cat ls mkdir mv grep chmod pwd rm cp shell
cat : cat.c
	$ gcc -o ./bin/cat cat.c
ls : ls.c
	$ gcc -o ./bin/ls ls.c
mkdir : mkdir.c
	$ gcc -o ./bin/mkdir mkdir.c
mv : mv.c
	$ gcc -o ./bin/mv mv.c
grep : grep.c
	$ gcc -o ./bin/grep grep.c
pwd : pwd.c
	$ gcc -o ./bin/pwd pwd.c
rm : rm.c
	$ gcc -o ./bin/rm rm.c
cp : cp.c
	$ gcc -o ./bin/cp cp.c
chmod : chmod.c
	$ gcc -o ./bin/chmod chmod.c
shell : shell.c
	$ gcc -o ./shell shell.c