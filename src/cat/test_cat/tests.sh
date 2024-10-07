#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
SUCCESS_leak=0
FAIL_leak=0
COUNTER_leak=0
DIFF=""

s21_command=(
    "./../s21_cat"
    )
sys_command=(
    "cat"
    )

tests=(
"FLAGS test_files/test_case_cat.txt"
"FLAGS test_files/test_case_cat.txt test_files/test_1_cat.txt"
)
flags=(
    "b"
    "e"
    "n"
    "s"
    "t"
    "v"
)
manual=(
"-s test_files/test_1_cat.txt"
"-b -e -n -s -t -v test_files/test_1_cat.txt"
"-b test_files/test_1_cat.txt"
"-t test_files/test_3_cat.txt"
"-n test_files/test_2_cat.txt"
"-n -b test_files/test_1_cat.txt"
"-s -n -e test_files/test_4_cat.txt"
"-n test_files/test_1_cat.txt"
"-n test_files/test_1_cat.txt test_files/test_2_cat.txt"
"-v test_files/test_5_cat.txt"
"-- test_files/test_5_cat.txt"
)

gnu=(
"-T test_files/test_1_cat.txt"
"-E test_files/test_1_cat.txt"
"-vT test_files/test_3_cat.txt"
"--number test_files/test_2_cat.txt"
"--squeeze-blank test_files/test_1_cat.txt"
"--number-nonblank test_files/test_4_cat.txt"
"-bnvste test_files/test_6_cat.txt"
)
run_test() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    "${s21_command[@]}" $param > "${s21_command[@]}".log
    "${sys_command[@]}" $param > "${sys_command[@]}".log
    DIFF="$(diff -s "${s21_command[@]}".log "${sys_command[@]}".log)"
    let "COUNTER++"
    if [ "$DIFF" == "Files "${s21_command[@]}".log and "${sys_command[@]}".log are identical" ]
    then
        let "SUCCESS++"
        echo "$COUNTER ok $param"
    else
        let "FAIL++"
        echo "$COUNTER bad $param"
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}
run_leak_test() {
    param=$(echo $@ | sed "s/FLAGS/$var/")
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose -q --log-file="${s21_command[@]}".log ./"${s21_command[@]}" $param > /dev/null
    leak=$(grep -ic -A10000 "LEAK SUMMARY:" "${s21_command[@]}".log || true)
    leak2=$(grep -ic -A10000 "ERROR SUMMARY: [^0]" "${s21_command[@]}".log || true)
    let "COUNTER_leak++"
    if [ "$leak" -eq "0" ] && [ "$leak2" -eq "0" ]
    then
        let "SUCCESS_leak++"
        echo "$COUNTER_leak - Success $param"
    else
        let "FAIL_leak++"
        echo "$COUNTER_leak - Fail $param"
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}

printf "\ntests with flags\n\n"
printf "\nman tests\n\n"
for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done

printf "\ntests with 1 flag\n\n"

for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done
printf "\ntests with 2 flag\n\n"

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                run_test "$i"
            done
        fi
    done
done
printf "\ntests with 3 flag\n\n"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    run_test "$i"
                done
            fi
        done
    done
done
printf "\ntests with 4 flag\n\n"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            for var4 in "${flags[@]}"
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] \
                && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
                && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then
                    for i in "${tests[@]}"
                    do
                        var="-$var1 -$var2 -$var3 -$var4"
                        run_test "$i"
                    done
                fi
            done
        done
    done
done
# 2 сдвоенных параметра
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                run_test "$i"
            done
        fi
    done
done

# 3 строенных параметра
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    run_test "$i"
                done
            fi
        done
    done
done

printf "\nleak tests with 1 flag\n\n"
for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_leak_test "$i"
    done
done


printf "\nleak tests with 2 flag\n\n"
for var2 in "${flags[@]}"
do
  for var1 in "${flags[@]}"
  do
    if [ $var1 != $var2 ]
    then
        for i in "${tests[@]}"
        do
            var="-$var1$var2"
            run_leak_test "$i"
        done
    fi
  done
done

printf "\ntests\n\n"

printf "\n"
echo "failed: $FAIL"
echo "succes: $SUCCESS"
echo "all: $COUNTER"
printf "\n"

printf "\nleak tests\n\n"

printf "\n"
echo "failed leaks: $FAIL_leak"
echo "succes leaks: $SUCCESS_leak"
echo "all leaks: $COUNTER_leak"
printf "\n"


printf "\ngnu tests\n\n"

FAIL=0
SUCCESS=0
COUNTER=0

for i in "${gnu[@]}"
do
    var="-"
    run_test $i
done

printf "\n"

echo "failed: $FAIL"
echo "succes: $SUCCESS"
echo "all: $COUNTER"
printf "\n"

