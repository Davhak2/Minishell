#!/bin/bash

# Путь к вашему minishell
MINISHELL=./minishell

# Счётчик успешных/неуспешных тестов
OK=0
FAIL=0

# Функция для одного теста: команда, ожидаемый вывод
run_test() {
    CMD="$1"
    EXPECT="$2"
    OUT=$($MINISHELL <<< "$CMD" 2>&1 | grep -v 'ms: ➜' | grep -v '^🐚' | sed '/^exit$/d' | sed '/^$/d')
    if [ "$OUT" = "$EXPECT" ]; then
        echo "✅ '$CMD' => OK"
        ((OK++))
    else
        echo "❌ '$CMD' => FAIL"
        echo "   Ожидалось: '$EXPECT'"
        echo "   Получено:  '$OUT'"
        ((FAIL++))
    fi
}

# Примеры тестов:
run_test "echo hello" "hello"
run_test "pwd" "$(pwd)"
touch testfile.c
run_test "ls *.c" "testfile.c"
rm testfile.c
run_test "export FOO=bar && echo \$FOO" "bar"
run_test "export FOO=bar && unset FOO && echo \$FOO" ""

echo
echo "Успешно: $OK, Ошибок: $FAIL"
[ $FAIL -eq 0 ]