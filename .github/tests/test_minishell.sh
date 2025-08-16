#!/bin/bash

MINISHELL=./minishell

OK=0
FAIL=0

run_test() {
    CMD="$1"
    EXPECT="$2"
    OUT=$($MINISHELL <<< "$CMD" 2>&1 | grep -v 'ms: ➜' | grep -v '^🐚' | sed '/^exit$/d' | sed '/^$/d')
    if [ "$OUT" = "$EXPECT" ]; then
        echo "✅ '$CMD' => OK"
        ((OK++))
    else
        echo "❌ '$CMD' => FAIL"
        echo "   Expected: '$EXPECT'"
        echo "   Got:      '$OUT'"
        ((FAIL++))
    fi
}

run_test "echo hello" "hello"
run_test "pwd" "$(pwd)"

touch testfile.c
run_test "ls *.c" "testfile.c"
rm testfile.c

run_test "export FOO=bar && unset FOO && echo \$FOO" ""
run_test "echo hello world | wc -w" "2"
run_test "echo one two three | grep two" "one two three"
run_test "echo abc | tr a-z A-Z" "ABC"
run_test "echo 123 > out.txt && cat out.txt" "123"
run_test "echo first > out.txt && echo second >> out.txt && cat out.txt" $'first\nsecond'
run_test "ls non_existent_file" "ls: cannot access 'non_existent_file': No such file or directory"
run_test "false" ""
run_test "true && echo ok" "ok"
run_test "false || echo fail" "fail"
run_test "env | grep ^PATH=" "$(env | grep ^PATH=)"
run_test "cd / && pwd" "/"

mkdir -p wildtest
touch wildtest/a.c wildtest/b.c wildtest/.hidden.c
run_test "cd wildtest && echo *.c" "a.c b.c"
run_test "cd wildtest && echo a*.c" "a.c"
run_test "cd wildtest && echo '*.c'" "*.c"
run_test "cd wildtest && echo *.cpp" "*.cpp"
run_test "cd wildtest && echo *" "a.c b.c"
cd ..
rm -rf wildtest


echo
echo "Success: $OK, Failures: $FAIL"
[ $FAIL -eq 0 ]