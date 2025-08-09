#!/bin/bash

MINISHELL=./minishell

OK=0
FAIL=0
TMP_DIR=$(mktemp -d)
trap 'rm -rf "$TMP_DIR"' EXIT

run() {
    CMD="$1"
    EXPECT_OUT="$2"
    EXPECT_RC="$3"
    OUT=$($MINISHELL <<< "$CMD"$'\n''echo __STATUS:$?' 2>&1 | grep -v 'ms: ➜' | grep -v '^🐚' | sed '/^exit$/d' | sed '/^$/d')
    RC=$(echo "$OUT" | tail -n1 | sed -n 's/^__STATUS:\([0-9][0-9]*\)$/\1/p')
    BODY=$(echo "$OUT" | sed '/^__STATUS:[0-9][0-9]*$/d')
    PASS=1
    if [ "$EXPECT_OUT" != "__ANY__" ] && [ "$BODY" != "$EXPECT_OUT" ]; then PASS=0; fi
    if [ -n "$EXPECT_RC" ] && [ "$RC" != "$EXPECT_RC" ]; then PASS=0; fi
    if [ $PASS -eq 1 ]; then
        echo "✅ '$CMD' => OK"
        ((OK++))
    else
        echo "❌ '$CMD' => FAIL"
        echo "   Ожидалось вывод: '$EXPECT_OUT'"
        echo "   Получено вывод:  '$BODY'"
        echo "   Ожидался код:    '${EXPECT_RC}'"
        echo "   Получен код:     '${RC}'"
        ((FAIL++))
    fi
}

touch "$TMP_DIR/in.txt"
echo "infile" > "$TMP_DIR/in.txt"
NOEXEC="$TMP_DIR/noexec.sh"
echo -e '#!/bin/sh\necho hi' > "$NOEXEC"
chmod -x "$NOEXEC"

run "echo hello" "hello" 0
run "echo -n hello" "hello" 0
run "echo 'a \$USER'" "a \$USER" 0
run "export FOO=bar; echo \"\$FOO\"" "bar" 0
run "export FOO=bar; unset FOO; echo \"\$FOO\"" "" 0

run "true" "" 0
run "false" "" 1
run "echo pre; false; echo post; echo \$?" $'pre\npost\n1' 0
run "false | true" "" 0
run "true | false" "" 1
run "echo hello | wc -w" "1" 0
run "printf 'a b c\n' | wc -w" "3" 0
run "echo hello world | tr a-z A-Z" "HELLO WORLD" 0

run "echo 123 > $TMP_DIR/out.txt; cat $TMP_DIR/out.txt" "123" 0
run "echo first > $TMP_DIR/append.txt; echo second >> $TMP_DIR/append.txt; cat $TMP_DIR/append.txt" $'first\nsecond' 0
run "cat < $TMP_DIR/in.txt" "infile" 0
run "cat < $TMP_DIR/absent.txt" "__ANY__" 1
run "echo hi > /root/forbidden.txt" "__ANY__" 1

run $'export A=Z;\ncat <<EOF\n$A\nEOF' "Z" 0
run $'export A=Z;\ncat <<\'EOF\'\n$A\nEOF' "\$A" 0

run "pwd > $TMP_DIR/p.out; cd /; pwd; cd - > /dev/null; pwd; cat $TMP_DIR/p.out" $'/\n'"$(pwd)"$'\n'"$(pwd)" 0
run "cd ..; echo \$?" "__ANY__" 0
run "cd /does/not/exist" "__ANY__" 1
run "cd too many args" "__ANY__" 1

run "echo \$PATH >/dev/null; /definitely/not/a/cmd" "__ANY__" 127
run "$NOEXEC" "__ANY__" 126
run "/" "__ANY__" 126

run "export FOO=bar | cat; echo \"\$FOO\"" "" 0

run "echo \"a   b\"" "a   b" 0
run "echo a\ \ b" "a  b" 0
run "echo \"*\"" "*" 0

run "echo | cat" "" 0
run "ls |" "__ANY__" 2
run "| ls" "__ANY__" 2
run "echo >" "__ANY__" 2
run "echo 'unterminated" "__ANY__" 2

touch "$TMP_DIR/file.c"
run "ls $TMP_DIR/*.c" "$TMP_DIR/file.c" 0

run "exit" "" 0
run "exit 42" "" 42
run "exit 256" "__ANY__" 0
run "exit abc" "__ANY__" 2
run "exit 1 2" "__ANY__" 1

echo
echo "Успешно: $OK, Ошибок: $FAIL"
[ $FAIL -eq 0 ]
