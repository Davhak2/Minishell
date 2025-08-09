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
        echo "   Ожидалось: '$EXPECT'"
        echo "   Получено:  '$OUT'"
        ((FAIL++))
    fi
}

run_test "echo hello" "hello"
run_test "pwd" "$(pwd)"

touch testfile.c
run_test "ls *.c" "testfile.c"
rm testfile.c

run_test "export FOO=bar && echo \$FOO" "bar"
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
run_test "export A=1 B=2 && echo \$A \$B" "1 2"
run_test "env | grep ^PATH=" "$(env | grep ^PATH=)"
run_test "cd / && pwd" "/"

mkdir -p wildtest
touch wildtest/a.c wildtest/b.c wildtest/.hidden.c
run_test "cd wildtest && echo *.c" "a.c b.c"
run_test "cd wildtest && echo a*.c" "a.c"
run_test "cd wildtest && echo '*.c'" "*.c"
run_test "cd wildtest && echo *.cpp" "*.cpp"
run_test "cd wildtest && echo .*c" ".hidden.c"
run_test "cd wildtest && echo *" "a.c b.c"
cd ..
rm -rf wildtest

# --- heredoc + SIGINT (CTRL+C) ---
run_heredoc_sigint_test() {
    local name="heredoc SIGINT"
    # Проверка наличия util-linux `script`
    if ! command -v script >/dev/null 2>&1; then
        echo "⚠️  Пропуск '$name': не найден 'script'"; return
    fi

    # Запуск minishell в PTY: посылаем SIGINT, когда он ждёт ввод heredoc
    # После прерывания отправляем команды для проверки: печатаем маркер, затем печатаем $?, потом exit
    RAW=$(
        script -qfec "
            ( sleep 0.25; kill -INT \$PPID ) &
            $MINISHELL
        " /dev/null <<'EOS'
cat <<EOF
line1
echo __AFTER__
echo $?
exit
EOS
    )

    # Нормализуем вывод: убираем CR, пустые строки, баннеры script (если вдруг есть)
    OUT=$(printf "%s\n" "$RAW" | tr -d '\r' | sed '/^Script started/d;/^Script done/d')
    # Отфильтруем твой prompt и визуальный мусор, как в run_test
    OUT=$(printf "%s\n" "$OUT" | grep -v 'ms: ➜' | grep -v '^🐚' | sed '/^exit$/d' | sed '/^$/d')

    # Проверки:
    # 1) Есть печать ^C отдельной строкой
    echo "$OUT" | grep -qx '\^C'; has_caretC=$?
    # 2) Команда не выполнилась: не должно быть "line1" от cat'а
    echo "$OUT" | grep -q '^line1$'; has_line1=$?
    # 3) После прерывания шелл жив и принял следующие команды: есть "__AFTER__"
    echo "$OUT" | grep -q '^__AFTER__$'; has_after=$?
    # 4) Статус сразу после прерывания heredoc — 130 (bash-совместимо)
    #    Мы напечатали "echo $?" как отдельную строку — она должна быть "130"
    status_line=$(echo "$OUT" | awk '/^[0-9]+$/{print; exit}')
    if [ "$has_caretC" -eq 0 ] && [ "$has_line1" -ne 0 ] && [ "$has_after" -eq 0 ] && [ "$status_line" = "130" ]; then
        echo "✅ '$name' => OK"
        ((OK++))
    else
        echo "❌ '$name' => FAIL"
        echo "   Полный вывод:"
        echo "----------------"
        echo "$OUT"
        echo "----------------"
        ((FAIL++))
    fi
}

run_heredoc_sigint_test


echo
echo "Успешно: $OK, Ошибок: $FAIL"
[ $FAIL -eq 0 ]